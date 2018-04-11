# Since we really really like JetBrains and enjoy working with CLion, this file
# is required to support different (technically independent) CMakeLists.txt in
# our subdirectories...

cmake_minimum_required(VERSION 3.5)

# Add the path to any emerging (sub-)project to the list below:

# add_subdirectory(src/)
# add_subdirectory(src/networking/demo)
add_subdirectory(src/ps4controller/demo)