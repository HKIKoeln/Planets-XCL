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
#include "XCLParsingProperty.h"
#include "XCLStringConverter.h"
// Constructors/Destructors
//

XCLParsingProperty::XCLParsingProperty ( ):XCLParsingSymbol()
{
}

XCLParsingProperty::~XCLParsingProperty ( )
{
}

XCLParsingProperty::XCLParsingProperty(const XCLParsingProperty& prop)
    :XCLParsingSymbol(prop)
{
  knownValue=prop.knownValue;
}

XCLParsingProperty& XCLParsingProperty::operator=(const XCLParsingProperty& prop)
{
  if(this!=&prop)
  {
    XCLParsingSymbol::operator=(prop);
    knownValue=prop.knownValue;
  }
  return *this;
}


BOOL XCLParsingProperty::isValid ()
{
#ifdef PlanetsDebug
  std::cout<<"------compare--------------"<<std::endl;
  std::cout<<getName().toStdString()<<"  "<<getIdentifier().toStdString()<<"\n";
  std::cout<<knownValue.toStdString()<<"--------"<<std::endl;
  std::cout<<getInterpretedValue().toStdString()<<"-------"<<std::endl;
#endif
  if(compare(knownValue,getInterpretedValue()))
  {
#ifdef PlanetsDebug
    std::cout<<"------gut--------------"<<std::endl;
#endif
    return TRUE;
  }
  else
    return FALSE;
}


/**
 * Set the value of knownValue
 * @param new_var the new value of knownValue
 */
void XCLParsingProperty::setKnownValue (const QString& new_var )
{
  knownValue = new_var;
  if(interpretationType == CHARACTER)
  {
    QString tmp;
    for(int i=0;i<knownValue.size();i++)
    {
      tmp.append(knownValue.at(i));
      //tmp.append(' ');
    }
    knownValue = tmp.mid(0,tmp.size());
  }
}


void XCLParsingProperty::print(QTextStream& debugOut, QString format)
{
  XCLParsingSymbol::print(debugOut, format);
  debugOut<<format<<"KnownValue: "<<knownValue<<"\n";
}


void XCLParsingProperty::print(QString format)
{
  QTextStream debugOut(stdout);

  print(debugOut, format);
}


/**
 * Get the value of knownValue
 * @return the value of knownValue
 */
const QString& XCLParsingProperty::getKnownValue ( )
{
  return knownValue;
}


enum XCLParsingExpression::EXPR_TYPE XCLParsingProperty::getType()
{
  return XCLParsingExpression::PROPERTY_TYPE;
}
