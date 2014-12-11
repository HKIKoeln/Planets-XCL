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
#ifndef XCELMAINPROCESSOR_H
#define XCELMAINPROCESSOR_H

#include "XCELProcessorInterface.h"
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxSymbol.h"
#include "XCLParsingItem.h"
#include "XCLProcessMethod.h"
#include "XCLProcessParameter.h"
#include "XCLInputNormalizer.h"
#include "XCELProcessorInterface.h"
#include "CStreamManager.h"
#include <QFileInfo>
class FileParserState;



class XCELMainProcessor : public XCELProcessorInterface
{
public:

  XCELMainProcessor()
  {
    grammar=NULL;
  }
  ~XCELMainProcessor()
  {
    if(grammar!=NULL)delete grammar;
  }
private://members

public:


  const XCLParsingItem& mainprocess(XCELReader* re, const XCLSyntaxItem& gr);


  const XCLParsingItem& getDebugTree()
  {
    return reader->getDebugTree();
  }

  QVector<XCLParsingExpression*>* process() ;
  //processing for XCEL operations

  void executeGrammarMethods(XCLSyntaxExpression* expr,
                             FileParserState& item,
                             XCLSyntaxProcess* element);
  //processing for binary operations
  void executeFileMethods( QByteArray* ba,
                           FileParserState& state,
                           XCLSyntaxProcess* element);

private:
  QVector<XCLParsingExpression*>* processPushXCELProcessing(
    XCLSyntaxProcess* element,
    FileParserState& state);

  QVector<XCLParsingExpression*>* processPullXCELProcessing(
    XCLSyntaxProcess* element,
    XCLParsingItem* parent,
    FileParserState& state);
  void processPushBinaryProcessing(
    XCLSyntaxProcess* element,
    QByteArray* binaryArray,
    FileParserState& state);
  void processConfigureParserProcessing(	XCLSyntaxProcess* process,
                                         FileParserState& state);


private:
  XCLSyntaxItem* grammar;
  QString actFilePath;
  QString actFileName;
};

#endif
