cmake_minimum_required(VERSION 3.9)
project(hw1)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(hw1
        beargit.c
        beargit.h
        init_test
        main.c
        Makefile
        util.h)
