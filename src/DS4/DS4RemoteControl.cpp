//
// Created by Julian Bock on 2018-04-12.
//

#include "DS4RemoteControl.h"

int main(int argc, char** argv)
{
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("dev") || 0 == commandlineArguments.count("freq") ||
        0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("safety-distance")) {
        std::cerr << argv[0] << " reads (wireless) controller input from the specified path and relays it to components on the BeagleBone."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --dev=<path toController> --freq=<int pollingRate> --cid=<OD4Session toComponents> --safety-distance=<int cm>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --dev=/dev/input/js0 --freq=50 --cid=111 --safety-distance=20" << std::endl;
        retVal = 1;
    } else {
        const std::string DEV = commandlineArguments["dev"];
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
        const float SAFETY_DISTANCE = std::stoi(commandlineArguments["safety-distance"]) /100.f;

        float direction = 1.f, distance = 0.f;
        int lastCmd = 0, targetGroup = 0, correctionEnabled = 0;
        float steering = 0.f, steeringCorrection = 0.f, speed = 0.f, speedCorrection = 0.f;

        cluon::OD4Session od4(CID, [&distance, &speedCorrection, &steeringCorrection](cluon::data::Envelope &&envelope) noexcept {
            switch (envelope.dataType()) {
                case 1039: {
                    opendlv::proxy::DistanceReading dr =
                            cluon::extractMessage<opendlv::proxy::DistanceReading>(std::move(envelope));
                    distance = dr.distance();
                } break;
                case 9012: {
                    MARBLE::Steering::Correction::GroundSteering gsrCorrection =
                            cluon::extractMessage<MARBLE::Steering::Correction::GroundSteering>(std::move(envelope));
                    steeringCorrection = gsrCorrection.steeringCorrection();
                } break;
                case 9013: {
                    MARBLE::Steering::Correction::PedalPosition ppCorrection =
                            cluon::extractMessage<MARBLE::Steering::Correction::PedalPosition>(std::move(envelope));
                    speedCorrection = ppCorrection.pedalCorrection();
                } break;
                default: break;
            }
        });

        auto atFrequency{[&correctionEnabled, &speed, &speedCorrection, &steering, &steeringCorrection,
                          &od4, &direction, &distance, &lastCmd, &targetGroup, &DEV, &FREQ, &CID, &SAFETY_DISTANCE]() -> bool {
            FILE *file = fopen(DEV.c_str(), "rb");
            if (file != nullptr) {
                DS4Event *event = (DS4Event *)malloc(sizeof(DS4Event));
                while (!feof(file)) {
                    if (fread(event, sizeof(DS4Event), 1, file)) {
                        if ((event->type &0x0F) == 1) {
                            switch (event->id) {
                                case X:
                                    if (event->data == 1) {
                                        switch (lastCmd) {
                                            case Circle: {
                                                if (targetGroup < 0) {
                                                    std::cout << "[DS4] Stopping to act as follower!" << std::endl;
                                                    MARBLE::V2V::StopFollow sf;
                                                    od4.send(sf);
                                                } else if (targetGroup > 0) {
                                                    std::cout << "[DS4] Stopping to act as leader!" << std::endl;
                                                    MARBLE::V2V::StopLead sl;
                                                    od4.send(sl);
                                                }
                                            } break;
                                            case Triangle: {
                                                std::cout << "[DS4] Starting to follow group: " << targetGroup << std::endl;
                                                MARBLE::V2V::StartFollow sf;
                                                sf.groupId((uint8_t) targetGroup);
                                                od4.send(sf);
                                            } break;
                                        } lastCmd = 0; targetGroup = 0;
                                    } break;
                                case Circle:   if (event->data == 1) { lastCmd = Circle; targetGroup = 0; } break;
                                case Triangle: if (event->data == 1) { lastCmd = Triangle; targetGroup = 0; } break;
                                case Square:   break;
                                case L1:
                                    if (event->data == 1) {
                                        correctionEnabled = correctionEnabled ? 0 : 1;
                                        std::cout << "[DS4] Steering correction: " << (correctionEnabled ? "ON" : "OFF") << std::endl;
                                    }
                                    break;
                                case R1:
                                    if (event->data == 1) {
                                        direction = direction > 0 ? -3 : 1;
                                        std::cout << "[DS4] Switching gears to: " << (direction > 0 ? "FWD" : "REV") << std::endl;
                                    } break;
                                case L2:       break;
                                case R2:       break;
                                case Share:    break;
                                case Options:  break;
                                case PS:       break;
                                case LStick:   break;
                                case RStick:   break;
                                default:       break;
                            }
                        }
                        else if ((event->type &0x0F) == 2) {
                            switch (event->id) {
                                case LStickX: {
                                    steering = absToPercentage(event->data)*(-1);
                                    MARBLE::Steering::Instruction::GroundSteering steeringInstruction;
                                    steeringInstruction.groundSteering(steering);
                                    od4.send(steeringInstruction);
                                    std::cout << "[DS4] GroundSteeringInstruction: " << steeringInstruction.groundSteering() << std::endl;
                                    opendlv::proxy::GroundSteeringReading steeringReading;
                                    steeringReading.groundSteering(correctionEnabled ? steering +steeringCorrection : steering);
                                    od4.send(steeringReading);
                                    std::cout << "[DS4] GroundSteeringReading: " << steeringReading.groundSteering() << std::endl;
                                } break;
                                case LStickY: break;
                                case L2Y:     break;
                                case RStickX: break;
                                case RStickY: break;
                                case R2Y: {
                                    speed  = (1.f +absToPercentage(event->data)) /20.f;
                                    speed += speed >= 0.0025f ? 0.1f : 0.f;
                                    speed *= direction;
                                    speed = speed < 0 || distance > SAFETY_DISTANCE ? speed : 0;
                                    MARBLE::Steering::Instruction::PedalPosition pedalInstruction;
                                    pedalInstruction.pedalPosition(speed);
                                    od4.send(pedalInstruction);
                                    std::cout << "[DS4] PedalPositionInstruction: " << pedalInstruction.pedalPosition() << std::endl;
                                    opendlv::proxy::PedalPositionReading pedalPositionReading;
                                    pedalPositionReading.position(correctionEnabled ? speed +speedCorrection : speed);
                                    od4.send(pedalPositionReading);
                                    std::cout << "[DS4] PedalPositionReading: " << pedalPositionReading.position() << std::endl;
                                } break;
                                case PadX: {
                                    if (lastCmd == Triangle && event->data != 0) {
                                        targetGroup += event->data > 0 ? 1 : -1;
                                        if (targetGroup < 0) targetGroup = 13;
                                        else if (targetGroup > 13) targetGroup = 0;
                                    }
                                } break;
                                case PadY: {
                                    if (lastCmd == Circle) targetGroup += event->data;
                                } break;
                                default:      break;
                            }
                        }
                    }
                    else if (ferror(file)) std::cout << "[ERROR] FREAD failed with errno '" << errno << "'!" << std::endl;
                    else if (feof(file)) std::cout << "[ERROR] EOF reached!" << std::endl;
                }
                free(event);
            }
            else std::cout << "[ERROR] file at '" << DEV << "' cannot be accessed!" << std::endl;
            return true;
        }};
        od4.timeTrigger(FREQ, atFrequency);
    }
    return retVal;
}

float absToPercentage(int16_t data) {
    return (float)(abs(data)/MAX *(data >= 0.f ? 1.f : -1.f));
}