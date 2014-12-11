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
SaxErrorHandler.h

Description: Default error handling for SAX parser
*****************************************************************************/
#ifndef _SAXERRORHANDLER_H
#define _SAXERRORHANDLER_H

#include <xercesc/sax2/DefaultHandler.hpp>
#include <exception>
class SaxErrorHandler : public DefaultHandler
{
public:
    SaxErrorHandler(ComparatorError *pError) {pErr= pError;}
    ~SaxErrorHandler() {pErr= NULL;}
    inline void warning(const SAXParseException& e)
    {
     /* do nothing */
    }
    inline void error(const SAXParseException& e)
    {
    std::string result;
    /* The following code is a hack so far; I first tried to convert lineNumber
    back to std:string using sprintf, but that doesn't work; there must be some
    incompatibility while casting xerces type XMLSSize_t(perhaps 32/64 bit?),
    which is a typedef for unsigned int data-type ; the function itoa() works
    for gcc under win32 but is not portable to all compilers; VH 25/08/2008 */
    #ifdef _DEVELOPVERSION
    char buffer[11];
    unsigned int lineNumber= e.getLineNumber();
    itoa(lineNumber, buffer, 10);
    result= "Error: SaxErrorHandler: File is not valid. Line: " + std::string(buffer);
    #endif
    #ifdef _RELEASEVERSION
    if(result.empty())    result= "Error: SaxErrorHandler: File is not valid.";
    #endif

    //pErr->registrateError(result);
    throw result;
    #ifdef DEBUG_XMLVALIDATION
    std::cout<<"Validation error in line " <<e.getLineNumber()<<std::endl;
    #endif
    }
    inline void fatalError(const SAXParseException& e)
    {
    std::string result;
    #ifdef DEBUG_XMLVALIDATION
    std::cout<<"Validation: Fatal error."<<std::endl;
    #endif
    result= "Validation: SAXParseException: Fatal error.";
    pErr->registrateError(result);
    throw result;
    //error(e);
    }
    ComparatorError *pErr;
};
#endif
