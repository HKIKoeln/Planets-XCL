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
#include "XCLSyntaxItem.h"
#include "XCLSyntaxExpression.h"
#include "XCLException.h"
#include "XCLParsingItem.h"
#include "XCLParsingSymbol.h"
#include <iostream>

XCLSyntaxExpression::XCLSyntaxExpression ()
    :XCLSyntaxElement()
{
  isMultiple = FALSE;
  isOptional = FALSE;
  start = LENGTH_MAX;
  end = LENGTH_MAX;
  length = LENGTH_MAX;
  hasJmpd=FALSE;
  isBigEndian=FALSE;
  positionType=XCLSyntaxExpression::UNDEFINED;
  lengthType=XCLSyntaxExpression::UNDEFINED;
  isExternalDefined = FALSE;
  normDataRelation = FALSE;
  objectType="";
  newObject=FALSE;
}


/**
 * Copy Constructor
 */
XCLSyntaxExpression::XCLSyntaxExpression(const XCLSyntaxExpression& expr)
    : XCLSyntaxElement(expr)
{
  isBigEndian=expr.isBigEndian;
  isMultiple=expr.isMultiple;
  isOptional=expr.isOptional;
  name=expr.name;
  originalName=expr.originalName;
  identifier=expr.identifier;
  start=expr.start;
  end=expr.end;
  length=expr.length;
  hasJmpd=expr.hasJmpd;
  positionType=expr.positionType;
  lengthType=expr.lengthType;
  valueType=expr.valueType;
  for(int i=0;i<expr.filterChain.size();i++)
    filterChain.append(expr.filterChain.at(i));
  isExternalDefined = expr.isExternalDefined;
  externalXCDL=expr.externalXCDL;
  normDataRelation = expr.normDataRelation;
  relations = expr.relations;
  objectType=expr.objectType;
  newObject=expr.newObject;
}


enum XCLParsingExpression::VAL_TYPE XCLSyntaxExpression::getValueType()
{
  return valueType;
}


void XCLSyntaxExpression::setValueType(enum XCLParsingExpression::VAL_TYPE t)
{
  valueType=t;
}


enum XCLSyntaxExpression::POS_TYPE XCLSyntaxExpression::getPositionType()
{
  return positionType;
}


void XCLSyntaxExpression::setPositionType(enum POS_TYPE t)
{
  positionType=t;
}


enum XCLSyntaxExpression::POS_TYPE XCLSyntaxExpression::getLengthType()
{
  return lengthType;
}


void XCLSyntaxExpression::setLengthType(enum POS_TYPE t)
{
  lengthType=t;
}


/**
* Copy Operator
*/
XCLSyntaxExpression& XCLSyntaxExpression::operator=(
  const XCLSyntaxExpression& expr)
{
  if(this!=&expr)
  {
    XCLSyntaxElement::operator=(expr);
    isBigEndian=expr.isBigEndian;
    isMultiple=expr.isMultiple;
    isOptional=expr.isOptional;
    name=expr.name;
    originalName=expr.originalName;
    identifier=expr.identifier;
    start=expr.start;
    end=expr.end;
    length=expr.length;
    hasJmpd=expr.hasJmpd;
    positionType=expr.positionType;
    lengthType=expr.lengthType;
    for(int i=0;i<expr.filterChain.size();i++)
      filterChain.append(expr.filterChain.at(i));
    isExternalDefined = expr.isExternalDefined;
    externalXCDL=expr.externalXCDL;
    normDataRelation = expr.normDataRelation;
    relations = expr.relations;
    objectType=expr.objectType;
    newObject=expr.newObject;
  }

  return *this;
}
void XCLSyntaxExpression::addRelation(XCLParsingExpression* rel)
{

	if(rel->getType() == XCLParsingExpression::ITEM_TYPE)
	{
		XCLParsingItem* item = new XCLParsingItem();
		*item = *(XCLParsingItem*)rel;
		relations.append(item);
	}
	else if(rel->getType() == XCLParsingExpression::SYMBOL_TYPE)
	{
		XCLParsingSymbol* symb = new XCLParsingSymbol();
		*symb = *(XCLParsingSymbol*)rel;
		relations.append(symb);
	}

}
void XCLSyntaxExpression::cleanRelations()
{
	relations.clear();
}
const QVector<XCLParsingExpression*>& XCLSyntaxExpression::getRelations()
{
return relations;
}

