cmake_minimum_required(VERSION 3.29)
project(ByteCache)

set(CMAKE_CXX_STANDARD 20)

add_executable(ByteCache main.cpp
        storage.cpp
        server.cpp
)
target_link_libraries(ByteCache ws2_32)

add_executable(Client1 Client1/client1.cpp)
target_link_libraries(Client1 ws2_32)

add_executable(Client2 Client2/client2.cpp)
target_link_libraries(Client2 ws2_32)

add_executable(Client3 Client3/client3.cpp)
target_link_libraries(Client3 ws2_32)
