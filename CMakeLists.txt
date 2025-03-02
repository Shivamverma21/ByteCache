cmake_minimum_required(VERSION 3.29)
project(ByteCache)

set(CMAKE_CXX_STANDARD 20)

add_executable(ByteCache main.cpp
        storage.cpp
        server.cpp
)
target_link_libraries(ByteCache ws2_32)

add_executable(Clients Clients/client1.cpp)
target_link_libraries(Clients ws2_32)
