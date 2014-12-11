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
#include "XCELValueIsNotInIndexProcessingMethod.h"
#include "XCLParsingSymbol.h"
#include "XCLParsingItem.h"
#include "XCLDataConverter.h"
#include "XCLInputNormalizer.h"

BOOL* XCELValueIsNotInIndexProcessingMethod::getMode()
{
  BOOL mode[]={FALSE,TRUE,FALSE,FALSE};
  return mode;
}





int XCELValueIsNotInIndexProcessingMethod::expectedParameters()
{
  return 0;
}


BOOL XCELValueIsNotInIndexProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar,
    const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELValueIsNotInIndexProcessingMethod::process(	XCELReader* reader,
    XCLSyntaxExpression* expr,
    QVector<XCLProcessParameter> param,
    QHash<QString,IndexedVector*> vectorHash)
{

  try
  {
    QString valueStr;
    FileParserState* state = reader->getState();
    XCLProcessParameter searchFor=param.at(0);
    XCLProcessParameter content=param.at(1);

    QString searchForString = searchFor.getValue(state->index);
    QString contentString = content.getValue(state->index);
    // std::cout<<"ValueIsNotInIndex\n";
    QList<XCLParsingExpression*>* result = state->index.get(searchForString);
    // std::cout<<"get succeeded\n";
    int count=0;

    //  std::cout<<"searchForString "<<searchForString.toStdString()<<"\n";
    //  std::cout<<"contentString "<<contentString.toStdString()<<"\n";
    for(int i=0;i<result->size();i++)
    {
    //  	std::cout<<"compare "<<result->at(i)->getInterpretedValue().toStdString()<<"\n";
      if(result->at(i)->getInterpretedValue() == contentString)
      {
        if(count > 0)
          return FALSE;
        count++;
      }
    }
    
     // std::cout<<"ValueIsNOt TRUE\n";
  }
  catch(XCLException exception)
  {
//     std::cout<<"ValueIsNotInIndex says:\n";
    exception.message();
  }
  //std::cout<<"ValueIsNOt TRUE\n";
  return TRUE;
  // useless statements for less compiler-warnings, params are in use
  (void)expr;
  (void)vectorHash;
}




BOOL XCELValueIsNotInIndexProcessingMethod::postprocess( XCLParsingItem& parseTree,
    QVector<XCLProcessParameter> param,
    QVector<XCLParsingExpression*> referenceData,
    FileParserState& state,
    QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)parseTree;
  (void)param;
  (void)referenceData;
  (void)state;
  (void)vectorHash;
}
