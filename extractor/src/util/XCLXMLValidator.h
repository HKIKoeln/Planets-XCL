/*
    * This module is part of the XCL software system
    *
    * Written by Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 2006 - 2009
    *
    * Copyright (C) 2006 - 2009 Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 
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
#ifndef __XCLXMLVALIDATOR_H
#define __XCLXMLVALIDATOR_H

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <qstring.h>
#include <qtextstream.h>

#include "XCLErrorHandler.h"
#include "Defines.h"
#include <iostream>
#include <string>

XERCES_CPP_NAMESPACE_USE

class XCLXMLValidator 
{
public:
  XCLXMLValidator ();
  ~XCLXMLValidator();
  BOOL validate(QString xmlFile, bool printElements=false);
  static void printChildElements(DOMNode *n);

private:
  void buildValidator();
private:
  DOMXCLErrorHandler* errorHandler;
  DOMBuilder        *parser;

    QString xmlFile;
    AbstractDOMParser::ValSchemes valScheme;
    bool                       doNamespaces;
    bool                       doSchema;
    bool                       schemaFullChecking;
    bool                       errorOccured;
    bool                       recognizeNEL;
    bool                       printOutEncounteredEles;
};
#endif //XCLXMLPARSER_H

