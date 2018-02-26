cmake_minimum_required(VERSION 3.5)

project(MARBLE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)

# enable_testing()
# add_executable(${PROJECT_NAME}-Runner ${CMAKE_CURRENT_SOURCE_DIR}/[...].cpp ${CMAKE_BINARY_DIR}/[...].cpp)
# target_link_libraries(${PROJECT_NAME}-Runner [...])

# add_test(NAME ${PROJECT_NAME}-Runner COMMAND ${PROJECT_NAME}-Runner)