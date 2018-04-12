//
// Created by julian on 4/11/18.
//

#include "Main.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
// #include <libexplain/ferror.h>

#define PATH "/dev/input/js1"
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
    L2Y = 2,
    RStickX = 3,
    RStickY = 4,
    R2Y = 5,
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

int main()
{
    while (true) {
        FILE *file = fopen(PATH, "r");
        if (file != nullptr) {
            DS4Event *event = (DS4Event *)malloc(sizeof(DS4Event));
            while (!feof(file)) {
                if (fread(event, sizeof(DS4Event), 1, file)) {
                    if ((event->type &0x0F) == 1) {
                        std::cout << "[DS4Button] ";
                        switch (event->id) {
                            case X: std::cout << "X " << event->data << std::endl; break;
                            case Circle: std::cout << "Circle " << event->data << std::endl; break;
                            case Triangle: std::cout << "Triangle " << event->data << std::endl; break;
                            case Square: std::cout << "Square " << event->data << std::endl; break;
                            case L1: std::cout << "L1 " << event->data << std::endl; break;
                            case R1: std::cout << "R1 " << event->data << std::endl; break;
                            case L2: std::cout << "L2 " << event->data << std::endl; break;
                            case R2: std::cout << "R2 " << event->data << std::endl; break;
                            case Share: std::cout << "Share " << event->data << std::endl; break;
                            case Options: std::cout << "Options " << event->data << std::endl; break;
                            case PS: std::cout << "PS " << event->data << std::endl; break;
                            case LStick: std::cout << "LStick " << event->data << std::endl; break;
                            case RStick: std::cout << "RStick " << event->data << std::endl; break;
                            default: std::cout << "¯\\_(ツ)_/¯" << std::endl;;
                        }
                    }
                    else if ((event->type &0x0F) == 2) {
                        std::cout << "[DS4Axis] " << std::fixed << std::setprecision(3);
                        switch (event->id) {
                            case LStickX: std::cout << "LStickX " << absToPercentage(event->data) << std::endl; break;
                            case LStickY: std::cout << "LStickY " << absToPercentage(event->data) << std::endl; break;
                            case L2Y: std::cout << "L2Y " << absToPercentage(event->data) << std::endl; break;
                            case RStickX: std::cout << "RStickX " << absToPercentage(event->data) << std::endl; break;
                            case RStickY: std::cout << "RStickY " << absToPercentage(event->data) << std::endl; break;
                            case R2Y: std::cout << "R2Y " << absToPercentage(event->data) << std::endl; break;
                            case PadX: std::cout << "PadX " << absToPercentage(event->data) << std::endl; break;
                            case PadY: std::cout << "PadY " << absToPercentage(event->data) << std::endl; break;
                            default: std::cout << "¯\\_(ツ)_/¯" << std::endl;;
                        }
                    }
                }
                else if (ferror(file)) std::cout << "[ERROR] FREAD failed with errno '" << ferror(file) << "'!" << std::endl;
                else if (feof(file)) std::cout << "[ERROR] EOF reached!" << std::endl;
            }
            free(event);
        }
        else {
            std::cout << "[ERROR] file at PATH cannot be accessed!" << std::endl; break;
        }
    }
}

float absToPercentage(int16_t data) {
    return abs(data)/MAX *(data >= 0 ? 1 : -1);
}