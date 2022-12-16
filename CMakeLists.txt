cmake_minimum_required(VERSION 3.21)
project(idz4)

set(CMAKE_CXX_STANDARD 17)

add_executable(idz4 main.cpp Visitor.cpp Visitor.h)
