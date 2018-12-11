cmake_minimum_required(VERSION 3.12)
project(sed C)

set(CMAKE_C_STANDARD 99)

add_library(sed sed.c sed.h)