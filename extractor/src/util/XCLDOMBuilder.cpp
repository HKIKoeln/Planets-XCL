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
#include "XCLDOMBuilder.h"
#include <QXmlInputSource>
#include <iostream>
#include "XCLException.h"
XCLDOMBuilder::XCLDOMBuilder()
{
}


XCLDOMBuilder::~XCLDOMBuilder()
{
}

const QDomDocument& XCLDOMBuilder::getDOM(QString filename)
{
 QDomDocument* doc = new QDomDocument();
try
{
 QFile file(filename);
 if(!file.exists())throw XCLException(filename+" not Found");
 if (!file.open(QIODevice::ReadOnly))throw XCLException(filename+" not Found");
     ;


        QString errorStr;
	int errorLine;
	int errorColumn; 
std::cout<<filename.toStdString()<<"\n";
	if(!doc->setContent(&file,
			   &errorStr,
			   &errorLine,
			   &errorColumn))
	{
	    QString error;
	    error.append("XMLIO: Parsing Error:");
	    error.append("Zeile: ");
	    error.append(errorLine);
	    error.append(" Spalte: ");
	    error.append(errorColumn);
	    error.append(errorStr);
            file.close();  
            throw XCLException(error);
	}
file.close();
return *doc;
}
catch(XCLException exception)
{
exception.message();
throw XCLException(exception.getMessage());
}
catch(...)
{
delete doc;
throw XCLException(filename+" is not wellformed\n");
}

}
