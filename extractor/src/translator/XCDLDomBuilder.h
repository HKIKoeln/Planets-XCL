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
#ifndef XCDLDOMBUILDER_H
#define XCDLDOMBUILDER_H
#include <QDomDocument>
#include <QVector>
#include <QString>
#include <QHash>
#include "XCDLProperty.h"
#include "XCDLPropertySet.h"
#include "XCDLObject.h"
#include "Defines.h"

class XCDLDomBuilder
{
public:
	QDomDocument create(	QString xcdlFilename,
			QString normData, 
			QVector<XCDLProperty> properties,
			QVector<XCDLPropertySet>  propertySets,
			QVector<XCDLObject*>   fileObjects,
			QString oT);
	void serialize();
private:
 	QDomDocument xcdldoc;
	QVector<XCDLProperty> properties;
	QVector<XCDLPropertySet> propertySets;
        QVector<XCDLObject*> 	  fileObjects;
	QString normDataString;
	QHash<QString,QDomElement> propertiesByName;
	QHash<QString,QDomElement> valueSetsById;
	QString objectType;
};

#endif
