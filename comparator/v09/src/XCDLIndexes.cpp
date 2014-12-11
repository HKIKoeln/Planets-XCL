#include <string>
#include <map>
#include <vector>
#include "XCDLIndexes.h"
#include "ComparisonConfigurationData.h"


/******************************************************************************
******************      class DynamicIndexes      *******************************
******************************************************************************/
DynamicIndexes :: DynamicIndexes()
{
PropsWithDataRef= NULL;
PropsWithoutDataRef= NULL;
dataRefFlag=false;
normDataRequiredFlag= false;
}


/******************************************************************************
******************      class RefParams       *******************************
******************************************************************************/
RefParams :: RefParams()
{
begin="\0";
end="\0";
idToNormDataElement="\0";
}

RefParams :: ~RefParams() {}



/******************************************************************************
******************      class DataRefIndex       *******************************
******************************************************************************/
DataRefIndex :: DataRefIndex()
{
ind="\0";
propSetId= "\0";
referencesBox= NULL;
pRefParams= NULL;
}

DataRefIndex :: ~DataRefIndex() {}


/******************************************************************************
******************      class ObjectIndex       *******************************
******************************************************************************/
ObjectIndex :: ObjectIndex()
{
objectId="\0";
dataId= "\0";
normDataIds= NULL;
propertyIndexBox= NULL;
propertySetIndexBox= NULL;
pPropIndex= NULL;
pPropSetIndex= NULL;
pDynIndex= new DynamicIndexes ();
}

ObjectIndex :: ~ObjectIndex() {}


PropertyIndex* ObjectIndex :: getProperty(std::string name)
{
if(propertyIndexBox!=NULL)
  {
  std::map <std::string, PropertyIndex*> :: iterator i;
  for(i= propertyIndexBox->begin();i!=propertyIndexBox->end();++i)
     {
     if((i->first).compare(name)==0) return i->second;
     }
  }
return NULL;   // in case of not found
}

void ObjectIndex :: print()
       {
       std::cout<<"ObjectId: "<<objectId<<std::endl;
       std::cout<<"DataId: "<<dataId<<std::endl;
       if(normDataIds!=NULL)
         {
        std::map <std::string, std::string> :: iterator i1;
        for(i1= normDataIds->begin();i1!=normDataIds->end();++i1)
           {
           std::cout<<"NormDataId: "<<i1->first<<std::endl;
           std::cout<<"NormDataType: "<<i1->second<<std::endl;
           }
         }
       if(propertyIndexBox!=NULL)
         {
         std::map <std::string, PropertyIndex*> :: iterator i2;
         for(i2= propertyIndexBox->begin();i2!=propertyIndexBox->end();++i2)
            {
            std::cout<<"XCLPropertyId: "<<i2->first<<std::endl;
            i2->second->print();
            std::cout<<std::endl;
            }
         }
       if(propertySetIndexBox!=NULL)
         {
         std::map <std::string, PropertySetIndex*> :: iterator i3;
         for(i3= propertySetIndexBox->begin();i3!=propertySetIndexBox->end();++i3)
            {
            std::cout<<"XCDLPropertySetId: "<<i3->first<<std::endl;
            i3->second->print();
            std::cout<<std::endl;
            }
         }
       }


/******************************************************************************
******************      class XCDLIndex     ***********************************
******************************************************************************/
XCDLIndex :: XCDLIndex()
{
XCDLId= "\0";
pObjIndex= NULL;
objectIndexBox= new std::map <std::string, ObjectIndex*> ();
}

XCDLIndex :: ~XCDLIndex() {}


/******************************************************************************
*******************      class PropertySetIndex      **************************
******************************************************************************/
PropertySetIndex :: PropertySetIndex()
{
valueSetRelationsBox= NULL;
pDataRefIndex= NULL;
propertySetId= "\0";
}

PropertySetIndex :: ~PropertySetIndex() {}


/******************************************************************************
******************     class PropertyIndex         ******************************
******************************************************************************/
PropertyIndex :: PropertyIndex()
{
propertyXCDLId="\0";
propertyId= "\0";
propertyName= "\0";
valueSetIndexBox= NULL;
pValSetIndex= NULL;
}

PropertyIndex :: ~PropertyIndex() {}


/******************************************************************************
*****************      class ValueSetIndex         ****************************
******************************************************************************/
ValueSetIndex :: ValueSetIndex()
{
valueSetID="\0";
objectRef= "\0";
labValVal="";
labValType="";
dataRefIndexBox= NULL;
pDataRefIndex= NULL;
rawValueFlag= false;
labValueFlag= false;
}
ValueSetIndex :: ~ValueSetIndex(){}