void XCLSyntaxExpression::setRelations(const QVector<XCLParsingExpression*>& rel)
{
relations = rel;
}

BOOL XCLSyntaxExpression::getNormDataRelation()
{
	return normDataRelation;
}
void XCLSyntaxExpression::setNormDataRelation(BOOL b)
{
	normDataRelation = b;
}



BOOL XCLSyntaxExpression::getIsBigEndian()
{
  return isBigEndian;
}


void XCLSyntaxExpression::setIsBigEndian(BOOL iB)
{
  isBigEndian=iB;
}


void XCLSyntaxExpression::setByteOrder(const QString& bo)
{
  if (bo=="bigEndian")
  {
    isBigEndian=TRUE;
    //std::cout<<"bigEndian\n";
  }
  else
  {
    isBigEndian=FALSE;
    //std::cout<<"LittleEndian\n";
  }
}


/**
 * Set the value of isMultiple
 * @param new_var the new value of isMultiple
 */
void XCLSyntaxExpression::setIsMultiple ( BOOL new_var )
{
  isMultiple = new_var;
}


/**
 * Get the value of isMultiple
 * @return the value of isMultiple
 */
BOOL XCLSyntaxExpression::getIsMultiple ( )
{
  return isMultiple;
}


void XCLSyntaxExpression::addFilter(const XCLFilter& filter)
{
for(int i=0;i<filterChain.size();i++)
{
XCLFilter f1= filterChain.at(i);
XCLFilter f2 = filter;
if(f1.getFilterName() == f2.getFilterName())
return;
}


filterChain.append( filter );
//std::cout<<"\nSyntax ADD Filter: "<<filterChain.size()<<" \n";
}


void XCLSyntaxExpression::setFilter(const XCLFilter& filter)
{
  filterChain.clear();
  filterChain.append(filter);
}


const QVector<XCLFilter>& XCLSyntaxExpression::getFilterChain()
{
  return filterChain;
}


const XCLFilter XCLSyntaxExpression::getFilter(const _UINT32 filterno)
{
  _LENGTH size = filterChain.size();
  if (filterno < size)
  {
    XCLFilter temp =  filterChain.at(filterno);
    return temp;
  }
  else
  {
    throw XCLException("no filter found");
  }
}


/**
 * Set the value of isOptional
 * @param new_var the new value of isOptional
 */
void XCLSyntaxExpression::setIsOptional ( BOOL new_var )
{
  isOptional = new_var;
}


/**
 * Get the value of isOptional
 * @return the value of isOptional
 */
BOOL XCLSyntaxExpression::getIsOptional ( )
{
  return isOptional;
}


/**
 * Set the value of name
 * @param new_var the new value of name
 */
void XCLSyntaxExpression::setName ( const QString& new_var )
{
  name = new_var;
}


/**
 * Get the value of name
 * @return the value of name
 */
const QString& XCLSyntaxExpression::getName ( )
{
  return name;
}


/**
 * Set the value of originalName
 * @param new_var the new value of originalName
 */
void XCLSyntaxExpression::setOriginalName ( const QString& new_var )
{
  originalName = new_var;
}


/**
 * Get the value of originalName
 * @return the value of originalName
 */
const QString& XCLSyntaxExpression::getOriginalName ( )
{
  return originalName;
}


/**
 * Set the value of identifier
 * @param new_var the new value of identifier
 */
void XCLSyntaxExpression::setIdentifier (const QString& new_var )
{
  identifier = new_var;
}


/**
 * Get the value of identifier
 * @return the value of identifier
 */
const QString& XCLSyntaxExpression::getIdentifier ( )
{
  return identifier;
}


/**
 * Get the value of start
 * @return the value of start
 */
_LENGTH XCLSyntaxExpression::getStart ( )
{
  return start;
}


/**
 * Get the value of end
 * @return the value of end
 */
_LENGTH XCLSyntaxExpression::getEnd ( )
{
  return end;
}


/**
 * Set the value of start
 * @param new_var the new value of start
 */
