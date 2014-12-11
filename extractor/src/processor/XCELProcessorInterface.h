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
#ifndef __XCELPROCESSORINTERFACE_H
#define __XCELPROCESSORINTERFACE_H
#include <QHash>
#include <QByteArray>
#include "XCLParseTreeFactory.h"

#include "XCELReader.h"
#include "XCLParsingItem.h"
#include "XCLProcessMethod.h"
class XCELProcessingMethod;
#include "XCELReader.h"
#include "XCLParseTreeFactory.h"
#include "CStreamManager.h"


/*! \class XCLProcessor XCLProcessor "XCLProcessor"
 *  \brief A class which executes processing methods on an existing XCLSyntaxExpression.
 
The class has one public function process(), which manages the processing of methods by calling the appropriate private function to perform the particular task.
 
  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */
class IndexedVector
{
public:
  _UINT32 index;
  QList<XCLParsingExpression*>* vector;

  void print()
  {
    std::cout<<"Index: "<<index<<"\n";
    /*
    #ifdef PlanetsDebug
    for(int i=0;i<vector->size();i++)
    {
    std::cout<<"Identifier "<<vector->at(i)->getIdentifier().toStdString()<<"\n";
    std::cout<<"Value "<<vector->at(i)->getInterpretedValue().toStdString()<<"\n";
    }
    #endif
    */
  }
};


class XCELProcessorInterface
{
	public:
		XCELProcessorInterface()
		{
			pFactory = new XCLParseTreeFactory();
			pFactory->setProcessor(this);
			reader = new XCELReader(this);
			streamManager = new CStreamManager();
		}
		
		XCELProcessorInterface(XCELReader* r)
		{
			pFactory = new XCLParseTreeFactory();
			pFactory->setProcessor(this);
			reader = r;
		}
		
		virtual ~XCELProcessorInterface()
		{
			delete reader;
			delete streamManager;
		}
		
		virtual QVector<XCLParsingExpression*>* process() = 0;

		virtual void executeGrammarMethods(XCLSyntaxExpression* expr,
			FileParserState& item,
			XCLSyntaxProcess* element)=0;


		/* //processing for binary operations
		virtual void executeFileMethods( QByteArray* ba,
			FileParserState& state,
			XCLSyntaxProcess* element)=0;
		*/
	public:
		void registerMethod(XCELProcessingMethod* meth);
		void setFactory(XCLParseTreeFactory* fac);

		QByteArray* switchToSource(const QString& uri);
		void changeDir(const QString& dir);
		void unzip(const QString& dir);

	protected:
		XCLSyntaxExpression* getNewGramm(XCLSyntaxProcess* element,
		QHash<QString,XCLSyntaxExpression*>* grammarIndex,
		FileParserState& state);

		QHash<QString,XCELProcessingMethod*> methodRegister;
		XCLParseTreeFactory* pFactory;
		QHash<QString,IndexedVector*> vectorHash;
		XCELReader* reader;
		XCLInputNormalizer normalizer;
		_LENGTH binaryArrayLength;
		CStreamManager* streamManager;
};

#endif
