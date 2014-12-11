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
XMLValidation.h

class for XML validation; uses Xercesc
******************************************************************************/

#ifndef _XMLVALIDATION_H
#define _XMLVALIDATION_H

class InputParams;
class ComparatorError;

class XMLValidation
{
public:
       XMLValidation(ComparatorError *pError);
       ~XMLValidation();
       void validateXMLFile(InputParams *pInParams, int choice); // validate XML files acc. to InputParams
       void validateXMLFile(char* XMLFileName, char* XMLSchemaName); // validate exactly one XML file
       ComparatorError *pErr;
};
#endif
