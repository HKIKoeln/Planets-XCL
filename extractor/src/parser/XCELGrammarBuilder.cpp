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

#include <QDomDocument>
#include <iostream>
#include <QDataStream>
#include <QTextStream>
#include <QByteArray>
#include "XCELGrammarBuilder.h"
#include "XCLSyntaxItem.h"
#include "XCLSyntaxSymbol.h"
#include "XCLSyntaxProperty.h"
#include "XCLSyntaxProcess.h"
#include "XCLException.h"

XCELGrammarBuilder::XCELGrammarBuilder()
{
  grammar = NULL;
gId=0;
  //std::cout<<"GRammars observer "<<grammar->getObserver()<<"\n";
  nextStartposition=0;
}


XCELGrammarBuilder::~XCELGrammarBuilder()
{
  if(grammar != NULL) delete grammar;
}


/*! \const XCLSyntaxItem& buildGrammar(QDomDocument* aDOM);
 *  \brief Translates the QDomDocument aDOM to a grammar structure of XCLSyntaxExpressions.
 *  \param aDOM a pointer to the DOM representation of an XCEL-File.
 *  \return the first XCLSyntaxItem of the grammar structure.
 
 * Calls buildItem() for every child in the DOM Tree.\n 
 * Returns a pointer to the first #XCLSyntaxItem of the grammar.
 */
const XCLSyntaxItem& XCELGrammarBuilder::buildGrammar(QDomDocument* aDOM)
{
  BOOL formatDescriptionExists=FALSE;
  myDOM = aDOM;
  if(grammar !=NULL)
    delete grammar;
  grammar= new XCLSyntaxItem();

  QDomNodeList childNodes = myDOM->firstChildElement().childNodes();
  _LENGTH size=childNodes.size();
  for(_LENGTH i=0;i<size;i++)
  {
    QDomNode actNode = childNodes.at(i);
    if(actNode.isElement())
    {
      QDomElement element = actNode.toElement();
      if(element.tagName() == "formatDescription")
      {
        formatDescriptionExists=TRUE;
        element=element.firstChildElement();
        buildItem(&(element),grammar);
      }
    }
  }
  if(!formatDescriptionExists)
    throw XCLException("This does not look like an XCEL\n");

  for(_LENGTH i=0;i<size;i++)
  {
    QDomNode actNode = childNodes.at(i);
    if(actNode.isElement())
    {
      QDomElement element = actNode.toElement();
      if(element.tagName() == "templates")
      {
        QDomNodeList templateChildNodes = element.childNodes();
        _LENGTH tcNSize = templateChildNodes.size();
        for(_LENGTH i=0;i<tcNSize;i++)
        {
          QDomNode actTemplateNode = templateChildNodes.at(i);
          if(actTemplateNode.isElement())
          {
            QDomElement atNelement=actTemplateNode.toElement();
            buildItem(&(atNelement),grammar);
          }
        }
      }
    }
  }

  return *grammar;
}


const XCLSyntaxItem& XCELGrammarBuilder::buildPreProcessingGrammar(QDomDocument* aDOM)
{

   myDOM = aDOM;
  if(grammar !=NULL)
    delete grammar;
  grammar= new XCLSyntaxItem();

  QDomNodeList childNodes = myDOM->firstChildElement().childNodes();
  _LENGTH size=childNodes.size();
  for(_LENGTH i=0;i<size;i++)
  {
    QDomNode actNode = childNodes.at(i);
    if(actNode.isElement())
    {
      QDomElement element = actNode.toElement();
      if(element.tagName() == "preProcessing")
      {
        element=element.firstChildElement();
        buildItem(&(element),grammar);
      }
    }
  }


  return *grammar;
}


const XCLSyntaxItem& XCELGrammarBuilder::buildPostProcessingGrammar(QDomDocument* aDOM)
{
  myDOM = aDOM;
  if(grammar !=NULL)
    delete grammar;
  grammar= new XCLSyntaxItem();

  QDomNodeList childNodes = myDOM->firstChildElement().childNodes();
  _LENGTH size=childNodes.size();
  for(_LENGTH i=0;i<size;i++)
  {
    QDomNode actNode = childNodes.at(i);
    if(actNode.isElement())
    {
      QDomElement element = actNode.toElement();
      if(element.tagName() == "postProcessing")
      {
        element=element.firstChildElement();
        buildItem(&(element),grammar);
      }
    }
  }


  return *grammar;
}


