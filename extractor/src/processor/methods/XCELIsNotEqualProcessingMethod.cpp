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
#include "XCELIsNotEqualProcessingMethod.h"
#include "XCLCalculator.h"
#include "XCELProcessorInterface.h"

BOOL* XCELIsNotEqualProcessingMethod::getMode()
{
  BOOL mod[]= {FALSE,TRUE,FALSE,TRUE};
  return mod;
}



int XCELIsNotEqualProcessingMethod::expectedParameters()
{
  return 2;
}


BOOL XCELIsNotEqualProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELIsNotEqualProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)

{
    
//         std::cout<<"\nEnter ISNOTEQUAL\n";
        FileParserState* state = reader->getState();
        XCLProcessParameter refP=param.at(0);
        QString refVal = refP.getValue(state->index);
        
        XCLProcessParameter p=param.at(1);
        QString val = p.getValue();
        
        //   std::cout<<"\n"<<val.toStdString()<<" == "<<refVal.toStdString()<<"\n";
        return refVal != val;
        // useless statements for less compiler-warnings, params are in use
        (void)expr;
        (void)vectorHash;
}


BOOL XCELIsNotEqualProcessingMethod::postprocess( XCLParsingItem& parseTree,
							QVector<XCLProcessParameter> param,
							QVector<XCLParsingExpression*> referenceData,
							FileParserState& state,
							QHash<QString,IndexedVector*> vectorHash)
{
//     std::cout<<"\nEnter ISNOTEQUAL POSTPROCESSING\n";
    QString refVal = referenceData.at(0)->getInterpretedValue();
    XCLProcessParameter p=param.at(0);
    QString val = p.getValue();
//     std::cout<<"\n"<<val.toStdString()<<" == "<<refVal.toStdString()<<"\n";
    return refVal == val;
    // useless statements for less compiler-warnings, params are in use
    (void)parseTree;
    (void)state;
    (void)vectorHash;
}
