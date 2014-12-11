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
#ifndef __XCLREADERFACTORY_H
#define __XCLREADERFACTORY_H

#include "XCELPreProcessor.h"
#include "XCELPostProcessor.h"
#include "XCELMainProcessor.h"
#include "XCLInputNormalizer.h"

class XCELProcessingMethod;
class XCLDataType;
class XCLParseTreeFactory;
class XCELFileParser;
class XCELReaderFactory
{
public:
XCELReaderFactory()
{
processor=new XCELMainProcessor();
postprocessor=new XCELPostProcessor();
preprocessor=new XCELPreProcessor();
}
public:
enum READERTYPE
{
PRE = 1,
PRO = 2,
POST = 3
};
public:
   void registerMethod(XCELProcessingMethod* method);
   void registerDataType(XCLDataType* dataType);
   XCELReader* createReader(enum READERTYPE type);
   
public:
   XCELPreProcessor* preprocessor;
   XCELMainProcessor* processor;
   XCELPostProcessor* postprocessor;
   XCLParseTreeFactory* parseTreeFactory;
   XCLInputNormalizer normalizer;	
};

#endif
