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
PropertyTagset.h

Description:
*****************************************************************************/
#ifndef PROPERTYTAGSET_H
#define PROPERTYTAGSET_H

#include <string>
#include <map>

class ValueSet
{
   public:
         ValueSet();
         ~ValueSet();
         void cleanUp();
         void printValueSet();  // for debugging

         std::string valueSetID;
         std::string rawVal;
         bool labVal;
         std::string labValVal;
         std::string labValType;
         bool dataRef;
         bool propRel;
};

class PropertyTagset
{
   public:
         PropertyTagset();
         ~PropertyTagset();

         void cleanUp();
         std::string propName;
         std::string propID;
         std::map <std::string, ValueSet*> *valueSets;
         ValueSet *pValueSet;
};
#endif
