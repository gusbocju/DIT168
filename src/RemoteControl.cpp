//
// Created by julian on 3/7/18.
//

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"

int main(int /*argc*/, char** /*argv*/) {
    cluon::OD4Session od4(112, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    bool fwd;
    while (1) {
        SteeringInstruction ins;
        switch (std::getchar()) {
            case 'w': ins.pedalPosition(50); ins.steeringAngle(45); fwd = true; break;
            case 'a': ins.pedalPosition(fwd ? 25 : -25); ins.steeringAngle(0); break;
            case 's': ins.pedalPosition(-50); ins.steeringAngle(45); fwd = false; break;
            case 'd': ins.pedalPosition(fwd ? 25 : -25); ins.steeringAngle(90); break;
            default:  ins.pedalPosition(0); ins.steeringAngle(45); break;
        } od4.send(ins);
    }
}
