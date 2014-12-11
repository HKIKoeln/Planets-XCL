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
SaxInterface.h

Description: Interface to Xerces-c Sax Parser
******************************************************************************/
#ifndef _SAXINTERFACE_H
#define _SAXINTERFACE_H

#include <xercesc/sax2/SAX2XMLReader.hpp>

class Comparer;
class ComparisonConfigurationHandler;
class ComparisonConfiguration;
class ComparatorError;
class NormData;
class InputParams;
class XCDLIndexGenerator;

class SaxInterface
{
      public:
             SaxInterface();
             SaxInterface(ComparatorError *pCE);
             ~SaxInterface();
             xercesc::SAX2XMLReader* initializeSax2Parser(char* sourceXCDL, xercesc::SAX2XMLReader* pParserSource);
             void terminateSax2Parser() ;
             void Sax2ParseFile(xercesc::SAX2XMLReader* pParser, char* XCDLFile); // for loading normData
             void startSaxParser(char* XCDLFile, XCDLIndexGenerator *pIG); // for creating XCDL index
             void startSaxParser(char* cocoFile, ComparisonConfigurationHandler* pR2In); // for parsing PCR
      //private:
             ComparatorError *pErr;
             InputParams *pInPa;
};
#endif
