#include "backend.h"
#include <iostream>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
//#include "RemoteControlMessages.hpp"
#include "../build/uimessages.hpp"


BackEnd::BackEnd(QObject *parent) : QObject(parent){
}

void BackEnd::moveW(){
    //SteeringInstruction ins;
    //ins.pedalPosition(0.5);
    //ins.steeringAngle(0);
    //std::cout << "sending " << ins.pedalPosition() << " / " << ins.steeringAngle() << " ..." << std::endl;
    cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    if(od4.isRunning()){
        UIMessage msg;
        msg.v2vgroup(0);
        msg.direction("w");
        od4.send(msg);
    }
    //std::cout << "moved! - W" << std::endl;

}
void BackEnd::moveA(){
    cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    if(od4.isRunning()){
        UIMessage msg;
        msg.v2vgroup(0);
        msg.direction("a");
        od4.send(msg);
    }
    //std::cout << "moved! - A" << std::endl;
}
void BackEnd::moveS(){
    cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    if(od4.isRunning()){
        UIMessage msg;
        msg.v2vgroup(0);
        msg.direction("s");
        od4.send(msg);
    }
   // std::cout << "moved! - S" << std::endl;
}
void BackEnd::moveD(){
    cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

    if(od4.isRunning()){
        UIMessage msg;
        msg.v2vgroup(0);
        msg.direction("d");
        od4.send(msg);
    }
    //std::cout << "moved! - D" << std::endl;
}

void BackEnd::stopX(){
  cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

  if(od4.isRunning()){
      UIMessage msg;
      msg.v2vgroup(0);
      msg.direction("x");
      od4.send(msg);
  }
}
void BackEnd::stopZ(){
  cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

  if(od4.isRunning()){
      UIMessage msg;
      msg.v2vgroup(0);
      msg.direction("z");
      od4.send(msg);
  }
}

void BackEnd::stop(){
  cluon::OD4Session od4(222, [](cluon::data::Envelope /*&&envelope*/) noexcept {});

  if(od4.isRunning()){
      UIMessage msg;
      msg.v2vgroup(0);
      msg.direction("x");
      od4.send(msg);
  }
    std::cout << "stopping" << std::endl;
}


void BackEnd::startV2V(){
    std::cout << "started v2v" << std::endl;
}
void BackEnd::stopV2V(){
    std::cout << "stopped v2v" << std::endl;
}
bool BackEnd::connection(int n){
    //Connection with group number n, if connection is succesful, return true, otherwise return false
    return true;
}
bool BackEnd::group1(){
    if (connection(1)){
        std::cout << "group1 connected" << std::endl;
        return true;
    }
    return false;
}
bool BackEnd::group2(){
    if (connection(2)){
        std::cout << "group2 connected" << std::endl;
        return true;
    }
    return false;
}
bool BackEnd::group3(){
    if (connection(3)){
        std::cout << "group3 connected" << std::endl;
        return true;
    }    return false;
}
bool BackEnd::group4(){
    if (connection(4)){
        std::cout << "group4 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group5(){
    if (connection(5)){
        std::cout << "group5 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group6(){
    if (connection(6)){
        std::cout << "group6 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group7(){
    if (connection(7)){
        std::cout << "group7 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group8(){
    if (connection(8)){
        std::cout << "group8 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group9(){
    if (connection(9)){
        std::cout << "group9 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group10(){
    if (connection(10)){
        std::cout << "group10 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group11(){
    if (connection(11)){
        std::cout << "group11 connected" << std::endl;
        return true;
    }return false;
}
bool BackEnd::group12(){
    if (connection(12)){
        std::cout << "group12 connected" << std::endl;
        return true;
    }return false;
}
