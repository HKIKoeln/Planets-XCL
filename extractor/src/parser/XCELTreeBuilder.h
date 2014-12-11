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
#ifndef XCELTREEBUILDER_H
#define XCELTREEBUILDER_H
#include "Defines.h"
#include "XCLSyntaxItem.h"
#include "XCLParsingItem.h"
#include "XCLTree.h"
#include <QDomDocument>
/*! \class XCELTreeBuilder XCELTreeBuilder.h "XCELTreeBuilder.h"
 *  \brief The XCELTreeBuilder creates a Parsing Tree of the binary file you want to process.

 This is achieved in three steps:\n
 \li 1. create a DOM representation of the corresponding XCEL
 \li 2. create a grammar from the DOM representation, i.e. translate the XML Elements to Objects from the basic type XCLSyntaxExpression
 \li 3. read the information from the binary file into the grammar objects.

  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */

class XCELTreeBuilder
{
public:
    XCELTreeBuilder();//!< Constructs the XCELTreeBuilder.

    ~XCELTreeBuilder();
    const XCLParsingItem& build(const QString& xcelfile, 
				const QString& binfile);
    const XCLParsingItem& getDebugTree()
{
return debugTree;
}
private:
  XCLParsingItem root;
  QString xcelFileName; //!< The name of the approprioate XCEL File to process the binary data.
  QString binaryFileName; //!< The name of the binary file to process. 
  QDomDocument xcelDOM; //!< The DOM representation of the XCEL file.

  BOOL validate(const QString& xcelFilename);
  XCLParsingItem debugTree;
};

#endif