/*! \void buildItem(QDomElement* itemElement, XCLSyntaxItem* itemToAdd);
 *  \brief Adds an item to the grammar.
 * \param itemElement pointer to the DOM-Element, which has to be included in the grammar.
 * \param itemToAdd pointer to the parent object.
 *
 * creates a new #XCLSyntaxItem and initializes its attributes with default values.\n
 * sets the #nextStartposition to the byte value, where the new XCLSyntaxItem starts.\n
 * recursively builds XCLExpressions for all child elements. \n
 * adds the new item to the parent item \a itemToAdd i.e. the XCLSyntaxItem the function has been called with. 
*/
void XCELGrammarBuilder::buildItem(QDomElement* itemElement,
                                   XCLSyntaxItem* itemToAdd)
{
  //std::cout<<"Generate Item\n";
  XCLSyntaxItem* item = new XCLSyntaxItem();

  //  item->setObserver(itemToAdd->getObserver());
  //  std::cout<<item->getObserver()<<"\n";
  setExpressionMembers(itemElement,item);
  // item->setLength(-1);
  //nextStartposition=item->getStart();
  QString order=itemElement->attribute("order");

  QString objectType =itemElement->attribute("objectType");
 // std::cout<<"ObjectType is: "<<objectType.toStdString()<<"\n";
  item->setObjectType(objectType);

  QString type = itemElement->attribute("xsi:type");
  item->setItemType(type);
  if(order == "all")
    item->setOrderType(XCLSyntaxItem::ALL);
  else if(order == "choice")
    item->setOrderType(XCLSyntaxItem::CHOICE);
  else if(order == "sequence")
    item->setOrderType(XCLSyntaxItem::SEQUENCE);

 

  QDomNodeList childNodes = itemElement->childNodes();
  QDomElement element;
  QDomNode actNode;
  _UINT32 size=childNodes.size();
  for(_UINT32 i=0;i<size;i++)
  {
    actNode = childNodes.at(i);

    if(actNode.isElement())
    {
      element = actNode.toElement();
      if(element.tagName() == "item")
      {
        //std::cout<<"Item gefunden "<<i<<std::endl;
        buildItem(&element,item);
      }
      else if(element.tagName() == "symbol")
      {
        //std::cout<<"Symbol gefunden "<<i<<std::endl;
        buildSymbol(&element,item);
      }
      else if(element.tagName() == "property")
      {
        //std::cout<<"Property gefunden "<<i<<std::endl;
        buildProperty(&element,item);
      }
      else if(element.tagName() == "processing")
      {
        //std::cout<<"Property gefunden "<<i<<std::endl;
        buildProcessing(&element,item);
      }
    }
  }
  //std::cout<<"HUHU1\n";
  //std::cout<<"item "<<item->getIdentifier().toStdString()<<"\n\n";
  //std::cout<<"HUHU2\n";
  itemToAdd->add(*item);
  //itemToAdd->print();
  //std::cout<<"HUHU\n";
  delete item;
}


