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
#include "XCLParsingExpression.h"
#include "XCLException.h"
#include "XCLParsingItem.h"
#include "XCLParsingSymbol.h"
// Constructors/Destructors
//

XCLParsingExpression::XCLParsingExpression ( )
{
  isBigEndian=FALSE;
  valueType = REGEXP;
  printme=FALSE;
  isExternalDefined=FALSE;
  normDataRelation = FALSE;
  length=LENGTH_MAX;
  start = LENGTH_MAX;
  interpretationType=CHARACTER;
  objectType="";
  newObject=FALSE;
}


XCLParsingExpression::~XCLParsingExpression ( )
{
	for(int i=0;i<relations.size();i++)
	{
		delete relations.at(i);
	}
}


XCLParsingExpression::XCLParsingExpression (const XCLParsingExpression& exp)
{
  name=exp.name;
  identifier=exp.identifier;
  start=exp.start;
  end=exp.end;
  length=exp.length;
  contextParent=exp.contextParent;
  originalName=exp.originalName;
  isBigEndian=exp.isBigEndian;
  interpretationType=exp.interpretationType;
  positionType=exp.positionType;
  valueType=exp.valueType;
  expr_type=exp.expr_type;
  filterChain=exp.filterChain;
  printme = exp.printme;
  isExternalDefined=exp.isExternalDefined;
  externalXCDL=exp.externalXCDL;
  normDataRelation = exp.normDataRelation;
  setRelations(exp.relations);
  objectType=exp.objectType;
  newObject=exp.newObject;
}


XCLParsingExpression& XCLParsingExpression::operator=(const XCLParsingExpression &exp)
{
  if(this!=&exp)
  {
    name=exp.name;
    identifier=exp.identifier;
    start=exp.start;
    end=exp.end;
    length=exp.length;
    contextParent=exp.contextParent;
    originalName=exp.originalName;
    isBigEndian=exp.isBigEndian;
    interpretationType=exp.interpretationType;
    positionType=exp.positionType;
    valueType=exp.valueType;
    expr_type=exp.expr_type;
    filterChain=exp.filterChain;
    printme=exp.printme;
    isExternalDefined=exp.isExternalDefined;
    externalXCDL=exp.externalXCDL;
    normDataRelation = exp.normDataRelation;
    setRelations(exp.relations);
    objectType=exp.objectType;
    newObject=exp.newObject;
  }
  return *this;
}
BOOL XCLParsingExpression::containsValue(const QString& val)
{
	return getInterpretedValue() == val;
}
BOOL XCLParsingExpression::compareByValue(XCLParsingExpression* expr)
{
	return getInterpretedValue() == expr->getInterpretedValue();
}
void XCLParsingExpression::addRelation(XCLParsingExpression* expr)
{
	if(expr->getType() == ITEM_TYPE)
	{
		XCLParsingItem* item = new XCLParsingItem(*((XCLParsingItem*)expr));
		relations.append(item);
	}
	else 
	{	
		XCLParsingSymbol* symb = new XCLParsingSymbol(*((XCLParsingSymbol*)expr));
		relations.append(symb);
	}

}
const QVector<XCLParsingExpression*>& XCLParsingExpression::getRelations()
{
return relations;
}




void XCLParsingExpression::setRelations(const QVector<XCLParsingExpression*>& rel)
{
	relations.clear();
	addRelations(rel);
}

void XCLParsingExpression::addRelations(const QVector<XCLParsingExpression*>& rels)
{
	for(int i=0;i<rels.size();i++)
	{
		addRelation(rels.at(i));
	}
}

BOOL XCLParsingExpression::getNormDataRelation()
{
	return normDataRelation;
}
void XCLParsingExpression::setNormDataRelation(BOOL b)
{
	normDataRelation = b;
}
void XCLParsingExpression::addFilter(const XCLFilter& filter)
{

for(int i=0;i<filterChain.size();i++)
{
XCLFilter f1= filterChain.at(i);
XCLFilter f2 = filter;
if(f1.getFilterName() == f2.getFilterName())
return;
}


filterChain.append( filter );

}


void XCLParsingExpression::addValueToPosition(_LENGTH val)
{
  if(start != LENGTH_MAX)
    start+=val;
  if(end != LENGTH_MAX)
    end += start ;
}


/**
 * Get the value of end
 * @return the value of end
 */
_LENGTH XCLParsingExpression::getEnd ( )
{
  return end;
}


