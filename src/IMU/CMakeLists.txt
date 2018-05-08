cmake_minimum_required(VERSION 3.5)

project(MARBLE.IMU)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/cmake-3.5/Modules/")

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/IMUMessages.cpp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND cluon-msc --cpp-sources --cpp-add-include-file=IMUMessages.hpp --out=${CMAKE_BINARY_DIR}/IMUMessages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/IMUMessages.odvd
        COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/IMUMessages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/IMUMessages.odvd
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IMUMessages.odvd)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/DS4Messages.cpp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND cluon-msc --cpp-sources --cpp-add-include-file=DS4Messages.hpp --out=${CMAKE_BINARY_DIR}/DS4Messages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/DS4Messages.odvd
        COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/DS4Messages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/DS4Messages.odvd
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/DS4Messages.odvd)

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} IMU.cpp MPU9250Device.cpp ${CMAKE_BINARY_DIR}/IMUMessages.cpp ${CMAKE_BINARY_DIR}/DS4Messages.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})

# enable_testing()
# add_executable(${PROJECT_NAME}-Runner ${CMAKE_CURRENT_SOURCE_DIR}/[...].cpp ${CMAKE_BINARY_DIR}/[...].cpp)
# target_link_libraries(${PROJECT_NAME}-Runner [...])

# add_test(NAME ${PROJECT_NAME}-Runner COMMAND ${PROJECT_NAME}-Runner)