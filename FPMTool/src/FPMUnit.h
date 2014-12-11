/***
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
***/
#ifndef FPMUNIT_H
#define FPMUNIT_H

#include <string>
#include <list>
#include "FPMUnitError.h"
#include "FileHandler.h"


class FPMUnit
{
  public:
      FPMUnit();
      ~FPMUnit();
      void executeFPMUnitCore(std::string puids);
      bool createFPMUnitOutput(std::string error);  // in case of errors
      std::string parseArgs(int argc, char* argv[]);
  private:
      bool checkIfDirectoryIs(const char* dir);
      bool checkInputSyntax(std::string puids);
      bool loadPUIDList(std::string filename);
      bool checkPUID(std::string puid);
      bool createFPMUnitOutput(std::list <std::string>* propLists); //creates standard output file
      std::string getPropertiesList(std::string puid); // returns property list for format X as string
      std::string PUIDList;
      FPMUnitError *pFPMError;
      FileHandler *pFH;
      std::list <std::string>* supportedPUIDs;
      std::string outputDir;
};
#endif
