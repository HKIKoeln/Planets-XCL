/*
    * This module is part of the XCL software system
    *
    * Written by  Volker Heydegger, Johanna Neumann,  Jan Schnasse, Manfred Thaller 2006 - 2007
    *
    * Copyright (C)  2006 - 2007 Volker Heydegger, Johanna Neumann,  Jan Schnasse, Manfred Thaller
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
#include "XCELMainProcessor.h"
#include "XCLStringConverter.h"
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
#include "Defines.h"
#include "XCLDataConverter.h"
#include "XCLCalculator.h"
#include "XCLFilter.h"
#include "XCELReader.h"
#include "XCLInputNormalizer.h"
#include "XCLSyntaxProcess.h"
#include "XCELProcessingMethod.h"



const XCLParsingItem& XCELMainProcessor::mainprocess(XCELReader* re, const XCLSyntaxItem& gr)
{
	reader=re;
	reader->setProcessor(this);
	grammar=new XCLSyntaxItem(gr);
	
	actFilePath = reader->getFileName();
	std::cout<<actFilePath.toStdString()<<"\n";
	QFileInfo file(actFilePath);
	
	if(!file.isDir())
	{
		actFileName = file.fileName();
		actFilePath = file.path();
	}
    // streamManager->setPath(actFilePath);
	streamManager->setPathApp(actFilePath.toStdString());
	
	if(!file.isDir())
		streamManager->open(actFileName.toStdString());

	#ifdef PlanetsDebug
	std::cout<<"File Path "+actFilePath.toStdString()<<"\n";
	std::cout<<"File Name "+actFileName.toStdString()<<"\n";
	#endif
	
	return reader->parseFile(actFileName,gr);
}



QVector<XCLParsingExpression*>* XCELMainProcessor::process()
{
  FileParserState* state = reader->getState();
  XCLSyntaxProcess* element=(XCLSyntaxProcess*)state->actElement;
  XCLParsingItem* parent = state->parent;
  QByteArray* binaryArray = reader->getBinaryArray();

  QVector<XCLParsingExpression*>*  expressions=new QVector<XCLParsingExpression*>();;
  //suche richtiges processing
  enum XCLSyntaxProcess::PROCESS_TYPE type=element->getType();
  if(type == XCLSyntaxProcess::PUSHXCEL)
  {
    delete expressions;
    expressions=processPushXCELProcessing(element,*state);

  }
  else if(type==XCLSyntaxProcess::PULLXCEL)
  {
    delete expressions;
    expressions=processPullXCELProcessing(element,parent,*state);
  }
  else if(type == XCLSyntaxProcess::PUSHBINARY)
  {
    processPushBinaryProcessing(element,binaryArray,*state);
  }
  else if(type == XCLSyntaxProcess::CONFIGPARSER)
  {
    processConfigureParserProcessing(element,*state);
  }

  return expressions;
}




QVector<XCLParsingExpression*>* XCELMainProcessor::processPushXCELProcessing(
  XCLSyntaxProcess* element,
  FileParserState& state)
{

#ifdef PlanetsDebug
  std::cout<<"PUSHXCEL PROCESSING\n";
#endif
  XCLSyntaxItem* grammar =reader->getGrammar();

  QHash<QString,XCLSyntaxExpression*>* grammarIndex=reader->getGrammarIndex();

  XCLSyntaxExpression* newGramm = getNewGramm(element,grammarIndex,state);

  grammar->replace(newGramm);

  grammarIndex->insert(newGramm->getIdentifier(), newGramm);

  QVector<XCLParsingExpression*>*  expressions = new QVector<XCLParsingExpression*>;

  return expressions;
}


QVector<XCLParsingExpression*>* XCELMainProcessor::processPullXCELProcessing(
  XCLSyntaxProcess* element,
  XCLParsingItem* parent,
  FileParserState& state)
{
#ifdef PlanetsDebug
  std::cout<<"PULL START///////////////////\n";
#endif


//  XCLSyntaxItem* grammar =reader->getGrammar();
  QHash<QString,XCLSyntaxExpression*>* grammarIndex=reader->getGrammarIndex();
  XCLSyntaxExpression* newGramm = getNewGramm(element,grammarIndex,state);



  XCLSyntaxItem* contextItem=new XCLSyntaxItem();

  contextItem->setItemType(XCLSyntaxItem::STRUCTURING);
  contextItem->setIdentifier("_ContextItem");
  contextItem->setIsOptional(TRUE);
  contextItem->setIsMultiple(FALSE);
  contextItem->setIdentifier("ContextItem");

  //contextItem->setIsOptional( TRUE);
  contextItem->setIsOptional(newGramm->getIsOptional());
  contextItem->setIsMultiple(newGramm->getIsMultiple());
  short type = newGramm->getElementType();
  switch(type)
  {
  case XCLParsingExpression::ITEM_TYPE:

    contextItem->add(*((XCLSyntaxItem*)newGramm));
    break;
  case XCLParsingExpression::SYMBOL_TYPE:

    contextItem->add(*((XCLSyntaxSymbol*)newGramm));
    break;
  case XCLParsingExpression::PROPERTY_TYPE:

    contextItem->add(*((XCLSyntaxProperty*)newGramm));
    break;
  }

  reader->getGrammarIndex()->insert(contextItem->getIdentifier(),contextItem);

  QVector<XCLParsingExpression*>* expressions = reader->parse(contextItem,parent,state);
  //state.addExpressionsToIndex(expressions);/*JN???*/
  return expressions;
}

