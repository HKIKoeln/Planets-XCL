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

Description: Individual Sax Handler class for generating index on XCDL file
*****************************************************************************/
#ifndef _XCDLINDEXGENERATOR_H
#define _XCDLINDEXGENERATOR_H

#include <xercesc/sax2/DefaultHandler.hpp>
#include <string>
#include "XercesString.h"

class ComparatorError;
class ComparisonConfiguration;
class XCDLIndex;
class DynamicIndexes;

class XCDLIndexGenerator : public DefaultHandler
{
public:
     XCDLIndexGenerator(ComparatorError *pError);
     XCDLIndexGenerator(ComparatorError *pError, ComparisonConfiguration *pReq2, XCDLIndex *pIndex);
     ~XCDLIndexGenerator();

     enum parseState {parseXCDL,
                      startXCDL,
                      endXCDL,
                      startObject,
                      endObject,
                      startData,
                      endData,
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
                      startPropertyValueSetObjRef,
                      endPropertyValueSetObjRef,
                      startPropertySet,
                      endPropertySet,
                      startPropertySetDataRef,
                      endPropertySetDataRef,
                      startPropertySetDataRefRef,
                      endPropertySetDataRefRef,
                      startPropertySetRef,
                      endPropertySetRef,
                      startValueSetRelations,
                      endValueSetRelations,
                      startValueSetRelationsRef,
                      endValueSetRelationsRef,
                      erroneous,
                      other           //= 24
                      };

       inline void startDocument()
       {
       #ifdef DEBUG_XCDLINDEXGENERATOR
       printf("starting document...\n");
       #endif
       }
       void endDocument();
       void startElement(const XMLCh* const uri,
                         const XMLCh* const localname,
                         const XMLCh* const qname,
                         const Attributes& attrs);
       void endElement(const XMLCh* const uri,
                       const XMLCh* const localname,
                       const XMLCh* const qname);
       void characters(const XMLCh* const chars,
                       const unsigned int length);

       bool PP5_checkIfPropertyIsRequested(ComparisonConfiguration* pCoco, std::string propertyName);
       inline void resetIndexPtr(XCDLIndex *pInd) {pIndex= pInd;}
       inline XCDLIndex* getIndexPtr() {return pIndex;}
       inline void resetParsingState() {parsingState=parseXCDL;}
       parseState parsingState;
       ComparatorError *pErr;
       XCDLIndex *pIndex;
       ComparisonConfiguration *pCoco;
       NormData *pND;
       std::string tempstring;
};
#endif
