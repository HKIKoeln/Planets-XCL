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
#ifndef _XCLEXCEPTION_H2
#define _XCLEXCEPTION_H2
#include <QString>
#include <QTextStream>
#include <iostream>
class XCLException
{
public:
	XCLException(const QString& message){msg=message;}

	void message()
	{
		//QTextStream cout(stdout);
#ifdef PlanetsDebug
		std::cout <<msg.toStdString()<<std::endl;
#endif
	}

	QString getMessage()
	{
	    return msg;
	}
	protected:
	    QString msg;

	};


class XCLOutOfRangeException : public XCLException
{
public:
	XCLOutOfRangeException():XCLException("OutOfRange"){}
	XCLOutOfRangeException(const QString& message):XCLException(message){}
};

class XCLDataTypeException : public XCLException
{
public:
	XCLDataTypeException():XCLException("Wrong Data Type"){}
	XCLDataTypeException(const QString& message):XCLException(message){}
};

class XCLPrintException : public XCLException
{
public:
	XCLPrintException():XCLException("I/O Exception"){}
	XCLPrintException(const QString& message):XCLException(message){}
};

#endif

