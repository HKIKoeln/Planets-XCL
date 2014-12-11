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
DataTagset.h

Description:
*****************************************************************************/
#ifndef DATATAGSET_H
#define DATATAGSET_H

#include <string>
#include <map>
#include <vector>

class NormData
{
   public:
         NormData();
         ~NormData();
         void cleanUp();
         void releaseNormDataTagsetsUint();
         void releaseNormDataTagsetsStr();
         void printNormdata();  // for debugging
         inline void loadNormDataAsIs(char* normData, const unsigned int length)
           {
           normDataAsIs = new char[length+1];
           strcpy(normDataAsIs, normData);
           }
         inline void loadNormDataAsStdStr(char* normDataAsIs) {normDataAsStdStr= std::string(normDataAsIs);}
         void loadNormDataAsStrings();
         //void loadNormDataAsUint(NormData *pND, char* normdata);
         void loadNormDataAsInt(std::string property, NormData *pND, char* normdata);

         // private:
         std::map <std::string, std::vector <char>* > *normDataTagsetsStr;
         std::map <std::string, std::vector <int>* > *normDataTagsetsUint;
         std::vector <std::string> *normDataAsStrings;
         std::vector <int> *normDataAsInt;
         char* normDataAsIs;
         std::string normDataAsStdStr;
         std::string normDataType;
         std::string key;
         unsigned int normDataLength;  // comes from Xercesc
};

class RawData
{
   public:
         RawData();
         ~ RawData();
};
#endif
