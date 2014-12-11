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
#include "XCELUnzipProcessingMethod.h"
#include "XCLDataConverter.h"

BOOL* XCELUnzipProcessingMethod::getMode()
{
  BOOL mod[]={TRUE,FALSE,FALSE,FALSE};
  return mod;
}

int XCELUnzipProcessingMethod::expectedParameters()
{
  return 1;
}


BOOL XCELUnzipProcessingMethod::preprocess(XCELReader* reader, 
					   XCLSyntaxItem& grammar, 
					   const XCLSyntaxProcess& preprocessing)
{
  std::cout<<"CALL UNZIP\n";
  XCLSyntaxProcess process = preprocessing;
  QVector<XCLProcessMethod> methods = process.getMethods();
  XCLProcessMethod method = methods.at(0);
  QVector<XCLProcessParameter> parameters = method.getParameters();

  if(parameters.size() == 1)
  {
    XCLProcessParameter param = parameters.at(0);
    QString targetDir = param.getValue();
    std::cout<<targetDir.toStdString()<<"\n";
    reader->getProcessor()->unzip(targetDir);
    reader->setFileName(targetDir);
  }

  return TRUE;
  // useless statements for less compiler-warnings, params are in use
  (void)grammar;
}


BOOL XCELUnzipProcessingMethod::process(XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)expr;
  (void)param;
  (void)vectorHash;
}


BOOL XCELUnzipProcessingMethod::postprocess( XCLParsingItem& parseTree,
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