/*! \void buildSymbol(QDomElement* itemElement, XCLSyntaxItem* itemToAdd);
 *  \brief Adds a symbol to the grammar.
 * \param itemElement pointer to the DOM-Element, which has to be included in the grammar.
 * \param itemToAdd pointer to the parent object.
 *
 * creates a new #XCLSyntaxSymbol and initializes its attributes with default values.\n
 * sets the #nextStartposition to the byte value, where the new XCLSyntaxSymbol ends.\n
 * adds the new symbol to the parent item \a itemToAdd i.e. the XCLSyntaxItem the function has been called with. 
*/
void XCELGrammarBuilder::buildSymbol(QDomElement* symbolElement,
                                     XCLSyntaxItem* itemToAdd)
{

  _UINT32 size=0;

  XCLSyntaxSymbol symbol;
  setExpressionMembers(symbolElement,&symbol);
  QString enc = symbolElement->attribute("encoding");

 if(!enc.isEmpty())
	symbol.setEncoding( enc);
/*
 QString length = symbolElement->attribute("length");
 if(!length.isEmpty())
 {
 symbol.setLength(length.toLong());
 if (symbol.getStart() < (LENGTH_MAX - symbol.getLength()-1) )
      symbol.setEnd( symbol.getStart()+symbol.getLength()-1 );
  symbol.setLengthType( XCLSyntaxExpression::FIXED );
 }*/

  //nextStartposition=symbol.getEnd()+1;


  /*QDomNode r=((QDomNodeList)(symbolElement->elementsByTagName("value"))).at(0);
  QDomElement value=r.toElement();*/
  QString interpretation = symbolElement->attribute("interpretation");
  if(!interpretation.isEmpty()) symbol.setInterpretation(interpretation);
  QString matchingBehaviour =  symbolElement->attribute("matchingBehaviour");

  if(matchingBehaviour == "regexp")
  {
    symbol.setValueType(XCLParsingExpression::REGEXP);

  }

 
  try
  {
  QDomElement semIntNode=getDirectChildElement(*symbolElement,"valueInterpretation");
  QDomNodeList keyNames = semIntNode.elementsByTagName("valueLabel");
  QDomNodeList keyValues = semIntNode.elementsByTagName("value");
  size = keyNames.size();
  for(_UINT32 i=0;i<size;i++)
  {
    symbol.addSemanticInterpretation
    (
      keyValues.at(i).toElement().text(),
      keyNames.at(i).toElement().text()
    );
  }
  }
  catch(XCLException exception)
  {

  }
  try
  {
  QDomElement nonValidValuesNode=getDirectChildElement(*symbolElement,"nonValidValues");
  QDomNodeList nonValVal = nonValidValuesNode.elementsByTagName("value");
  size = nonValVal.size();
  for(_UINT32 i=0;i<size;i++)
  {
    
    symbol.addNonValidValue
    (
      normalize(nonValVal.at(i).toElement().text())
    );
  }
 
  }
  catch(XCLException exception)
  {

  }
  QDomElement validValuesNode;
  try
  {
 validValuesNode=getDirectChildElement(*symbolElement,"validValues");
  QDomNodeList valVal = validValuesNode.elementsByTagName("value");
  size = valVal.size();
  for(_UINT32 i=0;i<size;i++)
  {
    symbol.addValidValue
    (
      normalize(valVal.at(i).toElement().text())
    );
  }
}
  catch(XCLException exception)
  {

  }

  
  
  // QDomElement validValueRange=validValuesNode.elementsByTagName("validValues").at(0).toElement();
  try
  {
    QString startRange=getDirectChildText(validValuesNode,"startRange");
    QString endRange=getDirectChildText(validValuesNode,"endRange");
    if(!startRange.isEmpty() && !endRange.isEmpty())
    {
      if(symbol.getInterpretation() != "ASCII")
      {
	int start =  startRange.toInt();
	int end =  endRange.toInt();
	QByteArray values;
	if(end>0)
	{
	  for(int i=start;i<=end;i++)
	  {
	    QString str;
	    symbol.addValidValue(str.setNum(i));
	  }
	}
      }
      else
      {
	int start = (int) (startRange.at(0).digitValue());
	int end =  (int) (endRange.at(0).digitValue());
	if(end>0)
	  QByteArray values;
	for(int i=start;i<=end;i++)
	{
	  QString s;
	  s.setNum(i);
	  symbol.addValidValue(s);
	}
      }
    }
  }
  catch(XCLException exception)
  {

  }
   QString value = symbolElement->attribute("value");
   if(!value.isEmpty())
    {
        value=normalize(value);
        symbol.addValidValue(value);
      /*  if(length.isEmpty())
        {
            symbol.setLength(value.size());
        }*/
    }

   
/*
   QString encoding = symbolElement->attribute("encoding");
   if(!encoding.isEmpty())
   symbol.setEncoding(encoding);
*/
  itemToAdd->add(symbol);

}


