//
// Created by julian on 3/7/18.
//

#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"
#include "../build/uimessages.hpp"

std::string control="nothing";

int main(int argc, char** argv) {

    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid") /*||
            0 == commandlineArguments.count("gid") */) {
        std::cerr << argv[0] << " sends (not just) steering instructions to the BeagleBone."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4Session toBeagleBone> /*--gid=<OD4Session to the UI>*/"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111 /*--gid=222*/" << std::endl;
        retVal = 1;
    }
    else{
        //std::cout << "help" << std::endl;
        uint16_t const CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
/*        uint16_t const GID = (uint16_t) std::stoi(commandlineArguments["gid"]);*/
        uint16_t const GID = 222;
        SteeringInstruction ins;
        ins.pedalPosition(0);
        ins.steeringAngle(0);
        cluon::OD4Session od41(CID, [](cluon::data::Envelope /*&&envelope*/) noexcept {});
        cluon::OD4Session od42(GID, [&od41, &ins](cluon::data::Envelope &&envelope) noexcept {

            UIMessage msg = cluon::extractMessage<UIMessage>(std::move(envelope));
            std::cout << "direction = " << msg.direction() << ", group # = " << msg.v2vgroup() << std::endl;

            switch (msg.direction().at(0)) {
                case 'w':   ins.pedalPosition(0.15);
                            break;
                case 'a':   ins.steeringAngle(45);
                            break;
                case 's':   ins.pedalPosition(-0.45);
                            break;
                case 'd':   ins.steeringAngle(-45);
                            break;
                case 'z':   // reset pedal position
                            ins.pedalPosition(0);
                            break;
                case 'x':   // reset steering angle
                            ins.steeringAngle(0);
                            break;
                default:    ins.pedalPosition(0);
                            ins.steeringAngle(0);
            }
            std::cout << "sending " << ins.pedalPosition() << " / " << ins.steeringAngle() << " ..." << std::endl;
            od41.send(ins);

        });
        while(1){}

    }


    return 0;
}
