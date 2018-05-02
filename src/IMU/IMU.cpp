//
// Created by julian on 4/27/18.
//

#include "IMU.hpp"

int main(int argc, char **argv) {
    int retVal = 0;
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq") ||
        0 == commandlineArguments.count("dev")) {
        std::cerr << argv[0] << " reads the IMU on the BeagleBoneBlue." << std::endl;
        std::cerr << "Usage:   " << argv[0]
                  << " --cid=<OD4Session components> --freq=<frequency> --dev=<device-node>" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111 --freq=10 --dev=/dev/i2c-2" << std::endl;
        retVal = 1;
    } else {
        const uint16_t CID = (uint16_t) std::stoi(commandlineArguments["cid"]);
        const uint16_t FREQ = (uint16_t) std::stoi(commandlineArguments["freq"]);
        const std::string DEV = commandlineArguments["dev"];

        od4 = std::make_shared<cluon::OD4Session>(CID, [](cluon::data::Envelope &&envelope) noexcept {});
        imu = std::make_shared<opendlv::proxy::miniature::MPU9250Device>(DEV);

        // Repeat at FREQ:
        auto atFrequency{[&od4, &imu]() -> bool {
            // Read IMU:
            opendlv::proxy::AccelerationReading accelerationReading = imu->readAccelerometer();
            opendlv::proxy::AngularVelocityReading angularVelocityReading = imu->readGyroscope();
            // Share values via OD4:
            od4->send(accelerationReading);
            od4->send(angularVelocityReading);
            return true;
        }};
        od4->timeTrigger(FREQ, atFrequency);
    }
    return retVal;
}