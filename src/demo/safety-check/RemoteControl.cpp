//
// Created by julian on 3/7/18.
//

#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "SafetyCheckMessages.hpp"

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
        uint16_t const CID = (uint16_t) std::stoi(commandlineArguments["cid"]);

        cluon::OD4Session od4(CID, [](cluon::data::Envelope /*&&envelope*/) noexcept {});
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (1) {
            SteeringInstruction ins;
            std::string input;
            std::getline(std::cin, input);
            switch (input.at(0)) {
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
        }
        #pragma clang diagnostic pop
    }
}
