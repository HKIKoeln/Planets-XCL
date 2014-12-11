/***************************************************
FileHandler.cpp

DESCRIPTION: Some basic functionality for file handling


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

#include <fstream>
#include <string>
#include "FileHandler.h"
#include "FPMUnitError.h"
#include <iostream>

std::string FileHandler :: loadFile(std::string filename, FPMUnitError *pFPMError)
{

std::string errorcode, str="\0";

try
  {
  std::ifstream filestr(filename.c_str()); 
  if(!filestr.good()) {errorcode= "FH_1";throw errorcode;} 
  
  getline(filestr,str,static_cast<std::ifstream::char_type>(std::ifstream::traits_type::eof()));
  if(str.compare("\0")==0) {errorcode= "FH_2"; throw errorcode;}
  }   
   
catch(std::string)
     {
     pFPMError->registrateError(errorcode, filename);         
     }
catch(...)
     {     
     pFPMError->registrateError("FH_3", filename);
     }    
        
  return str;
};


int FileHandler :: getFileSize(const char* fname)
{
  std::ifstream f;
  
  f.open(fname, std::ios_base::binary | std::ios_base::in);
  
  if (!f.good() || f.eof() || !f.is_open()) { return 0; }
  f.seekg(0, std::ios_base::beg);
  std::ifstream::pos_type begin_pos = f.tellg();
  f.seekg(0, std::ios_base::end);
  
  return static_cast<int>(f.tellg() - begin_pos);
};


 
