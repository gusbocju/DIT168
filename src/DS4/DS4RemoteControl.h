//
// Created by Julian Bock on 2018-04-12.
//

#ifndef PROJECT_DS4REMOTECONTROL_H
#define PROJECT_DS4REMOTECONTROL_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include "cluon/OD4Session.hpp"
#include "RemoteControlMessages.hpp"

#define MAX  32767.0

typedef enum {
    X = 0,
    Circle = 1,
    Triangle = 2,
    Square = 3,
    L1 = 4,
    R1 = 5,
    L2 = 6,
    R2 = 7,
    Share = 8,
    Options = 9,
    PS = 10,
    LStick = 11,
    RStick = 12
} DS4Button;

typedef enum {
    LStickX = 0,
    LStickY = 1,
    L2Y = 3,            // RStickX
    RStickX = 2,        // L2
    RStickY = 5,        // R2
    R2Y = 4,            // RStickY
    PadX = 6,
    PadY = 7
} DS4Axis;

typedef struct {
    uint32_t timestamp;
    int16_t  data;
    uint8_t  type;
    uint8_t  id;
} DS4Event;

float absToPercentage(int16_t data);

#endif //PROJECT_DS4REMOTECONTROL_H
