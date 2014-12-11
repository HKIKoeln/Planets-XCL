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
#ifndef NAMESLIBPROPERTY_H
#define NAMESLIBPROPERTY_H
#include <QString>
#include <QVector>


class NamesLibProperty
{
public:
	

	NamesLibProperty(QString value, QString id, QString documentation);
        NamesLibProperty(const NamesLibProperty& prop)
        {
            value=prop.value;
            id =prop.id;
            documentation=prop.documentation;
        }
	~NamesLibProperty();

	QString getPropertyValue();
	QString getPropertyId();
	QString getPropertyDocumentation();
	
	
	



private:
	QString value;				//Property-name
	QString id; 				//Property-id
	QString documentation;			//Property-documentation
	//QVector<QString> properties_vector;	//admissible values of property (zulässige Werte)
	
};
#endif

