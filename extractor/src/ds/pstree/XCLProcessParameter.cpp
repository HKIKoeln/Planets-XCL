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
#include "XCLProcessParameter.h"
#include "XCLParsingItem.h"
#include "XCLException.h"

// Constructors/Destructors
//

XCLProcessParameter::XCLProcessParameter ( )
{
  listRef=FALSE;
}


XCLProcessParameter::~XCLProcessParameter ( )
{
}


XCLProcessParameter::XCLProcessParameter(const XCLProcessParameter& p)
{
  interpretedValue=p.interpretedValue;
  valueReference=p.valueReference;
  nameReference=p.nameReference;
  listRef=p.listRef;
  valueType=p.valueType;
}


XCLProcessParameter& XCLProcessParameter::operator=(const XCLProcessParameter& p)
{
  if(this!=&p)
  {
    interpretedValue=p.interpretedValue;
    valueReference=p.valueReference;
    nameReference=p.nameReference;
    listRef=p.listRef;
    valueType=p.valueType;
  }
  return *this;
}


/**
 * Set the value of interpretedValue
 * @param new_var the new value of interpretedValue
 */
void XCLProcessParameter::setInterpretedValue ( const QString& new_var )
{
  if(!valueReference.isEmpty())throw XCLException("\nParameter kann nur einmal belegt werden\n");
  interpretedValue = new_var;

}


/**
 * Get the value of interpretedValue
 * @return the value of interpretedValue
 */
const QString& XCLProcessParameter::getInterpretedValue ( )
{
  return interpretedValue;
}


/**
 * Set the value of valueReference
 * @param new_var the new value of valueReference
 */
void XCLProcessParameter::setValueReference ( const QString& new_var )
{
  if(!interpretedValue.isEmpty())throw XCLException("\nParameter kann nur einmal belegt werden\n");
  valueReference = new_var;
}


/**
 * Set the value of nameReference
 * @param new_var the new value of nameReference
 */
void XCLProcessParameter::setNameReference ( const QString& new_var )
{
  if(!interpretedValue.isEmpty())throw XCLException("\nParameter kann nur einmal belegt werden\n");
  nameReference = new_var;
}


/**
 * Get the value of valueReference
 * @return the value of valueReference
 */
const QString& XCLProcessParameter::getValueReference ( )
{
  return valueReference;
}


/**
 * Get the value of nameReference
 * @return the value of nameReference
 */
const QString& XCLProcessParameter::getNameReference ( )
{
  return nameReference;
}


/**
 * @return QString
 */
const QString& XCLProcessParameter::getValue()
{
  if(valueReference.isEmpty() && nameReference.isEmpty() )
  {
    return interpretedValue;
  }
  throw XCLException("XCLProcessParameter::getValue(): Unable to get Value\n");
}


/**
 * @return QString
 */
const QString& XCLProcessParameter::getValue(XCLTree<XCLParsingExpression*>& index)
{
  try
  {
//std::cout<<"TRACE\n";
    if(valueReference.isEmpty() && nameReference.isEmpty() )
    {

      return interpretedValue;
    }
    else if((!listRef) && nameReference.isEmpty())
    {
      XCLParsingExpression* expr=index.get(valueReference)->last();
      interpretedValue = expr->getInterpretedValue();
      valueReference.clear();
      return  interpretedValue;
    }
    else if((!listRef) && valueReference.isEmpty())
    {

      QList<XCLParsingExpression*>* exprs=index.get(nameReference);
    /*  for(int i=0;i<exprs->size();i++)
      {
        XCLParsingExpression* expr=exprs->at(i);*/
XCLParsingExpression* expr=exprs->first();
        interpretedValue = (QString)expr->getInterpretedValue();

        return  interpretedValue;
    //  }

    }
    else
    {

      throw XCLException("You have to call getValues\n");
    }
  }


  catch(XCLException exception)
  {
    exception.message();
    throw XCLException("Parameter Reference unresolved\n");
  }

  throw XCLException("An unknown Error in XCLProcessParameter::getValue has occured\n");
}

