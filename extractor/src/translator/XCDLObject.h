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
#ifndef XCDLOBJECT_H
#define XCDLOBJECT_H


#include <QVector>
#include <QString>
#include "XCDLProperty.h"
#include "XCDLPropertySet.h"

/**
	@author Elona Chudobkaite <elona@ariel.hki.uni-koeln.de>
*/
class XCDLObject{
public:
   XCDLObject();
   XCDLObject(	QString normData, 
               			QVector<XCDLProperty> p,
                		QVector<XCDLPropertySet>  pS,
                		QString oT,
                  		QString oID);

    ~XCDLObject();
    
    const QString& getNormData();
    const QVector<XCDLProperty>& getProperties();
    const QVector<XCDLPropertySet>& getPropertySets();
    const QString& getObjectType()
    {
	//return parseTree.getObjectType();
        return objectType;
    }
    
    const QString& getObjectID();
    void setObjectID(const QString& oID);
  
    
    
    QVector<XCDLProperty> properties;
    QVector<XCDLPropertySet> propertySets;
    QString normDataString;
    QString objectType;
    QString objID;

};

#endif
