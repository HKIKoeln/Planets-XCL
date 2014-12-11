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
#ifndef __XCLFILTER_H
#define __XCLFILTER_H
#include <QString>
#include <QHash>

#include "Defines.h"



class XCLFilter
{
public:
	XCLFilter();
	XCLFilter(const XCLFilter& f);
        XCLFilter& operator=(const XCLFilter& itm);
	~XCLFilter();

        void setFilterName(const QString& name);        
        void setFilterValue(const QString& value);
	void addParam(const QString& paramname, const QString& paramvalue);
        const QString& getFilterName();
        const QString& getFilterValue();
        const QHash<QString,QString> getParams();
        void print(QString format="");
        int getIntPar(const QString& name,bool enforce);
        QString getStringPar(const QString &name,bool enforce);
        BOOL isEqual(const XCLFilter& f);
private:
       QString filtername;
       QString filtervalue;
       QHash<QString, QString> parameters; 

private:
		

  
};

#endif
