cmake_minimum_required(VERSION 3.10)
project(PythonGraphModule)
find_package(Python3 REQUIRED)

add_executable(PythonGraphModuleExec systemmodule.c)
