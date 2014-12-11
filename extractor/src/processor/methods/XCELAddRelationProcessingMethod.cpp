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
#include "XCELAddRelationProcessingMethod.h"
#include "XCLParsingSymbol.h"
#include "XCLParsingItem.h"
#include "XCLDataConverter.h"
#include "XCLInputNormalizer.h"

BOOL* XCELAddRelationProcessingMethod::getMode()
{
  BOOL mode[]={FALSE,TRUE,FALSE,FALSE};
  return mode;
}





int XCELAddRelationProcessingMethod::expectedParameters()
{
  return 0;
}


BOOL XCELAddRelationProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, 
					       const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use!
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELAddRelationProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
    expr->cleanRelations();
    XCLFilter filter;
    QString nameStr;
    QString valueStr;
    FileParserState* state = reader->getState();
    XCLProcessParameter queryNode=param.at(0);
    XCLProcessParameter value=param.at(1);
    QString queryNodeString = queryNode.getValue(state->index);
    QString valueString = value.getValue(state->index);
   // std::cout<<"Query Node: "<<queryNodeString.toStdString()<<" with "<<valueString.toStdString()<<"\n";

    QList<XCLParsingExpression*>* list = state->index.get(queryNodeString);
    for(int i=0;i<list->size();i++)
    {
	XCLParsingExpression* xprL =list->at(i);
     //   std::cout<<xprL->getIdentifier().toStdString()<<"\n"; 
	if(xprL->containsValue(valueString))
	{
		expr->addRelation(xprL);
		
	//	std::cout<<"MAtch\n";
		break;
	}
    }

 //  std::cout<<"Number of relations: "<<expr->getRelations().size()<<"\n";	
   return TRUE;
  // useless statements for less compiler-warnings, params are in use!
  (void)vectorHash;
}




BOOL XCELAddRelationProcessingMethod::postprocess( XCLParsingItem& parseTree,
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
