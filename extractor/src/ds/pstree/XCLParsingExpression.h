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


#ifndef XCLPARSINGEXPRESSION_H
#define XCLPARSINGEXPRESSION_H

#include <QString>
#include <QVector>
#include <QTextStream>
#include "Defines.h"
#include "XCLFilter.h"

class XCLParsingItem;
class XCLParsingSymbol;
class XCLParsingExpression
{
public://enums
  enum EXPR_TYPE
  {
    ITEM_TYPE = 1,
    SYMBOL_TYPE = 2,
    PROPERTY_TYPE = 3,
  };

  enum INTRPR_TYPE
  {
    NUMERIC = 0,
    LABEL = 1,
    CHARACTER = 2
  };

  enum POS_TYPE
  {
    FIXED = 0,
    SEQUENTIAL = 1,
    PROCESSED = 2,
    UNDEFINED = 3
  };
  enum VAL_TYPE
  {
    SIMPLE = 0,
    REGEXP = 1
  };
public:// Constructors/Destructors
  XCLParsingExpression ( );
  virtual ~XCLParsingExpression ( );
  XCLParsingExpression& operator=(const XCLParsingExpression &exp);
  XCLParsingExpression (const XCLParsingExpression& exp);
protected://members
  BOOL isExternalDefined;
  QString externalXCDL;
  QString name;//!< The name of an item as defined in the Names Lib Schemas.
  QString identifier;
  QString objectType;
  BOOL newObject;
  _LENGTH start;//!< The startposition of the item in the binary file in byte. 
  _LENGTH end;//!< The endposition of the item in the binary file in byte. 
  _LENGTH length;//!< The length of the item in byte. 
  XCLParsingItem* contextParent;
  QString originalName;//!< The original name of an item as given in the file format specification.
  BOOL isBigEndian;//!< The endianness of the values stored within the items children. The default is FALSE, i.e. Little Endian. 
  enum INTRPR_TYPE interpretationType;
  enum POS_TYPE positionType;//!< Specifies the startposition of an item if possible. 
  enum VAL_TYPE valueType;//!< Specifies the length of an item if possible.  
  enum EXPR_TYPE expr_type;
  QVector<XCLFilter> filterChain;//!< collects the filters and compression methods that apply to the values sotred within the items children.
   BOOL printme;
   BOOL normDataRelation;
   QVector<XCLParsingExpression*> relations;
 
public://pure virtuals
  virtual enum EXPR_TYPE getType ( )=0;
  virtual const QByteArray& getValue ( ) = 0;
  virtual const QString& getInterpretedValue ( ) = 0;
//  virtual const QString& getParsedValue ( ) = 0;
  virtual const QString& getInterpretation ( ) = 0;
  virtual void setInterpretedValue(const QString& val) = 0;
  virtual XCLParsingSymbol* getFirstProperty() = 0;
public://virtuals
  virtual _SIZE getLength ( );
  virtual const QString& getName ( );
  virtual void addValueToPosition(_LENGTH val);
  virtual void addFilter(const XCLFilter& filter);
  virtual const QVector<XCLFilter>& getFilterChain();
//  virtual void setFilterChain(const QVector<XCLFilter>& filterChain);
  virtual const XCLFilter getFilter(const _UINT32 filterno);
  virtual void setIsBigEndian ( BOOL new_var );
  virtual void print(QTextStream& stream, QString format="");
  virtual void print(QString format="");
  virtual enum INTRPR_TYPE getInterpretationType();
  virtual BOOL containsValue(const QString& val);
  virtual void setLength ( _LENGTH new_var );
public:
  BOOL compareByValue(XCLParsingExpression* expr);

  void addRelation(XCLParsingExpression* expr);
  void addRelations(const QVector<XCLParsingExpression*>& rels);
  const QVector<XCLParsingExpression*>& getRelations(); 
  void setRelations( const QVector<XCLParsingExpression*>& rel);



  BOOL getNormDataRelation();
  void setNormDataRelation(BOOL b);
  enum VAL_TYPE getValueType();

  void setValueType(enum VAL_TYPE t);

  
  void setInterpretationType(enum INTRPR_TYPE t);

  enum POS_TYPE getPositionType();
  void setPositionType(enum POS_TYPE t);

  void setParent ( XCLParsingItem* new_var );
  XCLParsingItem* getParent ( );

  void setIdentifier ( const QString& new_var );
  const QString& getIdentifier ( );

  void setObjectType ( const QString& objType );
  const QString& getObjectType ( );

  void setNewObject ( BOOL nObj );
  BOOL getNewObject();

  void setStart ( _LENGTH new_var );
  _LENGTH getStart ( );

  _LENGTH getEnd ( );
  void setEnd ( _LENGTH new_var );

  void setOriginalName (const QString& new_var );
  const QString& getOriginalName ( );

  void setName ( const QString& new_var );

  BOOL getIsBigEndian ( );
  const QVector<XCLParsingExpression*>& getSiblings();
  void setPrintMe(const QString& p);
void setPrintMe(BOOL p);
 BOOL getPrintMe();
 BOOL getIsExternalDefined();
 void setIsExternalDefined(BOOL tof);

const QString& getExternalXCDL();
void setExternalXCDL(const QString& e);
};

#endif // XCLPARSINGEXPRESSION_H
