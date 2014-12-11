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
#ifndef XCDLPROPERTY_H
#define XCDLPROPERTY_H
#include <iostream>
#include <QString>
#include "Defines.h"


class XCDLProperty
{
public: 
  XCDLProperty()
  {
  name = "";
  propertyId = "";
  xcdlId = "";
  value = "";
  type = "";
  dataRefIndicator = "";
  objectType="";
  newObject=FALSE;
  }
  const QString& getName();
  const QString& getPropertyID();
  const QString& getXCDLID();
  const QString& getValue();
  const QString& getType();
  const QString& getDataRefIndicator();
  const QString& getNamesLibID();

  void setName(const QString& );
  void setPropertyID(const QString& );
  void setXCDLID(const QString& );
  void setValue(const QString& );
  void setType(const QString& );
  void setDataRefIndicator(const QString& );
  void setNamesLibID(const QString& );
  void setObjectType ( const QString& objType );
  const QString& getObjectType ( );
  void setNewObject ( BOOL nObj );
  BOOL getNewObject();

  void print()
  {
        std::cout<<"\nPrint Property START------------------\n";
	std::cout<<"name: "<<name.toStdString()<<"\n";
	std::cout<<"propertyId: "<<propertyId.toStdString()<<"\n";
	std::cout<<"xcdlId: "<<xcdlId.toStdString()<<"\n";
	std::cout<<"value: "<<value.toStdString()<<"\n";
	std::cout<<"type: "<<type.toStdString()<<"\n";
	std::cout<<"dataRefIndicator: "<<dataRefIndicator.toStdString()<<"\n";
        std::cout<<"namesLibID: "<<namesLibID.toStdString()<<"\n";
	std::cout<<"Print Property END------------------\n\n";
  }
private:
  QString name;
  QString propertyId;
  QString xcdlId;
  QString namesLibID;
  QString value;
  QString type;
  QString dataRefIndicator;
  QString objectType;
  BOOL newObject;

};

#endif
