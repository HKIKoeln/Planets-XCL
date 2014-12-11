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
#ifndef XCLPARSINGITEM_H
#define XCLPARSINGITEM_H
#include "XCLParsingExpression.h"
#include "XCLTree.h"
#include <QString>
#include <QVector>
#include <QHash>


class XCLParsingSymbol;
class XCLParsingProperty;

class XCLParsingItem : public XCLParsingExpression
{
public:// Constructors/Destructors
  XCLParsingItem(); 
  XCLParsingItem (const XCLParsingItem& itm);
  XCLParsingItem& operator=(const XCLParsingItem& itm);
  virtual ~XCLParsingItem ( );
private://members
  QVector<XCLParsingExpression*> children; //!< List of all child elements of the parsingItem
  QString objectType;
public://inherited
  enum EXPR_TYPE getType();
  _SIZE getLength ( );
  const QByteArray& getValue ( ) ;
  const QString& getInterpretedValue ( ) ;
  const QString& getInterpretation ( );
  void 	print(QTextStream& debugOut, QString format="");
  void 	print(QString format="");
  enum INTRPR_TYPE 		getInterpretationType();
  void 	setInterpretedValue(const QString& val);
  BOOL 	containsValue(const QString& val);
  XCLParsingSymbol* getFirstProperty();
public://member methods
  void 	setChildren ( const QVector<XCLParsingExpression*>& new_var );
  const QVector<XCLParsingExpression*>& getChildren ( );
 void setObjectType(const QString& objT);
  const QString& getObjectType();
public:
  XCLParsingExpression* getLastChild();
  XCLParsingExpression* getFirstChild();
  void 	add(XCLParsingExpression* expr);
  void 	generateIDs();
  void 	generateIDs(XCLParsingItem* root,QString ip,_UINT32 iN,QHash<QString,QString>* idMap);
  XCLTree<XCLParsingExpression*>* getNameIndex();
  XCLTree<XCLParsingExpression*>* getIDIndex();
  void 	addChildren(const QVector<XCLParsingExpression*>& ch);
  const QHash<QString,XCLParsingExpression*>& getChildsByName();
  const QHash<QString,XCLParsingExpression*>& getChildsByName2();
  const QVector<XCLParsingExpression*>& getOrderedData(const QString& name);
  const QVector<XCLParsingExpression*>& getNormData();
  void removeExpressions(const QString& name);
  XCLParsingExpression* getChild(QString id);
  const QVector<XCLParsingExpression*>& getChildren(QString id);
  const QVector<XCLParsingExpression*>&  getObjectsLevel1();
  const QVector<XCLParsingExpression*>&  pickObjects();
};

#endif // XCLPARSINGITEM_H
