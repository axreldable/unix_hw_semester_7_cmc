cmake_minimum_required(VERSION 3.12)
project(tail C)

set(CMAKE_C_STANDARD 99)

add_library(tail tail.c tail.h)