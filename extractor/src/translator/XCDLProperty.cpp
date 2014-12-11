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
#include "XCDLProperty.h"


const QString& XCDLProperty::getName()
{
return name;
}
const QString& XCDLProperty::getPropertyID()
{
return propertyId;
}

const QString& XCDLProperty::getXCDLID()
{
return xcdlId;
}
const QString& XCDLProperty::getValue()
{
return value;
}
const QString& XCDLProperty::getType()
{
return type;
}
const QString& XCDLProperty::getDataRefIndicator()
{
return dataRefIndicator;
}
const QString& XCDLProperty::getNamesLibID()
{
return namesLibID;
}
void XCDLProperty::setName(const QString& n)
{
name=n;
}
void XCDLProperty::setPropertyID(const QString& pId)
{
propertyId=pId;
}
void XCDLProperty::setXCDLID(const QString& xId)
{
xcdlId=xId;
}
void XCDLProperty::setValue(const QString& v)
{
value=v;
}
void XCDLProperty::setType(const QString& t)
{
type=t;
}
void XCDLProperty::setDataRefIndicator(const QString& dI)
{
dataRefIndicator=dI;
}

void XCDLProperty::setNamesLibID(const QString& dI)
{
namesLibID=dI;
}
const QString& XCDLProperty::getObjectType( )
{
    return objectType;
}
void XCDLProperty::setObjectType ( const QString& objType )
{
    objectType = objType;
}

BOOL XCDLProperty::getNewObject()
{
    return newObject;
}
 void XCDLProperty::setNewObject(BOOL nObj)
{

    newObject=nObj;
}

