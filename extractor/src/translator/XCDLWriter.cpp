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
#include "XCDLWriter.h"
#include "XCLDOMBuilder.h"
#include "XCLStringConverter.h"
#include "XCLParsingSymbol.h"
#include "XCDLWriterFactory.h"
#include "XCLDataConverter.h"
#include "XCDLTreeBuilder.h"
#include "XCDLDomBuilder.h"
#include "XCDLObject.h"

XCDLWriter::XCDLWriter()
{
  displayRawData = FALSE;
  bDisplayNormData = TRUE;
}


XCDLWriter::~XCDLWriter()
{}


/*! \void XCDLWriter::write(const XCLParsingItem& pT, const QString& xcdlFilename)
 *  \brief .
 
 *  \param pt The parsing tree, which has to be transformed into a XCDL-file.
 * \param xcdlFilename the name of the XCDL-file created from the parsing tree.
 
 
 */
QDomDocument XCDLWriter::write(const XCLParsingItem& pT, const QString& xcdlFilename)
{
	XCDLTreeBuilder builder;
	XCDLDomBuilder domBuilder;
	// ### Diese Zeile waere evenuell ein Argument, den XCDLTreeBuilder bereits im
	// Header zu definieren, um den State hier nicht zwischenzuspeichern, sondern direkt
	// durchzureichen.
	builder.setDisplayNormData(bDisplayNormData);
	builder.create(pT,definedProperties);
// 	builder.print();
	// ### Hier nochmal ueberpruefen, ob die naechste Zeile wirklich auskommentiert werden kann.
	// XCDLObject *xcdlObject;
        QVector<XCDLObject*> 	  fileObjects=builder.getFileObjects();
//         int fileObjectsSize=(builder.getFileObjects()).size();
//         for(int i=0;i<fileObjectsSize;i++)
//         {
//             xcdlObject=fileObjects[i];
//         }

//       QVector<XCDLProperty> properties= builder.getProperties();
//       for(int i =0;i<properties.size();i++)
//       {
//           XCDLProperty property= properties.at(i);
// //           property.print(); 
//           QDomElement propertyElement;
//           QString name=property.getName();
// 	  std::cout<<"\n"<<name.toStdString();
//        }

        
        QDomDocument dom = domBuilder.create( xcdlFilename,
					      builder.getNormData(),
					      builder.getProperties(),
					      builder.getPropertySets(),
					      builder.getFileObjects(),
					      builder.getObjectType());
	return dom;
}


void XCDLWriter::setDefinedProperties(const QVector<NamesLibProperty *> defProps)
{
	definedProperties=defProps;
}

void XCDLWriter::setDisplayRawData(BOOL on)
{
		displayRawData=on;
}

void XCDLWriter::setDisplayNormData(BOOL bDisplayNormDataNewState)
{
	bDisplayNormData = bDisplayNormDataNewState;
}

