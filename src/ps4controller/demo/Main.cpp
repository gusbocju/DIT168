//
// Created by julian on 4/11/18.
//

#include "Main.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

typedef struct {
    char timestamp_b0;
    char timestamp_b1;
    char timestamp_b2;
    char timestamp_b3;
    char data_b0;
    char data_b1;
    char type;
    char id;
} PS4ControllerEvent;

int main()
{
    while (true) {
        FILE *file = fopen("/dev/input/js0", "rb");
        fseek(file, 0L, SEEK_END);
        long size = ftell(file);
        fseek(file, 0L, SEEK_SET);
        if (size >= 7) {
            PS4ControllerEvent *event = (PS4ControllerEvent *) malloc(sizeof(PS4ControllerEvent));
            while (!feof(file)) {
                if (fread(event, sizeof(PS4ControllerEvent), 1, file))
                    std::cout << "event.Type: " << event->type << " / event.Id: " << event->id << std::endl;
            }
            usleep(1000);
        }

    }
}