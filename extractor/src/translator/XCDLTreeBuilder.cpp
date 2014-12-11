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
#include "XCDLTreeBuilder.h"
#include "XCLException.h"
#include "XCLStringConverter.h"
#include "XCLParsingSymbol.h"


XCDLTreeBuilder::XCDLTreeBuilder()
{
	bDisplayNormData = TRUE;
	qsEmptyNormData  = "";
}


void XCDLTreeBuilder::setDisplayNormData(BOOL bDisplayNormDataNewState)
{
	bDisplayNormData = bDisplayNormDataNewState;
}



void XCDLTreeBuilder::create(	const XCLParsingItem& pT,
                                const QVector<NamesLibProperty*>& dP)
{
    
  xcdlPropertyID=0;
  definedProperties = dP;
  parseTree =pT;
//   QVector<XCLParsingExpression*>  objects=parseTree.getObjectsLevel1();
  QVector<XCLParsingExpression*>  objects= parseTree.pickObjects();
  
  //get ChilsByName2()==alle Kinder, die attribute name haben werden zurück geliefert.
  //Tree dart beliebig tief sein.
  //Falls ein Treeknoten ein Objekt ist, die Traversierung dieses Knotens wird nicht durchgeführt.
  mainObjectExpressionsByName=parseTree.getChildsByName2();
  expressionsByName.clear();
  XCLParsingItem * object;
  XCLParsingExpression* pe;
  objectIDNumber=2;//Die embedded objects werden gezaehlt. Die Zaelung faengt mit 2, da ObjectID="1" das MainObject ist.
  
//   QHashIterator<QString, XCLParsingExpression*> it(mainObjectExpressionsByName);
//   while (it.hasNext()) {
//       it.next();
//       std:: cout <<"\nmain Object_key= "<< it.key().toStdString() ;
//          
//   }
  
  
  for(int ii=0;ii<objects.size();ii++)//Schleife über Objekte
  {

	properties.clear();
	propertySets.clear();
	foundProperties.clear();
	normDataExpressions.clear();
	normDataRelations.clear();
	
	pe=objects.at(ii);
	object=(XCLParsingItem*) objects.at(ii);
	expressionsByName= object->getChildsByName();
	
	objectType=pe->getObjectType ();
	QString objectIdentifier=pe->getIdentifier();
	
	
// 	QHashIterator<QString, XCLParsingExpression*> it(expressionsByName);
// 	while (it.hasNext()) {
// 		it.next();
// 		std:: cout <<"\nkey= "<< it.key().toStdString() ;
// 		
//         }

getDefinedProperties();

  try
  {
      //  normDataExpressions ist ein QVector.
      //get NormData= alle Expretion, die name " normData" haben werden gesammelt.
  //  normDataExpressions=parseTree.getNormData();
    normDataExpressions=object->getNormData();
  }
  catch(XCLException excp)
  {
    excp.message();
  }
 
  //NormData wird in die Hexdezimal Zahl umgewandelt, falls es die Normdata vom bild ist.
  //NormData vom Text wird als Text ausgegeben+ Relationen zu properties nachgeschlagen.
  processNormData();
  addNormDataRelationsToValueSets();
  summerizeValueSets();
//   std::cout<<"\n----ENDE von XCDLTreeBuilder::create--------------------------\n";
//   print();
 
 
 QString objID;
// objID=".:0";
 QString objID2;
 objID2.setNum(objectIDNumber);
 objID+=objID2;
 
 XCDLObject *xcdlObject=new XCDLObject(getNormData(), 
                                       getProperties(),
                                               getPropertySets(),
                                                      getObjectType(),
                                      				objectIdentifier);
 fileObjects.append(xcdlObject);

 objectIDNumber=objectIDNumber+1;;
 
 }
 
 objectIDNumber=0;
  
 if(!mainObjectExpressionsByName.empty () )//Falls Mainobjekt welche Properties hat
 {
     objectIDNumber=1;//MainObject Id ist immer 1;
     properties.clear();
     propertySets.clear();
     foundProperties.clear();
     normDataExpressions.clear();
     normDataRelations.clear();

     expressionsByName= mainObjectExpressionsByName;
     objectType=parseTree.getObjectType();
 
  
//  std::cout<<"PRINT PARSE TREE XCDLTREEBUILDER\n";
//  parseTree.print();
//  Properties vom MAIN Object:
//   QHashIterator<QString, XCLParsingExpression*> it(expressionsByName);
//   while (it.hasNext()) {
//       it.next();
//       std:: cout <<"\nmain Object_key= "<< it.key().toStdString() ;
//          
//   }

     getDefinedProperties();

     try
     {
      //normDataExpressions ist ein QVector.
      //get NormData() = alle Expretion, die name "normData" haben werden gesammelt.
    normDataExpressions=parseTree.getNormData();
        
     }
     catch(XCLException excp)
     {
         excp.message();
     }
  
  //NormData wird in die Hexdezimal Zahl umgewandelt, falls es die Normdata vom bild ist.
  //NormData vom Text wird als Text ausgegeben+ Relationen zu properties nachgeschlagen.
     processNormData();

     addNormDataRelationsToValueSets();
     summerizeValueSets();
//      std::cout<<"\n----END XCDLTreeBuilder::create()--------------------------\n";
//      print();
 }


 if(!fileObjects.empty())
{
XCDLProperty actProperty2 = properties.at(0);
  if(properties.size()==1)//Es existiert kein main objekt, aber NUR ein sub_Objekt!!!!!!!!!!!!!
{
      objectType = fileObjects.at(0)->getObjectType();
      normData = fileObjects.at(0)->getNormData();
      properties = fileObjects.at(0)->getProperties();
      propertySets = fileObjects.at(0)->getPropertySets();
      QString objID = fileObjects.at(0)->getObjectID();
      //das Object in filesObject löschen!
      //fileObjects.clear();
      fileObjects.remove ( 0 );
}
// 	std::cout<<"\n pi==foi "<<pi.toStdString()<<" :: "<<foi.toStdString()<<"\n"; 
}
 
}

