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

    #ifdef __OS_WIN
    char buffer[11];
    unsigned int lineNumber= e.getLineNumber();
    itoa(lineNumber, buffer, 10);
    std::string result= "Error: SaxErrorHandler: File is not valid. Line: " + std::string(buffer);
    #endif
    #ifdef __OS_OSMAC
    std::string result= "Error: SaxErrorHandler: File is not valid.";
    #endif

    //pErr->registrateError(result);
    throw result;
    #ifdef DEBUG_XMLVALIDATION
    std::cout<<"Validation error in line " <<e.getLineNumber()<<std::endl;
    #endif
    }
    inline void fatalError(const SAXParseException& e)
    {
    #ifdef DEBUG_XMLVALIDATION
    std::cout<<"Validation: Fatal error."<<std::endl;
    #endif
    std::string result= "Validation: SAXParseException: Fatal error.";
    pErr->registrateError(result);
    throw result;
    }
    ComparatorError *pErr;
};
#endif
