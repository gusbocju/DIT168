//
// Created by Julian Bock on 2018-02-14.
//

#include <chrono>
#include <iostream>

#include "cluon/Receiver.hpp"

int main(int /*argc*/, char** /*argv*/) {

    cluon::UDPSender sender{"225.0.0.111", 1236};
    sender.send("Hello World!");
}