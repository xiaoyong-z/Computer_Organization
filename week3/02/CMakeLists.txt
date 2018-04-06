cmake_minimum_required(VERSION 3.9)
project(02)

set(CMAKE_CXX_STANDARD 11)

add_executable(02
        bit_ops.c
        lfsr.c
        vector-test.c
        vector.c
        vector.h)
