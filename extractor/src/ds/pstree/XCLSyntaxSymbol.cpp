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
#include "XCLSyntaxSymbol.h"
#include <QStringList>
// Constructors/Destructors
//

XCLSyntaxSymbol::XCLSyntaxSymbol () :XCLSyntaxExpression()
{
  interpretation="uint8";
  encoding="byte";
}


XCLSyntaxSymbol::XCLSyntaxSymbol(const XCLSyntaxSymbol& symb):XCLSyntaxExpression(symb)
{

  interpretation = symb.interpretation;
  encoding=symb.encoding;
  semanticInterpretation=symb.semanticInterpretation;
  validValues=symb.validValues;
  nonValidValues=symb.nonValidValues;
  nonValidValuesBA=symb.nonValidValuesBA;

  for(int i=0;i<symb.filterChain.size();i++)
    filterChain.append(symb.filterChain.at(i));

}


XCLSyntaxSymbol& XCLSyntaxSymbol::operator=(const XCLSyntaxSymbol& symb)
{
  if(this!=&symb)
  {
    XCLSyntaxExpression::operator=(symb);
    interpretation = symb.interpretation;
    encoding = symb.encoding;
    semanticInterpretation=symb.semanticInterpretation;
    validValues=symb.validValues;
    nonValidValues=symb.nonValidValues;
    nonValidValuesBA=symb.nonValidValuesBA;

    for(int i=0;i<symb.filterChain.size();i++)
      filterChain.append(symb.filterChain.at(i));

  }
  return *this;
}


XCLSyntaxSymbol::~XCLSyntaxSymbol ( )
{
}


enum XCLSyntaxElement::ELEMENT_TYPE XCLSyntaxSymbol::getElementType()const
{
  return XCLSyntaxElement::SYMBOL_TYPE;
}

void XCLSyntaxSymbol::setEncoding ( const QString& new_var )
{
	encoding = new_var;
}
const QString& XCLSyntaxSymbol::getEncoding ()
{
	if(encoding.isEmpty())
		return interpretation;
	return encoding;
}
/**
 * Set the value of interpretation
 * @param new_var the new value of interpretation
 */
void XCLSyntaxSymbol::setInterpretation ( const QString& new_var )
{
  interpretation = new_var;
}


/**
 * Get the value of interpretation
 * @return the value of interpretation
 */
const QString& XCLSyntaxSymbol::getInterpretation ( )
{
  return interpretation;
}


/**
 * Set the value of sematicInterpretation
 * @param new_var the new value of sematicInterpretation
 */
void XCLSyntaxSymbol::setSemanticInterpretation ( const QHash<QString,QString>& new_var )
{
  semanticInterpretation = new_var;
}


/**
 * Get the value of sematicInterpretation
 * @return the value of sematicInterpretation
 */
const QHash<QString,QString>& XCLSyntaxSymbol::getSemanticInterpretation ( )
{
  return semanticInterpretation;
}


/**
 * Set the value of validValues
 * @param new_var the new value of validValues
 */
void XCLSyntaxSymbol::setValidValues ( const QVector<QString>& new_var )
{
  validValues = new_var;
}


/**
 * Get the value of validValues
 * @return the value of validValues
 */
const QVector<QString>& XCLSyntaxSymbol::getValidValues ( )
{
  return validValues;
}


const QVector<QString>& XCLSyntaxSymbol::getNonValidValues ( )
{
  return nonValidValues;
}


const QVector<QByteArray>& XCLSyntaxSymbol::getNonValidValuesBA ( )
{
  return nonValidValuesBA;
}


void XCLSyntaxSymbol::addValidValue(const QString& valVal)
{
  length = getSize(valVal);
  if(length!=LENGTH_MAX)
    setLengthType(FIXED);
/*std::cout<<"length: " <<length;
std::cout<<"\tvalue: ;"<<valVal.toStdString()<<";"<<std::endl;*/
  validValues.append(valVal);
}

_LENGTH XCLSyntaxSymbol::getSize(const QString& valVal)
{

        if(interpretation == "ASCII")
        {
            //special behaviour 
            if(valVal == "NULL" ||
               valVal == "\n" ||
               valVal == "\r" ||
               valVal == "\t" ||
               valVal == " " ||
               valVal == "\f" ||
               valVal == "\x0" ||
               valVal == "&gt;" ||
               valVal == "&lt;")
                return 1;

            return valVal.size();
        }
        else if(interpretation == "uint8")
        {
                int i = valVal.count(" ");
                return i+1; 
        }

    return length;
}
void XCLSyntaxSymbol::addNonValidValue(const QString& nonValVal)
{
  nonValidValues.append(nonValVal);
  nonValidValuesBA.append(nonValVal.toAscii());
  length = getSize(nonValVal);
  if(length!=LENGTH_MAX)
    setLengthType(FIXED);
}


void XCLSyntaxSymbol::addSemanticInterpretation(const QString& key, const QString& val)
{
  // std::cout<<key.toStdString()<<" : "<< val.toStdString()<<"\n";
  QString k = key;
  if(!isBigEndian)
    revert(k);

  QString v = val;
  semanticInterpretation.insert(key,val);
  validValues.append(key);
}


void XCLSyntaxSymbol::revert(QString& str)
{
  QStringList strLst=str.split(" ");
  QString newStr;
  for(int i=strLst.size()-1;i>=0;i--)
  {
    newStr.append(strLst[i]);
    newStr.append(" ");
  }

  str=newStr.trimmed();
}


void XCLSyntaxSymbol::print(QString format)
{

  XCLSyntaxExpression::print(format);
  std::cout<<format.toStdString()<<"BitInterpretation: "<< interpretation.toStdString() <<std::endl;
  std::cout<<format.toStdString()<<"SemanticInterpretation:\n";
  QHashIterator<QString, QString> i(semanticInterpretation);
  while (i.hasNext())
  {
    i.next();
    std::cout<<format.toStdString()<<"\t"<< i.key().toStdString() << ": " << i.value().toStdString() << std::endl;
  }

  std::cout<<format.toStdString()<<"ValidValues:\n"<<format.toStdString();
  if(validValues.size()<125)
    for (int i=0;i<validValues.size();i++)
    {

      std::cout<< validValues.at(i).toStdString()<<" ";
    }
  else
    std::cout<<"Valid Values to big";
  std::cout<<"\n";

  std::cout<<format.toStdString()<<"NonValidValues:\n"<<format.toStdString();
  if(nonValidValues.size()<125)
    for (int i=0;i<nonValidValues.size();i++)
    {

      std::cout<< nonValidValues.at(i).toStdString()<<" ";
    }
  else
    std::cout<<"NonValid Values to big";
  std::cout<<"\n";
  for(int i=0;i<filterChain.size();i++)
    ((XCLFilter)filterChain.at(i)).print();
  std::cout<<"\n";

}
