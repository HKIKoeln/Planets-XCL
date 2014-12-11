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
#ifndef XCDLWRITERFACTORY_H
#define XCDLWRITERFACTORY_H

#include <QDomDocument>
#include "XCDLWriter.h"
#include "XCLParsingSymbol.h"

/*! \class XCDLWriterFactory XCDLWriterFactory.h "XCDLWriterFactory.h"
 *  \brief 


  @author Johanna Neumann <johanna.neumann@uni-koeln.de>
  @author Sebastian Beyl <sebastian.beyl@uni-koeln.de>
 */
class XCDLProperties
{
QString namesLib;
QVector<QString> properties;
};
class XCDLWriterFactory
{
public:

  XCDLWriterFactory ( QString configFile); //!< Empty Constructor
  virtual ~XCDLWriterFactory ( ); //!< Empty Destructor

  XCDLWriter* create();

  void setRawDataDisplay(BOOL on)
  {
	displayRawData = on;
  }
  const QVector<QString> getIgnoredProperties()
  {
    return ignoredProperties;
  }
  const QVector<NamesLibProperty *> getDefinedProperties()
  {
    return propertiesVector;
  }
  void setIgnoredProperties( const QVector<QString>& iP)
  {
    ignoredProperties=iP;
  }
  void  setDefinedProperties(const QVector<NamesLibProperty *>& dP)
  {
    propertiesVector=dP;
  }
  
  void  setDefinedProperties(const QVector<QString>& iP)
  {
      propertiesVector.clear();
      QString s;
      for (int i = 0; i < iP.size(); ++i) {
          s=iP.at(i);
          NamesLibProperty *nlp= new NamesLibProperty(s, "x", "x");
          BOOL hit =FALSE;
          for(int i=0;i<propertiesVector.size();i++)
          {

              if( propertiesVector.at(i)->getPropertyValue() == nlp->getPropertyValue())
                  hit=TRUE;
    
          }
          if(!hit)
              propertiesVector.append(nlp);
    
      }
     
  }
  
  void setDisplayNormData(BOOL);

private:
  const QVector<QString> getPropertyNamesFromEnumeration(QDomDocument doc);
  void removeIgnoredProperties(QVector<NamesLibProperty*>& definedProperties);
  QVector<NamesLibProperty *> propertiesVector;
  XCDLWriter* writer;
  QString configFile; //!< The configFile.
  QDomDocument configDOM; //!< The DOM representation of the configuration file.
  QVector<QString> definedProperties;
  QVector<QString> ignoredProperties;
  BOOL displayRawData;
  BOOL bDisplayNormData;
};

#endif // XCDLWRITERFACTORY_H
