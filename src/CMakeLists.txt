cmake_minimum_required(VERSION 3.9)

project(DIT168)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_executable(
        ${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)