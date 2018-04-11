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
        FILE *file = fopen("/dev/input/js0", "r");
        if (file != nullptr) {
            char *line;
            size_t len;
            ssize_t read;
            while ((read = getline(&line, &len, file)) != -1) {
                char type = line[6];
                char id = line[7];
                printf("type -> %x, id -> %x\n", type, id);
            }
            usleep(1000);
        }
        else break;
    }
}