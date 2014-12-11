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

#include "XCLDOMBuilder.h"
#include "XCDLWriterFactory.h"
#include "NamesLibXMLParser.h"

XCDLWriterFactory::XCDLWriterFactory ( QString qsConfigFile) 
{
    NamesLibXMLParser namesLibParser;
    configFile = qsConfigFile;
   // QVector<QString> definedProperties;

    XCLDOMBuilder dombuilder;
    configDOM=dombuilder.getDOM(configFile);
    //definedProperties.append("normData");
    QDomDocument namesLib;
    QDomNodeList libraryPaths = configDOM.elementsByTagName("namesLib");

    for (_LENGTH i = 0; i< libraryPaths.length(); i++)
    {
     //   namesLib = dombuilder.getDOM(libraryPaths.item(i).toElement().text());
	try
	{
		namesLibParser.parse(libraryPaths.item(i).toElement().text()); 
	}
	catch(XCLException exception)
	{
		exception.message();
	}
    }
    propertiesVector = namesLibParser.getProperties();

    removeIgnoredProperties(propertiesVector);
    displayRawData=FALSE;
}

XCDLWriterFactory::~XCDLWriterFactory ( ) { }


XCDLWriter* XCDLWriterFactory::create()
{

    writer = new XCDLWriter();
    writer->setDefinedProperties(propertiesVector);
    writer->setDisplayRawData(displayRawData); 
    writer->setDisplayNormData(bDisplayNormData);
    return writer;
}


/*! \fn const QVector<QString> XCDLWriterFactory::getPropertyNamesFromEnumeration(QDomDocument doc)
 *  \brief Selects all properties from the XCEL-Library and returns their names as a list of QStrings.

 *  \param doc The Dom Tree, which represents the content of the XCEL-Names-Library.

    Since properties are defined as enumerationlists, this function selects all properties available in the XCEL-Names-Library and returns their names as a QString list.

 *  \return A vector list of all properties defined in the XCEL-Names-Library contained in the DOM-Document.
 */
const QVector<QString> XCDLWriterFactory::getPropertyNamesFromEnumeration(QDomDocument doc)
{
  QVector<QString> enumerationValues;
  QDomNodeList enumerationNodes = doc.elementsByTagName("xs:simpleType");
  for(int i=0;i<  enumerationNodes.size();i++)
{	QDomElement elem=enumerationNodes.at(i).toElement();

	if(elem.attribute("id") == "properties")
	{

		enumerationNodes = elem.elementsByTagName("xs:enumeration");
		break;
	}
}

  QDomElement enumerationElement;
  for(int i=0;i<enumerationNodes.size();i++)
  {
    enumerationElement = enumerationNodes.at(i).toElement();
    enumerationValues.append(enumerationElement.attributeNode("value").value());
  }

if(enumerationValues.isEmpty())
	throw XCLException("Enumeration is Empty\n");
  return enumerationValues;
}


/*! \fn void XCDLWriterFactory::removeIgnoredProperties()
 *  \brief Checks the config-file for properties which have to be ignored and removes them from the list.

 */
void XCDLWriterFactory::removeIgnoredProperties(QVector<NamesLibProperty*>& definedProperties)
{
    QDomNodeList ignoredElements = configDOM.elementsByTagName("ignore");
    for(_LENGTH i=0; i<ignoredElements.length(); i++)
    {
//WARNING: the size of definedPropertities is not fix see remove Operation
        for(_VECTORLENGTH j = 0; j < definedProperties.size(); j++)
        {
            if (definedProperties[j]->getPropertyValue()==ignoredElements.item(i).toElement().text())
            {
                 ignoredProperties.append(ignoredElements.item(i).toElement().text());
                 definedProperties.remove(j);
            }
        }
    }

}


void XCDLWriterFactory::setDisplayNormData(BOOL bDisplayNormDataNewState)
{
    bDisplayNormData = bDisplayNormDataNewState;
}