/*! \void buildProperty(QDomElement* itemElement, XCLSyntaxItem* itemToAdd);
 *  \brief Adds a property to the grammar.
 * \param itemElement pointer to the DOM-Element, which has to be included in the grammar.
 * \param itemToAdd pointer to the parent object.
 *
 * creates a new #XCLSyntaxProeprty and initializes its attributes with default values.\n
 * sets the #nextStartposition to the byte value, where the XCLSyntaxProperty ends.\n
 * adds the new property to the parent item \a itemToAdd i.e. the XCLSyntaxItem the function has been called with. 
*/
void XCELGrammarBuilder::buildProperty(QDomElement* propertyElement,
                                       XCLSyntaxItem* itemToAdd)
{
  XCLSyntaxProperty property;
  setExpressionMembers(propertyElement,&property);
  nextStartposition=property.getEnd()+1;
  QDomNode r=((QDomNodeList)(propertyElement->elementsByTagName("value"))).at(0);
  QDomElement value=r.toElement();

  QString interpretation = propertyElement->attribute("interpretation");
  if(!interpretation.isEmpty()) property.setInterpretation(interpretation);

  QString matchingBehaviour =  propertyElement->attribute("matchingBehaviour");

  if(matchingBehaviour == "regexp")
  {
    property.setValueType(XCLParsingExpression::REGEXP);

  }

  QString valStr = value.text();
  property.setKnownValue(normalize(valStr));

  itemToAdd->add(property);
}


/*! \void buildProcessing(QDomElement* itemElement, XCLSyntaxItem* itemToAdd);
 *  \brief Adds a processing instruction element to the grammar.
 * \param itemElement pointer to the DOM-Element, which has to be included in the grammar.
 * \param itemToAdd pointer to the parent object.
 *
 * A XCLSyntaxProcess object can contain one or more processing methods. Every method can contain one or more parameters.\n
 * This functions searches the QDomElement "element" for methods and adds them to a new XCLSyntaxProcess object, which is itself added to the parent item \a itemToAdd i.e. the XCLSyntaxItem the function has been called with. 
*/
void XCELGrammarBuilder::buildProcessing(QDomElement* element,
    XCLSyntaxItem* itemToAdd)
{
  XCLSyntaxProcess process;
  QString type;
  QString xcelRef;
  QString xcelNameRef;
  QString methodName;
  QString parameter;
  QString value;


  type = element->attribute("type");

  xcelRef = element->attribute("xcelRef");
  xcelNameRef = element->attribute("xcelNameRef");
  process.setType(type);
  process.setXcelReference(xcelRef);
  process.setXcelNameReference(xcelNameRef);
  QDomNodeList methods = element->elementsByTagName("processingMethod");
  _LENGTH methodSize=methods.size();
  for( _LENGTH i=0;i<methodSize;++i)
  {
    methodName = methods.at(i).toElement().attribute("name");
    XCLProcessMethod method(methodName);

    QDomNodeList parameters = methods.at(i).toElement().elementsByTagName("param");
    _LENGTH paraSize = parameters.size();
    for(_LENGTH i = 0;i< paraSize;i++)
    {
      XCLProcessParameter par;

      if (parameters.at(i).toElement().attribute("value","_H64__")!="_H64__")
        par.setInterpretedValue(parameters.at(i).toElement().attribute("value","_H64__"));

      else if (parameters.at(i).toElement().attribute("valueRef","_H64__")!="_H64__")
      {
        par.setValueReference(parameters.at(i).toElement().attribute("valueRef","_H64__"));

      }
      else if (parameters.at(i).toElement().attribute("listRef","_H64__")!="_H64__")
      {
        par.setListRef(TRUE);
        par.setValueReference(parameters.at(i).toElement().attribute("listRef","_H64__"));

      }
      else if(parameters.at(i).toElement().attribute("nameRef","_H64__")!="_H64__")
        par.setNameReference(parameters.at(i).toElement().attribute("nameRef","_H64__"));

      QString valueType =  parameters.at(i).toElement().attribute("valueType");

      if (valueType == "simple")
        par.setValueType(XCLProcessParameter::SIMPLE);
      else if (valueType == "valueRef")
        par.setValueType(XCLProcessParameter::VALUEREF);
      else if(valueType == "nameRef")
        par.setValueType(XCLProcessParameter::NAMEREF);
      else if(valueType == "mathEx")
        par.setValueType(XCLProcessParameter::MATHEX);
      else
        par.setValueType(XCLProcessParameter::SIMPLE);

      method.addParameter(par);
    }

    process.addMethod(method);
  }

  itemToAdd->add(process);
}


