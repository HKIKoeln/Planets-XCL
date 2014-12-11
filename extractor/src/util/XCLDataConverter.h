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
#ifndef __XCLDATACONVERTER_H
#define __XCLDATACONVERTER_H
#include <QString>
#include <QHash>

#include "Defines.h"
#include "XCLFilter.h"
#include "XCLParsingItem.h"
#include "XCLException.h"
#include <QByteArray>
#include <QVector>
#include <QStringList>



class XCLDataConverter
{
public:
	
    XCLDataConverter();
    ~XCLDataConverter();

    QByteArray filter(QByteArray* filterMe, QVector<XCLFilter> filterChain);
private:

};
 


#endif
