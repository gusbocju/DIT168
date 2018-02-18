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

    cluon::OD4Session od4(111,
      [](cluon::data::Envelope &&envelope) noexcept {
          if (envelope.dataType() == 1001) {
              TextMessage receivedMsg = cluon::extractMessage<TextMessage>(std::move(envelope));
              std::cout << "Received '" << receivedMsg.text() << "' from '" << receivedMsg.sender() << "'." << std::endl;
          }
      });

    using namespace std::literals::chrono_literals;
    while (od4.isRunning()) {
        std::this_thread::sleep_for(1s);
    }
}
