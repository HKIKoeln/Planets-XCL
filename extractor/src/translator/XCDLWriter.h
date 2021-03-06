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
#ifndef XCDLWRITER_H
#define XCDLWRITER_H

#include "XCLParsingItem.h"
#include "XCLStringConverter.h"
//#include "XCDLWriterFactory.h"
#include <QDomDocument>
#include <QHash>
#include "XCLInputNormalizer.h"
#include "XCLTree.h"
#include "NamesLibProperty.h"


/*
class NormDataRelation
{
public:
	XCLParsingExpression* relatedExpression;
	int start;
	int end;
};
*/

class XCDLWriter
{
public:
  XCDLWriter();
  ~XCDLWriter();
  QDomDocument write(const XCLParsingItem& pT,const QString& xcdlFilename);
  void setDefinedProperties(const QVector<NamesLibProperty *> defProps);
  void setDisplayRawData(BOOL on);
  void setDisplayNormData(BOOL);
private:
  XCLParsingItem parseTree;
  QVector<NamesLibProperty *> definedProperties; 
  QDomDocument xcdldoc;
  BOOL displayRawData;
  BOOL bDisplayNormData;
};

#endif
