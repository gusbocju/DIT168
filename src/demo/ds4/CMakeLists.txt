cmake_minimum_required(VERSION 3.5)

project(MARBLE.DS4.Demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package(libcluon REQUIRED)
include_directories(SYSTEM ${CLUON_INCLUDE_DIRS})

include_directories(SYSTEM ${CMAKE_BINARY_DIR})
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} ${CLUON_LIBRARIES})

# enable_testing()
# add_executable(${PROJECT_NAME}-Runner ${CMAKE_CURRENT_SOURCE_DIR}/[...].cpp ${CMAKE_BINARY_DIR}/[...].cpp)
# target_link_libraries(${PROJECT_NAME}-Runner ${CLUON_LIBRARIES})

# add_test(NAME ${PROJECT_NAME}-Runner COMMAND ${PROJECT_NAME}-Runner)
