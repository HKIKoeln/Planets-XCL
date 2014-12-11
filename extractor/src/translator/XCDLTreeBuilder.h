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
#ifndef XCDLTREEBUILDER_H
#define XCDLTREEBUILDER_H

#include "XCDLProperty.h"
#include "XCDLPropertySet.h"
#include <QVector>
#include <QString>
#include <QHash>
#include "NamesLibProperty.h"
#include "XCLParsingItem.h"
#include "XCLInputNormalizer.h"
#include "XCDLObject.h"

class NormDataRelation
{
public:
	XCLParsingExpression* relatedExpression;
	int start;
	int end;
};

class XCDLTreeBuilder
{
public:
	XCDLTreeBuilder();
	void create(const XCLParsingItem& parseTree,const QVector<NamesLibProperty*>& definedProperties);
	void setDisplayNormData(BOOL);
	const QString& getNormData();
	const QVector<XCDLProperty>& getProperties();
	const QVector<XCDLPropertySet>& getPropertySets();
	const QString& getObjectType()
	{
	return objectType;
	}
	const QString& getObjectID()
        {
            return objID;
        }
        const QVector<XCDLObject*>& getFileObjects();
	void print();

	
	
private:
	void makeProperties( QList<XCLParsingExpression*> values,QString id,QString pId);
	void findAllRelations(XCLParsingItem* item, XCDLPropertySet& newPropertySet);
	BOOL isProperty(XCLParsingExpression* expr);
        void processNormData();
	void summerizeValueSets();
 	void addNormDataRelationsToValueSets();
	void getDefinedProperties();
private:
	QString normData;
	QString objectType;
	QVector<XCDLProperty> 	  properties;
	QVector<XCDLPropertySet>  propertySets;
	QVector<XCDLObject*> 	  fileObjects;
	XCLParsingItem parseTree;
	QVector<NamesLibProperty*> definedProperties;
	QHash<QString,XCLParsingExpression*> expressionsByName;
	QHash<QString,XCLParsingExpression*> mainObjectExpressionsByName;
        QVector<XCLParsingExpression*> foundProperties;
 	QVector<XCLParsingExpression*> normDataExpressions;
	XCLInputNormalizer in;
  	QVector<NormDataRelation> normDataRelations;
	XCDLObject xcdlObject;

	int objectIDNumber;
	QString objID;
        
        int xcdlPropertyID;

	BOOL bDisplayNormData;
	QString qsEmptyNormData;
	
};

#endif
