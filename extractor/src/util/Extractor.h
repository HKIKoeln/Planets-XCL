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
#ifndef _EXTRACTOR_H
#define _EXTRACTOR_H
#include <iostream>
#include <QFile>
#include "XCLException.h"
#include "XCLXMLValidator.h"
#include "XCELTreeBuilder.h"
#include "XCDLWriterFactory.h"
#include "NamesLibProperty.h"
#include "XCLInputNormalizer.h"

class Extractor
{

public:
 void extract(	const QString& binFile, 
			const QString& grammFile, 
			const QString& outputFile)
{

  XCDLWriterFactory factory("config.xml");
  QVector<QString> ignoredProperties;
  QVector<NamesLibProperty*> definedProperties;
  ignoredProperties=factory.getIgnoredProperties();
  definedProperties=factory.getDefinedProperties();


  factory.setDefinedProperties(definedProperties);
  factory.setIgnoredProperties(ignoredProperties);


      XCELTreeBuilder builder;
      XCLParsingItem parseTree;

      try
      {
        std::cout<<"Generate internal Representation...\n";
        parseTree=builder.build(grammFile,binFile);
        std::cout<<"Generate XCDL...";
	
        XCDLWriter* writer = factory.create();
        writer->write(parseTree,outputFile);
        std::cout<<"ready\n";
       
      }
      catch(XCLException excp)
      {
        excp.message();
        std::cout<<"Return 1\n";
      }
     
   
  
} 

};
#endif