void XCDLTreeBuilder::makeProperties( QList<XCLParsingExpression*> values,QString id,QString pId)
{


  for (int j=0;j<values.size();j++)
  {
    XCLParsingExpression* expr = values.at(j);
    XCDLProperty property;
    property.setName(expr->getName());
    property.setValue(expr->getInterpretedValue());//Falls expr==ParsingExpression Kinder hat, werden alle Values von diesen Kinderknoten zusammengefasst.
    property.setPropertyID(expr->getIdentifier());
    property.setXCDLID(id);
    property.setNamesLibID(pId);	
    property.setNewObject ( expr->getNewObject() );
    property.setObjectType (expr->getObjectType() );	

    if(expr->getNormDataRelation())
	property.setDataRefIndicator("normSpecific");
    else
	property.setDataRefIndicator("global");	

    QString type = expr->getInterpretation();
    
    if(type == "uint8" || type=="uint16" || type == "uint32")
      	type="int";
    else if(type == "uint32Rational")
      	type="rational";
    else if(type =="ASCII" || type =="utf-8" || type =="Latin1")
      	type="string";
    if(expr->getInterpretationType() == XCLParsingExpression::LABEL)
	type="XCLLabel";

    property.setType(type);
  //  property.print();
    QVector<XCLParsingExpression*> relations = expr->getRelations();
    XCDLPropertySet newPropertySet;
    if(!relations.isEmpty())
    {
  //    std::cout<<"\tExplicit Relations: \n";
      for(int k=0;k<relations.size();k++)
      {
	XCLParsingExpression* related = relations.at(k);//parseTree.getChild(relatedStr);
        if(related->getType() == XCLParsingExpression::ITEM_TYPE)
        {
//	  std::cout<<related->getName().toStdString()<<" "<<related->getIdentifier().toStdString()<<"\n"; 
          findAllRelations((XCLParsingItem*)related,newPropertySet);
        }
        else
	{
	  newPropertySet.addValueSetRelationId(related->getIdentifier());
	  newPropertySet.addValueSetRelationName(related->getName());	
 //         std::cout<<"\t\t"<<related->getIdentifier().toStdString()<<"\n";
	}
      }
    }

    if(expr->getParent()->getIdentifier() != parseTree.getIdentifier()&&  !expr->getParent()->getNewObject())
    {
  //    std::cout<<"\n\tImplicite Relations: \n";
      QVector<XCLParsingExpression*> rel = expr->getSiblings();

      for(int k=0;k<rel.size();k++)
      {
        XCLParsingExpression* related = rel.at(k);
	if(isProperty(related))
        {
	//	std::cout<<"\t\t"<<related->getIdentifier().toStdString()<<"\n";
		newPropertySet.addValueSetRelationId(related->getIdentifier());
		newPropertySet.addValueSetRelationName(related->getName());
	}
      }
    }
//### Bei Normdata-Relation neues PropertySet erzeugen33
   
    BOOL isAlreadyThere = FALSE;
    for(int i=0;i<propertySets.size();i++)
    {
	XCDLPropertySet oldPropertySet = propertySets.at(i);

	
	if(oldPropertySet == newPropertySet)
	{
		isAlreadyThere=TRUE;
		break;
	}

	if(oldPropertySet.contains(newPropertySet))
	{
		newPropertySet.merge(oldPropertySet);
		isAlreadyThere=TRUE;
		propertySets.replace(i,newPropertySet);
	}
	else if(newPropertySet.contains(oldPropertySet))
	{
		newPropertySet.merge(oldPropertySet);
		isAlreadyThere=TRUE;
		propertySets.replace(i,newPropertySet);
	}
	
	
    }

    if(!isAlreadyThere  &&  ! newPropertySet.isEmpty())
    	propertySets.append(newPropertySet);
    

    properties.append(property);

  }
  

  
  QVector<XCDLPropertySet> unifiedPropertySets;
   for(int i=0;i<propertySets.size();i++)
   {
	XCDLPropertySet propertySet = propertySets.at(i);
	for(int j=0;j<propertySets.size();j++)
	{
		if(j>i)
		{
			XCDLPropertySet oldPropertySet=propertySets.at(j);
			if(propertySet == oldPropertySet)
			{
				propertySet.merge(oldPropertySet);
				propertySets.remove(j);
			}
		}
	}
	unifiedPropertySets.append(propertySet);
   }
   propertySets = unifiedPropertySets;	

}

