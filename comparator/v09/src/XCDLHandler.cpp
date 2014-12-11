/*****************************************************************************
XCDLHandler.cpp

Description: Sax Handler class methods
*****************************************************************************/
#include <string>
#include <map>
#include <list>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLChTranscoder.hpp>
#include "defines.h"
#include "XercesString.h"
#include "ComparatorError.h"
#include "XCDLHandler.h"
#include "Comparer.h"
#include "ComparisonConfigurationHandler.h"
#include "ComparisonConfigurationData.h"
#include "SaxInterface.h"
#include "ComparerResults.h"
#include "ComparatorOutput.h"
#include "DataTagset.h"
#include "InputParams.h"
#include "XCDLIndexes.h"

///
///  --------------             elements parsing       -----------------------
///
void XCDLHandler :: startElement(const XMLCh* const uri,
                                    const XMLCh* const localname,
                                        const XMLCh* const qname,
                                            const Attributes& attrs)
{
if(parsingState==done || parsingState==erroneous) return;

#ifdef DEBUG_XCDLHANDLER
char *strLocalname = XMLString::transcode(localname);
printf("starting Element... %s\n", strLocalname);
XMLString::release(&strLocalname);
#endif

if(parsingState==searchNormData)
  {
  if(XMLString::equals(localname, elementToSearchFor))  // also possible: if(!XMLString::compareString(localname, elementToSearchFor))
    {                                                   // any differences???         /***        ***/
    parsingState= loadNormData;
    return;
    }
  }
}

///
///  ---------           Characters parsing             ------------
///
void XCDLHandler :: characters(
                              const XMLCh* const chars,
                                  const unsigned int length)
{
if(parsingState==erroneous || parsingState==done) return;

if(parsingState== loadNormData)
  {
  if(pIndex->pObjIndex->normDataIds!= NULL)
    {
    char* normData= XMLString::transcode(chars);
    // check type of normData, set variables and load:
    std::map <std::string, std::string> :: iterator iNormData;
    if(pIndex->pObjIndex->normDataIds!=NULL)
    {iNormData= pIndex->pObjIndex->normDataIds->begin();}
    else {/* todo */}
    if(!(iNormData->second).compare("text"))
      {
      // transcode to UTF8 for output:
      if(pInput->outNormdataFlag) transcodeToUTF8(chars);
      pNormData->loadNormDataAsIs(normData, length);
      // cast to unsigned int:
      *(unsigned int*)(&(pNormData->normDataLength))= length;
      pNormData->normDataType= iNormData->second;
      pNormData->key= iNormData->first;
      }
    else if (!(iNormData->second).compare("image"))
      {
      // for output:
      if(pInput->outNormdataFlag)  pNormData->loadNormDataAsIs(normData, length);
      pNormData->loadNormDataAsInt(std::string(element), pNormData, normData);
       *(unsigned int*)(&(pNormData->normDataLength))= length;
      pNormData->normDataType= iNormData->second;
      pNormData->key= iNormData->first;
      }
    else
      {
      //*** TODO NEXT VERSION:  catch error  ***//
      std::cout<<"Yet Unsupported information type!"<<std::endl;
      parsingState= erroneous;
      return;
      }

    parsingState= done;
    XMLString::release(&normData);
    return;
    }
  else {/*   TODO NEXT VERSION: catch error: no normData */}
  }
}

bool XCDLHandler :: PP5_checkIfPropertyIsRequested(Comparer *pComp, std::string tempstr)
{
std::list <MetricSet*> :: iterator iterReqProps;
MetricSet* pMS;

for(iterReqProps= pComp->pCoco->pCompSet->sets->begin();
                      iterReqProps!=pComp->pCoco->pCompSet->sets->end();
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

void XCDLHandler :: transcodeToUTF8(const XMLCh* const chars)
{
XMLTranscoder* utf8Transcoder;
XMLTransService::Codes failReason;
utf8Transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor("UTF-8", failReason, 16*1024);
size_t len = XMLString::stringLen(chars);
XMLByte* utf8 = new XMLByte((len * 4) + 1);
unsigned int eaten;
unsigned int utf8Len = utf8Transcoder->transcodeTo(chars, len, utf8, len * 4,
                                                    eaten, XMLTranscoder::UnRep_Throw);

utf8[utf8Len] = '\0';
pNormData->normDataAsStdStr = (char*)utf8;

delete [] utf8;
return;
}