/*! \void XCELGrammarBuilder::setExpressionMembers(QDomElement* element, XCLSyntaxExpression* expr)
 *  \brief Initializes the attributes of the XCLSyntaxExpression expr with values of the QDomElement element.
 * \param element pointer to the DOM-Element, which has to be included in the grammar.
 * \param expr pointer to the XCLSyntaxExpression, which has to be initialized.
*/
void XCELGrammarBuilder::setExpressionMembers(QDomElement* element,
    XCLSyntaxExpression* expr)
{
  QString nsUri="http://www.w3.org/2001/XMLSchema-instance";
  QString identifier = element->attribute("identifier");
  QString originalName = element->attribute("originalName");
  QString name = element->attribute("name");
  QString multiple = element->attribute("multiple");
  QString optional = element->attribute("optional");
  QString type = element->attribute("xsi:type");
  QString byteOrder = element->attribute("byteOrder");
  QString print = element->attribute("print");
  QString normDataRelation = element->attribute("normDataRelation");
  QString source=element->attribute("internalSource");
  expr->setSource(source);
  QString extSource=element->attribute("externalSource");
  expr->setExtSource(extSource);
  QString objectType = element->attribute("objectType"); 
  QString newObject = element->attribute("newObject");
  if(!objectType.isEmpty())
    expr->setObjectType(objectType);
  if(!newObject.isEmpty() && newObject=="true")
    expr->setNewObject(TRUE);

if(name==NULL)
try
{
  name = getDirectChildText(*element,"name");
 
}
catch(XCLException exception)
{

}


  expr->setOriginalName(originalName);

if(!identifier.isEmpty())
  expr->setIdentifier(identifier);
else
{
    gId++;
 QString id;
  
  id.setNum(gId);
  id.prepend("extrc_");
  expr->setIdentifier(id);
}
expr->setPrintMe(print);
  //  expr->setType(type);
  if(multiple=="true")expr->setIsMultiple(TRUE);
  else expr->setIsMultiple(FALSE);
  if(optional=="true")expr->setIsOptional(TRUE);
  else expr->setIsOptional(FALSE);
  if(!byteOrder.isEmpty())
    expr->setByteOrder(byteOrder);

if(normDataRelation == "true")
{
//	std::cout<<"HEY nDRelation! "<<expr->getIdentifier().toStdString()<<"\n";
	expr->setNormDataRelation(TRUE);
	//expr->setName("normData");
}

 expr->setName(name);

QString length;
QString startposition;

length = element->attribute("length");
startposition = element->attribute("startposition");


try
{
 // range=getDirectChildElement(*element,"range");
 // startposition = getDirectChildElement(range,"startposition");


  if(!startposition.isEmpty())
  {
    expr->setStart(startposition.toLong());
    expr->setPositionType( XCLSyntaxExpression::FIXED );
  }	
  else
  {
    expr->setStart(nextStartposition);
    expr->setPositionType( XCLSyntaxExpression::SEQUENTIAL );
  }
/*  //evaluate startposition information
  if(startposition.attribute("xsi:type") == "sequential")
  {
    expr->setStart(nextStartposition);
    expr->setPositionType( XCLSyntaxExpression::SEQUENTIAL );
  }
  else if(startposition.attribute("xsi:type") == "fixed")
  {
    expr->setStart(startposition.text().toLong());
    expr->setPositionType( XCLSyntaxExpression::FIXED );
  }
  else if(startposition.attribute("xsi:type") == "undefined")
  {
    expr->setStart(LENGTH_MAX);
    expr->setPositionType( XCLSyntaxExpression::UNDEFINED );
  }
  else if(startposition.attribute("xsi:type") == "processed")
  {
    expr->setStart(LENGTH_MAX);
    expr->setPositionType( XCLSyntaxExpression::PROCESSED );
  }*/
}
catch(XCLException exception)
{
   expr->setStart(nextStartposition);
   expr->setPositionType( XCLSyntaxExpression::SEQUENTIAL );
}
  try
  {
    if(!length.isEmpty())
    {
        expr->setLength(length.toLong());

        if (expr->getStart() < (LENGTH_MAX - expr->getLength()-1) )
            expr->setEnd( expr->getStart()+expr->getLength()-1 );

        expr->setLengthType( XCLSyntaxExpression::FIXED );
    }	
    else
    {
        expr->setLength(LENGTH_MAX);
        expr->setLengthType( XCLSyntaxExpression::PROCESSED );
    }
/*
  //evaluate length information
  if(length.attribute("xsi:type") == "fixed")
  {
    expr->setLength(length.text().toLong());
    if (expr->getStart() < (LENGTH_MAX - expr->getLength()-1) )
      expr->setEnd( expr->getStart()+expr->getLength()-1 );
    expr->setLengthType( XCLSyntaxExpression::FIXED );
  }
  else if(length.attribute("xsi:type") == "undefined")
  {
    expr->setLength(LENGTH_MAX);
    expr->setLengthType( XCLSyntaxExpression::UNDEFINED );
  }
  else if(length.attribute("xsi:type") == "processed")
  {
    expr->setLength(LENGTH_MAX);
    expr->setLengthType( XCLSyntaxExpression::PROCESSED );
  }*/
}
catch(XCLException exception)
{
	if(expr->getElementType()!=XCLSyntaxExpression::ITEM_TYPE)
	{
		expr->setLength(1);
		expr->setLengthType( XCLSyntaxExpression::FIXED);
	}
	
}
  /*if (expr->getLength() != LENGTH_MAX && expr->getStart() != LENGTH_MAX)
  {
    expr->setEnd(expr->getStart() + expr->getLength()-1);
    nextStartposition = expr->getEnd();
  }
  else
    expr->setEnd(LENGTH_MAX);

  if(expr->getEnd() == LENGTH_MAX)
    nextStartposition = LENGTH_MAX;*/



}


