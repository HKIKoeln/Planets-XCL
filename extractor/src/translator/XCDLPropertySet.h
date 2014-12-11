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
#ifndef XCDLPROPERTYSET_H
#define XCDLPROPERTYSET_H
#include "Defines.h"
#include <QVector>
#include <QString>

class XCDLPropertySet
{
public:
  XCDLPropertySet();
  const QVector<QString>& getValueSetRelationIDs();
  const QVector<QString>& getValueSetRelationNames();
  const QString& getDataRefIndicator();
  const QString& getNormDataId();
  const QVector<QString>& getStart();
  const QVector<QString>& getEnd();
  BOOL hasNormDataReference();
  const QString& getId();
  BOOL isEmpty();

  void setValueSetRelationIDs(const QVector<QString>&);
  void setValueSetRelationNames(const QVector<QString>&);
  void setDataRefIndicator(const QString&);
  void setNormDataId(const QString& );

  void setStart(const QVector<QString>& );
  void setEnd(const QVector<QString>& );

  void addStartNEnd(const QString&, const QString& );
  //void addEnd(const QString& );

  void setHasNormDataReference(BOOL b);
  void setId(const QString& id);

  void addValueSetRelationId(const QString& id);
  void addValueSetRelationName(const QString& name);
  BOOL contains(QString id);	
  void print(); 

  BOOL operator==(XCDLPropertySet& set);
  BOOL contains(XCDLPropertySet set);
  void add(XCDLPropertySet set);
  void merge(XCDLPropertySet set);

  void replaceID(QString oldID, const QString& newID);
private:
  QVector<QString> valueSetRelationIDs;
  QVector<QString> valueSetRelationNames;
  QString dataRefIndicator;
  QString normDataId;
  QVector<QString> start;
  QVector<QString> end;	
  BOOL hasRef;
  QString id;
};

#endif
