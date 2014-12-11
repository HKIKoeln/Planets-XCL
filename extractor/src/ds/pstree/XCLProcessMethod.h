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

#ifndef XCLPROCESSMETHOD_H
#define XCLPROCESSMETHOD_H
#include "XCLProcessParameter.h"
#include <QString>
#include <QVector>


class XCLProcessMethod
{
public:// Constructors/Destructors
  XCLProcessMethod ();
  XCLProcessMethod (const QString& mN );
  virtual ~XCLProcessMethod ( );
  XCLProcessMethod& operator=(const XCLProcessMethod& m);
private://members
  QVector<XCLProcessParameter> parameters;
  QString name;
public:
  void setName (const QString& new_var );
  const QString& getName ( );

  void setParameters (const QVector<XCLProcessParameter>& new_var );
  const QVector<XCLProcessParameter>& getParameters ( );
  void addParameter(const XCLProcessParameter& p);

  void print(QString format="");
};

#endif // XCLPROCESSMETHOD_H
