cmake_minimum_required(VERSION 3.6)
project(parser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp json_parser.cpp json_parser.h Automaton.cpp Automaton.h)
add_executable(parser ${SOURCE_FILES})