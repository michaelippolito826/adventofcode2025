# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(AOC1 VERSION 1.0)

# Set the C++ standard (use at least C++11 for modern C++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Find the cURL library
# This command searches the system for the necessary cURL headers and library files.
find_package(CURL REQUIRED)

# 2. Add an executable target
# Replace 'main.cpp' with the name of your source file.
add_executable(AOC1 main.cpp)

# 3. Link the executable to the cURL library
# This tells the linker to include the code from libcurl when building the executable.
target_link_libraries(AOC1 PRIVATE CURL::libcurl)