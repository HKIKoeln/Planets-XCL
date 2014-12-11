/*****************************************************************************
XCDLIndexGenerator.cpp

Description:
*****************************************************************************/
#include <string>
#include <map>
#include <list>
#include <xercesc/sax2/Attributes.hpp>
#include "defines.h"
#include "XercesString.h"
#include "ComparatorError.h"
#include "ComparisonConfigurationData.h"
#include "SaxInterface.h"
#include "XCDLIndexes.h"
#include "XCDLIndexGenerator.h"

// standard constructor (for default usage of Comparator as stand-alone version)
XCDLIndexGenerator :: XCDLIndexGenerator(ComparatorError *pError)
{
  #ifdef DEBUG_XCDLINDEXGENERATOR
  std::cout<<"XCDLIndexGenerator... Initializing object..."<<std::endl;
  #endif

  pIndex= new XCDLIndex();
  parsingState= parseXCDL;
  pErr= pError;
  pCoco= NULL;  // do not use this in stand-alone version
  }

// constructor for usage within Planets PP5 (using customized configfile (PCR)
XCDLIndexGenerator :: XCDLIndexGenerator(ComparatorError *pError, ComparisonConfiguration *pReq2, XCDLIndex *pInd)
{
  #ifdef DEBUG_XCDLINDEXGENERATOR
  std::cout<<"XCDLIndexGenerator... Initializing object..."<<std::endl;
  #endif

  parsingState= parseXCDL;
  pIndex= pInd;
  pErr= pError;
  pCoco= pReq2;
  }



XCDLIndexGenerator :: ~XCDLIndexGenerator() {}


