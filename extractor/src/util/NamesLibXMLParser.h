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
#include <QString>
#include <QDomNode>
#include <QVector>
#include "Defines.h"
#include "NamesLibProperty.h"


class NamesLibXMLParser
{
public:
	
 
	NamesLibXMLParser();
	~NamesLibXMLParser();
	
	
	void parse(QString NamesLibPath);		//parse() appends values from NamesLibPath-file into the data structure-properties
	QVector<NamesLibProperty *> getProperties();

	int hasValue(QString searchValue);
	int hasID(QString searchId);
	int remove(int qvectorPosition);


	

private:

	QVector<NamesLibProperty *> properties;

	void setProperties(QString NamesLibPath, QVector<NamesLibProperty *> &properties);
	void recurseNode(QDomNode node, QVector<NamesLibProperty *> &properties);
	
};

