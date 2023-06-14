cmake_minimum_required(VERSION 3.22)
project(CPUMonitor)

include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../bin)

add_subdirectory(./threads)
add_subdirectory(./common)
add_subdirectory(./dataStructures)
add_subdirectory(./tests)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})



add_executable(cpumon main.c)
target_link_libraries(cpumon PUBLIC Analyzer Printer Reader Logger Watchdog CPUMonUtil CPUStatQueue CPUPercQueue LogQueue ${CURSES_LIBRARIES})

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(cpumon PRIVATE "-Weverything" "-Wno-padded" "-Wno-disabled-macro-expansion")
else()
    target_compile_options(cpumon PRIVATE "-Wall" "-Wextra")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)