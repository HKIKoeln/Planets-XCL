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
#ifndef XCLSYNTAXITEM_H
#define XCLSYNTAXITEM_H
#include "XCLSyntaxExpression.h"
#include "Defines.h"
#include <QString>
#include <QVector>
#include <QHash>

class XCLSyntaxSymbol;
class XCLSyntaxProperty;
class XCLSyntaxProcess;

class XCLSyntaxItem : public XCLSyntaxExpression
{
public://enums
 enum ITEM_TYPE
  {
    STRUCTURING = 0,
    CONTEXT=1,
    DEFINITION=2
  };
  enum ORDER_TYPE
  {
    CHOICE = 0,
    ALL = 1,
    SEQUENCE = 2
  };
public:// Constructors/Destructors
  XCLSyntaxItem(const XCLSyntaxItem& itm);
  XCLSyntaxItem();
  virtual ~XCLSyntaxItem ( );
  XCLSyntaxItem& operator=(const XCLSyntaxItem& itm);
protected://Members
  enum ITEM_TYPE item_type;
  enum ORDER_TYPE order_type;
  QVector<XCLSyntaxElement*> children;
  void setByteOrder(const QString& bo);
  void setIsBigEndian(BOOL bo);
  QString objectType;
public: //inherited
  enum XCLSyntaxElement::ELEMENT_TYPE getElementType()const;
  const QString& getInterpretation();
  void setStart(_LENGTH s);
  void setEnd(_LENGTH e);
  void setLength(_LENGTH le);
  void print(QString format="");
public://Member methods
  void setItemType(enum ITEM_TYPE t);
  void setItemType(const QString& t);
  short getItemType();
  void setOrderType(enum ORDER_TYPE ot);
  enum ORDER_TYPE getOrderType();
  void setChildren ( const QVector<XCLSyntaxElement*>& new_var );
  void setObjectType(const QString& objT);
  const QString& getObjectType();
public://tree methods
  XCLSyntaxElement* getLastChild() const;
  XCLSyntaxElement* getFirstChild();

  const QVector<XCLSyntaxElement*>& getChildren ( );
  const QVector<XCLSyntaxElement*> getOptionalChildren ( );
  const QVector<XCLSyntaxElement*> getNonOptionalChildren ( );

  //void addChild(const XCLSyntaxElement& elem);
  BOOL replace(XCLSyntaxExpression* expr);
  BOOL remove(XCLSyntaxExpression* expr);

  void generateIDs();
  void generateIDs(QString ip,_UINT32 iN);
  void addChildsToGrammarIndex(QHash<QString,XCLSyntaxExpression*>& index);

  void add(const XCLSyntaxItem& expr);
  void add(const XCLSyntaxSymbol& expr);
  void add(const XCLSyntaxProperty& expr);
  void add(const XCLSyntaxProcess& expr);

	
private:
  void initAttributes ( ) ;

};

#endif // XCLSYNTAXITEM_H