////////////////////////////////////////////////
///      START ELEMENT      ////////////////////
////////////////////////////////////////////////
void XCDLIndexGenerator :: startElement(const XMLCh* const uri,
                         const XMLCh* const localname,
                         const XMLCh* const qname,
                         const Attributes& attrs)
{

if(parsingState==erroneous) return;

std::string str1;
char *strLocalname = XMLString::transcode(localname);

 #ifdef DEBUG_XCDLINDEXGENERATOR
  printf("starting Element... %s\n", strLocalname);
  #endif



  if(parsingState==parseXCDL)                        // parsing position <xcdl>
    {
    parsingState=startXCDL;

    const XercesString ELEM_xcdl("xcdl");
    if(!XMLString::compareString(localname, ELEM_xcdl))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <xcdl> Tag... %s\n", strLocalname);
      #endif

       // get attribute: id and store it in Index structure
      const XercesString XCDLID("id");
      const XercesString XCDLIDValue(attrs.getValue(XCDLID));
      char* xcdlid= XMLString::transcode(XCDLIDValue);
      str1= std::string(xcdlid);
      pIndex->XCDLId= str1;
      XMLString::release(&xcdlid);
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'parseXCDL\'. Tag: <xcdl> not found.");
      // TODO: Validierung?
      parsingState= erroneous;
      }
    }

  else if(parsingState==startXCDL || parsingState==endObject)  // parsing position <object>
    {
    const XercesString ELEM_object("object");

    if(!XMLString::compareString(localname, ELEM_object))
      {
      parsingState=startObject;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found object Tag... %s\n", strLocalname);
      #endif

       // create new objectIndex, get attribute id, store it in Index structure:
      const XercesString objectId("id");
      const XercesString objectIdValue(attrs.getValue(objectId));
      char* objectid= XMLString::transcode(objectIdValue);
      str1= std::string(objectid);
      if(pIndex->objectIndexBox== NULL) pIndex->objectIndexBox= new std::map <std::string, ObjectIndex*>();
      pIndex->pObjIndex= new ObjectIndex();
      pIndex->pObjIndex->objectId= str1;
      XMLString::release(&objectid);
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startXCDL\'. or \'endObject\'. Expected Tag: <object>.");
      parsingState= erroneous;
      // todo validation
      }
    }

  else if(parsingState==startObject ||
          parsingState==endData ||
          parsingState==endNormData ||
          parsingState==endProperty ||
          parsingState==endPropertySet)
    {
    const XercesString ELEM_data("data");
    const XercesString ELEM_normData("normData");
    const XercesString ELEM_property("property");
    const XercesString ELEM_propertySet("propertySet");

    if(!XMLString::compareString(localname, ELEM_data))  // parsing position: <data>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <data> Tag... %s\n", strLocalname);
      #endif
      parsingState=startData;

      // if (validator->dataFlag==false) validator->dataFlag=true;
      // else TODO: validation error (data element shall only appear one time)

      const XercesString ATTR_XCDLdataId("id");
      if(attrs.getValue(ATTR_XCDLdataId))
        {
         // get attribute id and store it:
         const XercesString XCDLdataIDValue(attrs.getValue(ATTR_XCDLdataId));
         char* xcdldataid= XMLString::transcode(XCDLdataIDValue);
         str1= std::string(xcdldataid);
         pIndex->pObjIndex->dataId= str1;
         XMLString::release(&xcdldataid);
         }
      else{/*  validation error (attribute id is required)*/}
      }

    else if(!XMLString::compareString(localname, ELEM_normData))  // parsing position: <normData>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <normData> Tag... %s\n", strLocalname);
      #endif
      parsingState=startNormData;

      std::string tmpNdType;

      // allocate memory for container which stores normData IDs:
      if(pIndex->pObjIndex->normDataIds==NULL) pIndex->pObjIndex->normDataIds= new std::map <std::string, std::string> ();

      // get id and throw it into normDataIds container:
      const XercesString ATTR_XCDLnormDataId("id");
      if(attrs.getValue(ATTR_XCDLnormDataId))
        {
        const XercesString XCDLnormDataIdValue(attrs.getValue(ATTR_XCDLnormDataId));
        char* xcdlnormdataid= XMLString::transcode(XCDLnormDataIdValue);
        str1= std::string(xcdlnormdataid);
        XMLString::release(&xcdlnormdataid);
        }
       // get type and throw it into normDataIds container:
      const XercesString ATTR_XCDLnormDataType("type");
      if(attrs.getValue(ATTR_XCDLnormDataType))
        {
        const XercesString XCDLnormDataTypeValue(attrs.getValue(ATTR_XCDLnormDataType));
        char* xcdlnormdatatype= XMLString::transcode(XCDLnormDataTypeValue);
        tmpNdType= std::string(xcdlnormdatatype);
        XMLString::release(&xcdlnormdatatype);
        }
      // store it:
      pIndex->pObjIndex->normDataIds->insert(make_pair(str1, tmpNdType));

      // allocate memory for dynamic index: list of properties with data references:
      if(pIndex->pObjIndex->pDynIndex==NULL) pIndex->pObjIndex->pDynIndex= new DynamicIndexes ();
      if(pIndex->pObjIndex->pDynIndex->PropsWithDataRef==NULL)
        pIndex->pObjIndex->pDynIndex->PropsWithDataRef= new std::map <std::string, bool> ();

      // for Planets PP5 usage, check if property is requested:
      bool requested;
      if(pCoco!=NULL) requested= PP5_checkIfPropertyIsRequested(pCoco, "normData");
      else requested= false;
      // store it:
      pIndex->pObjIndex->pDynIndex->PropsWithDataRef->insert(make_pair(std::string("normData"), requested));
      // set flag which indicates that normData is reqired for comparison:
      pIndex->pObjIndex->pDynIndex->normDataRequiredFlag= true;
      }

    else if(!XMLString::compareString(localname, ELEM_property))  // parsing position <property>
      {
      parsingState=startProperty;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <property> Tag... %s\n", strLocalname);
      #endif

      // allocate memmory for object:
      pIndex->pObjIndex->pPropIndex= new PropertyIndex ();
      // allocate memory for container which keeps property elements:
      if(pIndex->pObjIndex->propertyIndexBox==NULL) pIndex->pObjIndex->propertyIndexBox= new std::map <std::string, PropertyIndex*> ();


      // get XCDL wide property id and store it:
      const XercesString ATTR_XCDLpropertyID("id");
      if(attrs.getValue(ATTR_XCDLpropertyID))
        {
        const XercesString XCDLpropertyIDValue(attrs.getValue(ATTR_XCDLpropertyID));
        char* xcdlpropertyId= XMLString::transcode(XCDLpropertyIDValue);
        str1= std::string(xcdlpropertyId);
        pIndex->pObjIndex->pPropIndex->propertyXCDLId= str1;
        XMLString::release(&xcdlpropertyId);
        }
      }

    else if(!XMLString::compareString(localname, ELEM_propertySet))  // parsing position <propertySet>
      {
      parsingState=startPropertySet;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <propertySet> Tag... %s\n", strLocalname);
      #endif

      // allocate memory for object:
      pIndex->pObjIndex->pPropSetIndex= new PropertySetIndex ();
      // allocate memory for container which keeps propertySet elements:
      if(pIndex->pObjIndex->propertySetIndexBox==NULL) pIndex->pObjIndex->propertySetIndexBox= new std::map <std::string, PropertySetIndex*> ();


      // get propertySet id and store it:
      const XercesString ATTR_propertySetID("id");
      if(attrs.getValue(ATTR_propertySetID))
        {
        const XercesString propertySetIDValue(attrs.getValue(ATTR_propertySetID));
        char* propertySetId= XMLString::transcode(propertySetIDValue);
        str1= std::string(propertySetId);
        pIndex->pObjIndex->pPropSetIndex->propertySetId= str1;
        XMLString::release(&propertySetId);
        }
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startObject\' or \'endData\' or \'endNormData\' or \'endProperty\' or \'endPropertySet\'. Expected tag <data> or <normData> or <property> or <propertySet>.");
      // TODO: validator
      parsingState= erroneous;
      }
    }
