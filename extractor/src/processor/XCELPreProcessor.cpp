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
#include <iostream>
#include "XCLException.h"
#include <QVector>
#include "XCLSyntaxSymbol.h"
#include "XCLSyntaxElement.h"
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxItem.h"
#include "XCLSyntaxProperty.h"
#include "XCLSyntaxProcess.h"
#include "XCLDataConverter.h"
#include "XCLParsingItem.h"
#include "XCELProcessingMethod.h"

#include "XCELPreProcessor.h"

QVector<XCLParsingExpression*>* XCELPreProcessor::process()
{

  QVector<XCLParsingExpression*>*  expressions=new QVector<XCLParsingExpression*>();;
  FileParserState* state = reader->getState();
  XCLSyntaxProcess* element=(XCLSyntaxProcess*)state->actElement;
  XCLParsingItem* parent = state->parent;
  QByteArray* binaryArray = reader->getBinaryArray();

  enum XCLSyntaxProcess::PROCESS_TYPE type=element->getType();
  if(type == XCLSyntaxProcess::CONFIGPARSER)
  {

    processConfigureParserProcessing(element,*state);
  }
  /*else if(type==XCLSyntaxProcess::PULLXCEL)
  {
    delete expressions;
    expressions=processPullXCELProcessing(element,parent,*state);
  }*/
 /* else if(type == XCLSyntaxProcess::PUSHBINARY)
  {
    processPushBinaryProcessing(element,binaryArray,*state);
  }*/
 
  //  throw XCLException(QString("Process Type is not supported ")+type+" \n");
 
  return expressions;
  // useless statements for less compiler-warnings, vars ARE in use
  (void)parent;
  (void)binaryArray;
}

void XCELPreProcessor::executeGrammarMethods(XCLSyntaxExpression* expr,
    FileParserState& item,
    XCLSyntaxProcess* element)
{
  QVector<XCLProcessMethod> m=element->getMethods();
  _LENGTH size = m.size();
   XCELProcessingMethod* meth;
  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
    QList<XCELProcessingMethod*> meths = methodRegister.values(name);
    if(meths.size()>1)
    {
      //anzahl der Parameter prüfen
    }
    else
    {
    meth = meths.at(0);
    if(!meth->preprocess(reader,*grammar,*element))
	throw XCLException(m[i].getName());
    }
  }
  // useless statements for less compiler-warnings, vars ARE in use
  (void)expr;
  (void)item;
}


void XCELPreProcessor::executeFileMethods( QByteArray* ba,
    FileParserState& state,
    XCLSyntaxProcess* element)
{
    // ### Diese Methode ist tatsaechlich noch einmal diskussionswuerdig bzw. muss ausgebaut werden.
    // Bis dahin werden die Parameter gecastet
    // Useless statements for less compiler-warnings, Params are not in use in the moment
    (void)ba;
    (void)state;
    (void)element;
}

XCELReader* XCELPreProcessor::preprocess(const XCLSyntaxItem& fileDescriptionGrammar,
					const QString& path, 
					const XCLSyntaxItem& gr)
{
  grammar=new XCLSyntaxItem(gr); 
  //std::cout<<path.toStdString()<<"\n";
  reader->setFileName(path);
  reader->parseFile("",gr);
  return reader;
  // useless statement for less compiler-warnings, param IS in use
  (void)fileDescriptionGrammar;
}

void XCELPreProcessor::processConfigureParserProcessing(XCLSyntaxProcess* process,
							FileParserState& state)
{
  QVector<XCLProcessMethod> m=process->getMethods();
  _LENGTH size = m.size();
  XCELProcessingMethod* meth;
  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
  //  std::cout<<name.toStdString()<<"\n";
    QList<XCELProcessingMethod*> meths = methodRegister.values(name);
    if(meths.size()==0)
	{
		std::cout<<"No Method with name: "<<name.toStdString()<<" found\n";
		return;
	}
    if(meths.size()>1) //Method overloading
    {
      //anzahl der Parameter prüfen
    }
    else
    {
      meth = meths.at(0);
      meth->preprocess(reader,*reader->getGrammar(),*process);
    }
  }
  //reader->setSymbolDoesNotMatchHandle(new SkipWhiteSpaceHandle());
  // useless statement for less compiler-warnings, param IS in use
  (void)state;
}


