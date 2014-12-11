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
Request2InHandler.h

Description: Individual SAX Handler class for PLANETS PP5 request 2 input
*****************************************************************************/

#ifndef _REQUEST2INHANDLER_H
#define _REQUEST2INHANDLER_H

#include <xercesc/sax2/DefaultHandler.hpp>
#include "XercesString.h"
#include "ComparatorError.h"

class Comparer;
class Request2;

class Request2InHandler : public DefaultHandler
{
public:
       enum parsStat {
                      start,
                      startRequest2,
                      startMeasurementRequest,
                      startProperty,
                      endMeasurementRequest,
                      endRequest2,
                      erroneous
                      };

       Request2InHandler();
       Request2InHandler(ComparatorError *pCE);
       ~Request2InHandler();
// --------------      inline methods:    -----------------------
       inline void startDocument()
       {
       #ifdef DEBUG_REQUEST2
       printf("starting document...\n");
       #endif
       }
       inline void endDocument()
       {
       if(parsingState==endRequest2)
         {
         #ifdef DEBUG_REQUEST2
         printf("Request2 input file parsed successfully...\n");
         #endif
         }
       else {pErr->registrateError("Error: Request2InHandler: Wrong parsing state at end of parsing.");}
       }
       inline void characters(const XMLCh* const chars,
                       const unsigned int length) {}
//   ------------------------------------------------------------
       void startElement(const XMLCh* const uri,
                         const XMLCh* const localname,
                         const XMLCh* const qname,
                         const Attributes& attrs);
       void endElement(const XMLCh* const uri,
                       const XMLCh* const localname,
                       const XMLCh* const qname);

       ComparatorError *pErr;
       parsStat parsingState;
       Request2 *pRequest2;

};
#endif