void XCDLTreeBuilder::findAllRelations(XCLParsingItem* item,XCDLPropertySet& newPropertySet)
{
//std::cout<<"analyse:...\n";
//item->print();
  QVector<XCLParsingExpression*> rel= item->getChildren();
  for(int i=0;i<rel.size();i++)
  {
	
	XCLParsingExpression* related = rel.at(i);

    	if(related->getType() == XCLParsingExpression::ITEM_TYPE )
        {
	
 	  if(isProperty(related))
	  {	
     //    	std::cout<<"\t\t"<<related->getIdentifier().toStdString()<<"\n";
		newPropertySet.addValueSetRelationId(related->getIdentifier());
		newPropertySet.addValueSetRelationName(related->getName());
	  }
	  else	
          	findAllRelations((XCLParsingItem*)related,newPropertySet);
        }
        else
	{
	
 	  if(isProperty(related))
	  {	
      //   	std::cout<<"\t\t"<<related->getIdentifier().toStdString()<<"\n";
		newPropertySet.addValueSetRelationId(related->getIdentifier());
		newPropertySet.addValueSetRelationName(related->getName());
	  }	
        }

  }
}

BOOL XCDLTreeBuilder::isProperty(XCLParsingExpression* expr)
{
	BOOL hit=FALSE;
          for(int j=0;j<definedProperties.size();j++)
 	  {
		if(definedProperties.at(j)->getPropertyValue()==expr->getName())
		{
			hit=TRUE;
			break;
		} 
	  }
	return hit;
}

const QString& XCDLTreeBuilder::getNormData()
{
	if (bDisplayNormData == TRUE)
		return normData;
	else
		return qsEmptyNormData;
}

const QVector<XCDLProperty>& XCDLTreeBuilder::getProperties()
{
  return properties;
}

const QVector<XCDLPropertySet>& XCDLTreeBuilder::getPropertySets()
{
  return propertySets;
}

const QVector<XCDLObject*>& XCDLTreeBuilder::getFileObjects()
{
    return fileObjects;
}

void XCDLTreeBuilder::print()
{
  std::cout<<"Print internal XCDL representation---------------------------------------------------\n";
  std::cout<<"PROPERTIES----------------------------\n";
  for(int i=0;i<properties.size();i++)
  {
	//((XCDLProperty)properties.at(i)).print();
	
 	
  }  
/*std::cout<<"\tRelations:\n";
  for(int i=0;i<propertySets.size();i++)
  {
	
	((XCDLPropertySet)propertySets.at(i)).print();
 }*/
 std::cout<<"NormDataRelations----------------------------\n";
  for(int i=0;i<normDataRelations.size();i++)
  {
	std::cout<<normDataRelations.at(i).relatedExpression->getIdentifier().toStdString()<<"\t";
	std::cout<<normDataRelations.at(i).relatedExpression->getName().toStdString()<<"\n";
	std::cout<<normDataRelations.at(i).start<<"\n";
	std::cout<<normDataRelations.at(i).end<<"\n";
  } 
 std::cout<<"Print END-----------------------------------------------------------------------------\n";
 
}


