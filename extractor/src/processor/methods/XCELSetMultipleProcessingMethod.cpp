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
/*#include "XCELSetMultipleProcessingMethod.h"
#include "XCLSyntaxSymbol.h";

BOOL* XCELSetMultipleProcessingMethod::getMode()
{
 BOOL mod[]= {FALSE,TRUE,FALSE,FALSE};
 return mod;
}




int XCELSetMultipleProcessingMethod::expectedParameters()
{
}


BOOL XCELSetMultipleProcessingMethod::preprocess(XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
}


BOOL XCELSetMultipleProcessingMethod::process(	XCLSyntaxExpression* expr,
                                QVector<XCLProcessParameter> param,
                                FileParserState& state,QHash<QString,IndexedVector*> vectorHash)
{
XCLProcessParameter p=param.at(0);

    
        #ifdef PlanetsDebug
        std::cout<<"setMultiple: "<<p.getValue(state.index).toStdString()<<std::endl;
        #endif
        if (p.getValue(state.index) == "true" )
            expr->setIsMultiple(TRUE);
        else
            expr->setIsMultiple(FALSE);
    expr->print();
        std::cout<<"************************"<<std::endl;

  return TRUE;
}


BOOL XCELSetMultipleProcessingMethod::process(QByteArray* ba,   			//the complete file
				QVector<XCLProcessParameter> param,
                                FileParserState& state,QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
}


BOOL XCELSetMultipleProcessingMethod::postprocess( XCLParsingItem& parseTree,
							QVector<XCLProcessParameter> param,
							QVector<XCLParsingExpression*> referenceData,
							FileParserState& state,
							QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
}
*/