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
#ifndef XCLSYNTAXEXPRESSION_H
#define XCLSYNTAXEXPRESSION_H
#include "XCLParsingExpression.h"
#include "XCLSyntaxElement.h"
#include "XCLFilter.h"
#include "Defines.h"
#include <QString>
#include <QVector>

class XCLSyntaxItem;
class XCLSyntaxExpression : public XCLSyntaxElement
{
public:

  enum POS_TYPE
  {
    FIXED = 0,
    SEQUENTIAL = 1,
    PROCESSED = 2,
    UNDEFINED = 3
  };

public:
  XCLSyntaxExpression();
  ~XCLSyntaxExpression ( )
  {
	_LENGTH size= relations.size();
	for(uint i=0;i<size;i++)
	{
		delete relations.at(i);
	}
  }

  XCLSyntaxExpression(const XCLSyntaxExpression& expr);
  virtual XCLSyntaxExpression& operator=(const XCLSyntaxExpression& expr);
protected:
  QString externalXCDL;
  BOOL isExternalDefined;
  BOOL isBigEndian; //!< The endianness of the values stored within the items children. The default is FALSE, i.e. Little Endian. 
  BOOL isMultiple; //!< indicates whether an item can occur several times within a binary file or not. The default is FALSE.
  BOOL isOptional; //!< indicates whether an item has to occur within a binary file or not. The default is FALSE.
  QString name; //!< The name of an item as defined in the Names Lib Schemas.
  QString originalName; //!< The original name of an item as given in the file format specification.
  QString identifier; //!< A unique identifier used in the XCEL format description
  QString objectType;
  BOOL newObject;
  _LENGTH start; //!< The startposition of the item in the binary file in byte. The default is LENGTH_MAX.
  _LENGTH end; //!< The endposition of the item in the binary file in byte. The default is LENGTH_MAX.
  _LENGTH length; //!< The length of the item in byte. The default is LENGTH_MAX.
  enum POS_TYPE positionType;//!< Specifies the startposition of an item if possible. The default is UNDEFINED.
  enum POS_TYPE lengthType;//!< Specifies the length of an item if possible. The default is UNDEFINED. 
  enum XCLParsingExpression::VAL_TYPE valueType;
  BOOL hasJmpd;//!< indicates whether the parser had to change its reading position while parsing this item. The default is FALSE.
  QVector<XCLFilter> filterChain;//!< collects the filters and compression methods that apply to the values sotred within the items children.
  BOOL normDataRelation;
  QVector<XCLParsingExpression*> relations;
public:
  virtual const QString& getInterpretation() = 0;

public:
  void addRelation(XCLParsingExpression* rel);
  void cleanRelations();
  const QVector<XCLParsingExpression*>& getRelations(); 
  void setRelations(const QVector<XCLParsingExpression*>& rel);

  BOOL getNormDataRelation();
  void setNormDataRelation(BOOL b);
  void representsExternalXCDL(const QString& ext);
  BOOL getIsExternalDefined();
  void setIsExternalDefined(BOOL tof);
  const QString& getExternalXCDL();
  void setExternalXCDL(const QString& e);
  enum POS_TYPE getPositionType();
  void setPositionType(enum POS_TYPE t);
  enum POS_TYPE getLengthType();
  void setLengthType(enum POS_TYPE t);
  enum XCLParsingExpression::VAL_TYPE getValueType();
  void setValueType(enum XCLParsingExpression::VAL_TYPE t);


  virtual void setByteOrder(const QString& bo);
  virtual BOOL getIsBigEndian();
  virtual void setIsBigEndian(BOOL iB);

  virtual void setIsMultiple ( BOOL new_var );
  virtual BOOL getIsMultiple ( );

  virtual void addFilter(const XCLFilter& filter);
  virtual void setFilter(const XCLFilter& filter);
  virtual const QVector<XCLFilter>& getFilterChain();
  virtual const XCLFilter getFilter(const _UINT32 filterno);

  virtual void setIsOptional ( BOOL new_var );
  virtual BOOL getIsOptional ( );

  virtual void setName ( const QString& new_var );
  virtual const QString& getName ( );


  virtual void setOriginalName ( const QString& new_var );
  virtual const QString& getOriginalName ( );

  virtual void setIdentifier ( const QString& new_var );
  virtual const QString& getIdentifier ( );

  virtual void setObjectType(const QString& objT);
  virtual const QString& getObjectType();

  virtual BOOL getNewObject();
  virtual void setNewObject(BOOL nObj);

  virtual void setStart ( _LENGTH new_var );
  virtual _LENGTH getStart ( );

  virtual void setEnd ( _LENGTH new_var );
  virtual _LENGTH getEnd ( );

  virtual void setLength ( _LENGTH new_var );
  virtual _LENGTH getLength ( );

  virtual BOOL hasJumped();
  virtual void setHasJumped(BOOL hj);

  virtual void print(QString format="");
};

#endif // XCLSYNTAXEXPRESSION_H
