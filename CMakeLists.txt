cmake_minimum_required(VERSION 3.29)
project(ByteCache)

set(CMAKE_CXX_STANDARD 20)

add_executable(ByteCache main.cpp
        Storage.cpp)
