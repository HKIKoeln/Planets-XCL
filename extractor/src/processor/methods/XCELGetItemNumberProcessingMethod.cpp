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
#include "XCLParsingSymbol.h"
#include "XCELGetItemNumberProcessingMethod.h"
#include "XCLDataConverter.h"

BOOL* XCELGetItemNumberProcessingMethod::getMode()
{
  BOOL mod[]={FALSE,TRUE,FALSE,FALSE};
  return mod;
}





int XCELGetItemNumberProcessingMethod::expectedParameters()
{
return 1;
}



BOOL XCELGetItemNumberProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use!
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELGetItemNumberProcessingMethod::process(XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
   FileParserState* state = reader->getState();
   XCLTree<XCLParsingExpression*> index = state->index;
   XCLProcessParameter p1=param.at(0);
   XCLProcessParameter p2=param.at(1);
   QString val1 = p1.getValue(state->index);
   QString val2 = p2.getValue(state->index);
   
 
   QList<XCLParsingExpression*>* list = index.get(val1);

   XCLParsingExpression* exp;
 /* for(int i=0;i<list->size();i++)
   {
     std::cout<<list->at(i)->getIdentifier().toStdString()<<"\t";
     std::cout<<list->at(i)->getInterpretedValue().toStdString()<<"\n";
   }*/
#ifdef PlanetsDebug
    std::cout<<"ParamGetItemNumber: "<<val1.toStdString()<<" "<<val2.toStdString()<<"\n";
#endif
    int listIndex=val2.toInt();
    if(listIndex<0 || listIndex >= list->size())
	throw XCLException("XCELGetItemNumberProcessingMethod::process : Parameter "+val2+" not in list range\n");
    exp  = list->at(listIndex);

    if(expr->getElementType()==XCLSyntaxElement::SYMBOL_TYPE)
    {
    #ifdef PlanetsDebug
      std::cout<<"Got Item: "<<exp->getInterpretedValue().toStdString()<<"\n";
    #endif
	    list = index.get(expr->getIdentifier());
	    XCLParsingExpression* tmp=list->last();

	    ((XCLParsingSymbol*)(tmp))->setInterpretedValue(exp->getInterpretedValue());

    }
    return TRUE;
    // useless statements for less compiler-warnings, params are in use!
    (void)vectorHash;
}


BOOL XCELGetItemNumberProcessingMethod::postprocess( XCLParsingItem& parseTree,
							QVector<XCLProcessParameter> param,
							QVector<XCLParsingExpression*> referenceData,
							FileParserState& state,
							QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use!
  (void)parseTree;
  (void)param;
  (void)referenceData;
  (void)state;
  (void)vectorHash;
}
