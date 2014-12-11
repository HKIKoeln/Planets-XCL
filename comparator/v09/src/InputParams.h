/*
    * This module is part of the XCL software system
    *
    * Written by  Volker Heydegger, Sebastian Beyl, Jan Schnasse,
    *             Manfred Thaller, Elona Weiper    *
    *             2007 - 2009
    *
    * Copyright (C)  2007 - 2009
    * Volker Heydegger, Sebastian Beyl, Jan Schnasse, Manfred Thaller, Elona Weiper
    *
    * This program is free software; you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation; either version 2 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
*/
/*****************************************************************************
InputParams.h

Description: stores input parameters and includes methods for handling them
*****************************************************************************/
#ifndef INPUTPARAMS_H
#define INPUTPARAMS_H

#include <string>
#include <list>

class ComparatorError;
class ComparisonConfiguration;

class InputParams
{
public:
       InputParams(ComparatorError *pError);
       ~InputParams();
       void parseArgs(int argc, char* argv[]);
       void printInputParams();
       void printHelp();
       void cleanUp();
       ComparatorError *pErr;
        //private:

       char* sourceXCDL;
       char* targetXCDL;
       char* cocoFile;
       std::string copraName;
       std::string outDir;   // for option -o
       bool validationFlag;  // flag for input option '-novalidation'
       bool outFlag;         // check if out file is already created
       bool logFileFlag;
       bool outPropvalFlag;  // flag for input option '-outdata PROPVAL'
       bool outNormdataFlag; // flag for input option '-outdata NORMDATA'
       int applTarget;      // variable for declaring the target of the comparison
                            // 1= target is comparison output for Planets Plato tool,
                            // 0= comparator stand-alone
       int compsetCount;
       std::string nameSourceXCDL;
       std::string nameTargetXCDL;

       std::list <char*> targetXCDLs;
};
#endif


