//
// Created by julian on 4/11/18.
//

#include "Main.h"

int main(int argc, char** argv)
{
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("dev")) {
        std::cerr << argv[0] << " reads (wireless) controller input from the specified path."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --dev=<path toController>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --dev=/dev/input/js0" << std::endl;
        retVal = 1;
    } else {
        const std::string DEV = commandlineArguments["dev"];

        while (true) {
            FILE *file = fopen(DEV.c_str(), "rb");
            if (file != nullptr) {
                DS4Event *event = (DS4Event *)malloc(sizeof(DS4Event));
                while (!feof(file)) {
                    if (fread(event, sizeof(DS4Event), 1, file)) {
                        if ((event->type &0x0F) == 1) {
                            std::cout << "[DS4Button] ";
                            switch (event->id) {
                                case X: std::cout << "X " << event->data << std::endl; break;
                                case Circle: std::cout << "Circle " << event->data << std::endl; break;
                                case Triangle: std::cout << "Triangle " << event->data << std::endl; break;
                                case Square: std::cout << "Square " << event->data << std::endl; break;
                                case L1: std::cout << "L1 " << event->data << std::endl; break;
                                case R1: std::cout << "R1 " << event->data << std::endl; break;
                                case L2: std::cout << "L2 " << event->data << std::endl; break;
                                case R2: std::cout << "R2 " << event->data << std::endl; break;
                                case Share: std::cout << "Share " << event->data << std::endl; break;
                                case Options: std::cout << "Options " << event->data << std::endl; break;
                                case PS: std::cout << "PS " << event->data << std::endl; break;
                                case LStick: std::cout << "LStick " << event->data << std::endl; break;
                                case RStick: std::cout << "RStick " << event->data << std::endl; break;
                                default: std::cout << "¯\\_(ツ)_/¯" << std::endl;;
                            }
                        }
                        else if ((event->type &0x0F) == 2) {
                            std::cout << "[DS4Axis] " << std::fixed << std::setprecision(3);
                            switch (event->id) {
                                case LStickX: std::cout << "LStickX " << absToPercentage(event->data) << std::endl; break;
                                case LStickY: std::cout << "LStickY " << absToPercentage(event->data) << std::endl; break;
                                case L2Y: std::cout << "L2Y " << absToPercentage(event->data) << std::endl; break;
                                case RStickX: std::cout << "RStickX " << absToPercentage(event->data) << std::endl; break;
                                case RStickY: std::cout << "RStickY " << absToPercentage(event->data) << std::endl; break;
                                case R2Y: std::cout << "R2Y " << absToPercentage(event->data) << std::endl; break;
                                case PadX: std::cout << "PadX " << absToPercentage(event->data) << std::endl; break;
                                case PadY: std::cout << "PadY " << absToPercentage(event->data) << std::endl; break;
                                default: std::cout << "¯\\_(ツ)_/¯" << std::endl;;
                            }
                        }
                    }
                    else if (ferror(file)) std::cout << "[ERROR] FREAD failed with errno '" << errno << "'!" << std::endl;
                    else if (feof(file)) std::cout << "[ERROR] EOF reached!" << std::endl;
                }
                free(event);
            }
            else {
                std::cout << "[ERROR] file at '" << DEV << "' cannot be accessed!" << std::endl; break;
            }
        }
    }
    return retVal;
}

float absToPercentage(int16_t data) {
    return abs(data)/MAX *(data >= 0 ? 1 : -1);
}