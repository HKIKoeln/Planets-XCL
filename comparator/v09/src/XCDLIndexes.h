/*
    * This module is part of the XCL software system
    *
    * Written by  Sebastian Beyl, Volker Heydegger, Jan Schnasse,
    *             Manfred Thaller
    *             2007 - 2009
    *
    * Copyright (C)  2007 - 2009
    * Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller
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

#ifndef _XCDLINDEXES_H
#define _XCDLINDEXES_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include "defines.h"

// Dynamic indexes:
class DynamicIndexes
{
public:
DynamicIndexes();
~DynamicIndexes();

inline void print()
{
std::map <std::string, bool> :: iterator i;

if(PropsWithDataRef!=NULL)
  {
  std::cout<<"Properties with data references:"<<std::endl;
  for(i=PropsWithDataRef->begin();i!=PropsWithDataRef->end(); ++i)
     {
     std::cout<<(i->first)<<" is ";
     if(i->second==true) std::cout<<"available";
     else std::cout<<"not available";
     std::cout<<std::endl;
     }
  }

std::map <std::string, bool> :: iterator i2;

if(PropsWithoutDataRef!=NULL)
  {
  std::cout<<"Properties without data references:"<<std::endl;
  for(i2=PropsWithoutDataRef->begin();i2!=PropsWithoutDataRef->end(); ++i2)
     {
     std::cout<<(i2->first)<<" is ";
     if(i2->second==true) std::cout<<"available";
     else std::cout<<"not available";
     std::cout<<std::endl;
     }
  }
}
bool dataRefFlag;
bool normDataRequiredFlag;
std::map <std::string, bool> *PropsWithDataRef;    // set of properties with data references, second param is for optional usage (e.g. Planets PP5 usage, indicates if requested for comparison)
std::map <std::string, bool> *PropsWithoutDataRef; // set of properties without data references (second param also for optional usage)
};




// stores the parameters for data references used in propertySet:
class RefParams
{
public:
RefParams();
~RefParams();

inline void print()
    {
    std::cout<<"IdToNormDataElement: "<<idToNormDataElement<<std::endl;
    std::cout<<"Begin: "<<begin<<std::endl;
    std::cout<<"End: "<<end<<std::endl;
    }
std::string begin;
std::string end;
std::string idToNormDataElement;
};

// for storage of dataRef element parameters:
class DataRefIndex
{
public:
DataRefIndex();
~DataRefIndex();

// for usage with propertySet:
std::vector <RefParams*> *referencesBox;
RefParams *pRefParams;

inline void print1()
{
std::vector <RefParams*> :: iterator i;

if(referencesBox!=NULL)
  {
  for(i=referencesBox->begin();i!=referencesBox->end(); ++i)
     {
     (*i)->print();
     }
  }
}

// for usage with property:
std::string ind;
std::string propSetId;
inline void print2()
{
std::cout<<"DataRef, Ind: "<<ind<<" ";
std::cout<<"PropSetId: "<<propSetId<<std::endl;
}
};


/* Structure of propertySet element:

<propertySet id="propSetId">   // id is optional!!
   <valueSetRelations>    // shall appear exactly one time
       <ref valueSetId="valueSetId" name="nameOfReferencedProperty">  // id is required
       ...
  </valueSetRelations>
  <datRef>
     <ref begin="startPosition" end="endPosition" id="normDataElementId">
     ...
  </dataRef>
</propertySet>
*/
class PropertySetIndex
{
public:
PropertySetIndex();
~PropertySetIndex();

inline void print()
{
std::map <std::string, std::string> :: iterator i;//valueSetRelationsBox

if(valueSetRelationsBox!=NULL)
  {
  for(i= valueSetRelationsBox->begin();i!=valueSetRelationsBox->end();++i)
     {
     std::cout<<i->first<<std::endl<<i->second<<std::endl;
     }
  }


if(pDataRefIndex!=NULL) pDataRefIndex->print1();
}
std::string propertySetId;
std::map <std::string, std::string> *valueSetRelationsBox; // stores the 'ref' elements valueSetId (key) and optionally name attributes
DataRefIndex *pDataRefIndex;  // there is always only one single dataRef allowed
};


/*
class 'PropertyIndex' and 'ValueSetIndex' both pattern the structure of the property tagset:

<property id="p_1" source="sourceAttrValue" cat="catAttrValue"> // id is required
   <name id="uniqueXCLId" alias="someOtherName">...</name>  // name: min=1, max=1,  id is required
   <valueSet id="v_1>   // id is required; min=1, max=unbounded
        <rawValue ....        // each of these three elements: min=0, max=1 !!
        <labValue ....
        <objectRef  ....
        <dataRef  .....      // min=0, max=unbounded
   </valueSet>
   ...
</property>
*/

