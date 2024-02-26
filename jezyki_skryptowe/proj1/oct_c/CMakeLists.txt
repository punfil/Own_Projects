cmake_minimum_required(VERSION 3.10)
project(MyOct)

add_library(MyOctLib my_oct.c)


add_executable(OctTester my_oct_test.c)
target_link_libraries(OctTester PRIVATE MyOctLib)

add_executable(OctTesterPerf my_oct_perf_test.c)
target_compile_definitions(OctTesterPerf PRIVATE -D_POSIX_C_SOURCE=199309L)
target_link_libraries(OctTesterPerf PRIVATE MyOctLib)
