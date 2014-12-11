/*
    * This module is part of the XCL software system
    *
    * Written by  Sebastian Beyl, Volker Heydegger, Jan Schnasse,
    *             Manfred Thaller
    *             2007 - 2009
    *
    * Copyright (C)  2007 - 2009
    * Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller
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
class Request2;

class InputParams
{
public:
       InputParams();
       ~InputParams();
       int parseArgs(int argc, char* argv[]);
       void printInputParams();
       void checkForAppropriateCompSet(ComparatorError *pError, Request2 *pPCR);
       void cleanUp();
       char* sourceXCDL;
       char* targetXCDL;
       char* PCRFile;
       std::string CPRName;
       std::string outDir;   // for option -o
       bool outFlag;         // check if out file is already created
       int applTarget;      // variable for declaring the target of the comparison
                            // 1= target is comparison output for Planets Plato tool,
                            // 0= comparator stand-alone
       int compsetCount;
       std::string nameSourceXCDL;
       std::string nameTargetXCDL;

       std::list <char*> targetXCDLs;
};
#endif


