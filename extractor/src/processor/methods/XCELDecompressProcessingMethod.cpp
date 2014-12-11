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
#include "XCELDecompressProcessingMethod.h"
#include "XCLDataConverter.h"

BOOL* XCELDecompressProcessingMethod::getMode()
{
  BOOL mod[]={FALSE,TRUE,FALSE,FALSE};
  return mod;
}





int XCELDecompressProcessingMethod::expectedParameters()
{
return 3;
}



BOOL XCELDecompressProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use!
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELDecompressProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
   //TODO: check parameters !!
   //Test code for zlibDeflateInflate 
    FileParserState state = *reader->getState();
    XCLProcessParameter p=param.at(0);
    QString compressionMethod =p.getValue(state.index);
    p=param.at(1);
    QString startStr = p.getValue(state.index);
    p=param.at(2);
    QString endStr = p.getValue( state.index); 
 
    _LENGTH start= startStr.toLong( );
    _LENGTH end= endStr.toLong();
 
    QByteArray* ba = reader->getBinaryArray();
    QByteArray head = ba->left(start);
    QByteArray body = ba->mid(start,end);
    QByteArray tail = ba->mid(end,ba->size()); 

    XCLDataConverter convert;

    QVector<XCLFilter> filterChain;
    XCLFilter filter;
    filter.setFilterName("compression");
    filter.setFilterValue("zlibDeflateInflate"); 
    filterChain.append(filter);
    QByteArray cvalue=convert.filter(&body, filterChain);
    delete ba;
    ba =new QByteArray();
    *ba+=head;
    *ba+=body;
    *ba+=tail;
     return TRUE;
    // useless statements for less compiler-warnings, params are in use!
    (void)expr;
    (void)vectorHash;
}


BOOL XCELDecompressProcessingMethod::postprocess( XCLParsingItem& parseTree,
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

