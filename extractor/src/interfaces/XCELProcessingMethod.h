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
#ifndef __XCELPROCESSINGMETHOD_H
#define __XCELPROCESSINGMETHOD_H
#include "Defines.h"
#include "XCLSyntaxProcess.h"
#include "XCLSyntaxExpression.h"
#include <QString>
#include "XCELReader.h"
#include <QHash>
#include "XCELProcessorInterface.h"


class XCELProcessingMethod
{
public:
  //    XCELProcessingMethod();
  virtual ~XCELProcessingMethod(){};
  // returns an Array of length 4 every field indicates if the method
  // is implemented. e.g. return {FALSE,TRUE,TRUE,FALSE} means that both
  // processing methods are implemented but no pre- or postprocessing
  // is supported.
  virtual BOOL* getMode() = 0;

  // if two methods with the same name exists, the method with the right number
  // of parameters is choosen. The string given by the trigger method together
  // with the number of parameters must be unary otherwise an Exception is thrown
  // through configuring time
  virtual int expectedParameters() = 0;

  // a processor will choose the proper method by calling the trigger
  // the trigger must be identical with XCEL name of the method
  const QString& getTrigger(){return trigger;}
public:
  //Preprocessing: can modify the grammar
  virtual BOOL preprocess(	XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing) = 0;
  //Processing: can modify the grammar or the binary
  virtual BOOL process(	XCELReader* reader,
			XCLSyntaxExpression* expr,
                        QVector<XCLProcessParameter> param,
                        QHash<QString,IndexedVector*> vectorHash) = 0;

  //Postprocessing: can modify the parseTree
  virtual BOOL postprocess( XCLParsingItem& parseTree,
                            QVector<XCLProcessParameter> param,
                            QVector<XCLParsingExpression*> referenceData,
                            FileParserState& state,
                            QHash<QString,IndexedVector*> vectorHash) = 0;
protected:
  QString trigger;
};
 

#endif

