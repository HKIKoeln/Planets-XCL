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
#include "XCELSetInternalSourceProcessingMethod.h"

BOOL* XCELSetInternalSourceProcessingMethod::getMode()
{
 BOOL mod[]= {FALSE,TRUE,FALSE,FALSE};
  return mod;
}




int XCELSetInternalSourceProcessingMethod::expectedParameters()
{
  return 1;
}


BOOL XCELSetInternalSourceProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELSetInternalSourceProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{

  if(reader == NULL)
	  throw XCLException("XCELSetInternalSourceException.process() : NULL is not a Reader");
  if(expr == NULL)
	  throw XCLException("XCELSetInternalSourceException.process() : NULL is not an Expression");

  XCLProcessParameter p=param.at(0);

  FileParserState* state = reader->getState();

  expr->setSource( p.getValue(state->index).trimmed());
  return TRUE;
  // useless statements for less compiler-warnings, params are in use
  (void)vectorHash;
}



BOOL XCELSetInternalSourceProcessingMethod::postprocess(XCLParsingItem& parseTree,
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
