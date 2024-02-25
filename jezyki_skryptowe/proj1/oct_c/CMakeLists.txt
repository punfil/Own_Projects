cmake_minimum_required(VERSION 3.10)
project(MyOct)

add_library(MyOctLib my_oct.c)

add_executable(OctTester my_oct_test.c)
target_link_libraries(OctTester PRIVATE MyOctLib)
