//
// Created by Julian Bock on 2018-02-14.
//

#include <cstdint>
#include <chrono>
#include <iostream>
#include <string>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "DemoMessages.hpp"

int main(int /*argc*/, char** /*argv*/) {

    cluon::OD4Session od4(111, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    std::string name, text;
    std::cout << "Please enter your name: ";
    std::getline(std::cin, name);
    std::cout << "Alright, you're all set to spam! ";

    while (1) {
      std::cout << ">> ";
      std::getline(std::cin, text);

      TextMessage msg;
      msg.sender(name);
      msg.text(text);
      od4.send(msg);
    }
}
