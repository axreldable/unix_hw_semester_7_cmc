cmake_minimum_required(VERSION 3.12)
project(wc C)

set(CMAKE_C_STANDARD 99)

add_library(wc wc.c wc.h)