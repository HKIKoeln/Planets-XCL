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
#include <iostream>
#include "XCLDataConverter.h"
#include "XCLDecompressorLib.h"
#include "XCLPngLib.h"
#include "XCLPaletteLib.h"
#include "XCLRegExpLib.h"

XCLDataConverter::XCLDataConverter()
{
}

XCLDataConverter::~XCLDataConverter()
{}

QByteArray XCLDataConverter::filter(
  QByteArray* filterMe,
  QVector<XCLFilter> fC)
{

QVector<XCLFilter> filterChain =fC;
  static QStringList classTrigger;

  if (classTrigger.empty()==true)
  {
    classTrigger << "compression" << "pngFilterMethod" << "chunkyPalette" << "planarPalette" <<"regExp";
  }
  
  static QByteArray *(*filterSelector[])
  (QByteArray* filterMe, XCLFilter &filter) =
    {
      XCLCompressionSelector,
      XCLPngPrivateSelector,
      XCLPaletteChunkySelector,
      XCLPalettePlanarSelector,
      XCLRegExpSelector	
    };
    
  QByteArray *filterStep;
  int first=1;
  for (int i=0,j;i<filterChain.size();i++)
  {

    for (j=0;j<classTrigger.size(); j++)
    {
      if (filterChain[i].getFilterName()==classTrigger[j])
      {
        filterStep=(*filterSelector[j]) (filterMe ,filterChain[i]);
        if (first) first=0;
        else delete filterMe;
        if (filterStep==NULL)
		throw 
		XCLException(QObject::tr("Filter '%1' in filter class '%2' failed. Filter number %3,Filterselector number %4").arg(filterChain[i].getFilterName()).arg(filterChain[i].getFilterValue()).arg(i).arg(j));



        filterMe=filterStep;
        break;
      }
    }
    if (j>=classTrigger.size())
      throw XCLException(QObject::tr("Unknown filter class: '%1'").arg(filterChain[i].getFilterName()));
  }

  return *filterStep;
}
