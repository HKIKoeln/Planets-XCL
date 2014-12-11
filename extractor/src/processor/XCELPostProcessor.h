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
#ifndef __XCLPOSTPROCESSOR_H
#define __XCLPOSTPROCESSOR_H
#include <QString>
#include <QHash>

#include "XCELProcessorInterface.h"
#include "XCLParsingItem.h"
#include "XCLParsingSymbol.h"
#include "XCLSyntaxItem.h"
#include "XCLProcessParameter.h"

#include "Defines.h"
#include "XCELReader.h"



class XCELPostProcessor : public XCELProcessorInterface
{
public:
	XCELPostProcessor();
	
public:
	~XCELPostProcessor();

        QVector<XCLParsingExpression*>* process() ;

         void postprocess(XCLSyntaxItem* grammar,XCLParsingItem* root);
private:
        QVector<XCLParsingExpression*>* processPushXCELProcessing(
						XCLSyntaxProcess* element, 
						FileParserState& state);
	void executeFileMethods( 	QByteArray* ba,
               			FileParserState& state,
               			XCLSyntaxProcess* element);

       void executeGrammarMethods( XCLSyntaxExpression* expr,
                                 FileParserState& state,
                                 XCLSyntaxProcess* element);

        QVector<XCLParsingExpression*>* processPullXCELProcessing(
					XCLSyntaxProcess* element,
					XCLParsingItem* parent,
					FileParserState& state);
        void processPushBinaryProcessing(
					XCLSyntaxProcess* element,
					QByteArray* binaryArray,
					FileParserState& state);
private:
    XCLTree<XCLParsingExpression*>* index;
    XCLParsingItem* root;
};



#endif
