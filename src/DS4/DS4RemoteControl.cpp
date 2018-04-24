//
// Created by Julian Bock on 2018-04-12.
//

#include "DS4RemoteControl.h"

int main(int argc, char** argv)
{
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("dev") || 0 == commandlineArguments.count("freq") || 0 == commandlineArguments.count("cid")) {
        std::cerr << argv[0] << " reads (wireless) controller input from the specified path and relays it to components on the BeagleBone."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --dev=<path toController> --freq=<int pollingRate> --cid=<OD4Session toComponents>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --dev=/dev/input/js0 --freq=100 --cid=111" << std::endl;
        retVal = 1;
    } else {
        const std::string DEV = commandlineArguments["dev"];
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);

        float direction = 1.f;
        int lastCmd = 0, targetGroup = 0;
        cluon::OD4Session od4(CID, [](cluon::data::Envelope /*&&envelope*/) noexcept {});
        auto atFrequency{[&od4, &direction, &lastCmd, &targetGroup, &DEV, &FREQ, &CID]() -> bool {
            FILE *file = fopen(DEV.c_str(), "rb");
            if (file != nullptr) {
                DS4Event *event = (DS4Event *)malloc(sizeof(DS4Event));
                while (!feof(file)) {
                    if (fread(event, sizeof(DS4Event), 1, file)) {
                        if ((event->type &0x0F) == 1) {
                            switch (event->id) {
                                case X:
                                    switch (lastCmd) {
                                        case Circle: {
                                            if (targetGroup > 0) {
                                                std::cout << "[DS4] issuing StopFollow..." << std::endl;
                                                MARBLE::StopFollow sf;
                                                od4.send(sf);
                                            } else if (targetGroup < 0) {
                                                std::cout << "[DS4] issuing StopLead..." << std::endl;
                                                MARBLE::StopLead sl;
                                                od4.send(sl);
                                            }
                                        } break;
                                        case Triangle: {
                                            std::cout << "[DS4] issuing StartFollow with groupId '"
                                                         << targetGroup << "'..." << std::endl;
                                            MARBLE::StartFollow sf;
                                            sf.groupId((uint8_t)targetGroup);
                                            od4.send(sf);
                                        } break;
                                    }; lastCmd = 0; targetGroup = 0; break;
                                case Circle:   lastCmd = Circle; targetGroup = 0; break;
                                case Triangle: lastCmd = Triangle; targetGroup = 0; break;
                                case Square:   break;
                                case L1:       break;
                                case R1:       std::cout << "[DS4] switching gears..." << std::endl;
                                               direction = direction > 0 ? -1 : 1; break;
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
                                    opendlv::proxy::GroundSteeringReading steeringReading;
                                    steeringReading.groundSteering(absToPercentage(event->data)*(-1));
                                    od4.send(steeringReading);
                                    std::cout << "[DS4] sending new GroundSteeringReading: " << steeringReading.groundSteering() << std::endl;
                                } break;
                                case LStickY: break;
                                case L2Y:     break;
                                case RStickX: break;
                                case RStickY: break;
                                case R2Y: {
                                    float speed = 0;
                                    speed  = (1.f +absToPercentage(event->data)) /8.f;
                                    speed += speed >= 0.025f ? 0.1f : 0.f;
                                    speed *= direction;
                                    opendlv::proxy::PedalPositionReading pedalPositionReading;
                                    pedalPositionReading.position(speed);
                                    od4.send(pedalPositionReading);
                                    std::cout << "[DS4] sending new PedalPositionReading: " << pedalPositionReading.position() << std::endl;
                                } break;
                                case PadX: {
                                    if (lastCmd == Triangle) {
                                        targetGroup += event->data;
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