cmake_minimum_required(VERSION 3.12)
project(strings C)

set(CMAKE_C_STANDARD 99)

add_library(strings strings.c strings.h)