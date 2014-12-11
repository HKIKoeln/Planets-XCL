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
class Request2;
class SaxInterface;
class NormData;
class InputParams;

class XCDLHandler : public DefaultHandler
{
  public:
       enum parsStat {parseXCDL1,
                      parseXCDL2,
                      startXCDL1,
                      endXCDL1,
                      startXCDL2,
                      endXCDL2,
                      startComposition,
                      startObject,
                      endObject,
                      startData,
                      startNormData,
                      endNormData,
                      startProperty,
                      endProperty,
                      startPropertyName,
                      endPropertyName,
                      startPropertyValueSet,
                      endPropertyValueSet,
                      startPropertyValueSetRawVal,
                      endPropertyValueSetRawVal,
                      startPropertyValueSetLabVal,
                      endPropertyValueSetLabVal,
                      startPropertyValueSetLabValVal,
                      endPropertyValueSetLabValVal,
                      startPropertyValueSetLabValType,
                      endPropertyValueSetLabValType,
                      startPropertyValueSetDataRef,
                      endPropertyValueSetDataRef,
                      startPropertyValueSetPropRel,
                      endPropertyValueSetPropRel,
                      startPropertyValueSetRef,
                      endPropertyValueSetRef,
                      parseNextProperty,
                      endPropertySet,
                      erroneous,
                      skipAll,
                      other           //= 37
                      };

       XCDLHandler();
       XCDLHandler(Request2 *pR2, ComparatorError* pCE, InputParams *pInPa);
       XCDLHandler(PropertyTagset *pPropTag, NormData *pNData, Comparer *pComp, ComparatorError *pError);
       ~XCDLHandler();

//     ----------------     in line methods:   ------------------------
       inline void startDocument()
       {
       #ifdef DEBUG_XCDLHANDLER
       printf("starting document...\n");
       #endif
       }
       inline void startElement(const XMLCh* const uri,
                         const XMLCh* const localname,
                         const XMLCh* const qname,
                         const Attributes& attrs)
       {
       if(parsingState==erroneous || parsingState==skipAll) return;
       if(parser==1) startElementXCDL1(localname, attrs);
       else startElementXCDL2(localname, attrs);
       }
       inline void endElement(const XMLCh* const uri,
                       const XMLCh* const localname,
                       const XMLCh* const qname)
       {
       if(parsingState==erroneous || parsingState==skipAll) return;
       if(parser==1) endElementXCDL1(localname);
       else endElementXCDL2(localname);
       }
       inline void characters(const XMLCh* const chars,
                       const unsigned int length)
       {
       if(parsingState==erroneous || parsingState==skipAll) return;
       if(parser==1) charactersXCDL1(chars, length);
       else charactersXCDL2(chars, length);
       }
       inline SaxInterface* getSaxIFPtr() {return pSI;}
//      ----------------------------------------------------
       void cleanUpAll();
       void cleanUpXCDLHandler1();
       void cleanUpXCDLHandler2();
       void endDocument();
       void startElementXCDL1(const XMLCh* const localname, const Attributes& attrs);
       void startElementXCDL2(const XMLCh* const localname, const Attributes& attrs);
       void endElementXCDL1(const XMLCh* const localname);
       void endElementXCDL2(const XMLCh* const localname);
       void charactersXCDL1(const XMLCh* const chars, const unsigned int length);
       void charactersXCDL2(const XMLCh* const chars, const unsigned int length);
       bool PP5_checkIfPropertyIsRequested(Comparer *pComparer, std::string tempstr);

       short parser;
       parsStat parsingState;
       ComparatorError *pErr;
       InputParams *pInputParams;
       PropertyTagset *pPropertyTagset1;
       PropertyTagset *pPropertyTagset2;
       ComparatorOutput *pCO;
       NormData *pNormData1;
       NormData *pNormData2;
       Comparer *pComparer;

//private:
        SaxInterface *pSI;
};

#endif
