#include <map>
#include <list>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include "defines.h"
#include "ComparatorEngine.h"
#include "SaxInterface.h"
#include "XCDLHandler.h"
#include "XCDLIndexes.h"
#include "ComparatorError.h"
#include "DataTagset.h"
#include "Comparer.h"
#include "ComparerResults.h"
#include "ComparatorOutput.h"

void ComparatorEngine :: serializeComparatorResults(int typeOfOutput, ComparatorOutput *pCO)
{
if(typeOfOutput==1)
  {
  //pCO->outputForPlato(pComp);
  pCO->createCopraOutputEnd();
  }
else if(typeOfOutput==2)
  {
  pCO->createCopraOutputCore(pComp);
  pCO->createCopraOutputEnd();
  }
// add other typeOfOutput if available

return;
}

void ComparatorEngine :: compareXCDLs(char* sourceXCDL, char* targetXCDL)
{
xercesc::SAX2XMLReader *pParserSource= NULL;
xercesc::SAX2XMLReader *pParserTarget= NULL;
XCDLHandler *pXCDLHandlerSource= new XCDLHandler(0, "\0", pErr, pIndexSrc, pInParams);  // param #1: parsingState
XCDLHandler *pXCDLHandlerTarget= new XCDLHandler(0, "\0", pErr, pIndexTar, pInParams);  // param #1: parsingState


// initialize two instances of SAX2XMLReader, for both source and target XCDL:
try
  {
  if(pSI==NULL) pSI= new SaxInterface(pErr);
  pParserSource= pSI->initializeSax2Parser(sourceXCDL, pParserSource);
  pParserTarget= pSI->initializeSax2Parser(targetXCDL, pParserTarget);
  }
catch (...)
  {
  std::cout<<"Error during initialization of Xerces SAX2XMLReader."<<std::endl;
  pErr->registrateError("Error: compareXCDLs(): Error during initialization of Xerces SAX2XMLReader.");
  pErr->error=-1;
  if(pParserSource!= NULL) delete pParserSource;
  if(pParserTarget!= NULL) delete pParserTarget;
  return;
  }

int _case;
std::list <std::string> :: iterator i;
std::map <std::string, bool> :: iterator iProps1;
std::map <std::string, bool> :: iterator iProps2;

// allocate memory for new Comparer object and Results object, assign objects:
if(pComp==NULL) pComp= new Comparer(pErr, pIndexSrc, pIndexTar);
if(pResults==NULL) pResults= new Results();
pComp->setResultsPtr(pResults);
pComp->setConfigFilePtr(pConfig);

// generate list of properties with data reference which are requested for
// comparison (indicated by second parameter of map) AND available in both XCDLs:
if(pIndexSrc->pObjIndex->pDynIndex->PropsWithDataRef!= NULL &&
   pIndexTar->pObjIndex->pDynIndex->PropsWithDataRef!= NULL)
  {
  std::list <std::string> *PropsToCompareWithDataRef= new std::list <std::string> ();

  for(iProps1=pIndexSrc->pObjIndex->pDynIndex->PropsWithDataRef->begin();
        iProps1!=pIndexSrc->pObjIndex->pDynIndex->PropsWithDataRef->end();
                                                ++iProps1)
     {
     if(pIndexTar->pObjIndex->pDynIndex->PropsWithDataRef->find(iProps1->first)!=pIndexTar->pObjIndex->pDynIndex->PropsWithDataRef->end())
       {
       iProps2= pIndexTar->pObjIndex->pDynIndex->PropsWithDataRef->find(iProps1->first);
       if(iProps1->second==true && iProps2->second==true) PropsToCompareWithDataRef->push_back(iProps1->first);
       }
     }

  #ifdef DEBUG_COMPARATORENGINE
  std::cout<<"list of properties with dataRef to compare:"<<std::endl;
  for(i=PropsToCompareWithDataRef->begin();i!=PropsToCompareWithDataRef->end();++i)
     {
     std::cout<<*i<<std::endl;
     }
  system("PAUSE");
  #endif

  // compare properties with data references and compare normData ...
  for(i=PropsToCompareWithDataRef->begin(); i!=PropsToCompareWithDataRef->end();++i)
     {
     // ... normData:
     if(!(*i).compare("normData"))
       {// set parameters for Xercesc parsing and parse files:
       try
         {
          // start with source XCDL...
         if(pNormData1== NULL) pNormData1= new NormData ();
         pXCDLHandlerSource->setNormDataPtr(pNormData1);
         pXCDLHandlerSource->setParsingState(1);
         pXCDLHandlerSource->setElementSearchedFor("normData");
         pParserSource->setContentHandler(pXCDLHandlerSource);// these are Xercesc methods (can't be modified)!
         pParserSource->setErrorHandler(pXCDLHandlerSource);//  ...
         pSI->Sax2ParseFile(pParserSource, sourceXCDL);
         if(pErr->error!=0) {throw _case=0;}
         // ... and go on with target XCDL:
         if(pNormData2== NULL) pNormData2= new NormData ();
         pXCDLHandlerTarget->setNormDataPtr(pNormData2);
         pXCDLHandlerTarget->setParsingState(1);
         pXCDLHandlerTarget->setElementSearchedFor("normData");
         pParserTarget->setContentHandler(pXCDLHandlerTarget);// these are Xercesc methods (can't be modified)!
         pParserTarget->setErrorHandler(pXCDLHandlerTarget);  //  ...
         pSI->Sax2ParseFile(pParserTarget, targetXCDL);
         if(pErr->error!=0) {throw _case=0;}

         // assign objects and do comparison:
         pComp->setNormData1Ptr(pNormData1);
         pComp->setNormData2Ptr(pNormData2);
         pComp->compareNormData();

         if(pErr->error!=0) {throw _case=1;}
         #ifdef DEBUG_COMPARATORENGINE
         std::cout<<"NormData is compared..."<<std::endl;
         #endif
         }
       catch (int _case)
         {
         if(_case==0)
           {
           std::cout<<"SAX2 parsing error."<<std::endl;
           pErr->registrateError("Error: compareXCDLs(): SAX2 parsing error.");
           pErr->error=-1;
           if(PropsToCompareWithDataRef!=NULL) {delete PropsToCompareWithDataRef;}
           if(pNormData1!= NULL) delete pNormData1;
           if(pNormData2!= NULL) delete pNormData2;
           pSI->terminateSax2Parser();
           return;
           }
         else
           {
           pComp->pRes->pRS->errorcode= "Code_G_1";
           pComp->pRes->pRS->compStatus= 2;
           pErr->error= 0;
           if(pNormData1!= NULL) delete pNormData1;
           if(pNormData2!= NULL) delete pNormData2;
           pSI->terminateSax2Parser();
           continue;  // go on with next property
           }
         }
       }
     else
       //  ... properties:
       {
       pComp->comparePropertyWithDataRef((*i));
       #ifdef DEBUG_COMPARATORENGINE
       std::cout<<(*i)<<" is compared..."<<std::endl;system("PAUSE");
       #endif
       }
     }
  delete PropsToCompareWithDataRef;
  }

// generate list of properties WITHOUT data reference which are requested for
// comparison AND available in both XCDLs:
if(pIndexSrc->pObjIndex->pDynIndex->PropsWithoutDataRef!= NULL &&
      pIndexTar->pObjIndex->pDynIndex->PropsWithoutDataRef!= NULL)
  {
  std::list <std::string> *PropsToCompareWithoutDataRef= new std::list <std::string> ();
  for(iProps1=pIndexSrc->pObjIndex->pDynIndex->PropsWithoutDataRef->begin();
        iProps1!=pIndexSrc->pObjIndex->pDynIndex->PropsWithoutDataRef->end();
                                                ++iProps1)
     {
     if(pIndexTar->pObjIndex->pDynIndex->PropsWithoutDataRef->find(iProps1->first)!=pIndexTar->pObjIndex->pDynIndex->PropsWithoutDataRef->end())
       {
       iProps2= pIndexTar->pObjIndex->pDynIndex->PropsWithoutDataRef->find(iProps1->first);
       if(iProps1->second==true && iProps2->second==true) PropsToCompareWithoutDataRef->push_back(iProps1->first);
       }
     }
  #ifdef DEBUG_COMPARATORENGINE
  std::cout<<"list of properties to compare:"<<std::endl;
  for(i=PropsToCompareWithoutDataRef->begin();i!=PropsToCompareWithoutDataRef->end();++i)
     {
     std::cout<<*i<<std::endl;
     }
  system("PAUSE");
  #endif
  // compare properties WITHOUT data references ...
  for(i=PropsToCompareWithoutDataRef->begin(); i!=PropsToCompareWithoutDataRef->end();++i)
    {
    pComp->comparePropertyWithoutDataRef((*i));
    }

  delete PropsToCompareWithoutDataRef;
  }

return;
};


