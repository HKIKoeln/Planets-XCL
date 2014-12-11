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


#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include "FPMUnitError.h"


class FileHandler
{
public:
    FileHandler() {}
    ~FileHandler() {}
    std::string loadFile(std::string filename, FPMUnitError *pFPMError);
private:
    int getFileSize(const char* fname);
    long size;
};

#endif
