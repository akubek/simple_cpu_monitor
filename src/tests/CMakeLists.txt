add_executable(TestUtil test_util.c)
target_link_libraries(TestUtil PUBLIC CPUMonUtil)
add_test(NAME cpumon.testUtil COMMAND TestUtil)

add_executable(TestPercQ test_perc_q.c)
target_link_libraries(TestPercQ  PUBLIC CPUPercQueue)
add_test(NAME cpumon.testCPUPercQueue COMMAND TestPercQ)

add_executable(TestLogQ test_log_q.c)
target_link_libraries(TestLogQ  PUBLIC LogQueue)
add_test(NAME cpumon.testLogQueue COMMAND TestLogQ)