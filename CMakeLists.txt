cmake_minimum_required(VERSION 3.24)
project(compnet_lab1)

set(CMAKE_CXX_STANDARD 14)

include_directories(5)

add_executable(compnet_lab1 lab2/client/5.cpp)
if(MINGW)
    target_link_libraries(compnet_lab1 ws2_32)
endif()