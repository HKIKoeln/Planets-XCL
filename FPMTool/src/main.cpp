/***************************************************
main.cpp

DESCRIPTION: main routine for FPMTool

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
****************************************************************************/


//#define DEBUG_CONSOLE
#ifdef DEBUG_CONSOLE
#include <iostream>
#endif

#include <string>
#include "FPMUnit.h"


int main(int argc, char *argv[])
{
  FPMUnit* pFPM= new FPMUnit();

  if(argc==1) pFPM->createFPMUnitOutput("Error: No parameters transmitted. Check input.");
  else if(argc>4) pFPM->createFPMUnitOutput("Error: Too many parameters transmitted. Check input.");
  else if(argc==3) pFPM->createFPMUnitOutput("Error: Invalid number of parameters. Check input.");

  std::string puids= pFPM->parseArgs(argc, argv);
  if(puids.compare("\0")!=0)
    {
    pFPM->executeFPMUnitCore(puids);
    }
  delete pFPM;

  return 0;
};



