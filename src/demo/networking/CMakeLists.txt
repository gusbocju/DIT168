cmake_minimum_required(VERSION 3.9)

project(DIT168.Demo.Networking)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

add_executable(
        ${PROJECT_NAME}.Sender
        ${CMAKE_CURRENT_SOURCE_DIR}/Sender.cpp )

add_executable(
        ${PROJECT_NAME}.Receiver
        ${CMAKE_CURRENT_SOURCE_DIR}/Receiver.cpp )