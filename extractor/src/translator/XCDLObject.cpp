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
#include "XCDLObject.h"
#include <iostream>

XCDLObject::XCDLObject()
{
    
//     std::cout<<"\n--------------------in XCDLObject::XCDLObject() -------------------------- \n";
    
}

XCDLObject::XCDLObject(	QString normData, 
                					QVector<XCDLProperty> p,
                					QVector<XCDLPropertySet>  pS,
                					QString oT,
                      					QString oID){
                                    
                                    normDataString = normData;
                                    properties = p;
                                    propertySets = pS;
                                    objectType=oT;
                                    objID=oID;

                }


XCDLObject::~XCDLObject()
{
}

const QString& XCDLObject::getNormData()
{
    return normDataString;
}

const QVector<XCDLProperty>& XCDLObject::getProperties()
{
    return properties;
}

const QVector<XCDLPropertySet>& XCDLObject::getPropertySets()
{
    return propertySets;
}

const QString&  XCDLObject::getObjectID()
{
    return objID;
}
void  XCDLObject::setObjectID(const QString& oID)
{
    objID=oID;
}




