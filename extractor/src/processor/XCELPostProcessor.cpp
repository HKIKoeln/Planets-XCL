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
#include "XCELPostProcessor.h"
//#include "XCELFileParser.h"
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

XCELPostProcessor::XCELPostProcessor()
{

}


XCELPostProcessor::~XCELPostProcessor()
{

}

QVector<XCLParsingExpression*>* XCELPostProcessor::process()
{
  QVector<XCLParsingExpression*>*  expressions=new QVector<XCLParsingExpression*>();;
  FileParserState* state = reader->getState();
  XCLSyntaxProcess* element=(XCLSyntaxProcess*)state->actElement;
  XCLParsingItem* parent = state->parent;
  QByteArray* binaryArray = reader->getBinaryArray();

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
  //  throw XCLException(QString("Process Type is not supported ")+type+" \n");
 
    return expressions;
}




QVector<XCLParsingExpression*>* XCELPostProcessor::processPushXCELProcessing(
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
   QVector<XCLParsingExpression*>*  expressions =  new QVector<XCLParsingExpression*>();
  //  expressions->append(contextItem);
  return expressions;
}


QVector<XCLParsingExpression*>* XCELPostProcessor::processPullXCELProcessing(
  XCLSyntaxProcess* element,
  XCLParsingItem* parent,
  FileParserState& state )
{
    XCLSyntaxExpression* expr=new XCLSyntaxSymbol();
    QVector<XCLParsingExpression*> referencedData;
    referencedData = root->getOrderedData(element->getXcelNameReference());
    executeGrammarMethods(expr,state,element);
    QVector<XCLParsingExpression*>*  expressions =  new QVector<XCLParsingExpression*>();
    //  expressions->append(contextItem);
    return expressions;
    // useless statement for less compiler-warnings
    (void)parent;
}

void XCELPostProcessor::processPushBinaryProcessing(	XCLSyntaxProcess* element,
  							QByteArray* binaryArray,
  							FileParserState& state)
{
  executeFileMethods(binaryArray,state,element);
}


void XCELPostProcessor::executeGrammarMethods( XCLSyntaxExpression* expr,
                                 FileParserState& state,
                                 XCLSyntaxProcess* element)
{
  QVector<XCLParsingExpression*>* referencedData = new QVector<XCLParsingExpression*>(root->getOrderedData(element->getXcelNameReference()));

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
      if(!meth->postprocess(*root,m[i].getParameters(),*referencedData,state,vectorHash))
	throw XCLException(m[i].getName());

    }
  }
  // useless statement for less compiler-warnings
  (void)expr;
}

void XCELPostProcessor::executeFileMethods(QByteArray* ba,
                                FileParserState& state,
                                XCLSyntaxProcess* element)
{
  QVector<XCLParsingExpression*>* referencedData = new QVector<XCLParsingExpression*>(root->getOrderedData(element->getXcelNameReference()));

  QVector<XCLProcessMethod> m=element->getMethods();
  _LENGTH size = m.size();
  XCELProcessingMethod* meth;

  for (_LENGTH i=0;i<size;i++)
  {
    QString name=(m[i].getName());
    QList<XCELProcessingMethod*> meths = methodRegister.values(name);


   //  std::cout<<"Found processing"<<name.toStdString()<<"\n";
    if(meths.size()>1)
    {
      //anzahl der Parameter prüfen
    }
    else
    {
      meth = meths.at(0);
  
      if(!meth->postprocess(*root,m[i].getParameters(),*referencedData,state,vectorHash))
	throw XCLException(m[i].getName()+" sent FALSE\n");
    }
  }
  // useless statement for less compiler-warnings
  (void)ba;
}


void XCELPostProcessor::postprocess(XCLSyntaxItem* grammar,XCLParsingItem* r)
{
 root =r;
 index= root->getNameIndex();
 reader->registerParsedValues(index);
 reader->parseFile(NULL,*grammar);
}





