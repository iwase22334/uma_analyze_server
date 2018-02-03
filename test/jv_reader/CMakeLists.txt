cmake_minimum_required(VERSION 2.8)

add_definitions("-Wall -std=c++11")
include_directories(${CMAKE_SOURCE_DIR}/../include)

add_executable(JVRecordReader-ts JVRecordReader-ts.cpp)
add_executable(JVDataPool-ts JVDataPool-ts.cpp)