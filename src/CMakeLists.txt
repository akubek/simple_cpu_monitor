cmake_minimum_required(VERSION 3.22)
project(CPUMonitor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../bin)

add_executable(cpumon main.c)
