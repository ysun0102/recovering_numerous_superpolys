#!/bin/bash

PRESOLVE_LEVEL=-1

g++ -march=native -O3 -std=c++17 src/oficial_milptrivium.cpp -o recover_official -DPRESOLVE=${PRESOLVE_LEVEL} -I/${GUROBI_HOME}/include/ -L/${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi110 -lm 

./recover_official 0 1 2 4 6 8 10 11 12 13 15 17 19 21 23 25 26 28 29 30 31 32 34 36 38 40 43 45 47 49 51 53 54 55 58 60 62 64 66 67 69 71 73 75 77 79 | tee presolve_level${PRESOLVE_LEVEL}.log

