//
// Created by julian on 3/7/18.
//

#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"

bool key_pressed = false;


int main(int argc, char** argv) {
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid")) {
        std::cerr << argv[0] << " sends (not just) steering instructions to the BeagleBone."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4Session toBeagleBone>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111" << std::endl;
        retVal = 1;
    } else {

        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (1) {
            while(key_pressed){
                std::cout << "Key Pressed" << std::endl;
            }
        }
        #pragma clang diagnostic pop
    }

    void press(char button){
        key_pressed = true;
        cluon::OD4Session od4(112, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

        SteeringInstruction ins;

        switch (button) {
            case 'w':   ins.pedalPosition(0.5);
                ins.steeringAngle(0);   break;
            case 'a':   ins.pedalPosition(0.25);
                ins.steeringAngle(45);  break;
            case 's':   ins.pedalPosition(0);
                ins.steeringAngle(0);   break;
            case 'd':   ins.pedalPosition(0.25);
                ins.steeringAngle(-45); break;
            default:    ins.pedalPosition(0);
                ins.steeringAngle(0);
        }

        std::cout << "sending " << ins.pedalPosition() << " / " << ins.steeringAngle() << " ..." << std::endl;
        od4.send(ins);
        key_pressed = false;
    }

    // ideas on how to solve the thing bc its late and i can/t think
    // 1. methods which repeat the moving action redundantly
    // 2. global variable on buttons pressed being changed
}
