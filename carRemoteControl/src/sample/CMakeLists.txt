cmake_minimum_required(VERSION 3.5)
project(MARBLE)
find_package(JNI)

if(JNI_FOUND)
    message(STATUS "JNI DETECTED!")
    message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
else()
    message(WARNING "CRY MORTAL")
endif()

add_executable(${PROJECT_NAME}.Controller ${CMAKE_CURRENT_SOURCE_DIR}/Controller.cpp)
target_link_libraries(${PROJECT_NAME}.Controller)

#include_directories(/usr/lib/jvm/java-8-oracle/include/)
#include_directories(/usr/lib/jvm/java-8-oracle/include/linux/)
