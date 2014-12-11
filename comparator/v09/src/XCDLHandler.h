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
XCDLHandler.h

Description: Individual Sax Handler class for XCDL files
*****************************************************************************/
#ifndef _XCDLHANDLER_H
#define _XCDLHANDLER_H

#include <xercesc/sax2/DefaultHandler.hpp>
#include <string>
#include "XercesString.h"

class ComparatorError;
class PropertyTagset;
class Comparer;
class ComparatorOutput;
class ComparisonConfiguration;
class SaxInterface;
class NormData;
class InputParams;
class XercesString;
class XCDLIndex;

class XCDLHandler : public DefaultHandler
{
  public:
       XCDLHandler() {};
       XCDLHandler(int st, std::string elem, ComparatorError *pError, XCDLIndex *pI, InputParams *pInPut)
         {
         state= st;
         element= elem.c_str();
         elementToSearchFor= NULL;
         pNormData= NULL;
         pErr= pError;
         pIndex= pI;
         pInput= pInPut;
         }       // for source XCDL

       ~XCDLHandler() {};

//     ----------------     inline methods:   ------------------------
       inline void setNormDataPtr(NormData *pND) {pNormData= pND;}
       inline void setParsingState(int st) {state= st;}
       inline void setElementSearchedFor(char* elemSF) {element= elemSF;}
       inline void startDocument()
       {
       #ifdef DEBUG_XCDLHANDLER
       printf("starting document...\n");
       #endif
       // transcode the element to search for:
       elementToSearchFor= XMLString::transcode(element);
       if(state==1) parsingState= searchNormData;
       else parsingState= start;
       }

       inline void endElement(const XMLCh* const uri,
                       const XMLCh* const localname,
                       const XMLCh* const qname)
       {
       return;
       }
       inline void endDocument()
       {
       #ifdef DEBUG_XCDLHANDLER
       printf("end document...\n");
       #endif
       }
//      ----------------------------------------------------

       void startElement(const XMLCh* const uri,
                            const XMLCh* const localname,
                                const XMLCh* const qname,
                                    const Attributes& attrs);
       void characters(const XMLCh* const chars, const unsigned int length);
       bool PP5_checkIfPropertyIsRequested(Comparer *pComparer, std::string tempstr);
       void transcodeToUTF8(const XMLCh* const chars);
// private:
       enum parseState {
                     start,             // =0
                     searchNormData,
                     loadNormData,
                     done,
                     erroneous          //= 4
                     };
       parseState parsingState;
       int state;
       const char* element;
       const XMLCh* elementToSearchFor;
       NormData *pNormData;
       ComparatorError *pErr;
       XCDLIndex *pIndex;
       InputParams *pInput;
};
#endif

