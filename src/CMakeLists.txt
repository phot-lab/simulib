cmake_minimum_required(VERSION 3.21)
project(SimuLib-src)

add_library(Library STATIC tools.cpp init_gstate.cpp globals.cpp fft.cpp fiber.cpp)