void XCELMainProcessor::processPushBinaryProcessing(
  XCLSyntaxProcess* element,
  QByteArray* binaryArray,
  FileParserState& state)
{


  executeFileMethods(binaryArray,state,element);
  binaryArrayLength = binaryArray->size();
}




/*! \void XCELMainProcessor::process(XCLSyntaxExpression* expr, XCLParsingItem* item, const QVector<XCLProcessMethod>& methods, FileParserState&& state)
 *  \brief Called by the Factory class to manage the processing of methods performed on an existing XCLSyntaxEpression.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \param methods A list of the methods which have to be executed on the XCLSyntaxExpression expr.
 *  \param actState A reference to the current FileParserState&.
 *  \exception XCLException 
 
Calls for each method found in the list the appropriate function to perform the particular task.
**/
void XCELMainProcessor::executeGrammarMethods( XCLSyntaxExpression* expr, FileParserState& state, XCLSyntaxProcess* element)
{
  QVector<XCLProcessMethod> m=element->getMethods();
  _LENGTH size = m.size();
  XCELProcessingMethod* meth;
  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
    QList<XCELProcessingMethod*> meths = methodRegister.values(name);
    if(meths.size()==0)
	{
		std::cout<<"No Method with name: "<<name.toStdString()<<" found\n";
		return;
	}
    if(meths.size()>1)
    {
      //anzahl der Parameter prüfen
    }
    else
    {
      meth = meths.at(0);
      try
      {	
#ifdef PlanetsDebug
        std::cout<<m[i].getName().toStdString()<<"\n";
#endif
        if(!meth->process(reader,expr,m[i].getParameters(),vectorHash))
          throw XCLException(m[i].getName());
      }
      catch(XCLException exception)
      {
        exception.message();
      }
    }
  }
  // useless statement for less compiler-warnings
  (void)state;

}

void XCELMainProcessor::executeFileMethods( QByteArray* ba, FileParserState& state, XCLSyntaxProcess* element)
{

  QVector<XCLProcessMethod> m=element->getMethods();
  _LENGTH size = m.size();
  XCELProcessingMethod* meth;
  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
    QList<XCELProcessingMethod*> meths = methodRegister.values(name);
    if(meths.size()==0)
	{
		std::cout<<"No Method with name: "<<name.toStdString()<<" found\n";
		return;
	}
    if(meths.size()>1)
    {
      //anzahl der Parameter prüfen
    }
    else 
    {
      meth = meths.at(0);
      meth->process(reader,NULL,m[i].getParameters(),vectorHash);
    }
  }
  // useless statement for less compiler-warnings
  (void)state;
  (void)ba;
}


void XCELMainProcessor::processConfigureParserProcessing( XCLSyntaxProcess* process, FileParserState& state)
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
    if(meths.size()>1)
    {
      //anzahl der Parameter prüfen
    }
    else
    {
      meth = meths.at(0);
      if(!meth->process(reader,NULL,m[i].getParameters(),vectorHash))
      {	
	/**
	* 	Durch die ABfrage, ob ein bestimmtes Element bereits im Index ist, soll das 
	*	erneute Lesen verhindert werden.
	*/
	std::cout<<"Message causes exception: "<<m[i].getName().toStdString()<<"\n";
	 throw XCLException(m[i].getName());
      }
    }
  }
  // useless statement for less compiler-warnings
  (void)state;
}


/*
void XCELMainProcessor::performDecompress(QByteArray* ba,
			 QVector<XCLProcessParameter> param,
                         FileParserState& state)
{
 
}
! \void XCELMainProcessor::performSetLength(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the length of the XCLSyntaxExpression \a expr to the value given by the parameters of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 
 
 
 
void XCELMainProcessor::performSetLength(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
   
}
    
 
 *! \void XCELMainProcessor::performSetName(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the name of the XCLSyntaxExpression \a expr to the value given by the parameter of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 
 
 
 
 
void XCELMainProcessor::performSetName(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
 
}
 
 *! \void XCELMainProcessor::performSetByteOrder(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the byteorder of the XCLSyntaxExpression \a expr to the value given by the parameter of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 
 
 
void XCELMainProcessor::performSetByteOrder(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
   
}
 
 
 
 
 *! \void XCELMainProcessor::performSetFilter(XCLSyntaxExpression* expr, QVector<XCLProcessPanrameter> param,XCLParsingItem* item)
 *  \brief 
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \exception XCLException 
 
 
 
void XCELMainProcessor::performAddFilter(XCLSyntaxExpression* expr,QVector<XCLProcessParameter> param,FileParserState& item)
{
 
    
}
 
 *! \void XCELMainProcessor::performSetStart(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,XCLParsingItem* item)
 *  \brief Sets the startposition of the XCLSyntaxExpression \a expr to the value given by the parameter of the method.
 *  \param expr A pointer to the XCLSyntaxExpression, the processing method will be performed on.
 *  \param param A list of parameters for this method.
 *  \param item A pointer to the parent XCLParsingItem.
 *  \param state The current FileParserState&.
 *  \exception XCLException 
 
 
void XCELMainProcessor::performSetStart(XCLSyntaxExpression* expr,
                                   QVector<XCLProcessParameter> param,
                                   FileParserState& item)
{
 
  
}
 
void XCELMainProcessor::performSetInterpretation(XCLSyntaxExpression* expr, QVector<XCLProcessParameter> param,FileParserState& item)
{
  
}
 
 
 //expr=new XCLSyntaxItem();
 
*/
