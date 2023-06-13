cmake_minimum_required(VERSION 3.22)
project(CPUMonitor)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../bin)

add_subdirectory(./threads)
add_subdirectory(./common)
add_subdirectory(./dataStructures)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})



add_executable(cpumon main.c)
target_link_libraries(cpumon PUBLIC Analyzer Printer Reader Logger Watchdog SleepFor CPUMonUtil CPUStatQueue CPUPercQueue LogQueue ${CURSES_LIBRARIES})
#target_compile_options(cpumon PRIVATE -Wall -Wextra)
add_compile_options(-Wall -Wextra -g)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)