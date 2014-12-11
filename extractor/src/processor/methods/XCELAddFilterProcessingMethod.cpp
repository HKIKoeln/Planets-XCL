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
#include "XCELAddFilterProcessingMethod.h"
#include "XCLParsingSymbol.h"
#include "XCLParsingItem.h"
#include "XCLDataConverter.h"
#include "XCLInputNormalizer.h"

BOOL* XCELAddFilterProcessingMethod::getMode()
{
  BOOL mode[]={FALSE,TRUE,FALSE,FALSE};
  return mode;
}

int XCELAddFilterProcessingMethod::expectedParameters()
{
  return 0;
}


BOOL XCELAddFilterProcessingMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, 
					       const XCLSyntaxProcess& preprocessing)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use!
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELAddFilterProcessingMethod::process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash)
{
    XCLFilter filter;
    QString nameStr;
    QString valueStr;
    FileParserState* state = reader->getState();
    XCLProcessParameter filtername=param.at(0);
    XCLProcessParameter filtervalue=param.at(1);
    nameStr=filtername.getValue(state->index);
    valueStr=filtervalue.getValue(state->index);

    filter.setFilterName(nameStr.trimmed());

    //the filtervalue (i.e. zLibDeflateInflate, adam7..) is rerferenced by the second parameter
    filter.setFilterValue(valueStr.trimmed());

    // if the filter depends on further parameters the information is added here
    XCLProcessParameter filterparam;
    for (int i=2;i<param.size();i++)
    {
      filterparam = param.at(i);
      try
      {
        filter.addParam(filterparam.getNameReference(), filterparam.getValue(state->index));
      }
      catch(XCLException exception)
	{
	exception.message();
	}
    }
  expr->addFilter(filter);
  return TRUE;
  // useless statement for less compiler-warnings, param is in use
  (void)vectorHash;
}




BOOL XCELAddFilterProcessingMethod::postprocess( XCLParsingItem& parseTree,
						QVector<XCLProcessParameter> param,
						QVector<XCLParsingExpression*> referenceData,
						FileParserState& state,
						QHash<QString,IndexedVector*> vectorHash)
{
 // std::cout<<"Add FilterProcessingMEthod...\n";
  QString nameStr;
  QString valueStr;
  XCLFilter filter;
  try
  {
    //the filtername (i.e. compression, interlace..) is conteined by the value of first parameter
    XCLProcessParameter filtername=param.at(0);
    XCLProcessParameter filtervalue=param.at(1);
    nameStr=filtername.getValue(state.index);
    valueStr=filtervalue.getValue(state.index);

    filter.setFilterName(nameStr);

    //the filtervalue (i.e. zLibDeflateInflate, adam7..) is rerferenced by the second parameter
    filter.setFilterValue("Mu-Law");

    // if the filter depends on further parameters the information is added here
    XCLProcessParameter filterparam;
    for (int i=2;i<param.size();i++)
    {
      filterparam = param.at(i);
      try
      {
        filter.addParam(filterparam.getNameReference(), filterparam.getValue(state.index));
      }
      catch(XCLException exception)
	{
	exception.message();
	}
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
  referenceData.at(0)->addFilter(filter);

  try
  {
  if (referenceData.isEmpty())
      throw XCLException("Corrupted data");

  XCLParsingSymbol* newSymbol =  new XCLParsingSymbol();
  newSymbol->setName(referenceData.at(0)->getName());
  newSymbol->setIdentifier(referenceData.at(0)->getIdentifier());
  newSymbol->setIsBigEndian(referenceData.at(0)->getIsBigEndian());
  newSymbol->setInterpretation(referenceData.at(0)->getInterpretation());
    //the referencedData is collected in a QByteArray
    QByteArray convertedData;// = new QByteArray();
    // QByteArray convertedData2;
    for (int i=0;i<referenceData.size();i++)
    {
      convertedData+=referenceData[i]->getValue();
    }
    //if a filterChain exists the byteArray is sent to the XCLDataConverter
    if (referenceData[0]->getFilterChain().size()>0)
    {
      XCLDataConverter convert;
      convertedData = convert.filter(&convertedData, referenceData[0]->getFilterChain());
    }

    //the convertedData is then copied to a new ParsingSymbol which is added to the parseTree, while the original items are deleted from it.
    newSymbol->setValue(convertedData);
    //delete convertedData;
  /* XCLInputNormalizer in;
    QString* interpretedValue=in.normalizeValue((UCHAR*)convertedData.data(),convertedData.size(),newSymbol->getInterpretation(),                                          newSymbol->getIsBigEndian());

    newSymbol->setInterpretedValue(*interpretedValue);
    delete interpretedValue;*/
    //remove the filterChain
  //  QVector<XCLFilter> f;
    //newSymbol->cleanFilterChain();
    parseTree.removeExpressions(newSymbol->getName());
    
    parseTree.add(newSymbol);

  }
  catch (XCLException exception)
  {
  exception.message();
  //throw XCLException(exception.getMessage());
  }
  return TRUE;
  // useless statement for less compiler-warnings, param is in use!
  (void)vectorHash;
}
