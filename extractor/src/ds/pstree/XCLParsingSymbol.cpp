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
#include <QStringList>
#include <QRegExp>
#include "XCLParsingSymbol.h"
#include "XCLStringConverter.h"
#include "Defines.h"
#include "XCLInputNormalizer.h"
#include "XCLDataConverter.h"
// Constructors/Destructors
//

XCLParsingSymbol::XCLParsingSymbol ( ):XCLParsingExpression()
{
  initAttributes();
  privateLength=0;
  interpretedValue.clear();
  encoding="byte";
  isValidFlag=FALSE;
}


XCLParsingSymbol::~XCLParsingSymbol ( )
{}


XCLParsingSymbol::XCLParsingSymbol(const XCLParsingSymbol& s)
    :XCLParsingExpression(s)
{
  // XCLParsingExpression::operator=(s);
  value=s.value;
  interpretedValue = s.interpretedValue;
  interpretation = s.interpretation;
  semanticInterpretation = s.semanticInterpretation;
  validValues= s.validValues;
  nonValidValues= s.nonValidValues;
  privateLength = s.privateLength;
  compareValue =s.compareValue;
  encoding = s.encoding;
}


XCLParsingSymbol& XCLParsingSymbol::operator=(const XCLParsingSymbol& s)
{
  if(this!=&s)
  {
    XCLParsingExpression::operator=(s);
    value=s.value;
    interpretedValue = s.interpretedValue;
    interpretation=s.interpretation;
    semanticInterpretation = s.semanticInterpretation;
    validValues= s.validValues;
    nonValidValues= s.nonValidValues;
    privateLength = s.privateLength;
    compareValue =s.compareValue;
    encoding = s.encoding;
  }
  return *this;
}

XCLParsingSymbol* XCLParsingSymbol:: getFirstProperty()
{
	if(!validValues.isEmpty())
		return this;
	if(!nonValidValues.isEmpty())
		return this;

	return NULL;
}
/*! \enum XCLParsingSymbol::EXPR_TYPE XCLParsingSymbol::getType()
 *  \brief returns XCLParsingExpression::SYMBOL_TYPE.
 
 * instantiation of pure virtual function XCLParsingExpression::getType().
 */
enum XCLParsingSymbol::EXPR_TYPE XCLParsingSymbol::getType()
{
  return XCLParsingExpression::SYMBOL_TYPE;
}
void XCLParsingSymbol::setEncoding ( const QString& new_var )
{
  encoding = new_var;
}
const QString& XCLParsingSymbol::getEncoding ()
{
  return encoding;
}


/**
 * Set the value of interpretation
 * @param new_var the new value of interpretation
 */
void XCLParsingSymbol::setInterpretation ( const QString& new_var )
{

  interpretation = new_var;
  if(interpretationType!=LABEL)
  {
    if(interpretation == "ASCII" || interpretation == "Latin1" || interpretation == "utf-8")
    {
      setInterpretationType(CHARACTER);
    }
    else
    {
      setInterpretationType(NUMERIC);
    }
  }
}


/*! \const QString& XCLParsingSymbol::getInterpretation()
 *  \brief returns the interpretation of the symbols value as QString.
 */
const QString& XCLParsingSymbol::getInterpretation ( )
{
  return interpretation;
}


/**
 * Set the value of semanticInterpretation
 * @param new_var the new value of semanticInterpretation
 */
void XCLParsingSymbol::setSemanticInterpretation ( const QHash<QString,QString>& new_var )
{
  semanticInterpretation = new_var;
  if(!semanticInterpretation.isEmpty())
    interpretationType=XCLParsingExpression::LABEL;
}


/*! \QHash<QString,QString>& XCLParsingSymbol::getSemanticInterpretation()
 *  \brief returns a Hashtable which contains possible valuecodes for the symbol and their semantic labels.
 */
const QHash<QString,QString>& XCLParsingSymbol::getSemanticInterpretation ( )
{
  return semanticInterpretation;
}


/*! \void XCLParsingSymbol::setValidValues ( const QVector<QString>& new_var )
 *  \brief sets a list of values which are possible occurences for the symbol.
 *  \param new_var the new list of validValues
 */
