//
// Created by Julian Bock on 2018-02-14.
//

#include <chrono>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"

cluon::OD4Session *internal, *external;

using namespace std::chrono;
auto last = std::chrono::high_resolution_clock::now();

int main(int /*argc*/, char **/*argv*/) {
     internal = new cluon::OD4Session(111, [](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
            std::cout << "GroundSteering sent! " << receivedMsg.groundSteering() << std::endl;
        }
        else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
            std::cout << "PedalPosition sent! " << receivedMsg.position() << std::endl;
        }
    });

    external = new cluon::OD4Session(112, [](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == 2001) {
            std::cout << "SteeringInstruction received!" << std::endl;
            SteeringInstruction ins = cluon::extractMessage<SteeringInstruction>(std::move(envelope));
            opendlv::proxy::GroundSteeringReading msgSteering;
            opendlv::proxy::PedalPositionReading msgPedal;
            msgSteering.groundSteering(ins.steeringAngle());
            msgPedal.position(ins.pedalPosition());
            internal->send(msgSteering);
            internal->send(msgPedal);
            last = std::chrono::high_resolution_clock::now();
        }
    });

    while (1) {
        auto now = std::chrono::high_resolution_clock::now();
        auto dur = now -last;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        if (ms >= 3000) {
            SteeringInstruction brake;
            brake.pedalPosition(0);
            brake.steeringAngle(0);
            external->send(brake);
            last = std::chrono::high_resolution_clock::now();
        }
    }
}