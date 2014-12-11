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
#include "Defines.h"
#include "XCLProcessMethod.h"

// Constructors/Destructors
//
XCLProcessMethod::XCLProcessMethod ()
{
}


XCLProcessMethod::XCLProcessMethod (const QString& mN)
{

  name=mN;
}

XCLProcessMethod::~XCLProcessMethod ( )
{
}


XCLProcessMethod& XCLProcessMethod::operator=(const XCLProcessMethod& m)
{
  if(this != &m)
  {
    name = m.name;
    parameters = m.parameters;
  }
  return *this;
}


void XCLProcessMethod::addParameter(const XCLProcessParameter& p)
{
  parameters.append(p);
}


/**
 * Set the value of name
 * @param new_var the new value of name
 */
void XCLProcessMethod::setName ( const QString& new_var )
{
  name = new_var;
}


/**
 * Get the value of name
 * @return the value of name
 */
const QString& XCLProcessMethod::getName ( )
{
  return name;
}


/**
 * Set the value of parameters
 * @param new_var the new value of parameters
 */
void XCLProcessMethod::setParameters ( const QVector<XCLProcessParameter>& new_var )
{
  parameters = new_var;
}


/**
 * Get the value of parameters
 * @return the value of parameters
 */
const QVector<XCLProcessParameter>& XCLProcessMethod::getParameters ( )
{
  return parameters;
}


void XCLProcessMethod::print(QString format)
{
  _LENGTH size=parameters.size();
  for (_LENGTH i=0;i<size;i++)
  {
    parameters[i].print(format);
  }
}

