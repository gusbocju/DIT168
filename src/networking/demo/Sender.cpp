//
// Created by Julian Bock on 2018-02-14.
//

#include <cstdint>
#include <chrono>
#include <iostream>
#include <string>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

int main(int /*argc*/, char** /*argv*/) {

    cluon::OD4Session od4(111, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    std::string name, text;
    std::cout << "Please enter your name: ";
    std::getline(std::cin, name);
    std::cout << "Please enter your message: ";
    std::getline(std::cin, text);

    TextMessage msg;
    msg.sender(name);
    msg.text(text);
    od4.send(msg);

    return 0;
}
