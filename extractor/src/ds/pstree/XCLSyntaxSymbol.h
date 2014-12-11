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

#ifndef XCLSYNTAXSYMBOL_H
#define XCLSYNTAXSYMBOL_H
#include "XCLSyntaxExpression.h"
#include <QHash>
#include <QString>
class XCLSyntaxSymbol : public XCLSyntaxExpression
{
public:// Constructors/Destructors
  XCLSyntaxSymbol ();
  XCLSyntaxSymbol(const XCLSyntaxSymbol& symb);
  XCLSyntaxSymbol& operator=(const XCLSyntaxSymbol& symb);
  virtual ~XCLSyntaxSymbol ( );
protected://members
  QString interpretation;//!< The data type of the symbol's value. The default is uint8.
  QString encoding;
  QHash<QString,QString> semanticInterpretation;//!< The interpretation of the symbol's value as given in the file format description and defined in the Names Lib Schemas.
  QVector<QString> validValues;
  QVector<QString> nonValidValues;
  QVector<QByteArray> nonValidValuesBA;
public://inherited
   enum XCLSyntaxElement::ELEMENT_TYPE getElementType()const;
   void print(QString format="");
   virtual const QString& getInterpretation ( );
public:
  void setInterpretation ( const QString& new_var );
  void setEncoding ( const QString& new_var );
 const QString& getEncoding ();

  void addValidValue(const QString& valVal);
  void setValidValues ( const QVector<QString>& new_var );
  const QVector<QString>& getValidValues ( );

  void addNonValidValue(const QString& nonValVal);
  const QVector<QString>& getNonValidValues ( );
  const QVector<QByteArray>& getNonValidValuesBA ( );

  void addSemanticInterpretation(const QString& key, const QString& val);
  const QHash<QString,QString>& getSemanticInterpretation ( );
  void setSemanticInterpretation ( const QHash<QString,QString>& new_var );

private:
  void revert(QString& str);
  _LENGTH getSize(const QString& valVal);
};

#endif // XCLSYNTAXSYMBOL_H