const QString& XCLProcessParameter::getValues(XCLTree<XCLParsingExpression*>& index)
{
QString str;
  try
  {

//std::cout<<"ValRef "<<valueReference.toStdString()<<"\n";
//std::cout<<"TRACE\n";
    if(valueReference.isEmpty() && nameReference.isEmpty() )
    {

      return interpretedValue;
    }
    else if((!listRef) && nameReference.isEmpty())
    {
	
	QList<XCLParsingExpression*>* list = index.get(valueReference);
	for(int i=0;i<list->size();i++)
	{
	XCLParsingExpression* expr=list->at(i);
	interpretedValue.append(" ");
	interpretedValue.append(expr->getInterpretedValue());
	}
	//std::cout<<"TESTTEST "<<str.toStdString()<<"\n";
	valueReference.clear();
;
	return  interpretedValue;
    }
    else if((!listRef) && valueReference.isEmpty())
    {

      QList<XCLParsingExpression*>* exprs=index.get(nameReference);
      for(int i=0;i<exprs->size();i++)
      {
        XCLParsingExpression* expr=exprs->at(i);
        interpretedValue = (QString)expr->getInterpretedValue();

        return  interpretedValue;
      }

    }
    else
    {

      throw XCLException("You have to call getValues\n");
    }
  }


  catch(XCLException exception)
  {
    exception.message();
    throw XCLException("Parameter Reference unresolved\n");
  }

  throw XCLException("An unknown Error in XCLProcessParameter::getValue has occured\n");
}

const QByteArray& XCLProcessParameter::getValueAsBA(XCLTree<XCLParsingExpression*>* index)
{
  try
  {
    if(valueReference.isEmpty() && nameReference.isEmpty() )
    {
      QString str=interpretedValue;

      QTextStream in(&str,QIODevice::ReadOnly);
      int a;

      while(!in.atEnd())
      {
        in >> a;
        value.append(a);
      }

      return value;
    }
    else if  ((!listRef) && nameReference.isEmpty())
    {

      XCLParsingExpression* expr=index->get(valueReference)->last();

      value = expr->getValue();
      valueReference.clear();
      return  value;
    }
    else if((!listRef) && valueReference.isEmpty())
    {
      QList<XCLParsingExpression*>* exprs=index->get(nameReference);
      for(int i=0;i<exprs->size();i++)
      {
        XCLParsingExpression* expr=exprs->at(i);
        value = expr->getValue();
        return  value;
      }
    }
    else
    {
      throw XCLException("You have to call getValues\n");
    }
  }
  catch(XCLException exception)
  {
    exception.message();
    throw XCLException("Parameter Reference unresolved\n");
  }

  throw XCLException("An unknown Error in XCLProcessParameter::getValue has occured\n");
}


void XCLProcessParameter::setListRef(BOOL lR)
{
  listRef=lR;
}


BOOL XCLProcessParameter::getListRef()
{
  return listRef;
}



XCLProcessParameter::VAL_TYPE XCLProcessParameter::getValueType()
{
  return valueType;
}


void XCLProcessParameter::setValueType(XCLProcessParameter::VAL_TYPE type)
{
  valueType = type;
}


void XCLProcessParameter::print(QString format)
{
  if(!interpretedValue.isEmpty())
    std::cout<<format.toStdString()<<"interpretedValue: "<<interpretedValue.toStdString()<<std::endl;
  if(!valueReference.isEmpty())
    std::cout<<format.toStdString()<<"valueRef: "<<valueReference.toStdString()<<std::endl;
  if(!nameReference.isEmpty())
    std::cout<<format.toStdString()<<"nameRef: "<<nameReference.toStdString()<<std::endl;
  if(listRef)
    std::cout<<format.toStdString()<<"Reference A List"<<std::endl;
  else
    std::cout<<format.toStdString()<<"Reference A Single Value"<<std::endl;

}
