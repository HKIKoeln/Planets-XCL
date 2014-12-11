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

#ifndef XCLPROCESSPARAMETER_H
#define XCLPROCESSPARAMETER_H
#include "Defines.h"
#include <QString>
#include <QByteArray>
#include <QVector>
#include "XCLTree.h"

class XCLParsingExpression;
class XCLProcessParameter
{
public:// Constructors/Destructors
  XCLProcessParameter ( );
  virtual ~XCLProcessParameter ( );
  XCLProcessParameter(const XCLProcessParameter& p);
  XCLProcessParameter& operator=(const XCLProcessParameter& p);
  enum VAL_TYPE
  {
    SIMPLE = 0,
    VALUEREF = 1,
    NAMEREF = 2,
    MATHEX = 3
  };
private://members
  QString interpretedValue;
  QString valueReference;
  QString nameReference;
  QByteArray value;
  VAL_TYPE valueType;
  BOOL listRef;
public:
  const QString& getValue ( );
  const QString& getValue (XCLTree<XCLParsingExpression*>& index );
  const QString& getValues (XCLTree<XCLParsingExpression*>& index );
  const QByteArray& getValueAsBA(XCLTree<XCLParsingExpression*>* index);

  void setValueReference ( const QString& new_var );
  const QString& getValueReference ( );

  void setNameReference ( const QString& new_var );
  const QString& getNameReference ( );

  void setInterpretedValue ( const QString& new_var );

  void setListRef(BOOL lR);
  BOOL getListRef();

  VAL_TYPE getValueType();
  void setValueType(VAL_TYPE tpye);

  void print(QString format="");
private:
  const QString& getInterpretedValue ( );
};

#endif // XCLPROCESSPARAMETER_H
