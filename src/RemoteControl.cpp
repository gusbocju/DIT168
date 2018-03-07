//
// Created by julian on 3/7/18.
//

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"

int main(int /*argc*/, char** /*argv*/) {
    cluon::OD4Session od4(112, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    while (1) {
        SteeringInstruction ins;
        std::string input;
        std::getline(std::cin, input);
        switch (input.at(0)) {
            case 'w': ins.pedalPosition(0.5); ins.steeringAngle(0); break;
            case 'a': ins.pedalPosition(0.25); ins.steeringAngle(-45); break;
            case 's': ins.pedalPosition(0); ins.steeringAngle(0); break;
            case 'd': ins.pedalPosition(0.25); ins.steeringAngle(45); break;
            default:  ins.pedalPosition(0); ins.steeringAngle(0);
        }
        std::cout << "sending " << ins.pedalPosition() << " / " << ins.steeringAngle() << " ..." << std::endl;
        od4.send(ins);
    }
}
