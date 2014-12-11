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
#ifndef XCLSYNTAXPROPERTY_H
#define XCLSYNTAXPROPERTY_H
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxSymbol.h"

#include <QString>
class XCLSyntaxProperty : public XCLSyntaxSymbol
{
public:// Constructors/Destructors
  XCLSyntaxProperty ();
  XCLSyntaxProperty(const XCLSyntaxProperty& prop);
  XCLSyntaxProperty& operator=(const XCLSyntaxProperty& prop);
  virtual ~XCLSyntaxProperty ( );
private://members
  QString knownValue;//!< The properties value as given in the file format specification.
public:
  const QString& getKnownValue ( );
public:
 enum XCLSyntaxElement::ELEMENT_TYPE getElementType()const;
 BOOL compareValues();
 void setKnownValue ( const QString& new_var );
};

#endif // XCLSYNTAXPROPERTY_H