//NormData wird in die Hexdezimal Zahl umgewandelt, falls es die Normdata vom bild ist.
//NormData vom Text wird als Text ausgegeben+ Relationen zu properties nachgeschlagen bzw. ausgerechnet.
void XCDLTreeBuilder::processNormData()
{
	QByteArray v;
	normData = "";

	if(!normDataExpressions.isEmpty())
	{
		if(normDataExpressions[0]->getInterpretationType() == XCLParsingExpression::NUMERIC)
		{
//		std::cout<<"NOEM IS NUMERIC\n";
		for (int i=0;i<normDataExpressions.size();i++)
		{
			v.append(normDataExpressions[i]->getValue());
			v.append(" ");
		}
		v.remove(v.length()-1,1);
		QString* normString = in.normalizeValue( (UCHAR*)v.data(), v.size(), ((XCLParsingSymbol*)normDataExpressions[0])->getInterpretation(), normDataExpressions[0]->getIsBigEndian());
		normData = XCLStringConverter::decStringToHex(*normString);
		}
		else
		{
			int currentNormDataLength = 0;

//			NormData Positions:						(0 )	(1,2,3)	(4)				 	 (5)	(6,7,8)	(9)                          			(10)	(11,12)	(13)
//			normDataExpressions:	(fontMarker=/F1)	(F)	(ont)	( )	(fontMarker=/F2)	 (F)	(ont)	( )		(fontMarker=/F1)	(F)	(on)		(t)

			for (int i=0;i<normDataExpressions.size();i++)
			{
				if(!normDataExpressions[i]->getNormDataRelation())
				{
					QString interpretedValue = normDataExpressions[i]->getInterpretedValue();

					currentNormDataLength = currentNormDataLength + interpretedValue.size();
//					std::cout<<"\nnormDataLength="<<interpretedValue.size()<<"\n";
//					std::cout<<"\ncurrentNormDataLength="<<currentNormDataLength<<"\n";;
					normData.append(interpretedValue);
                
				}
				else // Property fontChange(name=identifier=fontMarker)
				{
					NormDataRelation nr;
					nr.relatedExpression = normDataExpressions[i];
					nr.start = currentNormDataLength;
//					std::cout<<"\nnr.start="<<nr.start<<"\n";
					if(!normDataRelations.isEmpty()){
//
						normDataRelations.last().end=currentNormDataLength-1;
//						std::cout<<"\nLast nr.start="<<normDataRelations.last().start<<"\n";
//						std::cout<<"\nLast nr.end="<< normDataRelations.last().end<<"\n";
					}
					normDataRelations.append(nr);
//					std::cout << nr.relatedExpression->getName().toStdString()<<"\n";  
				}
			}

			if(!normDataRelations.isEmpty())
			    normDataRelations.last().end=currentNormDataLength-1;
        
//			std::cout<<"\n normDataRelations: \n";
//			for (int i = 0; i < normDataRelations.size(); i++)
//			{
//				NormDataRelation nr =normDataRelations.at(i) ;
//				std::cout<<"\nnr.start="<<nr.start<<" --- ";
//				std::cout<<"nr.end="<<nr.end<<"\n";
//			}
        
		}
	}
}

