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
#include <iostream>
#include "XCLSyntaxProcess.h"
#include "XCLProcessMethod.h"
// Constructors/Destructors
//

XCLSyntaxProcess::XCLSyntaxProcess ():XCLSyntaxElement()
{
}


XCLSyntaxProcess::~XCLSyntaxProcess ( )
{}


XCLSyntaxProcess::XCLSyntaxProcess(const XCLSyntaxProcess& p)
    :XCLSyntaxElement(p)
{
  xcelReference=p.xcelReference;
  xcelNameReference=p.xcelNameReference;
  methods=p.methods;
  type=p.type;
}


XCLSyntaxProcess& XCLSyntaxProcess::operator=(const XCLSyntaxProcess& p)
{
  if(this!=&p)
  {
    xcelReference=p.xcelReference;
    xcelNameReference=p.xcelNameReference;
    methods=p.methods;
    type=p.type;
  }
  return *this;
}


void XCLSyntaxProcess::addMethod(const XCLProcessMethod& m)
{
  methods.append(m);
}


enum XCLSyntaxElement::ELEMENT_TYPE XCLSyntaxProcess::getElementType()const
{
  return XCLSyntaxElement::PROCESS_TYPE;
}


/**
 * Set the value of xcelReference
 * @param new_var the new value of xcelReference
 */
void XCLSyntaxProcess::setXcelReference ( const QString& new_var )
{
  xcelReference = new_var;
}


void XCLSyntaxProcess::setXcelNameReference ( const QString& new_var )
{
  xcelNameReference = new_var;
}


/**
 * Get the value of xcelReference
 * @return the value of xcelReference
 */
const QString& XCLSyntaxProcess::getXcelReference ( )
{
  return xcelReference;
}


const QString& XCLSyntaxProcess::getXcelNameReference ( )
{
  return xcelNameReference;
}


/**
 * Set the value of methods
 * @param new_var the new value of methods
 */
void XCLSyntaxProcess::setMethods ( const QVector<XCLProcessMethod>& new_var )
{
  methods = new_var;
}


/**
 * Get the value of methods
 * @return the value of methods
 */
const QVector<XCLProcessMethod>& XCLSyntaxProcess::getMethods ( )
{
  return methods;
}


void XCLSyntaxProcess::setType(const QString& t)
{
  QString tr=t;

  if(t=="pushXCEL")
    type=PUSHXCEL;
  else if(t=="pullXCEL")
    type=PULLXCEL;
  else if(t=="pushBinary")
  {
    type=PUSHBINARY;
  }
  else if(t=="implicite")
    type=IMPLICITE;
  else if(t=="configureParser")
    type=CONFIGPARSER;
}


enum XCLSyntaxProcess::PROCESS_TYPE XCLSyntaxProcess::getType()
{
  return type;
}


void XCLSyntaxProcess::print(QString format)
{
  //std::cout<<format.toStdString()<<"Type: "<<type<<std::endl;
  if (!xcelReference.isNull())
    std::cout<<format.toStdString()<<"xcelReference: "<<xcelReference.toStdString()<<std::endl;
  else
    std::cout<<format.toStdString()<<"xcelNameReference: "<<xcelNameReference.toStdString()<<std::endl;
  switch(type)
  {
  case PUSHXCEL:
    std::cout<<format.toStdString()<<"type: "<<"pushXCEL"<<std::endl;
    break;
  case PULLXCEL:
    std::cout<<format.toStdString()<<"type: "<<"pullXCEL"<<std::endl;
    break;
  case PUSHBINARY:
    std::cout<<format.toStdString()<<"type: "<<"pushBinary"<<std::endl;
    break;
  case IMPLICITE:
    std::cout<<format.toStdString()<<"type: "<<"implicite"<<std::endl;
    break;
  case CONFIGPARSER:
    std::cout<<format.toStdString()<<"type: "<<"configureParser"<<std::endl;
    break;
  }
  _LENGTH size = methods.size();
  for (_LENGTH i = 0 ; i <size;i++)
  {
    std::cout<<format.toStdString()<<"Method: "<<methods[i].getName().toStdString()<<std::endl;
    methods[i].print(format);
  }
}
