/*****************************************************************************
XCDLHandler.cpp

Description: Sax Handler class methods
*****************************************************************************/
#include <string>
#include <map>
#include <list>
#include <xercesc/sax2/Attributes.hpp>
#include "defines.h"
#include "XercesString.h"
#include "ComparatorError.h"
#include "XCDLHandler.h"
#include "Comparer.h"
#include "Request2InHandler.h"
#include "Request2Input.h"
#include "SaxInterface.h"
#include "PropertyTagset.h"
#include "ComparerResults.h"
#include "ComparatorOutput.h"
#include "DataTagset.h"
#include "InputParams.h"


// standard constructor
XCDLHandler :: XCDLHandler()
{
  #ifdef DEBUG_XCDLHANDLER
  std::cout<<"XCDLHandler... Initializing std. object..."<<std::endl;
  #endif

  parser= 0;
  parsingState= other;
  pPropertyTagset1= NULL;
  pPropertyTagset2= NULL;
  pComparer=NULL;
  pCO= NULL;
  pErr= NULL;
  pNormData1= NULL;
  pNormData2= NULL;
pSI= NULL;
}

// constructor for XCDL 1;
XCDLHandler :: XCDLHandler(Request2 *pR2, ComparatorError *pCE, InputParams *pInPa) :
               parser(1), pPropertyTagset1(NULL), pPropertyTagset2(NULL),
               pNormData1(NULL), pNormData2(NULL), pCO(NULL), pSI(NULL)
{
  #ifdef DEBUG_XCDL1
  std::cout<<"XCDLHandler... Initializing SAX parser for XCDL1..."<<std::endl;
  #endif

  parsingState= parseXCDL1;
  pErr= pCE;
  pInputParams= pInPa;
  pComparer=new Comparer(pR2, pErr);
}

//  constructor for XCDL 2;
XCDLHandler :: XCDLHandler(PropertyTagset *pPropTag, NormData *pNData, Comparer *pComp, ComparatorError *pError) :
               parser(2), pPropertyTagset2(NULL), pCO(NULL), pNormData2(NULL), pSI(NULL)
{
  #ifdef DEBUG_XCDL2
  std::cout<<"XCDLHandler... Initializing parser 2 object..."<<std::endl;
  #endif

  parsingState= parseXCDL2;
  pPropertyTagset1= pPropTag;
  pComparer=pComp;
  pErr= pError;
  pNormData1= pNData;
}

XCDLHandler :: ~XCDLHandler()
{
if(parser==2) cleanUpXCDLHandler2();
else if(parser==1) cleanUpXCDLHandler1();
else cleanUpAll();

if(pErr!=NULL) pErr= NULL;
if(pInputParams!=NULL) pInputParams= NULL;
#ifdef DEBUG_RELEASE
//std::cout<<"Released XCDLHandler obj ...\n"<<std::endl;
//system("PAUSE");
#endif
}

void XCDLHandler:: cleanUpXCDLHandler2()
{
if(pNormData2!=NULL) {delete pNormData2; pNormData2= NULL;}
if(pPropertyTagset2!=NULL) {delete pPropertyTagset2; pPropertyTagset2= NULL;}
if(pSI!=NULL) {delete pSI; pSI= NULL;}
if(pPropertyTagset1!=NULL) pPropertyTagset1= NULL;
if(pNormData1!=NULL) pNormData1= NULL;
if(pComparer!=NULL) pComparer= NULL;
#ifdef DEBUG_RELEASE
std::cout<<"Released XCDLHandler(2) obj ...\n"<<std::endl;
#endif
}

void XCDLHandler :: cleanUpXCDLHandler1()
{
if(pNormData1!=NULL) {delete pNormData1; pNormData1= NULL;}
if(pPropertyTagset1!=NULL) {delete pPropertyTagset1; pPropertyTagset1= NULL;}
if(pSI!=NULL) {delete pSI; pSI= NULL;}
if(pPropertyTagset2!=NULL) pPropertyTagset2= NULL;
if(pNormData2!=NULL) pNormData2= NULL;
if(pComparer!=NULL) {delete pComparer; pComparer= NULL;}
#ifdef DEBUG_RELEASE
std::cout<<"Released XCDLHandler(1) obj ...\n"<<std::endl;
#endif
}