const XCLFilter XCLParsingExpression::getFilter(const _UINT32 filterno)
{
  _UINT32 size = filterChain.size();
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


const QVector<XCLFilter>& XCLParsingExpression::getFilterChain()
{
  return filterChain;
}


/**
 * Get the value of identifier
 * @return the value of identifier
 */
const QString& XCLParsingExpression::getIdentifier ( )
{
  return identifier;
}


enum XCLParsingExpression::INTRPR_TYPE XCLParsingExpression::getInterpretationType()
{
  return interpretationType;
}


/**
 * Get the value of isBigEndian
 * @return the value of isBigEndian
 */
BOOL XCLParsingExpression::getIsBigEndian ( )
{
  return isBigEndian;
}


/**
 * @return _SIZE
 */
_SIZE XCLParsingExpression::getLength ( )
{
  return length;
}


/**
 * Get the value of name
 * @return the value of name
 */
const QString& XCLParsingExpression::getName ( )
{
  return name;
}


/**
 * Get the value of originalName
 * @return the value of originalName
 */
const QString& XCLParsingExpression::getOriginalName ( )
{
  return originalName;
}


enum XCLParsingExpression::POS_TYPE XCLParsingExpression::getPositionType()
{
  return positionType;
}


/**
 * Get the value of contextParent
 * @return the value of contextParent
 */
XCLParsingItem* XCLParsingExpression::getParent ( )
{
  return contextParent;
}


/**
 * Get the value of start
 * @return the value of start
 */
_LENGTH XCLParsingExpression::getStart ( )
{
  return start;
}


enum XCLParsingExpression::VAL_TYPE XCLParsingExpression::getValueType()
{
  return valueType;
}


/**
 * Set the value of end
 * @param new_var the new value of end
 */
void XCLParsingExpression::setEnd ( _LENGTH new_var )
{
  if(new_var != LENGTH_MAX)
  {
    end = new_var;
    if(length !=LENGTH_MAX)
      start=end-length+1;
    if(start != LENGTH_MAX)
      length=end-start+1;
  }
}

/*
void XCLParsingExpression::setFilterChain(const QVector<XCLFilter>& fCh)
{
  filterChain=fCh;
std::cout<<"Parsing "<<fCh.size()<<"\n";
}
*/

/**
 * Set the value of identifier
 * @param new_var the new value of identifier
 */
void XCLParsingExpression::setIdentifier ( const QString& new_var )
{
  identifier = new_var;
}


void XCLParsingExpression::setInterpretationType(enum INTRPR_TYPE t)
{
  interpretationType=t;
}


/**
 * Set the value of isBigEndian
 * @param new_var the new value of isBigEndian
 */
void XCLParsingExpression::setIsBigEndian ( BOOL new_var )
{
  isBigEndian = new_var;
}


/**
 * Set the value of length
 * @param new_var the new value of length
 */
void XCLParsingExpression::setLength ( _LENGTH new_var )
{
  if(new_var != LENGTH_MAX)
  {
    length = new_var;

    if(start != LENGTH_MAX)
      end=start+length-1;
    else if(end != LENGTH_MAX)
      start=end-length+1;
  }
}


/**
 * Set the value of name
 * @param new_var the new value of name
 */
void XCLParsingExpression::setName ( const QString& new_var )
{
  name = new_var;
}


/**
 * Set the value of originalName
 * @param new_var the new value of originalName
 */
void XCLParsingExpression::setOriginalName ( const QString& new_var )
{
  originalName = new_var;
}


/**
 * Set the value of contextParent
 * @param new_var the new value of contextParent
 */
void XCLParsingExpression::setParent ( XCLParsingItem* new_var )
{
  contextParent = new_var;
}


void XCLParsingExpression::setPositionType(enum POS_TYPE t)
{
  positionType=t;
}


const QVector<XCLParsingExpression*>& XCLParsingExpression::getSiblings()
{
  return contextParent->getChildren();
}


/**
 * Set the value of start
 * @param new_var the new value of start
 */
void XCLParsingExpression::setStart ( _LENGTH new_var )
{
  if(new_var != LENGTH_MAX)
  {

    start=new_var;
    if(length != LENGTH_MAX)
      end=start+length-1;
    else if(end != LENGTH_MAX)
      length = end-start+1;
  }
}


void XCLParsingExpression::setValueType(enum VAL_TYPE t)
{
  valueType=t;
}


void XCLParsingExpression::print(QString format)
{
  QTextStream debugOut(stdout);
  print(debugOut, format);
}


void XCLParsingExpression::print(QTextStream& debugOut, QString format)
{
	debugOut<<"\n"<<format<<"------------------------------------------------\n";
	debugOut<<format<<"| Name("<< name <<")\tIdentifier("<< identifier<<")\tnewObject("<< newObject<<")\tobjectType("<< objectType<<")\tstart("<< start <<")\tend("<< end <<")\tlength("<<length <<")\n";
	if(relations.size() != 0)
	{
		debugOut<<format<<"| Relations(";
		for(int i=0;i<relations.size();i++)
		{
			debugOut<<relations.at(i)->getIdentifier()<<", ";
		} 
		debugOut<<")\n";
	}
	
	/*
	if(identifier.isEmpty())debugOut<<"ident is empty\n";
	debugOut<<format<<"ByteOrder: ";
	if(isBigEndian)debugOut<<"BigEndian"<<"\n";
	else debugOut<<"LittleEndian"<<"\n";
	debugOut<<format<<"name: \t\t\t\t"<< name <<"\n";
	*/
	if(!filterChain.isEmpty())
	{
		debugOut<<format<<"FilterChain: \n";
		for(int i=0;i<filterChain.size();i++)
		{
			XCLFilter f=filterChain.at(i);f.print(format+"\t");
		}
	}
}

void XCLParsingExpression::setPrintMe(const QString& p)
{
if(p=="true")
	printme=TRUE;
}
BOOL XCLParsingExpression::getPrintMe()
{
return printme;
}
void XCLParsingExpression::setPrintMe(BOOL p)
{
printme = p;
}
BOOL XCLParsingExpression::getIsExternalDefined()
{
return isExternalDefined;
}
 void XCLParsingExpression::setIsExternalDefined(BOOL tof)
{
if(tof)
std::cout<<"EXTERNAL copied\n";
isExternalDefined = tof;
}

const QString& XCLParsingExpression::getExternalXCDL()
{
return externalXCDL;
}
void XCLParsingExpression::setExternalXCDL(const QString& e)
{
 externalXCDL=e;
}

const QString& XCLParsingExpression::getObjectType( )
{
  return objectType;
}
void XCLParsingExpression::setObjectType ( const QString& objType )
{
  objectType = objType;
}

BOOL XCLParsingExpression::getNewObject()
{
return newObject;
}
void XCLParsingExpression::setNewObject(BOOL nObj)
{
newObject=nObj;
}

