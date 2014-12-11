#! /bin/bash

g++ -I ../boost_1_34_1/ -o fpmTool ../src/main.cpp ../src/FPMUnit.cpp ../src/FPMUnitError.cpp ../src/FileHandler.cpp -L ../boost_1_34_1/libs/regex/build/gcc/ -lboost_regex-gcc-d-1_34
