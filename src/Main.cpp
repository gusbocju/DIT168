//
// Created by Julian Bock on 2018-02-14.
//

#include <chrono>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "RemoteControlMessages.hpp"

long last_instruction_timestamp = 0;
cluon::OD4Session *internal, *external;


int main(int /*argc*/, char **/*argv*/) {
     internal = new cluon::OD4Session(111, [](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
            std::cout << "GroundSteering sent! " << receivedMsg.groundSteering() << std::endl;
        }
        else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
            std::cout << "PedalPosition sent!" << receivedMsg.position() << std::endl;
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
        }
    });

    while (1) {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (now.time_since_epoch().count() -last_instruction_timestamp > 5000) {
            SteeringInstruction brake;
            brake.pedalPosition(0);
            brake.steeringAngle(0);
            external->send(brake);
        }
        last_instruction_timestamp = now.time_since_epoch().count();
    }
}