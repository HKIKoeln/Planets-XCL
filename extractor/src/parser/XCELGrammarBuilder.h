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
#ifndef XCELGRAMMARBUILDER_H
#define XCELGRAMMARBUILDER_H
#include "XCLSyntaxItem.h"
class QDomDocument;
class QDomElement;
/*! \class XCELGrammarBuilder XCELGrammarBuilder.h "XCELGrammarBuilder.h"
 *  \brief The XCELGrammarBuilder translates the XCEL-File into XCLSyntaxExpressions, i.e. Items, Symbols, Properties and Processing instructions.

    The class contains a pointer to the DOM representation of the XCEL-File #myDOM, which is initialized when a new grammar is build.\n 
    The XCLGrammarBuilder is called by the XCELTreeBuilder before reading the binary file.\n
    The function buildGrammar() returns a pointer to the first XCLSyntaxItem of the grammar.\n
    Every XCLSyntaxItem contains a QVector list with all of its children.

    @author Jan Schnasse <jan.schnasse@uni-koeln.de>
*/

class XCELGrammarBuilder
{
public:
    XCELGrammarBuilder();//!< constructor. Initializes the #grammar root object with an empty XCLSyntaxItem and sets the #nextStartposition to 0.
    ~XCELGrammarBuilder();
     const XCLSyntaxItem& buildGrammar(QDomDocument* aDOM);
     const XCLSyntaxItem& buildPreProcessingGrammar(QDomDocument* aDOM);
     const XCLSyntaxItem& buildPostProcessingGrammar(QDomDocument* aDOM);
private:
void buildItem(QDomElement* itemElement,
				   XCLSyntaxItem* itemToAdd);
void buildSymbol(QDomElement* symbolElement,
				   XCLSyntaxItem* itemToAdd);
void buildProperty(QDomElement* propertyElement,
				   XCLSyntaxItem* itemToAdd);
void buildProcessing(QDomElement* element,
    XCLSyntaxItem* itemToAdd);
void setExpressionMembers(QDomElement* element,
				XCLSyntaxExpression* expr);
QString getDirectChildText(QDomElement element,QString search);
QDomElement getDirectChildElement(QDomElement element,QString search);
const QString& normalize(QString str);

private:
 QDomDocument* myDOM;  //!< A pointer to the DOM representation of the XCEL-File used for processing
  XCLSyntaxItem* grammar; ///< A pointer to the first XCLSyntaxItem of the grammar strcuture.
    _UINT32 nextStartposition; //!< Stores the startposition of the current item to build in an unsigned int 32 byte.

_LENGTH gId;
};

#endif


