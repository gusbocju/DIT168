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

        cluon::OD4Session od4(CID, [](cluon::data::Envelope /*&&envelope*/) noexcept {});
        auto atFrequency{[&od4, &DEV, &FREQ, &CID]() -> bool {
            FILE *file = fopen(DEV.c_str(), "rb");
            if (file != nullptr) {
                DS4Event *event = (DS4Event *)malloc(sizeof(DS4Event));
                while (!feof(file)) {
                    if (fread(event, sizeof(DS4Event), 1, file)) {
                        if ((event->type &0x0F) == 1) {
                            switch (event->id) {
                                case X: break;
                                case Circle: break;
                                case Triangle: break;
                                case Square: break;
                                case L1: break;
                                case R1: break;
                                case L2: break;
                                case R2: break;
                                case Share: break;
                                case Options: break;
                                case PS: break;
                                case LStick: break;
                                case RStick: break;
                                default:;
                            }
                        }
                        else if ((event->type &0x0F) == 2) {
                            switch (event->id) {
                                case LStickX: {
                                    std::cout << "[DS4Controller] LStickX" << std::endl;
                                    opendlv::proxy::GroundSteeringReading steeringReading;
                                    steeringReading.groundSteering(absToPercentage(event->data));
                                    od4.send(steeringReading);
                                    std::cout << "[DS4Controller] sending new GroundSteeringReading: " << steeringReading.groundSteering() << std::endl;
                                } break;
                                case LStickY: std::cout << "[DS4Controller] LStickY" << std::endl; break;
                                case L2Y: std::cout << "[DS4Controller] L2Y" << std::endl; break;
                                case RStickX: std::cout << "[DS4Controller] RStickX" << std::endl; break;
                                case RStickY: std::cout << "[DS4Controller] RStickY" << std::endl; break;
                                case R2Y: {
                                    std::cout << "[DS4Controller] R2Y" << std::endl;
                                    opendlv::proxy::PedalPositionReading pedalPositionReading;
                                    pedalPositionReading.position(absToPercentage(event->data));
                                    od4.send(pedalPositionReading);
                                    std::cout << "[DS4Controller] sending new PedalPositionReading: " << pedalPositionReading.position() << std::endl;
                                } break;
                                case PadX: std::cout << "[DS4Controller] PadX" << std::endl; break;
                                case PadY: std::cout << "[DS4Controller] PadY" << std::endl; break;
                                default:;
                            }
                        }
                    }
                    else if (ferror(file)) std::cout << "[ERROR] FREAD failed with errno '" << errno << "'!" << std::endl;
                    else if (feof(file)) std::cout << "[ERROR] EOF reached!" << std::endl;
                }
                free(event);
            }
            else std::cout << "[ERROR] file at '" << DEV << "' cannot be accessed!" << std::endl;
        }};
        od4.timeTrigger(FREQ, atFrequency);
    }
    return retVal;
}

float absToPercentage(int16_t data) {
    return abs(data)/MAX *(data >= 0 ? 1 : -1);
}