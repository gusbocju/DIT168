#include "backend.h"
#include <iostream>
BackEnd::BackEnd(QObject *parent) : QObject(parent)
{

}

void BackEnd::moveW(){
    std::cout << "moved! - W" << std::endl;
}
void BackEnd::moveA(){
    std::cout << "moved! - A" << std::endl;
}
void BackEnd::moveS(){
    std::cout << "moved! - S" << std::endl;
}
void BackEnd::moveD(){
    std::cout << "moved! - D" << std::endl;
}


