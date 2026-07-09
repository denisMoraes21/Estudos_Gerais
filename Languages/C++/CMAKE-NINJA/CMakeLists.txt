cmake_minimum_required(VERSION 3.16)

project(CMAKE-NINJA)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(app_example
    src/main.cpp
    src/soma.cpp
)