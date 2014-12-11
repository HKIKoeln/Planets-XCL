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
#include "XCDLDomBuilder.h"	
#include <QTextStream>
#include <QFile>

#include "XCDLObject.h"
QDomDocument  XCDLDomBuilder::create( 	QString xcdlFilename,
				QString normData, 
				QVector<XCDLProperty> p,
				QVector<XCDLPropertySet> pS,
    				QVector<XCDLObject*> 	  fObjects,
				QString oT)
{
 
  objectType=oT;
  normDataString = normData;
  properties = p;
  propertySets = pS;	
  fileObjects=fObjects;
  serialize();	

  //Aufruf remove(QChar('\\')), um die Maskierungsbackslashes aus inputfilename zu entfernen, falls diese beim Aufrufen von Extractor hinzugefühgt worden sind.
  QFile file(xcdlFilename.remove(QChar('\\')));
  if (file.open(QFile::WriteOnly))
  {
    QTextStream xcdlStream(&file);
    xcdlStream<<"<?xml version='1.0' encoding='UTF-8'?>\n";
    xcdldoc.save(xcdlStream,4); 
  }
  file.close();
  return xcdldoc;
}

void XCDLDomBuilder::serialize()
{
  

  QDomElement rootElement = xcdldoc.createElement("xcdl");
  rootElement.setAttribute("id",0);
  rootElement.setAttribute("xmlns","http://www.planets-project.eu/xcl/schemas/xcl");
  rootElement.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
  rootElement.setAttribute("xsi:schemaLocation","http://www.planets-project.eu/xcl/schemas/xcl ../xcl/xcdl/XCDLCore.xsd");

  xcdldoc.appendChild(rootElement);
  //Schreibe MainObject aus
  
  //std::cout<<"Add Object to basic structure\n";
 QString objectID;
      _UINT32 objectNum=0;
      objectID.setNum(objectNum+1);
      objectID.prepend("o");
      QString normDataID;
      _UINT32 normDataNum=1;
      normDataID.setNum(normDataNum++);
      normDataID.prepend("nd");

      QDomElement objectElement = xcdldoc.createElement("object");
      objectElement.setAttribute("id",objectID);
      rootElement.appendChild(objectElement);
      QDomElement normDataElement = xcdldoc.createElement("normData");
      normDataElement.setAttribute("type",objectType);
      normDataElement.setAttribute("id",normDataID);
      objectElement.appendChild(normDataElement);

      QDomText normDataText = xcdldoc.createTextNode(normDataString);
      normDataElement.appendChild(normDataText);
      propertiesByName.clear();
      int objectIDNumber=2;//Die embedded objects werden gezaehlt. Die Zaelung faengt mit 2, da ObjectID="1" das MainObject ist.
      for(int i =0;i<properties.size();i++)
      {

          XCDLProperty property= properties.at(i);
//           property.print();
        
        
          QDomElement propertyElement;
          QString name=property.getName();
	//std::cout<<name.toStdString()<<"\n";
          if(!propertiesByName.contains(name))
          { 
              propertyElement = xcdldoc.createElement("property");
              propertyElement.setAttribute("id",property.getXCDLID());
              propertyElement.setAttribute("source","raw");
              propertyElement.setAttribute("cat","descr");
              objectElement.appendChild( propertyElement);
              QDomElement nameElement = xcdldoc.createElement("name");
		
              QDomText nameText=xcdldoc.createTextNode( name);
              nameElement.appendChild( nameText);
              propertyElement.appendChild( nameElement);
              nameElement.setAttribute("id",property.getNamesLibID());

              propertiesByName.insert(name,propertyElement);
          }
          else
              propertyElement = propertiesByName.value(name);//	Falls ein Property gleichen Namens als  QDomElement gibt, wird diese zurückgeliefert und nur valueSet vom nextProperty wird angehängt

          QDomElement valueSetElement = xcdldoc.createElement("valueSet");
          valueSetElement.setAttribute("id",property.getPropertyID());
          propertyElement.appendChild( valueSetElement );
          
          if(property.getNewObject()) // Property ist ein Objekt
          {
	      QString qsPID=property.getPropertyID();//Diese ID ist automatisch generiert.
              //objectRef tag ausschreiben
              QDomElement objectRefElement = xcdldoc.createElement("objectRef");
              QString objIDnr;
	      objIDnr.setNum(objectIDNumber);
              QString objID;
              objID.setNum(objectIDNumber);
	      objID.prepend(".:o");
              objectRefElement.appendChild(xcdldoc.createTextNode(objID));
              valueSetElement.appendChild(objectRefElement);	
              objectIDNumber=objectIDNumber+1;
              
              //object ausschreiben
              //1.)hohle Objekt aus der fileObjects liste mit entsprechenden ID
              //2.) Scheibe dieses Objeckt aus, bzw. hänge es  an xcdlROOT Element ein.
              int fileObjectsSize1=fileObjects.size();
              for(int i2=0;i2<fileObjectsSize1;i2++)

              {
      		
                  QString oooID=fileObjects[i2]->getObjectID();
		  if(oooID==qsPID){
			fileObjects[i2]->setObjectID(objIDnr);
			break;
		  }
              }
              
              
              
          }else{
	//labValue fuer 
          QDomElement labValueElement = xcdldoc.createElement("labValue");
          QDomElement valueElement = xcdldoc.createElement("val");
          QDomElement typeElement	= xcdldoc.createElement("type");
	
          valueElement.appendChild(xcdldoc.createTextNode(property.getValue()));
          typeElement.appendChild(xcdldoc.createTextNode(property.getType()));

          labValueElement.appendChild(valueElement);
          labValueElement.appendChild(typeElement);
          valueSetElement.appendChild(labValueElement);	
          }
        

          
          valueSetsById.insert(property.getPropertyID(),valueSetElement);
      }
 

      QVector<XCDLPropertySet> sets;
      for(int i=0;i<propertySets.size();i++)
      {
          XCDLPropertySet set = propertySets.at(i);
          QDomElement propertySet = xcdldoc.createElement("propertySet");
          QString propertySetId;
          propertySetId.setNum(i);
          propertySetId.prepend("id_");
          propertySet.setAttribute("id",propertySetId);
          set.setId(propertySetId);
          sets.append(set);
          QDomElement valueSetRelations = xcdldoc.createElement("valueSetRelations");
          QVector<QString> ids = set.getValueSetRelationIDs();
          QVector<QString> names = set.getValueSetRelationNames();

          for(int j=0;j<ids.size();j++)
          {
              QString id = ids.at(j);
              QString name= names.at(j);
              QDomElement ref = xcdldoc.createElement("ref");
              ref.setAttribute("valueSetId",id);
              ref.setAttribute("name",name);
              valueSetRelations.appendChild(ref);	
          }


          propertySet.appendChild(valueSetRelations);
          if(set.hasNormDataReference())
          {
              QVector<QString> startPositions = set.getStart();
              QVector<QString> endPositions = set.getEnd();
              QDomElement dataRef = xcdldoc.createElement("dataRef");
              for(int j=0;j<startPositions.size();j++)
              {
                  QString start = startPositions.at(j);
                  QString end = endPositions.at(j);
			
                  QDomElement dataRefRef=xcdldoc.createElement("ref");
                  dataRef.appendChild(dataRefRef);
                  dataRefRef.setAttribute("begin",start);
                  dataRefRef.setAttribute("end", end);
                  dataRefRef.setAttribute("id",normDataID);
			
              }
              propertySet.appendChild(dataRef);
          }
          objectElement.appendChild(propertySet);
      }
  
      for(int i=0;i<sets.size();i++)
      {
          XCDLPropertySet set = sets.at(i);
          QVector<QString> ids = set.getValueSetRelationIDs();
          BOOL hasRef=FALSE;
          if(set.hasNormDataReference())
              hasRef=TRUE;
          for(int j=0;j<ids.size();j++)
          {
              QString id = ids.at(j);
              QDomElement valueSetElement = valueSetsById.value(id);
              QDomElement dataRef	= xcdldoc.createElement("dataRef");
              if(hasRef)
                  dataRef.setAttribute("ind","normSpecific");
              else
                  dataRef.setAttribute("ind","global");
              dataRef.setAttribute("propertySetId",set.getId());
              valueSetElement.appendChild(dataRef);
          }
      }
 


//Schreibe die subOjekte aus 
//schleife über alle Objekte!
  int fileObjectsSize=fileObjects.size();
  for(int i=0;i<fileObjectsSize;i++)
  {
	objectType.clear();
	normDataString.clear();
	properties.clear();
	propertySets.clear();
	
	objectType=fileObjects[i]->getObjectType();
	normDataString = fileObjects[i]->getNormData();
	properties = fileObjects[i]->getProperties();
	propertySets = fileObjects[i]->getPropertySets();
	QString objID_x = fileObjects[i]->getObjectID();
	objID_x.prepend("o");;
	
	//std::cout<<"Add Object to basic structure\n";
	QString normDataID;
	normDataID.setNum(normDataNum++);
	normDataID.prepend("nd");
	
	QDomElement objectElement = xcdldoc.createElement("object");
	objectElement.setAttribute("id",objID_x);
	rootElement.appendChild(objectElement);
	QDomElement normDataElement = xcdldoc.createElement("normData");
	//QString objectType=parseTree.getObjectType();
	normDataElement.setAttribute("type",objectType);
	normDataElement.setAttribute("id",normDataID);
	objectElement.appendChild(normDataElement);
	
	QDomText normDataText = xcdldoc.createTextNode(normDataString);
	normDataElement.appendChild(normDataText);
	propertiesByName.clear();
	
	for(int i =0;i<properties.size();i++)
	{
	
		XCDLProperty property= properties.at(i);
// 		property.print();
		
		
		QDomElement propertyElement;
		QString name=property.getName();
		if(!propertiesByName.contains(name))
		{ 
			propertyElement = xcdldoc.createElement("property");
			propertyElement.setAttribute("id",property.getXCDLID());
			propertyElement.setAttribute("source","raw");
			propertyElement.setAttribute("cat","descr");
			objectElement.appendChild( propertyElement);
			QDomElement nameElement = xcdldoc.createElement("name");
			
			QDomText nameText=xcdldoc.createTextNode( name);
			nameElement.appendChild( nameText);
			propertyElement.appendChild( nameElement);
			nameElement.setAttribute("id",property.getNamesLibID());
	
			propertiesByName.insert(name,propertyElement);
		}
		else
		propertyElement = propertiesByName.value(name);//	Falls ein Property gleichen Namens als  QDomElement gibt, wird diese zurückgeliefert und nur valueSet vom nextProperty wird angehängt
	
		QDomElement valueSetElement = xcdldoc.createElement("valueSet");
		valueSetElement.setAttribute("id",property.getPropertyID());
		propertyElement.appendChild( valueSetElement );
		
		QDomElement labValueElement = xcdldoc.createElement("labValue");
		QDomElement valueElement = xcdldoc.createElement("val");
		QDomElement typeElement	= xcdldoc.createElement("type");
		
		valueElement.appendChild(xcdldoc.createTextNode(property.getValue()));
		typeElement.appendChild(xcdldoc.createTextNode(property.getType()));
	
		labValueElement.appendChild(valueElement);
		labValueElement.appendChild(typeElement);
		valueSetElement.appendChild(labValueElement);	
		valueSetsById.insert(property.getPropertyID(),valueSetElement);
	}
	

	QVector<XCDLPropertySet> sets;
	for(int i=0;i<propertySets.size();i++)
	{
		XCDLPropertySet set = propertySets.at(i);
		QDomElement propertySet = xcdldoc.createElement("propertySet");
		QString propertySetId;
		propertySetId.setNum(i);
		propertySetId.prepend("id_");
		propertySet.setAttribute("id",propertySetId);
		set.setId(propertySetId);
		sets.append(set);
		QDomElement valueSetRelations = xcdldoc.createElement("valueSetRelations");
		QVector<QString> ids = set.getValueSetRelationIDs();
		QVector<QString> names = set.getValueSetRelationNames();
	
		for(int j=0;j<ids.size();j++)
		{
			QString id = ids.at(j);
			QString name= names.at(j);
			QDomElement ref = xcdldoc.createElement("ref");
			ref.setAttribute("valueSetId",id);
			ref.setAttribute("name",name);
			valueSetRelations.appendChild(ref);	
		}
	
	
		propertySet.appendChild(valueSetRelations);
		if(set.hasNormDataReference())
		{
			QVector<QString> startPositions = set.getStart();
			QVector<QString> endPositions = set.getEnd();
	QDomElement dataRef = xcdldoc.createElement("dataRef");
			for(int j=0;j<startPositions.size();j++)
			{
				QString start = startPositions.at(j);
				QString end = endPositions.at(j);
				
				QDomElement dataRefRef=xcdldoc.createElement("ref");
				dataRef.appendChild(dataRefRef);
				dataRefRef.setAttribute("begin",start);
				dataRefRef.setAttribute("end", end);
				dataRefRef.setAttribute("id",normDataID);
				
			}
	propertySet.appendChild(dataRef);
		}
		objectElement.appendChild(propertySet);
	}
	
                for(int i=0;i<sets.size();i++)
                {
                        XCDLPropertySet set = sets.at(i);
                        QVector<QString> ids = set.getValueSetRelationIDs();
                        BOOL hasRef=FALSE;
                        if(set.hasNormDataReference())
                                hasRef=TRUE;
                        for(int j=0;j<ids.size();j++)
                        {
                                QString id = ids.at(j);
                                QDomElement valueSetElement = valueSetsById.value(id);
                                QDomElement dataRef	= xcdldoc.createElement("dataRef");
                                if(hasRef)
                                        dataRef.setAttribute("ind","normSpecific");
                                else
                                        dataRef.setAttribute("ind","global");
                                dataRef.setAttribute("propertySetId",set.getId());
                                valueSetElement.appendChild(dataRef);
                        }
                
                }
}

}


