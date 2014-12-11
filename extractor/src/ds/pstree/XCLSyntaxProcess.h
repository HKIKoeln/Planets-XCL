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
#ifndef XCLSYNTAXPROCESS_H
#define XCLSYNTAXPROCESS_H
#include "XCLSyntaxExpression.h"
#include "XCLSyntaxElement.h"
#include "XCLProcessMethod.h"
#include <QString>
#include <QVector>


class XCLSyntaxProcess : public XCLSyntaxElement
{
public://Members
 enum PROCESS_TYPE
    {
        PUSHXCEL = 0,
        PULLXCEL = 1,
        PUSHBINARY = 2,
        IMPLICITE = 3,
        CONFIGPARSER = 4
    };
public:// Constructors/Destructors

  XCLSyntaxProcess ();
  virtual ~XCLSyntaxProcess ( );
  XCLSyntaxProcess(const XCLSyntaxProcess& p);
  XCLSyntaxProcess& operator=(const XCLSyntaxProcess& p);
 
private://Members
  enum PROCESS_TYPE type;
  QString xcelReference; //!<The item to which the processing methods are applied, identified by its ID
  QString xcelNameReference;//!<The item to which the processing methods are applied, identified by its name
  QVector<XCLProcessMethod> methods;
  QVector<XCLFilter*> filterChain;
public://inherited
  enum XCLSyntaxElement::ELEMENT_TYPE getElementType()const;
  void print(QString format="");
public: 
  void addMethod(const XCLProcessMethod& m);

  void setXcelReference ( const QString& new_var );
  const QString& getXcelReference ( );

  void setXcelNameReference ( const QString& new_var );
  const QString& getXcelNameReference ( );

  void setType(const QString& t);
  enum PROCESS_TYPE getType();

  const QVector<XCLProcessMethod>& getMethods ( );
private:
  void setMethods ( const QVector<XCLProcessMethod>& new_var );
};

#endif // XCLSYNTAXPROCESS_H
