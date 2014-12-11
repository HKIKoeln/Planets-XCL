/***************************************************************************
SaxInterface.cpp

Description: SaxInterface methods
****************************************************************************/
// Home of Xerces-C++ grammar constants:
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include "defines.h"
#include "XercesString.h"
#include "ComparatorError.h"
#include "Request2Input.h"
#include "XCDLHandler.h"
#include "Request2InHandler.h"
#include "SaxErrorHandler.h"
#include "SaxInterface.h"
#include "Comparer.h"
#include "PropertyTagset.h"
#include "DataTagset.h"
#include "InputParams.h"

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif

#include <exception>
#include <stdexcept>
#include <stdlib.h>

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

SaxInterface :: SaxInterface()
{
    pErr= NULL;

    #ifdef DEBUG_SAXINTERFACE
    std::cout<<"Sax Interface .... Initalizing std. object..."<<std::endl;
    #endif
}

// constructor for initial object
SaxInterface :: SaxInterface(ComparatorError *pCE)
{
    pErr= pCE;

    #ifdef DEBUG_SAXINTERFACE
    std::cout<<"Sax Interface .... Initalizing pErr object..."<<std::endl;
    #endif
}

SaxInterface :: ~SaxInterface()
{
if(pErr!=NULL) pErr= NULL;
#ifdef DEBUG_SAXINTERFACE
std::cout<<"released Sax Interface obj...\n"<<std::endl;
#endif
}
// for loading Request2 Input:
void SaxInterface :: startSaxParser(char* file, Request2InHandler *pHandler)
{
SAX2XMLReader* pParser= NULL;
pErr->error= 0;

#ifdef DEBUG_SAXINTERFACE
std::cout<<"Initalizing file "<<file<<" ..."<<std::endl;
#endif

XMLPlatformUtils::Initialize();
pParser= XMLReaderFactory::createXMLReader();
pParser->setContentHandler(pHandler);
pParser->setErrorHandler(pHandler);

try
  {
   #ifdef DEBUG_SAXINTERFACE
   std::cout<<"Parsing file: "<<file<<"."<<std::endl;
   #endif
   pParser->parse(file);
   }
catch(...)
    {
    pErr->registrateError("Error: SaxInterface: Parse error while parsing request2 input file.");
    pErr->error= -1;
    }

delete pParser;

XMLPlatformUtils::Terminate();

return;
}

// for parsing first XCDL:
void SaxInterface :: startSaxParser(InputParams *pInPa, Request2 *pReq2)
{
   SAX2XMLReader* pParser= NULL;
   XCDLHandler* pHandler= NULL;

   // TODO:    get appropriate compSet

   #ifdef DEBUG_SAXINTERFACE
   std::cout<<"Initalizing file "<<(std::string(pInPa->sourceXCDL))<<" ..."<<std::endl;
   #endif

   XMLPlatformUtils::Initialize();

   pParser= XMLReaderFactory::createXMLReader();
   pHandler= new XCDLHandler(pReq2, pErr, pInPa);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

  try
    {
    #ifdef DEBUG_SAXINTERFACE
    std::cout<<"Parsing source XCDL file: "<<(std::string(pInPa->sourceXCDL))<<"."<<std::endl;
    #endif
    pParser->parse(pInPa->sourceXCDL);
    }
  catch (...)
    {
    #ifdef DEBUG_GENERAL
    std::cout<<"Error: SaxInterface: Trying to parse source XCDL file. Not possible."<<std::endl;
    std::cout<<"Please check if file names are typed in properly."<<std::endl;
    system("PAUSE");
    #endif
    pErr->registrateError("Error: SaxInterface: Parse error while trying to parse source XCDL file. Check input.");
    pErr->error=-1;
    }

  delete pParser;
  delete pHandler;

  XMLPlatformUtils::Terminate();

if(pErr->error<-1) pErr->error=-1;
}

// Method for parsing XCDL #2:
int SaxInterface :: startSaxParser(char* targetXCDL, PropertyTagset *pPTag, NormData *pNData, Comparer *pComp)
{
    SAX2XMLReader* pParser= NULL;
    XCDLHandler* pHandler= NULL;
    pErr->error= 0;

    #ifdef DEBUG_SAXINTERFACE
    std::cout<<"Initalizing file "<<std::string(targetXCDL)<<" ..."<<std::endl;
    #endif

    XMLPlatformUtils::Initialize();

    pParser= XMLReaderFactory::createXMLReader();
    pHandler= new XCDLHandler(pPTag, pNData, pComp, pErr);
    pParser->setContentHandler(pHandler);
    pParser->setErrorHandler(pHandler);

    try
      {
      #ifdef DEBUG_SAXINTERFACE
      std::cout<<"Parsing second XCDL file..."<<std::endl;
      system("PAUSE");
      #endif
      pParser->parse(targetXCDL);
      }
    catch (...)
      {
      pErr->registrateError("Error: SaxInterface: Parse error while parsing second XCDL file.");
      pErr->error= -1;
      }

  delete pParser;
  delete pHandler;


  XMLPlatformUtils::Terminate();

  if(pErr->error<-1) pErr->error=-1;

return pErr->error;
}


