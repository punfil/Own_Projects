#! /bin/bash

g++ main.cpp hash_function.cpp -Wall -Wextra -Wpedantic -O3 -fopenmp -march=native -o main
g++ test.cpp hash_function.cpp -Wall -Wextra -Wpedantic -O3 -fopenmp -march=native -o test