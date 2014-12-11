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
#include "XCLRegExpLib.h"

QByteArray *XCLRegExpSelector(QByteArray *filterMe, XCLFilter &filter)
{
filter.print();
QString str(*filterMe);
//std::cout<<str.size()<<" "<<str.toStdString()<<"\n";
//std::cout<<filter.getFilterValue().toStdString()<<"\n";
QString result;
QRegExp rx(filter.getFilterValue());
int count=0;
int pos= 0;
while((pos=rx.indexIn(str,pos))!=-1 && pos < str.size())
{
	if(rx.matchedLength()==0)break;
        std::cout<<rx.cap(1).toStdString()<<"\n";
	result += rx.cap(1);
//std::cout<<"Count "<<count<<" rx.matchedLength() "<<rx.matchedLength()<<" pos "<<pos<<"\n";
	++count;
	pos+=rx.matchedLength();
	
}
//delete filterMe;
filterMe = new QByteArray(result.toAscii());
return filterMe;
}