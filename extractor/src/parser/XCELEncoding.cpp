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
#include "XCELEncoding.h"
#include <iostream>
XCELEncoding::XCELEncoding()
{
  encoder = NULL;
}

void XCELEncoding::setEncoding(QString enc)
{
#ifdef PlanetsDebug
	std::cout<<"Encoding "<<enc.toStdString()<<"\n";
#endif
	if(enc == "utf-8")
		encoder = QTextCodec::codecForName("UTF-8");
}

int XCELEncoding::get(QString& result,QByteArray* ba, int start, int length)
{
QString str;
  if(encoder==NULL) return -1;
//std::cout<<"Encoding bA at start "<<ba->at(start)<<"\n";

  str = encoder->toUnicode(ba->mid(start,length*4));

str=str.mid(0,length);
length=str.toUtf8().size();
if(str=="\0")str="NULL";
else if(str =="<")str="&lt;";
else if(str ==">")str="&gt;";
result=str;
//std::cout<<"Start "<<start<<" Length "<<length<<" Encoding "<<result.toStdString()<<"\n";
  return length;
}
