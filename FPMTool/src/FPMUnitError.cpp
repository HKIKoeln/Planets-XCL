/***************************************************
FPMUnitError.cpp

DESCRIPTION: Exception class for FPMTool


    * This module is part of the XCL software system
    *
    * Copyright (C) 2007 - 2008 University of Cologne,
                    Historisch-Kulturwissenschaftliche Informationsverarbeitung
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



***************************************************/
//#define DEBUG_CONSOLE
#ifdef DEBUG_CONSOLE
#include <iostream>
#endif

#include <string>
#include <fstream>
#include "FPMUnitError.h"

void FPMUnitError :: registrateError(std::string errorcode, std::string option)
{
errCode=errorcode;   //  std::cout<<errCode;
handleError(errCode, option);
};


void FPMUnitError :: handleError(std::string errCode, std::string option)
{
std::ofstream logfile;

logfile.open("logfile.txt", std::ios_base::out | std::ios_base::app);

if(errCode.compare("FH_1")==0)
  {
           #ifdef DEBUG_CONSOLE
           std::cout << "Error: FileHandler::loadFile(): Error while attempting to open file "  << option.c_str() << std::endl;
           #endif
           logfile << "Error: FileHandler::loadFile(): Error while attempting to open file " << option.c_str() << std::endl;
           returnData="Internal Error: Code: " + errCode + ". Please contact service admin.";
           action=2;

  }
else if(errCode.compare("FH_2")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FileHandler::loadFile(): reading file to stream in " << option.c_str() << std::endl;
          #endif
          logfile << "Error: FileHandler::loadFile(): reading file to stream in " << option.c_str() << std::endl;
          returnData="Internal Error: Code: " + errCode + ". Please contact service admin.";
          action=2;

  }
else if(errCode.compare("FH_3")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FileHandler::loadFile(): unspecified error in " << option.c_str() << std::endl;
          #endif
          logfile << "Error: FileHandler::loadFile(): unspecified error in " << option.c_str() << std::endl;
          returnData="Internal Error: Code: " + errCode + ". Please contact service admin.";
          action=2;

  }
else if(errCode.compare("FPM_0")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::loadPUIDList(): unspecified error "<<std::endl;
          #endif
          logfile << "Error: FPMUnit::loadPUIDList(): unspecified error "<<std::endl;
          returnData="Internal Error: Code: " + errCode + ". Please contact service admin.";
          action=2;

  }

else if(errCode.compare("FPM_1")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::loadPUIDList(): Syntax error in file PUIDList: # sign missing at the end of puids. "<<std::endl;
          #endif
          logfile << "Error: FPMUnit::loadPUIDList(): Syntax error in file PUIDList: # sign missing at the end of puids. "<<std::endl;
          returnData= "Internal Error: Code: " + errCode + ". Please contact service admin.";
          action=2;

  }
else if(errCode.compare("FPM_2")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::checkInputSyntax(): Invalid input. PUID list is syntactically wrong. Must match: ((fmt_[0-9]+:)+) " <<std::endl;
          #endif
          logfile << "Error: FPMUnit::checkInputSyntax(): Invalid input. PUID list is syntactically wrong. Must match: ((fmt_[0-9]+:)+) " <<std::endl;
          returnData= "Client Error: Code: " + errCode + ". The transmitted PUID list has an invalid syntax. Allowed:  ((fmt_[0-9]+:)+) .";
          action=2;
  }
else if(errCode.compare("FPM_3")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::getPropertyLists(): Unable to load property list of file " << option.c_str() <<std::endl;
          #endif
          logfile << "Error: FPMUnit::getPropertyLists(): Unable to load property list of file " << option.c_str() <<std::endl;
          returnData= "Internal Error: Code: " + errCode + ". Please contact service admin.";
          action=2;
  }
else if(errCode.compare("FPM_4")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::createFPMUnitOutput(): Unable to create output file " << option.c_str() <<std::endl;
          #endif
          logfile << "Error: FPMUnit::createFPMUnitOutput(): Unable to create output file "<< option.c_str() <<std::endl;
          returnData= "Internal Error: Code: " + errCode + ". Please contact service admin.";
          action=2;
  }
else if(errCode.compare("IN_1")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::parseArgs(): Invalid input. More than one puidlist transmitted, this is not allowed. Check input. " <<std::endl;
          #endif
          logfile << "Error: FPMUnit::parseArgs(): Invalid input. More than one puidlist transmitted, this is not allowed. Check input. "<<std::endl;
          returnData= "Client Error: Code: " + errCode + ". Invalid input. More than one puidlist transmitted, this is not allowed. Check input.";
          action=2;
  }
else if(errCode.compare("IN_0")==0)
  {
          #ifdef DEBUG_CONSOLE
          std::cout << "Error: FPMUnit::parseArgs(): Invalid input: Invalid or uncomplete arguments. Check input. " <<std::endl;
          #endif
          logfile << "Error: FPMUnit::parseArgs(): Invalid input: Invalid or uncomplete arguments. Check input. "<<std::endl;
          returnData= "Client Error: Code: " + errCode + ". Invalid input: Invalid or uncomplete arguments. Check input.";
          action=2;
  }
else logfile << errCode <<"\n" ;

logfile.close();
};

int FPMUnitError :: getAction()
{
return action;
};
