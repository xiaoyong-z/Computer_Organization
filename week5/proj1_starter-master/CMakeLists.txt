cmake_minimum_required(VERSION 3.9)
project(proj1_starter_master)

set(CMAKE_CXX_STANDARD 11)

add_executable(proj1_starter_master
        input/combined.s
        input/comments.s
        input/imm.s
        input/labels.s
        input/p1_errors.s
        input/p2_errors.s
        input/pseudo.s
        input/simple.s
        log/ref/p1_errors_ref.txt
        log/ref/p2_errors_ref.txt
        out/ref/combined_ref.int
        out/ref/combined_ref.out
        out/ref/comments_ref.int
        out/ref/comments_ref.out
        out/ref/imm_ref.int
        out/ref/imm_ref.out
        out/ref/labels_ref.int
        out/ref/labels_ref.out
        out/ref/pseduo_ref.int
        out/ref/pseudo_ref.int
        out/ref/pseudo_ref.out
        out/ref/simple_ref.int
        out/ref/simple_ref.out
        src/tables.c
        src/tables.h
        src/translate.c
        src/translate.h
        src/translate_utils.c
        src/translate_utils.h
        src/utils.c
        src/utils.h
        assembler.c
        assembler.h
        Makefile
        run-valgrind
        test-assembler
        test_assembler.c
        test_output.txt)
