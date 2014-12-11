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
ComparisonConfigurationHandler.h

Description: Individual SAX Handler class for
XML based comparison configuration file
*****************************************************************************/

#ifndef _COMPARISONCONFIGURATIONHANDLER_H
#define _COMPARISONCONFIGURATIONHANDLER_H

#include <xercesc/sax2/DefaultHandler.hpp>
#include "XercesString.h"
#include "ComparatorError.h"

class Comparer;
class ComparisonConfiguration;

class ComparisonConfigurationHandler : public DefaultHandler
{
public:
       enum parsStat {
                      start,
                      startComparisonConfiguration,
                      startCompSet,
                      startProperty,
                      endCompSet,
                      endComparisonConfiguration,
                      erroneous
                      };

       ComparisonConfigurationHandler();
       ComparisonConfigurationHandler(ComparatorError *pCE);
       ~ComparisonConfigurationHandler();
// --------------      inline methods:    -----------------------
       inline void startDocument()
       {
       #ifdef DEBUG_ComparisonConfiguration
       printf("starting document...\n");
       #endif
       }
       inline void endDocument()
       {
       if(parsingState==endComparisonConfiguration)
         {
         #ifdef DEBUG_ComparisonConfiguration
         printf("ComparisonConfiguration input file parsed successfully...\n");
         #endif
         }
       else {pErr->registrateError("Error: ComparisonConfigurationHandler: Wrong parsing state at end of parsing.");}
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
       ComparisonConfiguration *pCoco;

};
#endif