/*
void XCELPostProcessor::process(XCLParsingItem& root, const XCLSyntaxItem& processingGrammar)
{
    index = *root.getNameIndex();
    #ifdef PlanetsDebug
    std::cout<<"postprocessing...."<<std::endl;
    #endif
    XCLSyntaxItem* grammar = new XCLSyntaxItem(processingGrammar);
 
    QVector<XCLSyntaxElement*> postprocessings = grammar->getChildren();
    _LENGTH size = postprocessings.size();
 
    for(_LENGTH i=0;i<size;i++)
    {
        //for each postprocessing resolve methods and apply changes to the parsing tree 'root'
        doPostprocessing(*(XCLSyntaxProcess*)postprocessings.at(i),root);
    }
    delete grammar;
}

void XCELPostProcessor::doPostprocessing(const XCLSyntaxProcess& postprocessing,XCLParsingItem& parseTree)
{
  QVector<XCLParsingExpression*> referencedData;
  XCLSyntaxProcess postp = postprocessing;
#ifdef PlanetsDebug
  std::cout<<"xcelNameReference: "<<postp.getXcelNameReference().toStdString()<<std::endl;
#endif

  referencedData = parseTree.getOrderedData(postp.getXcelNameReference());

  if (referencedData.isEmpty())
    throw XCLException("Corrupted data");

  QVector<XCLProcessMethod> m = postp.getMethods();
  _LENGTH size = m.size();

  for (_LENGTH j=0;j<size;j++)
  {
    QString name=(m[j].getName());

    if (name == "addFilter")
    {
      XCLFilter filter;
      try
      {
        performAddFilter(&filter, m[j].getParameters());
#ifdef PlanetsDebug
        filter.printFilter();
#endif

        referencedData.at(0)->addFilter(filter);
      }
      catch(XCLException exception){}
    }
    //possibly add more postprocessing methods

  }


  XCLParsingSymbol* newSymbol =  (XCLParsingSymbol*)(referencedData.at(0));

  //the referencedData is collected in a QByteArray
  QByteArray convertedData;// = new QByteArray();
  // QByteArray convertedData2;
  for (int i=0;i<referencedData.size();i++)
  {
    convertedData+=referencedData[i]->getValue();
    //std::cout<<referencedData[i]->getInterpretedValue().toStdString()<<"\n";
    //std::cout<<"AHAHAHAHAHAHAAHAHHAHA\n";
  }

  //if a filterChain exists the byteArray is sent to the XCLDataConverter
  if (referencedData [0]->getFilterChain().size()>0)
  {
    XCLDataConverter convert;
    convertedData = convert.filter(&convertedData, referencedData[0]->getFilterChain());
  }

  //the convertedData is then copied to a new ParsingSymbol which is added to the parseTree, while the original items are deleted from it.
  newSymbol->setValue(convertedData);
  //delete convertedData;

  //remove the filterChain
  QVector<XCLFilter> f;
  newSymbol->setFilterChain(f);


#ifdef PlanetsDebug
  std::cout<<"--------- new Symbol print -----------\n";
  newSymbol->print();
  std::cout<<"--------- new Symbol ende -----------\n";
#endif

  parseTree.removeExpressions(newSymbol->getName());

  //index.remove(referencedData[0]->getIdentifier());
  parseTree.add(newSymbol);
  //index.insert(newSymbol->getName(),newSymbol);
}


void XCELPostProcessor::performAddFilter(XCLFilter* filter,
                           const QVector<XCLProcessParameter>& param)
{
QString nameStr;
QString valueStr;
try
{
 
    //the filtername (i.e. compression, interlace..) is conteined by the value of first parameter
    XCLProcessParameter filtername=param.at(0);
    XCLProcessParameter filtervalue=param.at(1);
    nameStr=filtername.getValue(index);
    valueStr=filtervalue.getValue(index);
 
    filter->setFilterName(nameStr);
    #ifdef PlanetsDebug
    std::cout<<"new filter name: "<<filter->getFilterName().toStdString()<<std::endl;
    #endif
 
    //the filtervalue (i.e. zLibDeflateInflate, adam7..) is rerferenced by the second parameter
    
    filter->setFilterValue(valueStr);
    #ifdef PlanetsDebug    
    std::cout<<"new filter value: "<<filter->getFilterValue().toStdString()<<std::endl;
    #endif
    // if the filter depends on further parameters the information is added here
    XCLProcessParameter filterparam;
    for (int i=2;i<param.size();i++)
    {
        filterparam = param.at(i);
        try
	{ 
        filter->addParam(filterparam.getNameReference(), filterparam.getValue(index));
	}
	catch(XCLException exception){}	
    }
 
}
catch(XCLException exception)
{
#ifdef PlanetsDebug
  std::cout<<"new filter name: "<<nameStr.toStdString()<<std::endl;
  std::cout<<"new filter value: "<<valueStr.toStdString()<<std::endl;
 
 
exception.message();
 
#endif
throw XCLException("Referenced value is not in parse Tree\n");
}
 
}
*/
