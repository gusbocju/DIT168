//
// Created by julian on 4/27/18.
//

#include "IMU.hpp"

int main(int argc, char **argv) {
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq") ||
        0 == commandlineArguments.count("dev") || 0 == commandlineArguments.count("steering_scale") ||
        0 == commandlineArguments.count("speed_offset") ) {
        std::cerr << argv[0] << " reads the IMU on the BeagleBoneBlue." << std::endl;
        std::cerr << "Usage:   " << argv[0]
                  << " --cid=<OD4Session components> --freq=<frequency> --dev=<device-node> --steering_scale=<gyro_offset_in_deg to max_steering_correction> speed_offset=<float>" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111 --freq=10 --dev=/dev/i2c-2 --steering_scale=30 --speed_offset=-0.6" << std::endl;
        retVal = 1;
    } else {
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);
        const std::string DEV = commandlineArguments["dev"];
        const float STEERING_SCALE = std::stof(commandlineArguments["steering_scale"]);
        const float SPEED_OFFSET = std::stof(commandlineArguments["speed_offset"]);

        float steeringInstruction = 0.f, speedInstruction = 0.f;
        std::shared_ptr<cluon::OD4Session> od4;
        std::shared_ptr<opendlv::proxy::miniature::MPU9250Device> imu;

        imu = std::make_shared<opendlv::proxy::miniature::MPU9250Device>(DEV);
        od4 = std::make_shared<cluon::OD4Session>(CID,
              [&steeringInstruction, &speedInstruction](cluon::data::Envelope &&envelope) noexcept {
            switch (envelope.ID()) {
                case 9010: {
                    MARBLE::Steering::Instruction::GroundSteering steering =
                            cluon::extractMessage<MARBLE::Steering::Instruction::GroundSteering>(std::move(envelope));
                    steeringInstruction = steering.groundSteering();
                } break;
                case 9011: {
                    MARBLE::Steering::Instruction::PedalPosition speed =
                            cluon::extractMessage<MARBLE::Steering::Instruction::PedalPosition>(std::move(envelope));
                    speedInstruction = speed.pedalPosition();
                } break;
                default: break;
            }
        });

        // Repeat at FREQ:
        auto atFrequency{[&od4, &imu, &steeringInstruction, &speedInstruction, &SPEED_OFFSET, STEERING_SCALE]() -> bool {
            // Read IMU:
            opendlv::proxy::AccelerationReading accelerationReading = imu->readAccelerometer();
            opendlv::proxy::AngularVelocityReading angularVelocityReading = imu->readGyroscope();
            // Share readings:
            od4->send(accelerationReading);
            od4->send(angularVelocityReading);
            // Share calculated corrections:
            MARBLE::Steering::Correction::PedalPosition pedalPosition;
            pedalPosition.pedalCorrection(speedInstruction +SPEED_OFFSET);
            float correction = fabsf(angularVelocityReading.angularVelocityZ()) /STEERING_SCALE;
            correction *= angularVelocityReading.angularVelocityZ() > 0 ? -1.f : 1.f;
            MARBLE::Steering::Correction::GroundSteering groundSteering;
            groundSteering.steeringCorrection(speedInstruction == 0 ? steeringInstruction +correction : steeringInstruction);
            return true;
        }};
        od4->timeTrigger(FREQ, atFrequency);
    }
    return retVal;
}