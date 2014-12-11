/*
    * XCLParseTreeFactory.cpp
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
#include "XCLParsingExpression.h"
#include "XCLParsingProperty.h"
#include "XCLParsingSymbol.h"
#include "XCLParsingItem.h"

#include "XCLSyntaxElement.h"
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxProcess.h"
#include "XCLSyntaxProperty.h"
#include "XCLSyntaxSymbol.h"
#include "XCLSyntaxItem.h"

#include "XCLParseTreeFactory.h"
#include "XCELProcessorInterface.h"

//#include "XCELFileParser.h"
// Constructors/Destructors
//

XCLParseTreeFactory::XCLParseTreeFactory ( )
{
}

XCLParseTreeFactory::~XCLParseTreeFactory ( )
{
}


/**
   * @return XCLParsingSymbol*
   * @param  syntSymb
   */
XCLParsingSymbol* XCLParseTreeFactory::create(const XCLSyntaxSymbol& syntSymb )
{
  XCLParsingSymbol* parsingSymbol = new XCLParsingSymbol();

  XCLSyntaxSymbol syntaxSymbol=syntSymb;
  setExpressionValues(*parsingSymbol,syntaxSymbol);

  parsingSymbol->setInterpretation( syntaxSymbol.getInterpretation());
  parsingSymbol->setValidValues(syntaxSymbol.getValidValues());
  parsingSymbol->setNonValidValues(syntaxSymbol.getNonValidValues());

  parsingSymbol->setSemanticInterpretation(syntaxSymbol.getSemanticInterpretation());

  for(int i=0;i<syntaxSymbol.getFilterChain().size();i++)
    parsingSymbol->addFilter( syntaxSymbol.getFilter(i));

  parsingSymbol->setEncoding( syntaxSymbol.getEncoding());

  /*  if(!parsingSymbol->getSemanticInterpretation().isEmpty())
      parsingSymbol->setInterpretationType( XCLParsingExpression::LABEL);*/
  return parsingSymbol;
}


/**
 * @return XCLParsingItem*
 * @param  syntItem
 */
XCLParsingItem* XCLParseTreeFactory::create(const XCLSyntaxItem& syntItem )
{
  XCLParsingItem* parsingItem = new XCLParsingItem();
  XCLSyntaxItem syntaxItem=syntItem;
  setExpressionValues(*parsingItem,syntaxItem);
  parsingItem->setObjectType(((XCLSyntaxItem&)syntItem).getObjectType());
  return parsingItem;
}


/**
 * @return XCLParsingProperty*
 * @param  syntProp
 */
XCLParsingProperty* XCLParseTreeFactory::create(const XCLSyntaxProperty& syntProp )
{
  XCLParsingProperty* parsingProperty = new XCLParsingProperty();
  XCLSyntaxProperty syntaxProperty=syntProp;
  setExpressionValues(*parsingProperty,syntaxProperty);
  parsingProperty->setValidValues(syntaxProperty.getValidValues());
  parsingProperty->setSemanticInterpretation(syntaxProperty.getSemanticInterpretation());
  parsingProperty->setInterpretation( syntaxProperty.getInterpretation());
  if(!parsingProperty->getSemanticInterpretation().isEmpty())
    parsingProperty->setInterpretationType( XCLParsingExpression::LABEL);
  parsingProperty->setKnownValue(syntaxProperty.getKnownValue());
  return parsingProperty;
}


/**\XCLSyntaxExpression* XCLParseTreeFactory::create(const XCLSyntaxProcess& syntProc, const XCLParsingItem& pItem,XCLSyntaxItem sItem,XCELFileParserState& state, QHash<QString,XCLSyntaxExpression*>* grammarIndex
   \brief Transforms a XCLSyntaxProcess to an XCLSyntaxEpression.
  * \param syntProc the processing instruction element that has to be transformed into a SyntaxExpression
  * \param pItem the parent in the parsing tree
  * \param sItem the root of the grammar structure
  * \param state the XCELFileParserState
  * \param grammarIndex the table holding all grammar elements, accesible by their ID.
  * \return XCLSyntaxExpression*
 
A processing instruction is used to either push information onto an existing XCLSyntaxExpression or pull a new XCLSyntaxExpression into the parse tree. 
The XCLSyntaxProcess in either case referrs to the particular XCLSyntaxExpression by its ID. \n
This function retrieves the referred element from the XCELFileParser::grammarIndex and calls XCLProcessor::process() for further processing.
  */
XCLSyntaxExpression* XCLParseTreeFactory::create(
    const XCLSyntaxProcess& syntProc,
    FileParserState& pItem,
    XCLSyntaxItem sItem,
    QHash<QString,XCLSyntaxExpression*>* grammarIndex
    )
{
  throw XCLException("Someone has called deprecated Method\n");
  /*  XCLSyntaxItem syntaxItem=sItem;
      XCLSyntaxProcess sProc=syntProc;
      XCLSyntaxExpression* expr;
   
      if (grammarIndex->contains(sProc.getXcelReference()))
        expr=grammarIndex->value(sProc.getXcelReference());
   
      else
        throw XCLException("No expression with the name '"+sProc.getXcelReference()+"' found! Please check the 'XcelRef's in the XCEL-file!");
   
      expr->setParent(NULL);
      QVector<XCLProcessMethod> methods=sProc.getMethods();
   
      QVector<XCLParsingExpression*> refData;
   
   
      if(!methods.isEmpty())
        processor->process(expr,pItem,methods,refData);
      return expr;*/
  
  // Unused vars, no compiler-warning, because "in use", USELESS BLOCK!!!
  (void)syntProc;
  (void)pItem;
  (void)sItem;
  (void)grammarIndex;
}


void XCLParseTreeFactory::setExpressionValues(XCLParsingExpression& pExpr,
    XCLSyntaxExpression& sExpr)
{
  pExpr.setEnd( sExpr.getEnd());
  pExpr.setStart(sExpr.getStart());
  pExpr.setLength( sExpr.getLength());
  pExpr.setIdentifier( sExpr.getIdentifier());
  pExpr.setName( sExpr.getName());
  pExpr.setOriginalName( sExpr.getOriginalName());
  pExpr.setIsBigEndian( sExpr.getIsBigEndian());
  pExpr.setInterpretationType(XCLParsingExpression::NUMERIC);
  pExpr.setValueType(sExpr.getValueType());
//  pExpr.setFilterChain(sExpr.getFilterChain());
  pExpr.setPrintMe(sExpr.getPrintMe());
  //pExpr.setValueType(valueType);
    for(int i=0;i<sExpr.getFilterChain().size();i++)
      pExpr.addFilter(sExpr.getFilter(i));
  pExpr.setIsExternalDefined(sExpr.getIsExternalDefined());
  pExpr.setExternalXCDL(sExpr.getExternalXCDL());
  pExpr.setNormDataRelation(sExpr.getNormDataRelation());
  pExpr.setRelations(sExpr.getRelations());
  pExpr.setObjectType( sExpr.getObjectType());
  pExpr.setNewObject( sExpr.getNewObject());

}

void XCLParseTreeFactory::setProcessor(XCELProcessorInterface* proc)
{
  processor=proc;
}
