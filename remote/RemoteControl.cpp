//
// Created by julian on 3/7/18.
//

#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"
#include "remotecontrol.h"

std::string control="nothing";

void remote(uint16_t cid) {
        cluon::OD4Session od4(cid, [](cluon::data::Envelope /*&&envelope*/) noexcept {});
        std::cout << cid <<std::endl;
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (1) {
            SteeringInstruction ins;
            switch (control.at(0)) {
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

void press_a(){
    control="a";
}

void press_nothing(){
    control="nothing";
}
