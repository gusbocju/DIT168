//
// Created by julian on 4/11/18.
//

#include "Main.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

int main()
{
    while (1) {
        std::ifstream ifs("/dev/input/js0");
        if (ifs.is_open())
            std::cout << ifs.rdbuf() << std::endl;
        usleep(1000);
    }
}