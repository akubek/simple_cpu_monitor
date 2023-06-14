add_library(CPUStatQueue cpustat_queue.c)
add_library(CPUPercQueue cpuperc_queue.c)
add_library(LogQueue log_queue.c)
target_link_libraries(LogQueue PUBLIC CPUMonUtil)