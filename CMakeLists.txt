# Since we really really like JetBrains and enjoy working with CLion, this file
# is required to support different (technically independent) CMakeLists.txt in
# our subdirectories...

cmake_minimum_required(VERSION 3.5)

find_program(LSB_RELEASE lsb_release)
execute_process(COMMAND ${LSB_RELEASE} -is
                OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
                OUTPUT_STRIP_TRAILING_WHITESPACE)

# Add the path to any emerging (sub-)project to the list below:

if(${LSB_RELEASE_ID_SHORT} STREQUAL "Alpine")
    add_subdirectory(src/DS4)
    add_subdirectory(src/V2V)
    add_subdirectory(src/demo/ds4)
    add_subdirectory(src/demo/networking)
    add_subdirectory(src/demo/safety-check)
else()
    add_subdirectory(src/DS4)
    add_subdirectory(src/V2V)
    add_subdirectory(src/IMU)
    add_subdirectory(src/demo/ds4)
    add_subdirectory(src/demo/networking)
    add_subdirectory(src/demo/safety-check)
endif()