void XCDLTreeBuilder::summerizeValueSets()
{

  QHash<QString,XCDLProperty> forming;
  QHash<QString,QString> idMap;
  for(int i=0;i<properties.size();i++)
  {
	XCDLProperty actProperty = properties.at(i);
	if(forming.contains(actProperty.getName()+":"+actProperty.getValue()))
	{
		XCDLProperty oldProperty = forming.value(actProperty.getName()+":"+actProperty.getValue());	
		idMap.insert(actProperty.getPropertyID(),oldProperty.getPropertyID());
		//std::cout<<"Map :\t"<<actProperty.getPropertyID().toStdString()<<"\tto "<<oldProperty.getPropertyID().toStdString()<<"\n";
	}
	else
	{
		forming.insert(actProperty.getName()+":"+actProperty.getValue(),actProperty);
	}
  }
  QList<QString> keys=idMap.keys();
  for(int i=0;i<keys.size();i++)
  {
	QString key = keys.at(i);
	
	for(int j=0;j<propertySets.size();j++)
	{
	   XCDLPropertySet propSet = propertySets.at(j);
	   propSet.replaceID(key,idMap.value(key));
	   propertySets.replace(j,propSet);
	}	

	for(int j=0;j<properties.size();j++)
  	{
		XCDLProperty actProperty = properties.at(j);
		QString id=actProperty.getPropertyID();
		if( key == id)
			properties.remove(j);
  	}
  }

  BOOL* pSetMap = new BOOL[propertySets.size()];
  for(int i=0;i<propertySets.size();i++)pSetMap[i]=TRUE;
  QVector<XCDLPropertySet> newPropSets;
  
  for(int i=0;i<propertySets.size();i++)
  {
        
	XCDLPropertySet iPropSet = propertySets.at(i);
        if(pSetMap[i])
        {
		for(int j=0;j<propertySets.size();j++)
		{
		XCDLPropertySet jPropSet = propertySets.at(j);
		if(i!=j && iPropSet == jPropSet)
		{
			iPropSet.merge(jPropSet);
			pSetMap[j] = FALSE;
		}
		else
		{
			if(pSetMap[j])
			pSetMap[j] = TRUE;
		}
			
		}
		newPropSets.append(iPropSet);
        }
  }
  propertySets = newPropSets;
  
  
  
}
void XCDLTreeBuilder::addNormDataRelationsToValueSets()
{
//     std::cout<<"propertySets:\n";
//     for(int i=0;i<propertySets.size();i++)
//     {
// 	
//         ((XCDLPropertySet)propertySets.at(i)).print();
//     }
//     std::cout<<"propertySets:\n";
  
    
//   	 std::cout<<"\n normDataRelations: \n";
//          for (int i = 0; i < normDataRelations.size(); i++) 
//          {
//              NormDataRelation nr =normDataRelations.at(i) ;
//              std::cout<<"\n"<<nr.relatedExpression->getName().toStdString()<<"\n";  
//              std::cout<<"\nnr.start="<<nr.start<<" --- ";
//              std::cout<<"nr.end="<<nr.end<<"\n";
//          }
    
	for(int i=0;i<normDataRelations.size();i++)
        {
		XCLParsingExpression* expr = normDataRelations.at(i).relatedExpression;
		QString start;
		QString end;
		start.setNum(normDataRelations.at(i).start);
		end.setNum(normDataRelations.at(i).end);
		
			
		for(int j=0;j<propertySets.size();j++)
		{
			XCDLPropertySet theSet = propertySets.at(j);
			if(theSet.contains(expr->getIdentifier()))
			{
// 				if(normDataRelations.at(i).start > normDataRelations.at(i).end)
// 				{
// 					propertySets.remove(j);
// 					break;
// 				}
//std::cout<<"---------------------"<<expr->getIdentifier().toStdString()<<"\n";
			theSet.setHasNormDataReference(TRUE);
			theSet.addStartNEnd(start,end);
			propertySets.remove(j);
			propertySets.append(theSet);
			break;
			}
		}
	}
	
}

void XCDLTreeBuilder::getDefinedProperties()
{
	
	for(int i=0;i<definedProperties.size();i++)
     {
         xcdlPropertyID=xcdlPropertyID+1;
         
        if(definedProperties[i]->getPropertyValue() == "normData")
         {
             definedProperties.remove(i);
        }
         else
             if(expressionsByName.contains(definedProperties[i]->getPropertyValue()))
         {
		//values ist eine Liste von properties mit gleichen Namen
             QList<XCLParsingExpression*> values =
                     expressionsByName.values(definedProperties[i]->getPropertyValue());
             QString id;
             id.setNum(xcdlPropertyID);
             id.prepend("p");
             makeProperties(values,id,definedProperties[i]->getPropertyId());

             for (int j=0;j<values.size();j++)
             {
                 foundProperties.append(values.at(j));//wo werden dies später verwendet?
             }
         }
     }
     
       //Properties sortieren:
     QMap<QString, XCDLProperty> map;
     for (int i = 0; i < properties.size(); i++) {
         XCDLProperty actProperty = properties.at(i);
         QString actPropertyName=actProperty.getName() ;
         map.insertMulti( actPropertyName, actProperty);
//          std::cout << "\n "<<i<<"  = "<<actPropertyName.toStdString() << "\n";
     }
     properties.clear();
     foreach (XCDLProperty value, map){
         XCDLProperty actProperty = value;
         properties.append(actProperty);
     }
     
     
//      for (int i = 0; i < properties.size(); ++i) {
//          XCDLProperty actProperty = properties.at(i);
//          QString actPropertyName=actProperty.getName() ;
//          std::cout << "\n "<<i<<"  = "<<actPropertyName.toStdString() << "\n";
//      }
     
     
 }