class ValueSetIndex
{
public:
      ValueSetIndex();
      ~ValueSetIndex();
      inline std::vector <DataRefIndex*>* getDataRefIndexBox() { if(dataRefIndexBox!=NULL) return dataRefIndexBox;}
      inline std::string getLabValVal() {return labValVal;}
      inline void print()
      {
      std::vector <DataRefIndex*> :: iterator i;
      std::cout<<"ValueSetID: "<<valueSetID<<std::endl;
      std::cout<<"ObjectRef: "<<objectRef<<std::endl;
      std::cout<<"labValVal: "<<labValVal<<std::endl;
      std::cout<<"labValType: "<<labValType<<std::endl;

      if(dataRefIndexBox!=NULL)
        {
        for(i= dataRefIndexBox->begin();i!=dataRefIndexBox->end();++i)
           {
           (*i)->print2();
           }
        }
      }

      std::string valueSetID;
      bool rawValueFlag;
      bool labValueFlag;
      std::string labValVal;
      std::string labValType;
      std::string objectRef;
      std::vector <DataRefIndex*> *dataRefIndexBox; // for usage with <property> !!
      DataRefIndex *pDataRefIndex;                  //  ...
};

class PropertyIndex
{
public:
       PropertyIndex();
       ~PropertyIndex();
       inline std::vector <ValueSetIndex*>* getValueSetIndexBox() {if(valueSetIndexBox!=NULL) return valueSetIndexBox;}
       inline void print()
       {
       std::cout<<"PropertyXCDLId: "<<propertyXCDLId<<std::endl;
       std::cout<<"PropertyId: "<<propertyId<<std::endl;
       std::cout<<"PropertyName: "<<propertyName<<std::endl;
       std::cout<<"PropertyAlias: "<<propertyAlias<<std::endl;

       std::vector <ValueSetIndex*> :: iterator i;
       if(valueSetIndexBox!=NULL)
         {
         for(i=valueSetIndexBox->begin();i!=valueSetIndexBox->end();++i)
            {
            (*i)->print();
            }
         }
       }
       std::string propertyXCDLId; // required XCDL wide property ID
       std::string propertyId; // the unique XCL property ID, =attribute 'id' of element 'name'
       std::string propertyName; // name of the property
       std::string propertyAlias;
       std::vector <ValueSetIndex*> *valueSetIndexBox; // contains all valueSets from a single property
       ValueSetIndex *pValSetIndex;
};


/*
class 'ObjectIndex' patterns the structure of the object tagset:

<object id="o_1">     // min=1, max=unbounded, id is required
   <data id=”data_1”>...</data>  // min=0, max=1,  id is required
   ...
   <normData id="normData_1">...</normData> // min=0, max=unbounded, id is required
   ...
   <property id=”property_1">...</property>  // min=0, max=unbounded, id is required
   ...
   <propertySet id="propSet_1">...</propertySet>  // min=0, max=unbounded, id is optional!
   ...
</object>
*/

class ObjectIndex
{
public:
       ObjectIndex();
       ~ObjectIndex();
       inline std::map <std::string, PropertyIndex*>* getPropertyBox() {return propertyIndexBox;}
       inline std::map <std::string, PropertySetIndex*>* getPropertySetBox() {return propertySetIndexBox;}
       inline std::map <std::string, std::string>* getNormDataIds() {return normDataIds;}
       PropertyIndex* getProperty(std::string name);

       void print();

       std::string objectId;  // required object ID
       std::string dataId; // optional data ID (only if data element is present)
       // private:
       std::map <std::string, std::string> *normDataIds; // not empty if normData elements are set, first element is id, second type
       std::map <std::string, PropertyIndex*> *propertyIndexBox; // version 0.3: key is propertyName     /*** TODO next vwrsion: key should be unique XCL property ID  ***/
       std::map <std::string, PropertySetIndex*> *propertySetIndexBox; // key is XCDL propertySetId, if available, else internal
       PropertyIndex *pPropIndex;
       PropertySetIndex *pPropSetIndex;
       DynamicIndexes *pDynIndex;
};




/*
class 'XCDLIndex' patterns the low level structure of the XCDL file:

<xcdl id="i_1">    // attribute 'id' is required
  <object id="o_1">...</object>  // objects: min=1, max=unbounded
  <object id="o_2">...</object>
  ...
</xcdl>

*/
class XCDLIndex
{
public:
       XCDLIndex();
       ~XCDLIndex();
       inline void print()
       {
       std::cout<<"XCDLId: "<<XCDLId<<std::endl;
       if(objectIndexBox!=NULL)
         {
         std::map <std::string, ObjectIndex*> :: iterator i;//valueSetRelationsBox
         for(i= objectIndexBox->begin();i!=objectIndexBox->end();++i)
            {
            std::cout<<"ObjectId in XCDLIndex: "<<i->first<<std::endl;
            (i->second)->print();
            std::cout<<std::endl;
            }
         }
       }

       std::string XCDLId;
       std::map <std::string, ObjectIndex*> *objectIndexBox; // container for storage of the objects; id is objectId
       ObjectIndex *pObjIndex;  // pointer to a single object stored in objectIndexMap
};


#endif
