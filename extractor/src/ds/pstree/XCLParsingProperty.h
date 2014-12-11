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


#ifndef XCLPARSINGPROPERTY_H
#define XCLPARSINGPROPERTY_H
#include "XCLParsingSymbol.h"
#include "XCLParsingExpression.h"

#include <QString>

/*! \class XCLParsingProperty XCLParsingSymbol.h "XCLParsingSymbol.h" XCLParsingExpression.h "XCLParsingExpression.h"
 *  \brief 
 
  @author Jan Schnasse <jan.schnasse@uni-koeln.de>
 */
class XCLParsingProperty : public XCLParsingSymbol
{
public: // Constructors/Destructors
  XCLParsingProperty ( );
  XCLParsingProperty(const XCLParsingProperty& prop);
  XCLParsingProperty& operator=(const XCLParsingProperty& prop);
  virtual ~XCLParsingProperty ( );
private://members
  QString knownValue;//!< The properties given value as defined in the format specification
public://inherited
  enum EXPR_TYPE getType();
  void print(QTextStream& debugOut, QString format="");
  void print(QString format="");
public:
  BOOL isValid();
  void setKnownValue ( const QString& new_var );
private:
  const QString& getKnownValue ( );
};

#endif // XCLPARSINGPROPERTY_H
