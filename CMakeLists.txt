# Since we really really like JetBrains and enjoy working with CLion, this file
# is required to support different (technically independent) CMakeLists.txt in
# our subdirectories...

cmake_minimum_required(VERSION 3.5)

FIND_PROGRAM(APK apk)

# Add the path to any emerging (sub-)project to the list below:

if(NOT APK)
    add_subdirectory(src/DS4)
    add_subdirectory(src/V2V)
    add_subdirectory(src/IMU)
    add_subdirectory(src/demo/ds4)
    add_subdirectory(src/demo/networking)
    add_subdirectory(src/demo/safety-check)
elseif(APK)
    add_subdirectory(src/V2V)
    add_subdirectory(src/demo/ds4)
    add_subdirectory(src/demo/networking)
    add_subdirectory(src/demo/safety-check)
endif()