void XCDLHandler :: cleanUpAll()
{
if(pPropertyTagset1!=NULL) {delete pPropertyTagset1; pPropertyTagset1= NULL;}
if(pPropertyTagset2!=NULL) {delete pPropertyTagset2; pPropertyTagset2= NULL;}
if(pNormData1!=NULL) {delete pNormData1; pNormData1= NULL;}
if(pNormData2!=NULL) {delete pNormData2; pNormData2= NULL;}
if(pComparer!=NULL) {delete pComparer; pComparer= NULL;}
if(pSI!=NULL) {delete pSI; pSI= NULL;}
#ifdef DEBUG_RELEASE
std::cout<<"Released XCDLHandler(All) obj ...\n"<<std::endl;
#endif
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
///      START ELEMENT XCDL 1     //////////////
////////////////////////////////////////////////
void XCDLHandler :: startElementXCDL1(const XMLCh* const localname, const Attributes& attrs)
{
if(parsingState==parseNextProperty || parsingState==erroneous || parsingState==other) return;

std::string str1;
bool isOrNot;
char *strLocalname = XMLString::transcode(localname);


  #ifdef DEBUG_XCDLHANDLER
  printf("starting Element... %s\n", strLocalname);
  #endif


  if(parsingState==parseXCDL1)
    {
    parsingState=startXCDL1;

    const XercesString ELEM_xcdl("xcdl");
    if(!XMLString::compareString(localname, ELEM_xcdl))
      {
      #ifdef DEBUG_XCDL1
      printf("found <xcdl> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'parseXCDL1\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startXCDL1)  // successors: composition, object
    {
    const XercesString ELEM_composition("composition");
    const XercesString ELEM_object("object");

    if(!XMLString::compareString(localname, ELEM_composition))
      {
      parsingState=startComposition;

      #ifdef DEBUG_XCDL1
      printf("found composition Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_object))
      {
      parsingState=startObject;

      #ifdef DEBUG_XCDL1
      printf("found object Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startXCDL1\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startObject)
    {
    const XercesString ELEM_data("data");
    const XercesString ELEM_normData("normData");
    const XercesString ELEM_property("property");

    if(!XMLString::compareString(localname, ELEM_data))  // not yet supported!!
      {
      parsingState=startData;

      #ifdef DEBUG_XCDL1
      printf("found <data> Tag... %s\n", strLocalname);
      #endif
      }

    else if(!XMLString::compareString(localname, ELEM_normData))
      {
      #ifdef DEBUG_XCDL1
      printf("found <normData> Tag... %s\n", strLocalname);
      #endif

      std::string tmp= std::string(strLocalname);
      //if(pInputParams->applTarget==1) {
      isOrNot= PP5_checkIfPropertyIsRequested(pComparer, tmp);
      if(isOrNot== true)
        {
        parsingState=startNormData;   //succeeding: read chars
//      %%%  new objects pNormData1, pPropertyTagset1                %%%
        if(pNormData1==NULL) pNormData1= new NormData();
        if(pPropertyTagset1==NULL) pPropertyTagset1= new PropertyTagset();
        pPropertyTagset1->propName=tmp;

        // get type and id of normData; both attributes are mandatory!!
        // get attribute: type
        const XercesString normDataType("type");
        const XercesString normDataTypeValue(attrs.getValue(normDataType));
        char* normdatatype= XMLString::transcode(normDataTypeValue);
        str1= std::string(normdatatype);
        pNormData1->normDataType=str1;
        XMLString::release(&normdatatype);
        str1="\0";

        // get attribute: id
        const XercesString ATTR_normDataID("id");
        if(attrs.getValue(ATTR_normDataID))
          {
          const XercesString normDataIDValue(attrs.getValue(ATTR_normDataID));
          char* normdataid= XMLString::transcode(normDataIDValue);
          str1= std::string(normdataid);
          pNormData1->key=str1;
          XMLString::release(&normdataid);
          }

///  !!!!    only supported:     int normData comparison     !!!!!!
        if(pNormData1->normDataTagsetsUint==NULL)
          {
          pNormData1->normDataTagsetsUint= new std::map <std::string, std::vector <int>* > ();
          }
        }
      else
        {
        parsingState= parseNextProperty;
        #ifdef DEBUG_XCDL1
        std::cout<<"Norm Data comparison is not requested."<<std::endl;
        #endif
        }
      }

    else if(!XMLString::compareString(localname, ELEM_property))
      {
      if(pPropertyTagset1==NULL) pPropertyTagset1= new PropertyTagset();
      parsingState=startProperty;

      #ifdef DEBUG_XCDL1
      printf("found <property> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startObject\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endNormData)  // previous read norm data tag, new one follows OR property tag follows
    {                                 // !!!! TODO:  only supported : one single normData Tagset !!!!!!!!!!
    const XercesString ELEM_temp01("normData");
    const XercesString ELEM_temp02("property");

    if(!XMLString::compareString(localname, ELEM_temp01))
      {
      parsingState=startNormData;
      pErr->registrateError("Error: XCDLHandler: Multiple normData Tags not supported in this version.");
      parsingState= erroneous;
      #ifdef DEBUG_XCDL1
      printf("found another <normData> Tag... %s\n", strLocalname);
      #endif
      }

    else if(!XMLString::compareString(localname, ELEM_temp02))
      {
      // after comparing the normData, the newly found property is processed:
      if(pPropertyTagset1==NULL) {pPropertyTagset1= new PropertyTagset();}
      parsingState=startProperty;

      #ifdef DEBUG_XCDL1
      printf("found <property> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endNormData\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startProperty)
    {
    parsingState=startPropertyName;

    const XercesString ELEM_name("name");
    if(!XMLString::compareString(localname, ELEM_name))
      {
      #ifdef DEBUG_XCDL1
      printf("found <name> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startProperty\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endPropertyName || parsingState==endPropertyValueSet)
    {
    const XercesString ELEM_valueSet("valueSet");   // valueSet tag MUST follow after property name

    if(!XMLString::compareString(localname, ELEM_valueSet))
      {
      parsingState=startPropertyValueSet;
// %%%   new object pValueSet  (class ValueSet)         %%%
      if(pPropertyTagset1->pValueSet==NULL)
        {
        pPropertyTagset1->pValueSet= new ValueSet();
        }
      // get attribute: id
      const XercesString ATTR_valueSetID("id");
      if(attrs.getValue(ATTR_valueSetID))
        {
        const XercesString valueSetIDValue(attrs.getValue(ATTR_valueSetID));
        char* valuesetid= XMLString::transcode(valueSetIDValue);
        str1= std::string(valuesetid);
        pPropertyTagset1->pValueSet->valueSetID= str1;
        XMLString::release(&valuesetid);
        }
      else{
      #ifdef DEBUG_XCDL1
      printf("found <valueSet> Tag... %s\n", strLocalname);  system("PAUSE");
      #endif
           }
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endPropertyName\' or \'endPropertyValueSet\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertyValueSet ||
          parsingState==endPropertyValueSetRawVal ||
          parsingState==endPropertyValueSetLabVal ||
          parsingState==endPropertyValueSetDataRef ||
          parsingState==endPropertyValueSetPropRel ||
          parsingState==endPropertyValueSetRef
          )
    {
    const XercesString ELEM_rawVal("rawVal");
    const XercesString ELEM_labVal("labValue");
    const XercesString ELEM_dataRef("dataRef");
    const XercesString ELEM_propRel("propRel");
    const XercesString ELEM_ref("ref");

    if(!XMLString::compareString(localname, ELEM_rawVal))
      {
      parsingState=startPropertyValueSetRawVal;

      #ifdef DEBUG_XCDL1
      printf("found <rawVal> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_labVal))
      {
      parsingState=startPropertyValueSetLabVal;
      pPropertyTagset1->pValueSet->labVal=true;
      #ifdef DEBUG_XCDL1
      printf("found <labValue> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_dataRef))
      {
      parsingState=startPropertyValueSetDataRef;

      #ifdef DEBUG_XCDL1
      printf("found <dataRef> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_propRel))
      {
      parsingState=startPropertyValueSetPropRel;

      #ifdef DEBUG_XCDL1
      printf("found <propRel> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_ref))
      {
      parsingState=startPropertyValueSetRef;

      #ifdef DEBUG_TMP
      printf("found <ref> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startPropertyValueSet\' or \'endPropertyValueSet\'.(1)");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertyValueSetLabVal)
    {
    parsingState= startPropertyValueSetLabValVal;


    const XercesString ELEM_val("val");
    if(!XMLString::compareString(localname, ELEM_val))
      {
      #ifdef DEBUG_XCDL1
      printf("found <val> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startPropertyValueSetLabVal\'.(2)");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endPropertyValueSetLabValVal)
    {
    parsingState=startPropertyValueSetLabValType;


    const XercesString ELEM_type("type");
    if(!XMLString::compareString(localname, ELEM_type))
      {
      #ifdef DEBUG_XCDL1
      printf("found <type> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endPropertyValueSetLabValVal\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endProperty)
    {
    parsingState=startProperty;
    // allocate new memory:
    if(pPropertyTagset1==NULL) pPropertyTagset1= new PropertyTagset();


    const XercesString ELEM_tmpProp("property");
    const XercesString ELEM_PropertySet("propertySet");
    if(!XMLString::compareString(localname, ELEM_tmpProp))
      {
      #ifdef DEBUG_XCDL1
      printf("found next <property> Tag in XCDL 1... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_PropertySet))
      {
      #ifdef DEBUG_XCDL1
      printf("found  <propertySet> Tag in XCDL 1... %s\n", strLocalname);
      #endif
      parsingState= other;
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endProperty\'.");
      parsingState= erroneous;
      }
    }
  else if(parsingState==endPropertySet)
    {
    const XercesString ELEM_nextPropertySet("propertySet");
    if(!XMLString::compareString(localname, ELEM_nextPropertySet))
      {
      parsingState=other;
      #ifdef DEBUG_XCDL1
      printf("found another <property> Tag in XCDL 1... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endPropertySet\'.");
      parsingState= erroneous;
      }
    }
  else if(parsingState==endObject)
    {
    parsingState=startObject;

    #ifdef SAFE_MODE
    const XercesString ELEM_tmpObj("object");
    if(!XMLString::compareString(localname, ELEM_tmpObj))
      {
      printf("found another <object> Tag... %s\n", strLocalname);
      printf("Exception: multiple objects not implemented yet\n program will terminate now\n");
      //system("PAUSE");
      exit(0);
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endObject\'.");
      parsingState= erroneous;
      }
    #endif
    }
  else
    {
    #ifdef DEBUG_XCDL1
    pErr->registrateError("Warning: XCDLHandler: Parsing state not handled.");
    #endif
    }

XMLString::release(&strLocalname);
}

///////////////////////////////////////////////
///////    CHARACTERS XCDL1     ///////////////
///////////////////////////////////////////////

void XCDLHandler::charactersXCDL1(
  const XMLCh* const chars,
  const unsigned int length)
{
  if(parsingState==parseNextProperty || parsingState==erroneous || parsingState==other) return;

  char* strChars = XMLString::transcode(chars);
  std::string tmp;
  bool isOrNot;

/*
  tmp= std::string(strChars);
  std::cout << "max_size: " << tmp.max_size() << "\n";
   std::vector<int> v1(length);
   std::cout << "The capacity of v1 is " << v1.capacity() <<std::endl; system("PAUSE");
*/


  #ifdef DEBUG_XCDL1
  printf("chars [ %-0.*s ] \n", length, strChars);
  std::cout<<"Parsing State:"<<parsingState<<std::endl;
  #endif

  // %%%    create new objects:  normDataTagsetsUint, normDataAsUint (both class DataTagset)   %%%
  if(parsingState==startNormData)
    {
    pNormData1->storeNormDataAsIs(strChars);
    pNormData1->normDataAsUint= new std::vector <int> ();
    pNormData1->loadNormDataAsUint(pNormData1, strChars);
    }

  else if(parsingState==startPropertyName)
    {
    tmp=std::string(strChars);

    // specific for applTarget==1 (PLANETS PP5):
    // check if property is requested:
    isOrNot= PP5_checkIfPropertyIsRequested(pComparer, tmp);
    if(isOrNot== true)
      {
      pPropertyTagset1->propName=tmp;
      XMLString::release(&strChars);
      return;
      }
    // else set new parsingState
    else
      {
      parsingState= parseNextProperty;
      }

    #ifdef DEBUG_XCDL1
    std::cout<<"Property "<<tmp<<" is not requested."<<std::endl;
    #endif
    }

  else if(parsingState==startPropertyValueSetRawVal)
    {
    tmp=std::string(strChars);
    pPropertyTagset1->pValueSet->rawVal=tmp;
    }

  else if(parsingState==startPropertyValueSetLabValVal)
    {
    tmp=std::string(strChars);
    pPropertyTagset1->pValueSet->labValVal=tmp;

    }

  else if(parsingState==startPropertyValueSetLabValType)
    {
    tmp=std::string(strChars);
    pPropertyTagset1->pValueSet->labValType=tmp;
    }

  else if(parsingState==startPropertyValueSetDataRef)
    {
    pPropertyTagset1->pValueSet->dataRef=true;
    }

  else if(parsingState==startPropertyValueSetPropRel)
    {
    pPropertyTagset1->pValueSet->propRel=true;
    }

  else {/* uncontrolled parsing state*/}

XMLString::release(&strChars);
}
//////////////////////////////////////////
//////     END ELEMENT XCDL 1    /////////
//////////////////////////////////////////

void XCDLHandler::endElementXCDL1(const XMLCh* const localname)
{
  int error=0;
  char *strLocalname = XMLString::transcode(localname);

  #ifdef DEBUG_XCDL1
  printf("...end element %s.\n", strLocalname);
  #endif

  if(parsingState==parseNextProperty)
    {
    const XercesString ELEM_tmpObj1("property");
    const XercesString ELEM_tmpObj2("normData");
    if(!XMLString::compareString(localname, ELEM_tmpObj1))  // reading position: </property>
      {
      parsingState=endProperty;
      #ifdef DEBUG_XCDL1
      printf("...end of unmatched property...\n");
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_tmpObj2)) // reading position </normData>
      {
      parsingState= endNormData;
      }
    else
      {
      XMLString::release(&strLocalname);
      return;
      }
    }

  else if(parsingState==startNormData)
    {
    //   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //    parse and load second XCDL, compare:
    //   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if(pSI==NULL) pSI= new SaxInterface(pErr);
      error= pSI->startSaxParser(pInputParams->targetXCDL, pPropertyTagset1, pNormData1, pComparer);

      if(error==-1) {parsingState=erroneous; return;}

      // release memory:
      if(pNormData1!=NULL) {delete pNormData1; pNormData1= NULL;}
      if(pPropertyTagset1!=NULL) {delete pPropertyTagset1; pPropertyTagset1= NULL;}
      //if(pSI!=NULL) {pSI->cleanUp();}
      #ifdef DEBUG_XCDL1
      std::cout<<"Continue to parsing XCDL 1 after norm data comparison..."<<std::endl;   system("PAUSE");
      #endif
    parsingState=endNormData;
    }

  else if(parsingState==startPropertyName)
    {
    parsingState=endPropertyName;
    }

  else if(parsingState==startPropertyValueSetRawVal)
    {
    parsingState=endPropertyValueSetRawVal;
    }

  else if(parsingState==startPropertyValueSetLabValVal)
    {
    parsingState=endPropertyValueSetLabValVal;
    }

  else if(parsingState==startPropertyValueSetLabValType)
    {
    parsingState=endPropertyValueSetLabValType;
    }

  else if(parsingState==endPropertyValueSetLabValType)
    {
    parsingState=endPropertyValueSetLabVal;
    }

  else if(parsingState==startPropertyValueSetDataRef)
    {
    parsingState=endPropertyValueSetDataRef;
    }

  else if(parsingState==startPropertyValueSetPropRel)
    {
    parsingState=endPropertyValueSetPropRel;
    }
  else if(parsingState==startPropertyValueSetRef)
    {
    parsingState=endPropertyValueSetRef;
    }
  else if(parsingState==endPropertyValueSetRef)  // case: </propRel>
    {
    parsingState=endPropertyValueSetPropRel;
    }
  else if(parsingState==startPropertyValueSet ||  /* case empty valueSet */
          parsingState==endPropertyValueSetRawVal ||
          parsingState==endPropertyValueSetLabVal ||
          parsingState==endPropertyValueSetDataRef ||
          parsingState==endPropertyValueSetPropRel)// reading position: </valueSet>
    {
    parsingState=endPropertyValueSet;

     if((pPropertyTagset1->valueSets)==NULL)
      {
      pPropertyTagset1->valueSets= new std::map <std::string, ValueSet*> ();
      }
    std::string str1= pPropertyTagset1->pValueSet->valueSetID;
    pPropertyTagset1->valueSets->insert(make_pair(str1, pPropertyTagset1->pValueSet));
    }

  else if(parsingState==endPropertyValueSet) // reading position: </property>
    {
    #ifdef DEBUG_XCDL1
    std::cout<<"Starting SAX Parser for XCDL 2...."<<std::endl;
    #endif
    //#ifdef DEBUG_REQUEST2
    //pComparer->pRequest2->printRequest2Input();
    //system("PAUSE");
    //#endif

    // load and parse target XCDL :
    if(pSI==NULL) pSI= new SaxInterface(pErr);
    error= pSI->startSaxParser(pInputParams->targetXCDL, pPropertyTagset1, pNormData1, pComparer);

    if(error==-1) {parsingState=erroneous; return;}

    // release all memory for stored property-values of XCDL1:
    if(pPropertyTagset1!=NULL) {delete pPropertyTagset1; pPropertyTagset1= NULL;}

    #ifdef DEBUG_XCDL1
    std::cout<<"Continue to parsing XCDL 1 ...."<<std::endl;
    system("PAUSE");
    #endif

    parsingState= endProperty;
    }
  else if(parsingState==other)
    {
    const XercesString ELEM_tmpPropertySet("propertySet");
    if(!XMLString::compareString(localname, ELEM_tmpPropertySet)) // reading position: </propertySet>
      {
      parsingState=endPropertySet;
      #ifdef DEBUG_XCDL1
      printf("...end of propertySet...\n");
      #endif
      }
    else
      {
      XMLString::release(&strLocalname);
      return;
      }
    }
  else if(parsingState==endProperty || parsingState==endPropertySet)  // currently reading: </object>
    {
    parsingState=endObject;
    }

  else if(parsingState==endObject)
    {
    parsingState=endXCDL1;
    }

 XMLString::release(&strLocalname);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
///      START ELEMENT XCDL 2    //////////////
////////////////////////////////////////////////
void XCDLHandler :: startElementXCDL2(const XMLCh* const localname, const Attributes& attrs)
{

if(parsingState==parseNextProperty || parsingState==erroneous || parsingState==other)   {/*std::cout<<"Return..."<<std::endl;*/ return;}

std::string str1;

char *strLocalname = XMLString::transcode(localname);

  #ifdef DEBUG_XCDLHANDLER
  printf("starting Element... %s\n", strLocalname);
  #endif

  if(parsingState==parseXCDL2)
    {
    parsingState=startXCDL2;

    #ifdef DEBUG_XCDL2
    std::map<std::string, ValueSet*> :: iterator iter;
    ValueSet* pValSet;
    std::cout<<"Values of property \""<<pPropertyTagset1->propName<<"\":"<<std::endl;
    if(pPropertyTagset1->valueSets!= NULL)
      {
      iter= pPropertyTagset1->valueSets->begin();
      pValSet= iter->second;
      pValSet->printValueSet();
      }
    else std::cout<<"none"<<std::endl;
    #endif

    const XercesString ELEM_xcdl("xcdl");
    if(!XMLString::compareString(localname, ELEM_xcdl))
      {
      #ifdef DEBUG_XCDL2
      printf("found <xcdl> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'parseXCDL2\'.");
      parsingState= erroneous;
      }

    }

  else if(parsingState==startXCDL2)  // successors: composition, object
    {
    const XercesString ELEM_composition("composition");
    const XercesString ELEM_object("object");

    if(!XMLString::compareString(localname, ELEM_composition))
      {
      parsingState=startComposition;

      #ifdef DEBUG_XCDL2
      printf("found composition Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_object))
      {
      parsingState=startObject;

      #ifdef DEBUG_XCDL2
      printf("found object Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startXCDL2\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startObject)
    {
    const XercesString ELEM_data("data");
    const XercesString ELEM_normData("normData");
    const XercesString ELEM_property("property");

    if(!XMLString::compareString(localname, ELEM_data))
      {
      parsingState=startData;

      #ifdef DEBUG_XCDL2
      printf("found <data> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_normData))
      {
      parsingState=startNormData;   //succeeding: read chars
      std::string tmp= std::string(strLocalname);
// %%%  new objects: pNormData2, pPropertyTagset2 (both class DataTagsets)   %%%
      if(pNormData2==NULL) pNormData2= new NormData();
      if(pPropertyTagset2==NULL) pPropertyTagset2= new PropertyTagset();
      pPropertyTagset2->propName=tmp;

      // get type and id of normData; both attributes are mandatory!!
      // get attribute: type
      const XercesString normData2Type("type");
      const XercesString normData2TypeValue(attrs.getValue(normData2Type));
      char* normdata2type= XMLString::transcode(normData2TypeValue);
      str1= std::string(normdata2type);
      pNormData2->normDataType=str1;
      XMLString::release(&normdata2type);
      str1="\0";

      // get attribute: id
      const XercesString normData2ID("id");
      const XercesString normData2IDValue(attrs.getValue(normData2ID));
      char* normdata2id= XMLString::transcode(normData2IDValue);
      str1= std::string(normdata2id);
      pNormData2->key=str1;
      XMLString::release(&normdata2id);

      #ifdef DEBUG_XCDL2
      printf("found <normData> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_property))
      {
      if(pPropertyTagset2==NULL) pPropertyTagset2= new PropertyTagset();
      parsingState=startProperty;

      #ifdef DEBUG_XCDL2
      printf("found <property> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startObject\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endNormData)  // previous read norm data tag, new one follows OR property tag follows
    {
    const XercesString ELEM_temp01("normData");
    const XercesString ELEM_temp02("property");

    if(!XMLString::compareString(localname, ELEM_temp01))
      {
      parsingState=startNormData;

      #ifdef DEBUG_XCDL2
      printf("found another <normData> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_temp02))
      {
      if(pPropertyTagset2==NULL) pPropertyTagset2= new PropertyTagset();
      parsingState=startProperty;

      #ifdef DEBUG_XCDL2
      printf("found <property> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endNormData\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startProperty)
    {
    parsingState=startPropertyName;

    const XercesString ELEM_name("name");
    if(!XMLString::compareString(localname, ELEM_name))
      {
      #ifdef DEBUG_XCDL2
      printf("found <name> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startProperty\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endPropertyName || parsingState==endPropertyValueSet)
    {
    const XercesString ELEM_valueSet("valueSet");   // valueSet tag MUST follow after property name

    if(!XMLString::compareString(localname, ELEM_valueSet))
      {
      parsingState=startPropertyValueSet;
      #ifdef DEBUG_XCDL2
      printf("found <valueSet> Tag... %s\n", strLocalname);
      #endif

      if(pPropertyTagset2->pValueSet==NULL) pPropertyTagset2->pValueSet= new ValueSet();

      // get attribute: id
      const XercesString valueSetID("id");
      const XercesString valueSetIDValue(attrs.getValue(valueSetID));
      char* id= XMLString::transcode(valueSetIDValue);
      str1= std::string(id);

      pPropertyTagset2->pValueSet->valueSetID=str1;

      XMLString::release(&id);
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endPropertyName\' or \'endPropertyValueSet\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertyValueSet ||
          parsingState==endPropertyValueSetRawVal ||
          parsingState==endPropertyValueSetLabVal ||
          parsingState==endPropertyValueSetDataRef ||
          parsingState==endPropertyValueSetPropRel ||
          parsingState==endPropertyValueSetRef
          )
    {
    const XercesString ELEM_rawVal("rawVal");
    const XercesString ELEM_labVal("labValue");
    const XercesString ELEM_dataRef("dataRef");
    const XercesString ELEM_propRel("propRel");
    const XercesString ELEM_ref("ref"); //   HACK!!

    if(!XMLString::compareString(localname, ELEM_rawVal))
      {
      parsingState=startPropertyValueSetRawVal;

      #ifdef DEBUG_TMP
      printf("found <rawVal> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_labVal))
      {
      parsingState=startPropertyValueSetLabVal;
      pPropertyTagset2->pValueSet->labVal=true;

      #ifdef DEBUG_TMP
      printf("found <labVal> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_dataRef))
      {
      parsingState=startPropertyValueSetDataRef;

      #ifdef DEBUG_TMP
      printf("found <dataRef> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_propRel))
      {
      parsingState=startPropertyValueSetPropRel;

      #ifdef DEBUG_TMP
      printf("found <propRel> Tag... %s\n", strLocalname);
      #endif
      }
    else if(!XMLString::compareString(localname, ELEM_ref))
      {
      parsingState=startPropertyValueSetRef;

      #ifdef DEBUG_TMP
      printf("found <ref> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startPropertyValueSet\' or \'endPropertyValueSet*\'.(3)");
      parsingState= erroneous;
      }
    }

  else if(parsingState==startPropertyValueSetLabVal)
    {
    parsingState= startPropertyValueSetLabValVal;

    const XercesString ELEM_val("val");
    if(!XMLString::compareString(localname, ELEM_val))
      {
      #ifdef DEBUG_XCDL2
      printf("found <val> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'startPropertyValueSetLabVal\'.(4)");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endPropertyValueSetLabValVal)
    {
    parsingState=startPropertyValueSetLabValType;

    const XercesString ELEM_type("type");
    if(!XMLString::compareString(localname, ELEM_type))
      {
      #ifdef DEBUG_XCDL2
      printf("found <type> Tag... %s\n", strLocalname);
      #endif
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endPropertyValueSetLabValVal\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endProperty || parsingState==endPropertySet)
    {
    parsingState=startProperty;

    const XercesString ELEM_tmpProp("property");
    const XercesString ELEM_PropertySet("propertySet");

    if(!XMLString::compareString(localname, ELEM_tmpProp))
      {
      #ifdef DEBUG_XCDL2
      printf("found next <property> Tagset in XCDL 2... %s\n", strLocalname);
      #endif
      }
     else if(!XMLString::compareString(localname, ELEM_PropertySet))
      {
      #ifdef DEBUG_XCDL2
      printf("found  <propertySet> Tag in XCDL 2... %s\n", strLocalname);
      #endif
      parsingState= other;
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endProperty\'.");
      parsingState= erroneous;
      }
    }

  else if(parsingState==endObject)
    {
    parsingState=startObject;

    const XercesString ELEM_tmpObj("object");
    if(!XMLString::compareString(localname, ELEM_tmpObj))
      {
      printf("found another <object> Tag... %s\n", strLocalname);
      printf("Exception: multiple objects not implemented yet\n program will terminate now\n");
      system("PAUSE");
      exit(0);
      }
    else
      {
      pErr->registrateError("Error: XCDLHandler: Unexpected tag in parsing state \'endObject\'(2).");
      parsingState= erroneous;
      }
    }
   else
    {
    #ifdef DEBUG_XCDL2
    pErr->registrateError("Warning: XCDLHandler: Parsing state not handled.");
    #endif
    }

XMLString::release(&strLocalname);
}

////////////////////////////////////////////////
///////    CHARACTERS XCDL 2      //////////////
///////////////////////////////////////////////

void XCDLHandler::charactersXCDL2(
  const XMLCh* const chars,
  const unsigned int length)
{
  if(parsingState==parseNextProperty || parsingState==erroneous || parsingState==other) return;

  char* strChars = XMLString::transcode(chars);
  std::string tmp;

  #ifdef DEBUG_XCDL2
  printf("chars [ %-0.*s ] \n", length, strChars);
  #endif


  if(parsingState==startNormData)
    {
    if(pPropertyTagset1->propName.compare("normData")==0)
      {
// %%%    create new objects:  normDataTagsetsUint, normDataAsUint (both class DataTagset)   %%%
      if(pNormData2->normDataTagsetsUint==NULL)
        {
        pNormData2->normDataTagsetsUint= new std::map <std::string, std::vector <int>* > ();
        }
      pNormData2->storeNormDataAsIs(strChars);
      pNormData2->normDataAsUint= new std::vector <int> ();
      pNormData2->loadNormDataAsUint(pNormData2, strChars);

      // this is where we compare the normData which we now have stored
      // in the containers:
      int ok= pComparer->compareNormData(pPropertyTagset1, pNormData1, pNormData2);
      if(ok<0)
        {
        pErr->registrateError("Error: XCDLHandler: Error while comparing normData.");
        parsingState= erroneous;
        }
      else parsingState= skipAll;
      }
    #ifdef DEBUG_XCDL2
    std::cout<<"Compared normData... "<<std::endl; system("PAUSE");
    #endif
    }

  else if(parsingState==startPropertyName)
    {
    tmp=std::string(strChars);
    //str2=pPropertyTagset1->propName;
    if(!tmp.compare(pPropertyTagset1->propName))
      {
      #ifdef DEBUG_XCDL2
      std::cout<<"Found matching property name: "<<tmp<<"."<<std::endl;
      #endif

      pPropertyTagset2->propName=tmp;
      }
    else parsingState= parseNextProperty;
    }

  else if(parsingState==startPropertyValueSetRawVal)
    {
    tmp=std::string(strChars);
    pPropertyTagset2->pValueSet->rawVal=tmp;
    }

  else if(parsingState==startPropertyValueSetLabValVal)
    {
    tmp=std::string(strChars);
    pPropertyTagset2->pValueSet->labValVal=tmp;
    }

  else if(parsingState==startPropertyValueSetLabValType)
    {
    tmp=std::string(strChars);
    pPropertyTagset2->pValueSet->labValType=tmp;
    }

  else if(parsingState==startPropertyValueSetDataRef)
    {
    pPropertyTagset2->pValueSet->dataRef=true;
    }

  else if(parsingState==startPropertyValueSetPropRel)
    {
    pPropertyTagset2->pValueSet->propRel=true;
    }
  else {/* uncontrolled parsing state*/}

XMLString::release(&strChars);
}

//////////////////////////////////////////
//////     END ELEMENT XCDL 2    /////////
//////////////////////////////////////////

void XCDLHandler::endElementXCDL2(const XMLCh* const localname)
{
  if(parsingState==erroneous) return;

  int ok=0;
  char *strLocalname = XMLString::transcode(localname);

  #ifdef DEBUG_XCDL2
  printf("...end element %s.\n", strLocalname);
  #endif

  if(parsingState==parseNextProperty)
    {
    const XercesString ELEM_tmpObj("property");
    if(!XMLString::compareString(localname, ELEM_tmpObj)) // reading position: </property>
      {
      parsingState=endProperty;
      #ifdef DEBUG_XCDL2
      printf("...end of unmatched property...\n");
      #endif
      }
    else
      {
      XMLString::release(&strLocalname);
      return;
      }
    }

  else if(parsingState==startNormData)
    {
    parsingState=endNormData;
    }

  else if(parsingState==startPropertyName)
    {
    parsingState=endPropertyName;
    }

  else if(parsingState==startPropertyValueSetRawVal)
    {
    parsingState=endPropertyValueSetRawVal;
    }

  else if(parsingState==startPropertyValueSetLabValVal)
    {
    parsingState=endPropertyValueSetLabValVal;
    }

  else if(parsingState==startPropertyValueSetLabValType)
    {
    parsingState=endPropertyValueSetLabValType;
    }

  else if(parsingState==endPropertyValueSetLabValType)
    {
    parsingState=endPropertyValueSetLabVal;
    }

  else if(parsingState==startPropertyValueSetDataRef)
    {
    parsingState=endPropertyValueSetDataRef;
    }

  else if(parsingState==startPropertyValueSetPropRel)
    {
    parsingState=endPropertyValueSetPropRel;
    }
  else if(parsingState==startPropertyValueSetRef)
    {
    parsingState=endPropertyValueSetRef;
    }
  else if(parsingState==endPropertyValueSetRef)  // case: </propRel>
    {
    parsingState=endPropertyValueSetPropRel;
    }
  else if(parsingState==startPropertyValueSet ||      /* case empty valueSet */
          parsingState==endPropertyValueSetRawVal ||
          parsingState==endPropertyValueSetLabVal ||
          parsingState==endPropertyValueSetDataRef ||
          parsingState==endPropertyValueSetPropRel
          ) // reading position: </valueSet>
    {
    parsingState=endPropertyValueSet;

    if((pPropertyTagset2->valueSets)==NULL)
     {
     pPropertyTagset2->valueSets= new std::map <std::string, ValueSet*> ();
     }
    std::string str1=  pPropertyTagset2->pValueSet->valueSetID;
    pPropertyTagset2->valueSets->insert(make_pair(str1, pPropertyTagset2->pValueSet));
    }

  else if(parsingState==endPropertyValueSet) // reading position: </property>
    {
    parsingState= endProperty;

    #ifdef DEBUG_XCDL2
    std::cout<<"\n Starting to compare property \""<<pPropertyTagset1->propName<<"\":"<<std::endl<<std::endl<<std::endl;

    std::cout<<"Property Values before comparison:"<<std::endl;
    std::map<std::string, ValueSet*> :: iterator iter;
    ValueSet* pValSet;
    if(pPropertyTagset1->valueSets!=NULL)
      {
      iter= pPropertyTagset1->valueSets->begin();
      pValSet= iter->second;
      pValSet->printValueSet();
      }
    std::cout<<std::endl;
    if(pPropertyTagset2->valueSets!=NULL)
      {
      iter= pPropertyTagset2->valueSets->begin();
      pValSet= iter->second;
      pValSet->printValueSet();
      }
    system("PAUSE");
    #endif

    // this is where we compare the two properties which we now have stored
    // in the containers:
    ok= pComparer->compareProperties(pPropertyTagset1, pPropertyTagset2);

    if(ok<0)
      {
      //pErr->registrateError("Error: XCDLHandler: Error while comparing properties.");
      parsingState= erroneous;
      }
    else parsingState= skipAll;
    }
  else if(parsingState==other)
    {
    const XercesString ELEM_tmpPropertySet("propertySet");
    if(!XMLString::compareString(localname, ELEM_tmpPropertySet)) // reading position: </propertySet>
      {
      parsingState=endPropertySet;
      #ifdef DEBUG_XCDL2
      printf("...end of propertySet...\n");
      #endif
      }
    else
      {
      XMLString::release(&strLocalname);
      return;
      }
    }
  else if(parsingState==endProperty || parsingState==endPropertySet)  // currently reading: </object>
    {
    parsingState=endObject;
    }
  else if(parsingState==endObject)
    {
    parsingState=endXCDL2;
    }

 XMLString::release(&strLocalname);
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Basic implementations of Sax Handler ( inherited from DefaultHandler):////
/////////////////////////////////////////////////////////////////////////////

void XCDLHandler::endDocument()
{
  #ifdef DEBUG
  printf("end document...\n");
  #endif

  if(parsingState==endXCDL1)
    {
    #ifdef DEBUG_RESULT
    printf("XCDL1 parsed...\n");
    std::cout<<"Results for compared Properties: "<<std::endl;
    pComparer->pRes->printComparerResults();
    std::cout<<std::endl;
    system("PAUSE");
    #endif

    if(pCO==NULL) pCO= new ComparatorOutput(pInputParams);
    pCO->outputForPlato(pComparer);
    delete pCO; pCO= NULL;

    // set parser variable to zero, this cleans up all of XCDLHandler object:
    parser=0;
    }

  else if(parsingState==endXCDL2 || parsingState==skipAll || parsingState==erroneous)
    {
    //if(pPropertyTagset2!= NULL) {delete pPropertyTagset2;}
    //if(pNormData2!= NULL) {delete pNormData2;}

    #ifdef DEBUG_XCDL2
    printf("XCDL2 parsed...\n");// system("PAUSE");
    #endif
    if(parsingState==erroneous)
      {
      pErr->error= -1;
      }
    }
  else
    {
    #ifdef SAFE_MODE
    std::cout<<"Warning: Wrong parsing state: "<<parsingState<<"."<<std::endl;
    #endif
    }
}

bool XCDLHandler :: PP5_checkIfPropertyIsRequested(Comparer *pComp, std::string tempstr)
{
std::list <MetricSet*> :: iterator iterReqProps;
MetricSet* pMS;

for(iterReqProps= pComp->pRequest2->pMR->sets->begin();
                      iterReqProps!=pComp->pRequest2->pMR->sets->end();
                      iterReqProps++)
  {
  pMS= *iterReqProps;
  if(!(pMS->propertyName).compare(tempstr))
    {
    return true;
    }
  }
return false;
}

