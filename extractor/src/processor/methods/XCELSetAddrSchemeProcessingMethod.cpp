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
#include "XCELSetAddrSchemeProcessingMethod.h"
#include "XCLCalculator.h"
#include "XCELProcessorInterface.h"

BOOL* XCELSetAddrSchemeProcessingMethod::getMode()
{
  BOOL mod[]= {FALSE,TRUE,FALSE,TRUE};
  return mod;
}



int XCELSetAddrSchemeProcessingMethod::expectedParameters()
{
  return 2;
}


BOOL XCELSetAddrSchemeProcessingMethod::preprocess(	XCELReader* reader, 
							XCLSyntaxItem& grammar, 
							const XCLSyntaxProcess& preprocessing)
{
  XCLSyntaxProcess proc = preprocessing;
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
}


BOOL XCELSetAddrSchemeProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)

{
  XCLProcessParameter p1 = param.at(0);

  if(param.size() > 1)
  {
  XCLProcessParameter p2 = param.at(1);
  reader->setAddressingScheme(p1.getValue(),p2.getValueAsBA(NULL));
  }
  else
  {
  reader->setAddressingScheme(p1.getValue());
  }
  return TRUE;
  // useless statements for less compiler-warnings, params are in use
  (void)expr;
  (void)vectorHash;
}


BOOL XCELSetAddrSchemeProcessingMethod::postprocess( XCLParsingItem& parseTree,
							QVector<XCLProcessParameter> param,
							QVector<XCLParsingExpression*> referenceData,
							FileParserState& state,
							QHash<QString,IndexedVector*> vectorHash)
{
  // ### Ich weiss nicht, ob dies richtig ist, aber wir werden sehen...
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)parseTree;
  (void)param;
  (void)referenceData;
  (void)state;
  (void)vectorHash;
}
