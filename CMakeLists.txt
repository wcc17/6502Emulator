cmake_minimum_required(VERSION 3.2)
project(6502emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(6502emulator ${SOURCE_FILES} instructions.h instructions.c util.h util.c 6502.h 6502.c )