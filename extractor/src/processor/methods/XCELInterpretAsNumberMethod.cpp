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
#include "XCELInterpretAsNumberMethod.h"
#include "XCLCalculator.h"
#include "XCELProcessorInterface.h"
#include "XCLParsingSymbol.h"

BOOL* XCELInterpretAsNumberMethod::getMode()
{
  BOOL mod[]= {FALSE,TRUE,FALSE,TRUE};
  return mod;
}



int XCELInterpretAsNumberMethod::expectedParameters()
{
  return 2;
}


BOOL XCELInterpretAsNumberMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELInterpretAsNumberMethod::process(	XCELReader* reader,
    XCLSyntaxExpression* expr,
    QVector<XCLProcessParameter> param,
    QHash<QString,IndexedVector*> vectorHash)

{

  FileParserState* state = reader->getState();
  bool check=FALSE;
  XCLProcessParameter p=param.at(0);
  QString b = p.getValue(state->index);
  short base = b.toShort();

  XCLParsingExpression* pExp;// = state->index.get(expr->getIdentifier())->last();
  QList< XCLParsingExpression*>* list = state->index.get(expr->getIdentifier());
  QString iValue="";
  for(int j=0;j<list->size();j++)
  {
    pExp = list->at(j);
    iValue = pExp->getInterpretedValue();




 // QString iValue = pExp->getInterpretedValue();
  QByteArray ba;
  int i=0;
  QString hex;
  for(int i=0;i<iValue.size();i+=2)
  {
    hex = iValue.mid(i,2);
    int a = hex.toInt( &check,base);
    if(a!=0)
    ba.append(a);
  //  std::cout<<"NEXT NUMBER "<<a<<" "<<hex.toStdString()<<" "<<(char) a<<"\n";
  }

  //int a=iValue.toInt(&check,base);

  XCLInputNormalizer normalizer;
  QString* normalized = normalizer.normalizeValue((UCHAR*)ba.data(),ba.size(),
                        "ASCII",
                        TRUE);

  pExp->setInterpretedValue(*normalized);


	/*#ifdef PlanetsDebug
	std::cout<<pExp->getInterpretedValue().toStdString();
	#endif*/
	
  }
/* list = state->index.get(expr->getIdentifier());
	for(int j=0;j<list->size();j++)
	{
		std::cout<<list->at(j)->getInterpretedValue().toStdString()<<"\n";
	}*/
  return check;
  // useless statement for less compiler-warnings, params are in use
  (void)vectorHash;
}


BOOL XCELInterpretAsNumberMethod::postprocess( XCLParsingItem& parseTree,
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