void XCLParsingSymbol::setValidValues ( const QVector<QString>& new_var )
{
  validValues = new_var;
  for(_SIZE i = 0; i < validValues.size(); i++)
    if(validValues.at(i).count(' ') + 1 > privateLength)
      privateLength = validValues.at(i).count(' ') + 1; // = anzahl der Werte
}


/*! \void XCLParsingSymbol::setNonValidValues ( const QVector<QString>& new_var )
 *  \brief sets a list of values which are not allowed to occur for the symbol.
 *  \param new_var the new list of nonValidValues
 */
void XCLParsingSymbol::setNonValidValues ( const QVector<QString>& new_var )
{
  nonValidValues = new_var;
  for(_SIZE i = 0; i < nonValidValues.size(); i++)
  {
    if(nonValidValues.at(i).count(' ') + 1 > privateLength)
    {
      privateLength = nonValidValues.at(i).count(' ')+1; // = anzahl der Werte
    }
    if(interpretation=="ASCII")
    {
      QString str = nonValidValues.at(i);
      str.remove(QChar(' '));
      nonValidValues.replace(i,str);
    }
    if(interpretation=="utf-8")
    {
      QString str = nonValidValues.at(i);
      str.remove(QChar(' '));
      nonValidValues.replace(i,str);
    }
  }
}


/**
 * Get the value of validValues
 * @return the value of validValues
 */
const QVector<QString>& XCLParsingSymbol::getValidValues ( )
{
  return validValues;
}


/**
sets raw value as QByteArray and as QString 
*/
void XCLParsingSymbol::setValue (const QByteArray& v)
{
  value = v;
  interpretedValue="";
}


const QByteArray& XCLParsingSymbol::getValue ( )
{

  return value;
}


void XCLParsingSymbol::setInterpretedValue(const QString& v)
{
  interpretedValue=v;
  if(interpretationType==LABEL)
  {
    try
    {	
    interpretedValue = getFixedLabel();	
    }
   catch(XCLException exception)
	{
		exception.message();
	}
  }
}


/**
 * @return QString
 */
const QString& XCLParsingSymbol::getInterpretedValue ( )
{
	if(interpretedValue.isEmpty() && !value.isEmpty())
	{
		XCLInputNormalizer normalizer;
		QString* isdf=normalizer.normalizeValue((UCHAR*)value.data(),
			value.size(),
			getInterpretation(),
			getIsBigEndian());
		interpretedValue=*isdf;
		delete isdf;
	}
	return interpretedValue;
}


const QString& XCLParsingSymbol::getFixedLabel()
{
	if(!semanticInterpretation.contains(interpretedValue))
	throw XCLException("NO SEMANTIC INTERPRETATION");
	isValidFlag = TRUE;
	QString* str = new QString(semanticInterpretation.value(interpretedValue));
	return *str;
}


BOOL XCLParsingSymbol::isValid()
{
#ifdef PlanetsDebug
    std::cout<<getIdentifier().toStdString()<<" Comparevalue "<<getCompareValue().toStdString()<<"\n";
    std::cout<<"Interpreted Value: "<<interpretedValue.toStdString()<<"\n";
#endif

  if(isValidFlag)
        return TRUE;
  
  isValidFlag=FALSE;

  if(interpretationType!=LABEL)
  {
    isValidFlag = checkNonValidValues();
    if (!isValidFlag)
      return isValidFlag;

    isValidFlag = checkValidValues();
    if(!isValidFlag)
      return isValidFlag;

  }
  else
  {

    if(interpretedValue.isEmpty())
    {
      isValidFlag=FALSE;
    }
    else
    {
      if(!semanticInterpretation.contains(interpretedValue))
        return FALSE;
      else
	{
        isValidFlag=TRUE;
	}
    }

  }
  return isValidFlag;
}


BOOL XCLParsingSymbol::checkNonValidValues()
{

  if(!nonValidValues.isEmpty())
  {

    for (int i=0;i<nonValidValues.size();i++)
    {
      if(compare(nonValidValues.at(i),getCompareValue()))
      {

        return FALSE;
      }
    }
  }
  return TRUE;
}


