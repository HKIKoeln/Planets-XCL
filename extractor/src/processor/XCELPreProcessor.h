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
#ifndef __XCLPREPROCESSOR_H
#define __XCLPREPROCESSOR_H
#include <QString>
#include <QHash>

#include "XCELProcessorInterface.h"
#include "XCLParsingItem.h"
#include "XCLParsingSymbol.h"
#include "XCLSyntaxItem.h"
#include "XCLProcessParameter.h"

#include "Defines.h"



class XCELPreProcessor : public XCELProcessorInterface
{
public:

  XCELPreProcessor()
  {
    grammar=NULL;
  }
  ~XCELPreProcessor()
  {
    if(grammar!=NULL)delete grammar;
  }

public:
  XCELReader* preprocess(const XCLSyntaxItem& fileDescriptiongrammar,
		const QString& path, 
		const XCLSyntaxItem& gr);

public://inherited
  QVector<XCLParsingExpression*>* process();

  void executeGrammarMethods(XCLSyntaxExpression* expr,
                             FileParserState& item,
                             XCLSyntaxProcess* element);
  void executeFileMethods( QByteArray* ba,
                           FileParserState& state,
                           XCLSyntaxProcess* element);
void processConfigureParserProcessing(	XCLSyntaxProcess* process,
					FileParserState& state);
private:
  XCLSyntaxItem* grammar;
};




#endif
