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
#include "XCELSetIdentifierProcessingMethod.h"
#include "XCLSyntaxSymbol.h";

BOOL* XCELSetIdentifierProcessingMethod::getMode()
{
 BOOL mod[]= {FALSE,TRUE,FALSE,FALSE};
 return mod;
}




int XCELSetIdentifierProcessingMethod::expectedParameters()
{
  return 1;
}


BOOL XCELSetIdentifierProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELSetIdentifierProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
    FileParserState* state = reader->getState();
    XCLProcessParameter p=param.at(0);

    if (expr->getElementType()==XCLSyntaxElement::SYMBOL_TYPE 
	|| 
	expr->getElementType() == XCLSyntaxElement::PROPERTY_TYPE)
    {   

        ((XCLSyntaxSymbol*)expr)->setIdentifier(p.getValue(state->index));
    }
    return TRUE;
    // useless statements for less compiler-warnings, params are in use
    (void)vectorHash;
}



BOOL XCELSetIdentifierProcessingMethod::postprocess( XCLParsingItem& parseTree,
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
