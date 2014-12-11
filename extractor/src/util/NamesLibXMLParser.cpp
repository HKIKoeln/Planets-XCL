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
#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QVector>

#include "NamesLibXMLParser.h"
#include <iostream>


NamesLibXMLParser::NamesLibXMLParser(){ }

NamesLibXMLParser::~NamesLibXMLParser(){ }

void NamesLibXMLParser::parse(QString NamesLibPath){
	
	//initialization of this->propertiesVector
	setProperties( NamesLibPath, this->properties);

}
QVector<NamesLibProperty *> NamesLibXMLParser::getProperties(){

	return this->properties;
}

void NamesLibXMLParser::setProperties(QString namesLibPath, QVector<NamesLibProperty *> &properties)
{

	QDomDocument doc;
 	QFile file(namesLibPath);
 	if (!file.open(QIODevice::ReadOnly))
	{

		std::cout<<"NamesLibXMLParser error: File \""<<namesLibPath.toStdString()<<"\"cannot be opened.\n";
    		 return;
	}
 	if (!doc.setContent(&file)) {
     		file.close();
		std::cout<<"NamesLibXMLParser error: The content of the file \""<<namesLibPath.toStdString()<<"\" was not successfully parsed.";


     		return;
 	}
 	file.close();

	
	
 	QDomElement docElem = doc.documentElement();

	QDomNodeList qdl= docElem.elementsByTagName( "xs:enumeration" );
	for(_LENGTH l = 0; l < qdl.length(); l++)
	{
		QDomNode n2X =qdl.at(l);
		QDomElement e2X=n2X.toElement();
		if(!e2X.isNull()) {	
			if(n2X.hasAttributes ()){
				QString attValue =e2X.attribute ("value");
				QString attId = e2X.attribute ("id");
				QString documentation;
				QDomNodeList qdl2= e2X.elementsByTagName( "xs:documentation" );
				for(_LENGTH l = 0; l < qdl2.length(); l++)
				{
					QDomNode n =qdl2.at(l);
					QDomElement d_e=n.toElement();
					documentation=d_e.text();
					documentation = documentation.simplified();
				}

				if(attValue != NULL && attId != NULL ){
					NamesLibProperty *nlp= new NamesLibProperty(attValue, attId, documentation);
                                       BOOL hit =FALSE;
                                         for(int i=0;i<properties.size();i++)
                                        {
                                          
                                            if( properties.at(i)->getPropertyId() == nlp->getPropertyId())
                                                    hit=TRUE;
    
                                        }
                                        if(!hit)
					properties.append(nlp);
				}
			}
		}
	}
	
}




//search for Value(=propertyname)in the object.
//returns the position of search-value in the object, if search-value was found.
//returns -1,  if search-value was not found.
int NamesLibXMLParser::hasValue(QString searchValue){
	
	int qvectorPosition = -1;
	for (int ih = 0; ih < this->properties.size(); ih++) {
     		
		NamesLibProperty *nlp = this->properties.at(ih);
		QString v = nlp->getPropertyValue();
		QString id = nlp->getPropertyId();
		if(v == searchValue){
			qvectorPosition=ih;
			return qvectorPosition;
		}
	}
	return qvectorPosition;
}

//search for id in the object.
//returns the position of search-id in the object, if search-id was found.
//returns -1,  if search-id was not found.
int NamesLibXMLParser::hasID(QString searchId){
	
	int qvectorPosition = -1;
	for (int ih = 0; ih < this->properties.size(); ih++) {
     		
		NamesLibProperty *nlp = this->properties.at(ih);
		QString v = nlp->getPropertyValue();
		QString id = nlp->getPropertyId();
		if(id == searchId){
			qvectorPosition=ih;
			return qvectorPosition;
		}
	}
	return qvectorPosition;
}

//remove a element from object
//returns 0, if the remove was successful.
//returns -1, if the remove was not successful.
int NamesLibXMLParser::remove(int qvectorPosition)
{
	if(qvectorPosition < this->properties.size() && qvectorPosition >= 0 )
	{
		this->properties.remove ( qvectorPosition, 1 );
		return 0;
	}
	else 
		return -1;

}







