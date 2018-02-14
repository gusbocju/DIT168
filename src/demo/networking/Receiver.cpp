//
// Created by Julian Bock on 2018-02-14.
//

#include <chrono>
#include <iostream>

#include "cluon/UDPSender.hpp"
#include "cluon/UDPReceiver.hpp"

int main(int /*argc*/, char** /*argv*/) {

    cluon::UDPReceiver receiver("225.0.0.111", 1236,
                                [](std::string &&data, std::string &&/*from*/,
                                   std::chrono::system_clock::time_point &&/*timepoint*/) noexcept {
                                    std::cout << "Received " << data.size() << " bytes." << std::endl;
                                });

    using namespace std::literals::chrono_literals;
    while (receiver.isRunning()) {
        std::this_thread::sleep_for(1s);
    }
}