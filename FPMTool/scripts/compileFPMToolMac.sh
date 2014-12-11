#! /bin/bash

g++ -lboost_regex-mt -L/opt/local/lib -I /opt/local/include/ -o fpmTool ../src/main.cpp ../src/FPMUnit.cpp ../src/FPMUnitError.cpp ../src/FileHandler.cpp 
