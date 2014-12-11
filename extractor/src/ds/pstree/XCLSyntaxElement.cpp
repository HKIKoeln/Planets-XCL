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
#include "XCLSyntaxElement.h"
#include "XCLSyntaxItem.h"
// Constructors/Destructors
//

XCLSyntaxElement::XCLSyntaxElement ()
{
  structuralParent=NULL;
  printme=FALSE;
}


/**
* Virtual Destructor
*/
XCLSyntaxElement::~XCLSyntaxElement ( )
{}


/**
  * Copy Constructor
*/
XCLSyntaxElement::XCLSyntaxElement(const XCLSyntaxElement& elem)
{
  structuralParent=elem.structuralParent;
  printme=elem.printme;
  source=elem.source;
  extSource=elem.extSource;
}


/**
* Copy Operator
*/
XCLSyntaxElement& XCLSyntaxElement::operator=(const XCLSyntaxElement& elem)
{
  if(this!=&elem)
  {
    structuralParent=elem.structuralParent;
    printme = elem.printme;
    source=elem.source;
    extSource=elem.extSource;
}
  return *this;
}


void XCLSyntaxElement::print(QString format)
{
  //if(structuralParent!=NULL)
  //std::cout<<format.toStdString()<<"Parent : "<<structuralParent->getIdentifier().toStdString()<<"\n";
  
  //useless statement for less compiler-warnings, format is NOT in use NOW!
  (void)format;
}


/*! \void XCLSyntaxElement::setParent ( XCLSyntaxItem* item )
 *  \brief Set the value of structuralParent.
 *  \param item The parent XCLSyntaxItem.
 
 
 */
void XCLSyntaxElement::setParent ( XCLSyntaxItem* item )
{

  if(item==NULL || item->getItemType()==XCLSyntaxItem::STRUCTURING)
    structuralParent = item;
  else
    structuralParent = item->getParent();
}


/**
 * Get the value of structuralParent
 * @return the value of structuralParent
 */
XCLSyntaxItem* XCLSyntaxElement::getParent ( )
{
  return structuralParent;
}

void XCLSyntaxElement::setPrintMe(const QString& p)
{
if(p=="true")
	printme=TRUE;
}
BOOL XCLSyntaxElement::getPrintMe()
{
return printme;
}


