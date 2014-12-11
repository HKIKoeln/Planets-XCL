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
#ifndef XCLPARSETREEFACTORY_H
#define XCLPARSETREEFACTORY_H





/*! \class XCLParseTreeFactory XCLParseTreeFactory.h "XCLParseTreeFactory.h"
 *  \brief The XCLParseTreeFactory creates XCLParseExpressions from XCLSyntaxEpressions.

 * This is a factory class for XCLParseTreeExpressions. 
 * Calling create() with a XCLSyntaxItem will create a XCLParseItem, calling create() with a XCLSyntaxSymbol will create a XCLParseSymbol and so on.

  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */
class XCELFileParserState;
class XCLParsingExpression;
class XCLParsingSymbol;
class XCLParsingItem;
class XCLParsingProperty;

class XCLSyntaxExpression;
class XCLSyntaxSymbol;
class XCLSyntaxItem;
class XCLSyntaxProperty;
class XCLSyntaxProcess;

class XCELProcessorInterface;
class FileParserState;

class XCLParseTreeFactory
{
public:// Constructors/Destructors
  XCLParseTreeFactory ( );
  virtual ~XCLParseTreeFactory ( );
private:
  XCELProcessorInterface* processor;
public:
  XCLParsingSymbol* create(const XCLSyntaxSymbol& syntSymb );
  XCLParsingItem* create(const XCLSyntaxItem& syntItem );
  XCLParsingProperty* create(const XCLSyntaxProperty& syntProp );
  XCLSyntaxExpression* create(  const XCLSyntaxProcess& syntProc, 
                                FileParserState& pItem,
                                XCLSyntaxItem sItem,
                                QHash<QString,XCLSyntaxExpression*>* grammarIndex);

 void setProcessor(XCELProcessorInterface* proc);
private:
  void setExpressionValues(XCLParsingExpression& pExpr, 
			   XCLSyntaxExpression& sExpr);

};

#endif // XCLPARSETREEFACTORY_H