/*! \overload QString XCELGrammarBuilder::getDirectChildText(QDomElement element,QString search)
 * \return the name of the child element as QString. 
 
 * Returns a direct child of QDomElement "element" with a tag name matching the given search string. The name of the child element is returned as QString.
*/
QString XCELGrammarBuilder::getDirectChildText(QDomElement element,QString search)
{
 QDomElement result;
  try
  {
  result = getDirectChildElement(element,search);
  }
  catch(XCLException exception)
  {
   return "";
  }
  return result.text();
}


/*! \QDomElement XCELGrammarBuilder::getDirectChildElement(QDomElement element, QString search)
 *  \brief Returns a direct child of QDomElement "element" with a tag name matching the given search string.
 * \param element the DOM-Element, which is searched for the child. 
 * \param search the name of the tag searched for.
 * \return the child element with the matching tag name or an empty QDomElement.
 
 *  Searches the children of QDomElement "element" for a tag named "search" and returns the child as QDomElement or returns an empty QDomElement if no matching tag has been found. 
*/
QDomElement XCELGrammarBuilder::getDirectChildElement(QDomElement element,QString search)
{
  QDomElement nameElement;
  QDomNodeList childNodes = element.childNodes();//((QDomNodeList)(element->elementsByTagName("name"))).at(0);
  _UINT32 size=childNodes.size();
  for(_UINT32 i=0;i<size;i++)
  {
    if(childNodes.at(i).isElement())
    {

      nameElement = childNodes.at(i).toElement();

      if(nameElement.tagName() == search)
      {

        return nameElement;
      }
    }
  }
  throw XCLException("No Child with name "+search+"found\n"); 
}


const QString& XCELGrammarBuilder::normalize(QString str)
{
    if(str == "LF")
      str="\n";

    else if(str == "CR")
      str="\r";

    /*else if(str == "NULL")
    {
      str="\0";

      std::cout<<"Null String has length: "<<str.size()<<"\n";
    }*/
    else if(str == "TAB")
      str="\t";
    else if(str == "SP")
      str=" ";
    else if(str == "FF")
      str="\f";
    else if(str == "ETX")
      str="\x0";
    else if(str == "GT")
      str="&gt;";
    else if(str == "LT")
      str="&lt;";
 return str;
}