/*************      PropertySet    ********************************************/
   else if(parsingState==startPropertySet)    // position: <valueSetRelations>
    {
    const XercesString ELEM_valSetRel("valueSetRelations");

    if(!XMLString::compareString(localname, ELEM_valSetRel))
      {
      parsingState=startValueSetRelations;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <valueSetRelations> Tag... %s\n", strLocalname);
      #endif
      // allocate memory for valueSetRelations container:
      if(pIndex->pObjIndex->pPropSetIndex->valueSetRelationsBox==NULL)
         pIndex->pObjIndex->pPropSetIndex->valueSetRelationsBox= new std::map <std::string, std::string> ();
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertySet\'. Expected tag <valueSetRelations>.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startValueSetRelations || parsingState==endValueSetRelationsRef)    // parsing position: <ref>, must follow at least one time
    {
    const XercesString ELEM_valSetRelRef("ref");

    if(!XMLString::compareString(localname, ELEM_valSetRelRef))
      {
      parsingState=startValueSetRelationsRef;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <ref> Tag... %s\n", strLocalname);
      #endif

      // get valueSetId (mandatory) and name of referenced property (optional) and store them:
      const XercesString ATTR_valSetRelRefId("valueSetId");
      const XercesString ATTR_valSetRelRefName("name");
      std::string str2="\0";
      if(attrs.getValue(ATTR_valSetRelRefId))
        {
        const XercesString valSetRelRefIdValue(attrs.getValue(ATTR_valSetRelRefId));
        char* valuesetid= XMLString::transcode(valSetRelRefIdValue);
        str1= std::string(valuesetid);
        XMLString::release(&valuesetid);
        }
      if(attrs.getValue(ATTR_valSetRelRefName))
        {
        const XercesString valSetRelRefNameValue(attrs.getValue(ATTR_valSetRelRefName));
        char* valsetname= XMLString::transcode(valSetRelRefNameValue);
        str2= std::string(valsetname);
        XMLString::release(&valsetname);
        }
      pIndex->pObjIndex->pPropSetIndex->valueSetRelationsBox->insert(make_pair(str1 , str2));
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startValueSetRelations\'. Expected tag <ref>.");
      parsingState= erroneous;
      // TODO validator
      }
    }

  else if(parsingState==endValueSetRelations)    // parsing position: <dataRef> , min=0, max=1;
    {
    const XercesString ELEM_propSetDataRef("dataRef");

    if(!XMLString::compareString(localname, ELEM_propSetDataRef))
      {
      parsingState=startPropertySetDataRef;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <dataRef> Tag (case propertySet)... %s\n", strLocalname);
      #endif
      // allocate memory for dataRefIndex object:
      if(pIndex->pObjIndex->pPropSetIndex->pDataRefIndex==NULL)
         pIndex->pObjIndex->pPropSetIndex->pDataRefIndex= new DataRefIndex ();
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'endValueSetRelations\'. Expected tag <dataRef>.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertySetDataRef || parsingState==endPropertySetDataRefRef)    // parsing position: <ref>
    {
    const XercesString ELEM_propSetRef("ref");

    if(!XMLString::compareString(localname, ELEM_propSetRef))
      {
      parsingState=startPropertySetDataRefRef;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <ref> Tag (case propertySet, dataRef)... %s\n", strLocalname);
      #endif

      // allocate memory for object:
      pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->pRefParams= new RefParams();
      // allocate memory for referencesBox, refParams objects:
      if(pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->referencesBox==NULL)
         pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->referencesBox= new std::vector <RefParams*> ();



      //get attributes begin, end, id and store them:
      const XercesString ATTR_begin("begin");
      const XercesString ATTR_end("end");
      const XercesString ATTR_id("id");

      if(attrs.getValue(ATTR_begin))
        {
        const XercesString propSetDataRefRefBegin(attrs.getValue(ATTR_begin));
        char* begin= XMLString::transcode(propSetDataRefRefBegin);
        str1= std::string(begin);
        pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->pRefParams->begin= str1;
        XMLString::release(&begin);
        }
      if(attrs.getValue(ATTR_end))
        {
        const XercesString propSetDataRefRefEnd(attrs.getValue(ATTR_end));
        char* end= XMLString::transcode(propSetDataRefRefEnd);
        str1= std::string(end);
        pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->pRefParams->end= str1;
        XMLString::release(&end);
        }
      if(attrs.getValue(ATTR_id))
        {
        const XercesString propSetDataRefRefId(attrs.getValue(ATTR_id));
        char* refID= XMLString::transcode(propSetDataRefRefId);
        str1= std::string(refID);
        pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->pRefParams->idToNormDataElement= str1;
        XMLString::release(&refID);
        }
      // throw it all into referencesBox:
      pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->referencesBox->push_back(pIndex->pObjIndex->pPropSetIndex->pDataRefIndex->pRefParams);
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertySetDataRef\' or \'endPropertySetDataRefRef\'. Expected tag <ref>.");
      parsingState= erroneous;
      }
    }

/*******    PROPERTY ELEMENTS    ****************************************/
  else if(parsingState==startProperty)    // position: <name>
    {
    const XercesString ELEM_name("name");

    if(!XMLString::compareString(localname, ELEM_name))
      {
      parsingState=startPropertyName;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <name> Tag... %s\n", strLocalname);
      #endif

      // get XCL wide property id (mandatory) and alias of property (optional) and store them:
      const XercesString ATTR_propertyId("id");
      const XercesString ATTR_propAlias("alias");

      if(attrs.getValue(ATTR_propertyId))
        {
        const XercesString propertyIDValue(attrs.getValue(ATTR_propertyId));
        char* propertyid= XMLString::transcode(propertyIDValue);
        str1= std::string(propertyid);
        pIndex->pObjIndex->pPropIndex->propertyId= str1;
        XMLString::release(&propertyid);
        }
      if(attrs.getValue(ATTR_propAlias))
        {
        const XercesString propAlias(attrs.getValue(ATTR_propAlias));
        char* propalias= XMLString::transcode(propAlias);
        str1= std::string(propalias);
        pIndex->pObjIndex->pPropIndex->propertyAlias= str1;
        XMLString::release(&propalias);
        }
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startProperty\'. Expected tag <name>.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endPropertyName || parsingState==endPropertyValueSet)
    {
    const XercesString ELEM_valueSet("valueSet");   // valueSet tag MUST follow after property name;
                                                    // position: <valueSet>
    if(!XMLString::compareString(localname, ELEM_valueSet))
      {
      parsingState=startPropertyValueSet;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <valueSet> Tag... %s\n", strLocalname);
      #endif

      // allocate memory for object:
      pIndex->pObjIndex->pPropIndex->pValSetIndex= new ValueSetIndex();
      // allocate memory for valueSet container and valueSets:
      if(pIndex->pObjIndex->pPropIndex->valueSetIndexBox==NULL)
         pIndex->pObjIndex->pPropIndex->valueSetIndexBox= new std::vector <ValueSetIndex*> ();

      // get attribute: id and store it
      const XercesString ATTR_propertyValueSetID("id");
      if(attrs.getValue(ATTR_propertyValueSetID))
        {
        const XercesString valueSetIDValue(attrs.getValue(ATTR_propertyValueSetID));
        char* valuesetid= XMLString::transcode(valueSetIDValue);
        str1= std::string(valuesetid);
        pIndex->pObjIndex->pPropIndex->pValSetIndex->valueSetID= str1;
        XMLString::release(&valuesetid);
        }
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'endPropertyName\' or \'endPropertyValueSet\'. Expected tag <valueSet>.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertyValueSet ||
          parsingState==endPropertyValueSetRawVal ||
          parsingState==endPropertyValueSetLabVal ||
          parsingState==endPropertyValueSetObjRef ||
          parsingState==endPropertyValueSetDataRef
          )
    {
    const XercesString ELEM_rawVal("rawValue");
    const XercesString ELEM_labVal("labValue");
    const XercesString ELEM_objRef("objRef");
    const XercesString ELEM_dataRef("dataRef");

    if(!XMLString::compareString(localname, ELEM_rawVal))  // parsing position <rawValue>
      {
      parsingState=startPropertyValueSetRawVal;

      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <rawVal> Tag... %s\n", strLocalname);
      #endif
      // set flag for raw value tagset:
      pIndex->pObjIndex->pPropIndex->pValSetIndex->rawValueFlag= true;
      }

    else if(!XMLString::compareString(localname, ELEM_labVal)) // parsing position <labValue>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <labValue> Tag... %s\n", strLocalname);
      #endif

      parsingState=startPropertyValueSetLabVal;
      // set flag for labelled value tagset:
      pIndex->pObjIndex->pPropIndex->pValSetIndex->labValueFlag= true;
      }

    else if(!XMLString::compareString(localname, ELEM_objRef))  // parsing position <objRef>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <objRef> Tag... %s\n", strLocalname);
      #endif

      parsingState=startPropertyValueSetObjRef;
      }

    else if(!XMLString::compareString(localname, ELEM_dataRef))  // parsing position <datRef>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <dataRef> Tag... %s\n", strLocalname);
      #endif
      parsingState=startPropertyValueSetDataRef;

      // allocate memory for object:
      pIndex->pObjIndex->pPropIndex->pValSetIndex->pDataRefIndex= new DataRefIndex ();
      // allocate memory for dateRefIndexBox container  and dataRef object:
      if(pIndex->pObjIndex->pPropIndex->pValSetIndex->dataRefIndexBox==NULL)
         pIndex->pObjIndex->pPropIndex->pValSetIndex->dataRefIndexBox= new std::vector <DataRefIndex*> ();

      // allocate memory for dynamic index: list of properties with data references:
      if(pIndex->pObjIndex->pDynIndex==NULL) pIndex->pObjIndex->pDynIndex= new DynamicIndexes ();
      if(pIndex->pObjIndex->pDynIndex->PropsWithDataRef==NULL)
        pIndex->pObjIndex->pDynIndex->PropsWithDataRef= new std::map <std::string, bool> ();

      // for Planets PP5 usage, check if property is requested:
      bool available;
      if(pCoco!=NULL)
        {
        available= PP5_checkIfPropertyIsRequested(pCoco, pIndex->pObjIndex->pPropIndex->propertyName);
        }
      else available= false;
      // store it:
      pIndex->pObjIndex->pDynIndex->PropsWithDataRef->insert(make_pair(pIndex->pObjIndex->pPropIndex->propertyName, available));
      pIndex->pObjIndex->pDynIndex->dataRefFlag= true;
       // set flag which indicates that normData is reqired for comparison:
      if(pIndex->pObjIndex->pDynIndex->normDataRequiredFlag== false)
                      pIndex->pObjIndex->pDynIndex->normDataRequiredFlag= true;

      //get attributes ind and propertySetId and store them:
      const XercesString ATTR_propertyDataRefInd("ind");  // mandatory attribute
      const XercesString ATTR_propertyDataRefPropSetId("propertySetId");  // optional!

      if(attrs.getValue(ATTR_propertyDataRefInd))
        {
        const XercesString dataRefIndValue(attrs.getValue(ATTR_propertyDataRefInd));
        char* datarefind= XMLString::transcode(dataRefIndValue);
        str1= std::string(datarefind);
        pIndex->pObjIndex->pPropIndex->pValSetIndex->pDataRefIndex->ind= str1;
        XMLString::release(&datarefind);
        }
      if(attrs.getValue(ATTR_propertyDataRefPropSetId))
        {
        const XercesString dataRefPropSetId(attrs.getValue(ATTR_propertyDataRefPropSetId));
        char* datarefpropsetid= XMLString::transcode(dataRefPropSetId);
        str1= std::string(datarefpropsetid);
        pIndex->pObjIndex->pPropIndex->pValSetIndex->pDataRefIndex->propSetId= str1;
        XMLString::release(&datarefpropsetid);
        }
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertyValueSet\' or \'endPropertyValueSet[*]\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertyValueSetLabVal  ||
          parsingState==endPropertyValueSetLabValVal ||
          parsingState==endPropertyValueSetLabValType)
    {
    const XercesString ELEM_val("val");      // max=unbounded!    (??)
    const XercesString ELEM_type("type");    // max=unbounded!    (??)

    if(!XMLString::compareString(localname, ELEM_val))   // parsing position <val>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <val> Tag... %s\n", strLocalname);
      #endif

      parsingState= startPropertyValueSetLabValVal;
      }
    else if(!XMLString::compareString(localname, ELEM_type))   // parsing position <type>
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("found <type> Tag... %s\n", strLocalname);
      #endif

      parsingState= startPropertyValueSetLabValType;
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertyValueSetLabVal\' or \'endPropertyValueSetLabValVal\' or \'endPropertyValueSetLabValType\'. Expected tag <val> or <type>.");
      parsingState= erroneous;
      // todo validation
      }
    }
  else
    {
    #ifdef DEBUG_XCDLINDEXGENERATOR
    //pErr->registrateError("Warning: XCDLIndexGenerator: Uncontrolled parsing state.");
    //system("PAUSE");
    #endif
    }

XMLString::release(&strLocalname);
}

///////////////////////////////////////////////
///////    CHARACTERS  ////////////////////////
///////////////////////////////////////////////

void XCDLIndexGenerator::characters(
  const XMLCh* const chars,
  const unsigned int length)
{
  // ??  TODO: store length of chars in data and normData tagsets  ??
  // do not process for cases:
  if(parsingState==erroneous                   ||
     parsingState==startNormData               ||
     parsingState==startXCDL                   ||
     parsingState==startObject                 ||
     parsingState==startData                   ||
     parsingState==startProperty               ||
     parsingState==startPropertySet            ||
     parsingState==startPropertyValueSet       ||
     parsingState==startPropertyValueSetLabVal ||
     parsingState==startPropertyValueSetRawVal ||
     parsingState==startPropertyValueSetDataRef
     ) return;

  char* strChars = XMLString::transcode(chars);
  std::string tmp;

  #ifdef DEBUG_XCDLINDEXGENERATOR
  printf("chars [ %-0.*s ] \n", length, strChars);
  #endif

  if(parsingState==startPropertyName)
    {
    tmp=std::string(strChars);
    pIndex->pObjIndex->pPropIndex->propertyName= tmp;
    }
  else if(parsingState==startPropertyValueSetLabValVal)
    {
    tmp=std::string(strChars);
    // store labValue value:
    pIndex->pObjIndex->pPropIndex->pValSetIndex->labValVal= tmp;
    }
  else if(parsingState==startPropertyValueSetLabValType)
    {
    tmp=std::string(strChars);
    // store labValue value:
    pIndex->pObjIndex->pPropIndex->pValSetIndex->labValType= tmp;
    }
  else if(parsingState==startPropertyValueSetObjRef)
    {
    tmp=std::string(strChars);
    // store object reference:
    pIndex->pObjIndex->pPropIndex->pValSetIndex->objectRef= tmp;
    }
  else if(parsingState==startData)
    {
    /*    TODO:  raw data parsing not supported yet (version 0.3)    */
    }

  else
    {
    /*validation : uncontrolled parsing state
    pErr->registrateError("Warning: XCDLIndexGenerator: Uncontrolled parsing state in characters().");
    #ifdef DEBUG_XCDLINDEXGENERATOR
    printf("Warning: XCDLIndexGenerator: Uncontrolled parsing state in characters().\n");
    //system("PAUSE");
    #endif*/
    }

XMLString::release(&strChars);
}


//////////////////////////////////////////
//////     END ELEMENT XCDL      /////////
//////////////////////////////////////////

void XCDLIndexGenerator::endElement(const XMLCh* const uri,
                       const XMLCh* const localname,
                       const XMLCh* const qname)
{
  if(parsingState==erroneous) return;
  char *strLocalname = XMLString::transcode(localname);

  #ifdef DEBUG_XCDLINDEXGENERATOR
  printf("...end element %s.\n", strLocalname);
  #endif

  if(parsingState==startXCDL)
    {
    // validation:  empty XCDL tagset not allowed
    pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startXCDL\'. Expected <object> tag to follow.");
    parsingState=erroneous;
    }
  if(parsingState==startObject)
    {
    // validation:  empty object tagset not allowed
    pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startObject\'. Expected >data> or <normData> or <property> or propertySet> tag to follow.");
    parsingState=erroneous;
    }

  else if(parsingState==startData)
    {
    const XercesString ELEM_dataEnd("data");      // reading Position:  </data>
    if(!XMLString::compareString(localname, ELEM_dataEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("... found closing </data> tag...\n");
      #endif
      parsingState= endData;
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startData\' at closing element. Expected tag </data>.");
      parsingState= erroneous;
      // validation error
      }
    }
  else if(parsingState==startNormData)
    {
    const XercesString ELEM_normDataEnd("normData");      // reading Position:  </normData>
    if(!XMLString::compareString(localname, ELEM_normDataEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
       printf("... found closing </normData> tag...\n");
      #endif
      parsingState= endNormData;
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startNormData\' at closing element. Expected tag </normData>.");
      parsingState= erroneous;
      //validation error
      }
    }

  else if(parsingState==startProperty)        // parsing position error, name element must follow
    {
    // TODO validation
    pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startProperty\'. Expected <name> tag to follow.");
    parsingState=erroneous;
    }

  else if(parsingState==startPropertySet)        // parsing position error, valueSetRelations element must follow
    {
    // TODO validation
    pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertySet\'. Expected <valueSetRelations> tag to follow.");
    parsingState=erroneous;
    }

  else if(parsingState==startPropertyName)
    {
    const XercesString ELEM_nameEnd("name");      // reading Position:  </name>
    if(!XMLString::compareString(localname, ELEM_nameEnd))
      {
      parsingState= endPropertyName;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...end of name tag...\n");
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertyName\' at closing element. Expected tag </name>.");
      parsingState= erroneous;
      // TODO validation
      }
    }

  else if(parsingState==endPropertyName)        // parsing position error, valueSet element must follow
    {
    // TODO validation
    pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'endPropertyName\'. Expected <valueSet> tag to follow.");
    parsingState=erroneous;
    }


  else if(parsingState==startPropertyValueSetRawVal)
    {
    const XercesString ELEM_rawValEnd("rawValue");      // reading Position:  </rawValue>
    if(!XMLString::compareString(localname, ELEM_rawValEnd))
      {
      parsingState=endPropertyValueSetRawVal;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...end of <rawValue> tag...\n");
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertyValueSetRawVal\' at closing element. Expected tag </rawValue>.");
      parsingState= erroneous;
      // TODO validation
      }
    }

  else if(parsingState==startPropertyValueSetLabValVal)
    {
    const XercesString ELEM_labValValEnd("val");      // reading Position:  </val>
    if(!XMLString::compareString(localname, ELEM_labValValEnd))
      {
      parsingState=endPropertyValueSetLabValVal;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...end of <val> tag...\n");
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertyValueSetLabValVal\' at closing element. Expected tag </val>.");
      parsingState= erroneous;
      // TODO validation
      }
    }

  else if(parsingState==startPropertyValueSetLabValType)
    {
    const XercesString ELEM_labValTypeEnd("type");      // reading Position:  </type>
    if(!XMLString::compareString(localname, ELEM_labValTypeEnd))
      {
      parsingState=endPropertyValueSetLabValType;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...end of <type> tag...\n");
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'startPropertyValueSetLabValType\' at closing element. Expected tag </type>.");
      parsingState= erroneous;
      // TODO validation
      }
    }

  else if(parsingState==endPropertyValueSetLabValType) // reading position </labValue>
    {
    parsingState=endPropertyValueSetLabVal;
    #ifdef DEBUG_XCDLINDEXGENERATOR
    printf("...found closing </labValue> tag...\n");
    #endif
    }

  else if(parsingState==startPropertyValueSetDataRef)          // reading Position:  </dataRef>
    {
    const XercesString ELEM_propValSetDataRefEnd("dataRef");
    if(!XMLString::compareString(localname, ELEM_propValSetDataRefEnd))
      {
      parsingState=endPropertyValueSetDataRef;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </dataRef> tag...\n");
      #endif

      // throw DataRefIndex object into dataRefIndexBox:
      pIndex->pObjIndex->pPropIndex->pValSetIndex->dataRefIndexBox->push_back(pIndex->pObjIndex->pPropIndex->pValSetIndex->pDataRefIndex);
      }
    }

  else if(parsingState==endPropertyValueSetDataRef ||      // reading Position:  </valueSet>
          parsingState==endPropertyValueSetObjRef  ||
          parsingState==endPropertyValueSetLabVal  ||
          parsingState==endPropertyValueSetRawVal  ||
          parsingState==startPropertyValueSet)
    {
    const XercesString ELEM_propValSetEnd("valueSet");
    if(!XMLString::compareString(localname, ELEM_propValSetEnd))
      {
      parsingState=endPropertyValueSet;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </valueSet> tag...\n");
      #endif

      // throw ValSetIndex object into ValueSetIndexBox:
      pIndex->pObjIndex->pPropIndex->valueSetIndexBox->push_back(pIndex->pObjIndex->pPropIndex->pValSetIndex);
      }
    }

  else if(parsingState==endPropertyValueSet) // reading position: </property>
    {
    const XercesString ELEM_propertyEnd("property");
    if(!XMLString::compareString(localname, ELEM_propertyEnd))
      {
      parsingState=endProperty;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </property> tag...\n");
      #endif
      // check if property has data reference, if not store it in acc. dynamic index:
      if(pIndex->pObjIndex->pDynIndex->dataRefFlag==false)
        {
        if(pIndex->pObjIndex->pDynIndex->PropsWithoutDataRef==NULL) pIndex->pObjIndex->pDynIndex->PropsWithoutDataRef= new std::map <std::string, bool> ();

        // for Planets PP5 usage, check if Property is requested:
        bool available;
        if(pCoco!=NULL)
          {
          available= PP5_checkIfPropertyIsRequested(pCoco, pIndex->pObjIndex->pPropIndex->propertyName);
          }
        else available= false;
        // store it:
        pIndex->pObjIndex->pDynIndex->PropsWithoutDataRef->insert(make_pair(pIndex->pObjIndex->pPropIndex->propertyName, available));
        }
      else pIndex->pObjIndex->pDynIndex->dataRefFlag=false;     // reset dataRef flag

      // throw propertyIndex object into propertyIndexBox:
      pIndex->pObjIndex->propertyIndexBox->insert(make_pair(pIndex->pObjIndex->pPropIndex->propertyName, pIndex->pObjIndex->pPropIndex));
      }
    }

  else if(parsingState==startValueSetRelationsRef)    // parsing position </ref>
    {
    const XercesString ELEM_valueSetRelationsRefEnd("ref");
    if(!XMLString::compareString(localname, ELEM_valueSetRelationsRefEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </ref> tag...\n");
      #endif

      parsingState=endValueSetRelationsRef;
      }
    }

  else if(parsingState==endValueSetRelationsRef)       // parsing position </valueSetRelations>
    {
    const XercesString ELEM_valueSetRelationsEnd("valueSetRelations");
    if(!XMLString::compareString(localname, ELEM_valueSetRelationsEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </valueSetRelations> tag...\n");
      #endif

      parsingState=endValueSetRelations;
      }
    }

  else if(parsingState==startPropertySetDataRefRef)       // parsing position </ref>, in tagset: <propertySet>
    {
    const XercesString ELEM_propertySetDataRefRefEnd("ref");
    if(!XMLString::compareString(localname, ELEM_propertySetDataRefRefEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </ref> tag (case: propertySet, dataRef)...\n");
      #endif

      parsingState=endPropertySetDataRefRef;
      }
    }

  else if(parsingState==endPropertySetDataRefRef)       // parsing position </dataRef>, in tagset: <propertySet>
    {
    const XercesString ELEM_propertySetDataRefEnd("dataRef");
    if(!XMLString::compareString(localname, ELEM_propertySetDataRefEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </dataRef> tag (case: propertySet)...\n");
      #endif

      parsingState=endPropertySetDataRef;
      }
    }
  else if(parsingState==endValueSetRelations || parsingState==endPropertySetDataRef)   // first case: there's no datRef element; parsing position </propertySet>
    {
    const XercesString ELEM_propertySetEnd("propertySet");
    if(!XMLString::compareString(localname, ELEM_propertySetEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </propertySet> tag...\n");
      #endif

      // throw propertySetIndex object into propertySetIndexBox:
      pIndex->pObjIndex->propertySetIndexBox->insert(make_pair(pIndex->pObjIndex->pPropSetIndex->propertySetId, pIndex->pObjIndex->pPropSetIndex));

      parsingState=endPropertySet;
      }
    }

  else if(parsingState==startPropertyValueSetObjRef)
    {
    const XercesString ELEM_objRefEnd("objRef");
    if(!XMLString::compareString(localname, ELEM_objRefEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </objRef> tag...\n");
      #endif

      parsingState=endPropertyValueSetObjRef;
      }
    }

  else if(parsingState==endData ||              // parsing position </object>
          parsingState==endNormData ||
          parsingState==endProperty ||
          parsingState==endPropertySet)
    {
    const XercesString ELEM_objectEnd("object");
    if(!XMLString::compareString(localname, ELEM_objectEnd))
      {
      parsingState=endObject;
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </object> tag...\n");
      #endif
      // throw objectIndex object into objectIndexBox:
      pIndex->objectIndexBox->insert(make_pair(pIndex->pObjIndex->objectId, pIndex->pObjIndex));
      }
    else
      {
      // TODO validation
      pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'endData\' or \'endNormData\' or \'endProperty\' or \'endPropertySet\'. Expected </object> tag to follow.");
      parsingState=erroneous;
      }
    }

  else if(parsingState==endObject)
    {
    const XercesString ELEM_XCDLEnd("xcdl");
    if(!XMLString::compareString(localname, ELEM_XCDLEnd))
      {
      #ifdef DEBUG_XCDLINDEXGENERATOR
      printf("...found closing </xcdl> tag...\n");
      #endif
      parsingState=endXCDL;
      }
    else
      {
      // TODO validation
      pErr->registrateError("Validation Error: XCDLIndexGenerator: Unexpected tag in parsing state \'endObject\'. Expected </xcdl> tag to follow.");
      parsingState=erroneous;
      }
    }
/*******     EXCEPTIONS      ***********************************************/
 else if(parsingState==endPropertyName)        // parsing position </property>; error
    {
    // TODO validation
    pErr->registrateError("Error: XCDLIndexGenerator: Unexpected tag in parsing state \'endPropertyName\'. Expected <valueSet> tag to follow.");
    parsingState=erroneous;
    }

 XMLString::release(&strLocalname);
}


/////////////////////////////////////////////////////////////////////////////
// Basic implementations of Sax Handler ( inherited from DefaultHandler):////
/////////////////////////////////////////////////////////////////////////////
void XCDLIndexGenerator::endDocument()
{
  #ifdef DEBUG_XCDLINDEXGENERATOR
  printf("end document...\n");
  #endif

  if(parsingState==endXCDL)
    {
    #ifdef DEBUG_XCDLINDEXGENERATOR
    printf("XCDL index generated...\n");
    system("PAUSE");
    #endif
    }
  else if(parsingState==erroneous)
    {
    #ifdef DEBUG_XCDLINDEXGENERATOR
    std::cout<<"XCDL file is not valid."<<std::endl;
    #endif
    }
  else
    {
    #ifdef DEBUG_XCDLINDEXGENERATOR
    std::cout<<"Warning: Wrong parsing state: "<<parsingState<<"."<<std::endl;
    system("PAUSE");
    #endif
    }
}

bool XCDLIndexGenerator :: PP5_checkIfPropertyIsRequested(ComparisonConfiguration *pCoco, std::string propertyName)
{
std::list <MetricSet*> :: iterator i;

for(i= pCoco->pCompSet->sets->begin();i!= pCoco->pCompSet->sets->end(); ++i)
  {
  if(!((*i)->propertyName).compare(propertyName))
    {
    return true;
    }
  }
return false;
}
