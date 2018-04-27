cmake_minimum_required(VERSION 3.5)

project(remote LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
find_package(libcluon REQUIRED)

include_directories(SYSTEM ${CLION_INCLUDE_DIRS})

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/RemoteControlMessages.cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND cluon-msc --cpp-sources --cpp-add-include-file=RemoteControlMessages.hpp --out=${CMAKE_BINARY_DIR}/RemoteControlMessages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/RemoteControlMessages.odvd
    COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/RemoteControlMessages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/RemoteControlMessages.odvd
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/RemoteControlMessages.odvd)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/uimessages.cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND cluon-msc --cpp-sources --cpp-add-include-file=uimessages.hpp --out=${CMAKE_BINARY_DIR}/uimessages.cpp ${CMAKE_CURRENT_SOURCE_DIR}/uimessages.odvd
    COMMAND cluon-msc --cpp-headers --out=${CMAKE_BINARY_DIR}/uimessages.hpp ${CMAKE_CURRENT_SOURCE_DIR}/uimessages.odvd
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/uimessages.odvd)


include_directories(SYSTEM ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}.Remote RemoteControl.cpp ${CMAKE_BINARY_DIR}/RemoteControlMessages.cpp ${CMAKE_BINARY_DIR}/uimessages.cpp)

target_link_libraries(${PROJECT_NAME}.Remote ${CLUON_LIBRARIES})

add_executable(${PROJECT_NAME}.Window main_window.cpp qml.qrc res_file.qrc
                                      backend.cpp backend.h
                                      ${CMAKE_BINARY_DIR}/uimessages.cpp)
target_link_libraries(${PROJECT_NAME}.Window ${CLUON_LIBRARIES} Qt5::Core Qt5::Quick)

add_executable(${PROJECT_NAME}.Main  Main.cpp
    ${CMAKE_BINARY_DIR}/RemoteControlMessages.cpp
    )
target_link_libraries(${PROJECT_NAME}.Main ${CLUON_LIBRARIES})

#target_link_libraries(${PROJECT_NAME}.Logic ${CLUON_LIBRARIES})

#target_link_libraries(${PROJECT_NAME}.Window Qt5::Core Qt5::Quick)