BOOL XCLParsingSymbol::checkValidValues()
{
  if(!validValues.isEmpty())
  {

    for (int i=0;i<validValues.size();i++)
    {
      if(compare(validValues.at(i),getCompareValue()))
      {

        return TRUE;
      }
    }

    return FALSE; //NO valid value was found otherwise the return statement were reached
  }
  return TRUE;
}


BOOL XCLParsingSymbol::compare(const QString& strOrPat,const  QString& str2)
{
  if(interpretationType == CHARACTER 
	&& 
     valueType == REGEXP)
  {

    QRegExp regexp(strOrPat);
    return regexp.exactMatch(str2);
  }
  else //if(valueType == SIMPLE)
  {
    return strOrPat == str2;
  }
}


/*! \void XCLParsingSymbol::initAttributes()
 *  \brief initializes the attributes of the symbol with standard values.
 
 * sets uint8 as the default interpreation for the value of a symbol.
 */
void XCLParsingSymbol::initAttributes ( )
{
	interpretation="uint8";
}


_LENGTH XCLParsingSymbol::getPrivateLength()
{
	return privateLength;
}

_LENGTH XCLParsingSymbol::getLength(QByteArray* array)
{
  if(!nonValidValues.isEmpty() && privateLength > XCLParsingExpression::getLength())
  {
    _LENGTH mylen=LENGTH_MAX;
    _LENGTH tmp=0;
    for (int i=0;i<nonValidValues.size();i++)
    {
      tmp=array->indexOf(nonValidValues.at(i),start);
      if(tmp < mylen)
        mylen=tmp-start;
    }

    if(mylen != LENGTH_MAX)
      setLength(mylen);
  }
  return XCLParsingExpression::getLength();

}

const QString& XCLParsingSymbol::getCompareValue()
{
  if(privateLength <= length)
    return getInterpretedValue();
  else
  {
    return compareValue;
  }
}


void XCLParsingSymbol::setCompareValue(const QByteArray& cV)
{
	//std::cout<<"Symbol setCompareValue\n";
	XCLInputNormalizer normalizer;
	QString* isdf=normalizer.normalizeValue((UCHAR*)cV.data(),
		cV.size(),
		getInterpretation(),
		getIsBigEndian());
	compareValue=*isdf;
	delete isdf;
}


void XCLParsingSymbol::print(QTextStream& debugOut, QString format)
{
	/* Sebastian Beyl, 23.1.2009: Uebergangsweise Umsetzung mit Compiler-Flags, um das Problem
	   des Loggings zu umgehen. Als Bug beschrieben. */
#ifdef PlanetsDebug
	XCLParsingExpression::print(debugOut, format);
	//  debugOut<<format<<"Value: "<< getValue().toStdString()<<"\n";
	debugOut<<format<<"| Interpretation("<< interpretation<<")\tValue("<< getInterpretedValue()<<")\n";
	/* debugOut<<format<<"value Type: \tSIMPLE = 0,\tREGEXP = 1\t"<< getValueType()<<"\n";*/
	/* debugOut<<format<<"interpretation Type: \tNUMERIC = 0,\tLABEL = 1,\tCHARACTER = 2\t"<< getInterpretationType()<<"\n"
	*/
	debugOut<<format<<"| Labels(";
	
	QHashIterator<QString, QString> i(semanticInterpretation);
	while (i.hasNext())
	{
		i.next();
		debugOut<<format<<"\t"<< i.key() << ": " << i.value() << ", ";
	}
	debugOut<<format<<")\n";
	debugOut<<format<<"| ValidValues("<<format;
	for (int i=0;i<validValues.size();i++)
	{
		debugOut<< validValues.at(i)<<",";
	}
	/*for(int i=0;i<filterChain.size();i++)
	((XCLFilter)filterChain.at(i)).print();*/
	debugOut<<")\n"<<format<<" ---------------------------------------------------\n\n";
#endif

#ifndef PlanetsDebug
	getInterpretedValue();
        (void)debugOut;
        (void)format;
#endif

}


void XCLParsingSymbol::print(QString format)
{
	QTextStream debugOut(stdout);
	print(debugOut, format);
}
