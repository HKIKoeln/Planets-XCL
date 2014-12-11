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
#include "XCDLPropertySet.h"
#include <iostream>

XCDLPropertySet::XCDLPropertySet()
{
hasRef=FALSE;
}

const QVector<QString>& XCDLPropertySet::getValueSetRelationIDs()
{
return valueSetRelationIDs;
}
const QVector<QString>& XCDLPropertySet::getValueSetRelationNames()
{
return valueSetRelationNames;
}
const QString& XCDLPropertySet::getDataRefIndicator()
{
return dataRefIndicator;
}
const QString& XCDLPropertySet::getNormDataId()
{
return normDataId;
}
const QVector<QString>& XCDLPropertySet::getStart()
{
return start;
}
const QVector<QString>& XCDLPropertySet::getEnd()
{
return end;
}
void XCDLPropertySet::setValueSetRelationIDs(const QVector<QString>& vR)
{
valueSetRelationIDs = vR;
}
void XCDLPropertySet::setValueSetRelationNames(const QVector<QString>& vR)
{
valueSetRelationNames = vR;
}
void XCDLPropertySet::setDataRefIndicator(const QString& dR)
{
dataRefIndicator=dR;
}
void XCDLPropertySet::setNormDataId(const QString& nId)
{
normDataId=nId;
}
void XCDLPropertySet::setStart(const QVector<QString>& s)
{
start =s;
}
void XCDLPropertySet::setEnd(const QVector<QString>& e)
{
end=e;
}

void XCDLPropertySet::addValueSetRelationId(const QString& id)
{
valueSetRelationIDs.append(id);
}

void XCDLPropertySet::addValueSetRelationName(const QString& name)
{
valueSetRelationNames.append(name);
}

void XCDLPropertySet::print()
{
  if(hasRef)
  {
	//std::cout<<"\t"<<start.toStdString();
	//std::cout<<"\t"<<end.toStdString()<<"\n";
  }
  for(int i=0;i<valueSetRelationIDs.size();i++)
  {
	std::cout<<"\t"<<valueSetRelationIDs.at(i).toStdString();
        std::cout<<"\t"<<valueSetRelationNames.at(i).toStdString()<<"\n";
  }
}

BOOL XCDLPropertySet::contains(QString id)
{
	for(int i=0;i<valueSetRelationIDs.size();i++)
	{
		if(valueSetRelationIDs.at(i)==id)
			return TRUE;
	}
	return FALSE;
}

BOOL XCDLPropertySet::hasNormDataReference()
{
return hasRef;
}

BOOL XCDLPropertySet::isEmpty()
{
    if(getValueSetRelationIDs().isEmpty())
    	return TRUE;
   else
       return FALSE;
}

void XCDLPropertySet::setHasNormDataReference(BOOL b)
{
hasRef=b;
}

BOOL XCDLPropertySet::operator==(XCDLPropertySet& set)
{
	if(set.getValueSetRelationIDs().size() != valueSetRelationIDs.size())
	{
		return FALSE;
	}
	for(int i=0;i<valueSetRelationIDs.size();i++)
	{
		if(!set.contains(valueSetRelationIDs.at(i)))
		{
			return FALSE;	
		}
	}
	if(!contains(set))return FALSE;
	return TRUE;
}

BOOL XCDLPropertySet::contains(XCDLPropertySet set)
{
	QVector<QString> ids = set.getValueSetRelationIDs();
	for(int i=0;i<ids.size();i++)
	{
		if(!contains(ids.at(i)))
		{
			return FALSE;	
		}
	}
	return TRUE;
}

void XCDLPropertySet::add(XCDLPropertySet set)
{
	QVector<QString> ids = set.getValueSetRelationIDs();
	QVector<QString> names = set.getValueSetRelationNames();
	for(int i=0;i<ids.size();i++)
	{
		valueSetRelationIDs.append(ids.at(i)); 
		valueSetRelationNames.append(names.at(i));
	}
	
}

void XCDLPropertySet::merge(XCDLPropertySet set)
{
	QVector<QString> ids = set.getValueSetRelationIDs();
	QVector<QString> names = set.getValueSetRelationNames();
	for(int i=0;i<ids.size();i++)
	{
		if(!contains(ids.at(i)))
		{
			valueSetRelationIDs.append(ids.at(i)); 
			valueSetRelationNames.append(names.at(i));
		}
	}
	QVector<QString> setStartpositions = set.getStart();
	QVector<QString> setEndpositions = set.getEnd();
	for(int i=0;i<setStartpositions.size();i++)
 	{
		addStartNEnd(setStartpositions.at(i),setEndpositions.at(i));
	}
}

const QString& XCDLPropertySet::getId()
{
return id;
}

void XCDLPropertySet::setId(const QString& i)
{
	id=i;
}
void XCDLPropertySet::addStartNEnd(const QString& s,const QString& e)
{
	if(!end.isEmpty())
        {
		QString lastStr = end.last();
		long last = lastStr.toULong();
		long act = s.toULong();
		
		if(act == last+1)
		{
			//start.replace(start.size()-1,s);
			end.replace(end.size()-1,e);
			return;
		}
        }

	
	start.append(s);
	end.append(e);
	
}
/*
void XCDLPropertySet::addEnd(const QString& e)
{
end.append(e);
}
*/
void XCDLPropertySet::replaceID(QString oldID, const QString& newID)
{
	for(int i=0;i<valueSetRelationIDs.size();i++)
	{
		if(valueSetRelationIDs.at(i) == oldID)
		{
			valueSetRelationIDs.replace(i,newID);

		//	std::cout<<"Replace :\t"<<oldID.toStdString()<<"\twith "<<newID.toStdString()<<"\n";
		}
	}
}