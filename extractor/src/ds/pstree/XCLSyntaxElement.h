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

#ifndef XCLSYNTAXELEMENT_H
#define XCLSYNTAXELEMENT_H

#include <QString>
#include <QVector>
#include "Defines.h"

class XCLSyntaxItem;
class XCLSyntaxElement
{
    public://enums
	enum ELEMENT_TYPE
	{
	    ITEM_TYPE = 1,
	    SYMBOL_TYPE = 2,
	    PROPERTY_TYPE = 3,
	    PROCESS_TYPE = 4
	};
    public:// Constructors/Destructors
	XCLSyntaxElement ();
	virtual ~XCLSyntaxElement ( );
	XCLSyntaxElement(const XCLSyntaxElement& elem);
	virtual XCLSyntaxElement& operator=(const XCLSyntaxElement& elem);
    protected://members
	XCLSyntaxItem* structuralParent; //!< 
  	BOOL printme;
	QString source;
	QString extSource;
    public:
	virtual enum ELEMENT_TYPE getElementType() const = 0;
	virtual void print(QString format="");
	void setParent ( XCLSyntaxItem* new_var );
        void setPrintMe(const QString& p);
	BOOL getPrintMe();
	XCLSyntaxItem* getParent ( );
  void setSource(const QString& s)
	{
		source=s;
	}
  const QString& getSource(){return source;}
  void setExtSource(const QString& s)
	{
		extSource=s;
	}
  const QString& getExtSource(){return extSource;}
};

#endif // XCLSYNTAXELEMENT_H
