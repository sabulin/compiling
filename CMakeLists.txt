cmake_minimum_required(VERSION 3.23)
project(mylab2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(mylab2
        GrammaticalAnalysis.cpp
        GrammaticalAnalysis.h
        LexicalAnalysis.cpp
        LexicalAnalysis.h
        main.cpp
        datastructure.h)
