cmake_minimum_required(VERSION 3.5)

project(MARBLE.SafetyCheck)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/SafetyCheckMessages.cpp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND cluon-msc --cpp-sources --cpp-add-include-file=SafetyCheckMessages.hpp --out=${CMAKE_BINARY_DIR}/SafetyCheckMessages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/SafetyCheckMessages.odvd
        COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/SafetyCheckMessages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/SafetyCheckMessages.odvd
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/SafetyCheckMessages.odvd)

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}.Main Main.cpp ${CMAKE_BINARY_DIR}/SafetyCheckMessages.cpp)
target_link_libraries(${PROJECT_NAME}.Main ${CLUON_LIBRARIES})

add_executable(${PROJECT_NAME}.RemoteControl RemoteControl.cpp ${CMAKE_BINARY_DIR}/SafetyCheckMessages.cpp)
target_link_libraries(${PROJECT_NAME}.RemoteControl ${CLUON_LIBRARIES})

# enable_testing()
# add_executable(${PROJECT_NAME}-Runner ${CMAKE_CURRENT_SOURCE_DIR}/[...].cpp ${CMAKE_BINARY_DIR}/[...].cpp)
# target_link_libraries(${PROJECT_NAME}-Runner [...])

# add_test(NAME ${PROJECT_NAME}-Runner COMMAND ${PROJECT_NAME}-Runner)