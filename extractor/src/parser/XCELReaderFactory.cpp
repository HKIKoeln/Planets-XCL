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
#include "XCELReaderFactory.h"
#include "XCLXMLValidator.h"
#include "XCLDOMBuilder.h"
#include "XCELGrammarBuilder.h"
#include "XCELReader.h"
#include "XCELPostProcessor.h"
#include "XCELPreProcessor.h"


void XCELReaderFactory::registerMethod(XCELProcessingMethod* method)
{
  preprocessor->registerMethod(method);
  processor->registerMethod(method);
  postprocessor->registerMethod(method);
}


void XCELReaderFactory::registerDataType(XCLDataType* dataType)
{
  //normalizer.registerDataType(dataType);
  
  // Useless statement for less compiler-warning
  (void)dataType;
}


XCELReader* XCELReaderFactory::createReader(enum READERTYPE type)
{
  XCELReader* reader;
/*  XCELProcessorInterface* proc;
  switch(type)
  {
  case PRO:
    proc = &processor;
    break;
  case POST:
    proc = &postprocessor;
    break;
  case PRE:
    proc=&preprocessor;
    break;
  default:;
    //throw XCELException("XCELReaderFactory :: Type: "+type+"unkown\n");
  };

 / reader = new XCELReader();

  parseTreeFactory->setProcessor(proc);
  proc->setFactory(parseTreeFactory);

  //reader->setNormalizer(&normalizer);
  reader->setProcessor(proc);
  reader->setDataObject(dataObject);
*/
  return reader;
  // useless statement for less compiler-warning
  (void)type; 
}