void XCLSyntaxExpression::setStart ( _LENGTH new_var )
{

  if(new_var > LENGTH_MAX)throw XCLException("Start out of Range");
  start=new_var;
/*  if(length != LENGTH_MAX)
 {
std::cout<<"XCLSyntaxExpression::setStart Length "<<length<<"\n";
    end=start+length-1;
}
  else if(end != LENGTH_MAX)
    length = end-start+1;*/
  // positionType=FIXED;

}


/**
 * Set the value of end
 * @param new_var the new value of end
 */
void XCLSyntaxExpression::setEnd ( _LENGTH new_var )
{
  if(new_var > LENGTH_MAX)throw XCLException("End out of Range");
  end = new_var;
  if(length !=LENGTH_MAX)
    start=end-length+1;
  else if(start != LENGTH_MAX)
    length=end-start+1;
}


/**
 * Set the value of length
 * @param new_var the new value of length
 */
void XCLSyntaxExpression::setLength ( _LENGTH new_var )
{
  if(new_var > LENGTH_MAX)throw XCLException("Length out of Range");
  length = new_var;
  if(start != LENGTH_MAX)
    end=start+length-1;
  else if(end != LENGTH_MAX)
    start=end-length+1;
}


/**
 * Get the value of length
 * @return the value of length
 */
_LENGTH XCLSyntaxExpression::getLength ( )
{
  return length;
}


BOOL XCLSyntaxExpression::hasJumped()
{
  return hasJmpd;
}


void XCLSyntaxExpression::setHasJumped(BOOL hj)
{
  hasJmpd=hj;
}

 void XCLSyntaxExpression::representsExternalXCDL(const QString& ext)
  {
//std::cout<<"Set EXTERNAL to TRUE\n";
	isExternalDefined=TRUE;
	externalXCDL=ext;
  }
 BOOL XCLSyntaxExpression::getIsExternalDefined()
{
return isExternalDefined;
}
 void XCLSyntaxExpression::setIsExternalDefined(BOOL tof)
{
isExternalDefined = tof;
}
const QString& XCLSyntaxExpression::getExternalXCDL()
{
return externalXCDL;
}
void XCLSyntaxExpression::setExternalXCDL(const QString& e)
{
 externalXCDL=e;
}
const QString& XCLSyntaxExpression::getObjectType()
{
return objectType;
}
void XCLSyntaxExpression::setObjectType(const QString& objT)
{
 objectType=objT;
}

BOOL XCLSyntaxExpression::getNewObject()
{
return newObject;
}
void XCLSyntaxExpression::setNewObject(BOOL nObj)
{
 newObject=nObj;
}
void XCLSyntaxExpression::print(QString format)
{
  XCLSyntaxElement::print(format);
  std::cout<<format.toStdString()<<"name: "<< name.toStdString() <<std::endl;
  std::cout<<format.toStdString()<<"originalName: "<< originalName.toStdString() <<std::endl;
  //  std::cout<<format.toStdString()<<"type: "<< type.toStdString() <<std::endl;
  std::cout<<format.toStdString()<<"identifier: "<< identifier.toStdString() <<std::endl;
  std::cout<<format.toStdString()<<"objectType: "<< objectType.toStdString() <<std::endl;
  std::cout<<format.toStdString()<<"newObject: ";
  if(newObject) std::cout<<"TRUE"<<std::endl;
  else std::cout<<"FALSE"<<std::endl;
  std::cout<<format.toStdString()<<"byteOrder: ";
  if(isBigEndian)std::cout<<"BigEndian"<<std::endl;
  else std::cout<<"LittleEndian"<<std::endl;
  std::cout<<format.toStdString()<<"multiple: ";
  if(isMultiple) std::cout<<"TRUE"<<std::endl;
  else std::cout<<"FALSE"<<std::endl;
  std::cout<<format.toStdString()<<"optional: ";
  if(isOptional) std::cout<<"TRUE"<<std::endl;
  else std::cout<<"FALSE"<<std::endl;
  std::cout<<format.toStdString()<<"start: "<< start <<std::endl;
  std::cout<<format.toStdString()<<"end: "<< end <<std::endl;
  std::cout<<format.toStdString()<<"length: "<< length <<std::endl;
  if(identifier.isEmpty())std::cout<<"ident is empty\n";
}
