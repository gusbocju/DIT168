//
// Created by julian on 4/27/18.
//

#ifndef PROJECT_IMU_H
#define PROJECT_IMU_H

#include <iostream>

#include "cluon/OD4Session.hpp"
#include "cluon/UDPSender.hpp"
#include "cluon/UDPReceiver.hpp"
#include "cluon/Envelope.hpp"
#include "IMUMessages.hpp"
#include "MPU9250Device.h"

class IMU {

};

std::shared_ptr<cluon::OD4Session> od4;
std::shared_ptr<opendlv::proxy::miniature::MPU9250Device> imu;

#endif //PROJECT_IMU_H
