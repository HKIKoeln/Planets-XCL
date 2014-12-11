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
#include "XCLSyntaxProperty.h"

// Constructors/Destructors
//

XCLSyntaxProperty::XCLSyntaxProperty () :XCLSyntaxSymbol()
{
}


XCLSyntaxProperty::XCLSyntaxProperty(const XCLSyntaxProperty& prop)
    :XCLSyntaxSymbol(prop)
{
  knownValue=prop.knownValue;
}


XCLSyntaxProperty& XCLSyntaxProperty::operator=(const XCLSyntaxProperty& prop)
{
  if(this!=&prop)
  {
    XCLSyntaxExpression::operator=(prop);
    knownValue=prop.knownValue;
  }
  return *this;
}


XCLSyntaxProperty::~XCLSyntaxProperty ( )
{
}


enum XCLSyntaxElement::ELEMENT_TYPE XCLSyntaxProperty::getElementType()const
{
  return XCLSyntaxElement::PROPERTY_TYPE;
}


/**
 * Set the value of knownValue
 * @param new_var the new value of knownValue
 */
void XCLSyntaxProperty::setKnownValue ( const QString& new_var )
{
  knownValue = new_var;
}


/**
 * Get the value of knownValue
 * @return the value of knownValue
 */
const QString& XCLSyntaxProperty::getKnownValue ( )
{
  return knownValue;
}




