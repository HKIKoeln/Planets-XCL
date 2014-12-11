#! /bin/bash

g++ -I ../../../../../opt/xercesc/include/ -o comparator main.cpp ComparatorInterface.cpp ComparatorEngine.cpp ComparatorError.cpp Comparer.cpp ComparerMeasure.cpp ComparerMeasureDataRef.cpp ComparerMeasureValueSet.cpp ComparatorOutput.cpp ComparerResults.cpp DataTagset.cpp InputParams.cpp ComparisonConfigurationHandler.cpp ComparisonConfigurationData.cpp SaxInterface.cpp XCDLHandler.cpp XercesString.cpp XMLValidation.cpp XCDLIndexes.cpp XCDLIndexGenerator.cpp -L ../../../../../opt/xercesc/lib/ -lxerces-c 
