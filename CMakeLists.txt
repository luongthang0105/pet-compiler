cmake_minimum_required(VERSION 3.20)

# define project name
project(hydrogen)

# define C++ standard to be C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_executable(hydro src/main.cpp)

