//
// Created by Julian Bock on 2018-02-14.
//

#include <chrono>
#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "SafetyCheckMessages.hpp"

using namespace std::chrono;

int main(int argc, char **argv) {
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("internal") || 0 == commandlineArguments.count("external") ||
        0 == commandlineArguments.count("freq")) {
        std::cerr << argv[0] << " translates/ relays instructions from the RC to the respective component on the BeagleBone."
                  << std::endl;
        std::cerr << "Usage:   " << argv[0]
                  << " --freq=<frequency> --internal=<OD4Session components> --external=<OD4Session remoteControl>"
                  << std::endl;
        std::cerr << "Example: " << argv[0] << " --freq=10 --internal=111 --external=112" << std::endl;
        retVal = 1;
    } else {
        uint16_t const INTERNAL_CID = (uint16_t) std::stoi(commandlineArguments["internal"]);
        uint16_t const EXTERNAL_CID = (uint16_t) std::stoi(commandlineArguments["external"]);
        float const FREQ = std::stof(commandlineArguments["freq"]);

        std::shared_ptr<cluon::OD4Session> internal, external;
        auto last = std::chrono::high_resolution_clock::now();

        internal = std::make_shared<cluon::OD4Session>(INTERNAL_CID, [](cluon::data::Envelope &&envelope) noexcept {
            if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
                opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(
                        std::move(envelope));
                std::cout << "GroundSteering sent! " << receivedMsg.groundSteering() << std::endl;
            } else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
                opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(
                        std::move(envelope));
                std::cout << "PedalPosition sent! " << receivedMsg.position() << std::endl;
            }
        });

        external = std::make_shared<cluon::OD4Session>(EXTERNAL_CID, [&internal, &last](cluon::data::Envelope &&envelope) noexcept {
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

        auto atFrequency{[&external, &last]() -> bool {
            auto dur = std::chrono::high_resolution_clock::now() - last;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
            if (ms >= 1000) {
                SteeringInstruction brake;
                brake.pedalPosition(0);
                brake.steeringAngle(0);
                external->send(brake);
                last = std::chrono::high_resolution_clock::now();
            }
            return true;
        }};
        internal->timeTrigger(FREQ, atFrequency);
    }

    return retVal;
}