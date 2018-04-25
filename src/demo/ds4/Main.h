//
// Created by julian on 4/11/18.
//

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include "cluon/OD4Session.hpp"

#define MAX  32767.0

// UBUNTU
/*
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
} DS4Buttons;

typedef enum {
    LStickX = 0,
    LStickY = 1,
    L2Y = 2,
    RStickX = 3,
    RStickY = 4,
    R2Y = 5,
    PadX = 6,
    PadY = 7
} DS4Axis; */

// DEBIAN

typedef enum {
    X = 1,
    Circle = 2,
    Triangle = 3,
    Square = 0,
    L1 = 4,
    R1 = 5,
    L2 = 6,
    R2 = 7,
    Share = 8,
    Options = 9,
    PS = 12,
    LStick = 10,
    RStick = 11
} DS4Buttons;

typedef enum {
    LStickX = 0,
    LStickY = 1,
    L2Y = 3,
    RStickX = 2,
    RStickY = 5,
    R2Y = 4,
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

class Main {

};

#endif //PROJECT_MAIN_H
