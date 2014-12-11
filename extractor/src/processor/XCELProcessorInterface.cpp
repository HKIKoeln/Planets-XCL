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
#include "XCELProcessorInterface.h"
#include "XCELProcessingMethod.h"


void XCELProcessorInterface::registerMethod(XCELProcessingMethod* meth)
{
QString trigger(meth->getTrigger());
//std::cout<<"Register method: "<<trigger.toStdString()<<"\n";
methodRegister.insert(meth->getTrigger(),meth);
}

void XCELProcessorInterface::setFactory(XCLParseTreeFactory* fac)
{
pFactory=fac;
}

XCLSyntaxExpression* XCELProcessorInterface::getNewGramm(XCLSyntaxProcess* element,
							QHash<QString,XCLSyntaxExpression*>* grammarIndex,
							FileParserState& state)
{
//Fetch referenced grammatical Expression
  XCLSyntaxExpression* newGramm;
  if (grammarIndex->contains(element->getXcelReference()))
    newGramm=grammarIndex->value(element->getXcelReference());
  else
    throw XCLException("No expression with the name '"+element->getXcelReference()+"' found! Please check the 'XcelRef's in the XCEL-file!");

  newGramm->setParent(NULL);
  QVector<XCLProcessMethod> methods=element->getMethods();


  if(!methods.isEmpty())
    executeGrammarMethods(newGramm,state,element);


//--Fetch End
return newGramm;
}


QByteArray* XCELProcessorInterface::switchToSource(const QString& uri)
{
	QString u = uri;

	// Stops Method, if no file is given (empty stack?)
	if (u.length() == 0)
	{
		std::cout << "No file given!" << "\"\n";
		return NULL;
	}

	//std::cout<<"Try to switch Source \""<<u.toStdString()<<"\"\n";
	if(streamManager->open(u.toStdString()))
	{
		// return the complete file as a QByteArray
		BYTE* ba = streamManager->getStream();
		QByteArray* byteArray = new QByteArray((char*)ba,streamManager->getStreamSize());
		return byteArray;
		// return streamManager->getStream();
	}
	else
	{
		std::cout << "Error opening input file \""<<u.toStdString()<<"\".\nTo extract the input file, ensure that the file path is correct. If the input file name contains specific characters, these must be masked and the inputfile name must be set in quotation marks.\n\n";
		throw XCLException("Exception in XCELProcessorInterface: CStreamReader cannot open input file!");
		return NULL;
	}
}

void XCELProcessorInterface::changeDir(const QString& dir)
{
	streamManager->setPath(dir.toStdString());
}

void XCELProcessorInterface::unzip(const QString& dir)
{
	// Sending filename and Directory to deflate-function
	streamManager->deflate(reader->getFileName().toStdString(),dir.toStdString());
	streamManager->setPath(dir.toStdString());
}