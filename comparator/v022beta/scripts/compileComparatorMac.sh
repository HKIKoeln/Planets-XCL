#! /bin/bash

g++ -lxerces-c -L/opt/local/lib -I /opt/local/include/ -o comparator ../src/main.cpp ../src/ComparatorInterface.cpp ../src/ComparatorError.cpp ../src/Comparer.cpp ../src/ComparerMeasure.cpp ../src/ComparatorOutput.cpp ../src/ComparerResults.cpp ../src/DataTagset.cpp ../src/InputParams.cpp ../src/PropertyTagset.cpp ../src/Request2InHandler.cpp ../src/Request2Input.cpp ../src/SaxInterface.cpp ../src/timestamp.cpp ../src/XCDLHandler.cpp ../src/XercesString.cpp ../src/XMLValidation.cpp 
