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
#include "ComparisonConfigurationData.h"
#include "XCDLHandler.h"
#include "ComparisonConfigurationHandler.h"
#include "XCDLIndexGenerator.h"
#include "SaxErrorHandler.h"
#include "SaxInterface.h"
#include "Comparer.h"
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

SaxInterface :: SaxInterface(ComparatorError *pCE)
{
pErr= pCE;
}

SaxInterface :: ~SaxInterface()
{
if(pErr!=NULL) pErr= NULL;
}

// initializes SAX2XMLReader:
SAX2XMLReader* SaxInterface :: initializeSax2Parser(char* XCDLFile, xercesc::SAX2XMLReader *pParser)
{
#ifdef DEBUG_SAXINTERFACE
std::cout<<"Initalizing file "<<(std::string(XCDLFile))<<" ..."<<std::endl;
#endif

XMLPlatformUtils::Initialize();

if(pParser== NULL)  pParser= XMLReaderFactory::createXMLReader();

return pParser;
}

void SaxInterface :: terminateSax2Parser() {xercesc::XMLPlatformUtils::Terminate();}


// for loading normData:
void SaxInterface :: Sax2ParseFile(xercesc::SAX2XMLReader *pParser, char* XCDLFile)
{
  try
    {
    #ifdef DEBUG_SAXINTERFACE
    std::cout<<"Parsing XCDL file: "<<(std::string(XCDLFile))<<"."<<std::endl;
    #endif
    pParser->parse(XCDLFile);
    }
  catch (...)
    {
    pErr->message= "Error: XercescSaxInterface: Parse error while trying to parse file \'" + std::string(XCDLFile) + "\' for normData loading. Check input for validity.";
    pErr->registrateError(pErr->message);
    pErr->error=-1;
    }

return;
}


// for loading PCR Input:
void SaxInterface :: startSaxParser(char* file, ComparisonConfigurationHandler *pHandler)
{
SAX2XMLReader* pParser;
pErr->error= 0;

#ifdef DEBUG_SAXINTERFACE
std::cout<<"Initalizing file "<<file<<" ..."<<std::endl;
#endif
try
  {
  XMLPlatformUtils::Initialize();
  pParser= XMLReaderFactory::createXMLReader();
  pParser->setContentHandler(pHandler);
  pParser->setErrorHandler(pHandler);

   #ifdef DEBUG_SAXINTERFACE
   std::cout<<"Parsing file: "<<file<<"."<<std::endl;
   #endif
   pParser->parse(file);
  }
catch(...)
  {
  pErr->message= "Error: SaxInterface: Parse error while trying to parse PCR file \'" + std::string(file) + "\'. Check input for validity.";
  pErr->registrateError(pErr->message);
  pErr->error=-1;
  }

delete pParser;

XMLPlatformUtils::Terminate();

return;
}

// for creating XCDL indexes:
void SaxInterface :: startSaxParser(char* XCDLFile, XCDLIndexGenerator *pHandler)
{
SAX2XMLReader* pParser;
pErr->error= 0;

#ifdef DEBUG_SAXINTERFACE
std::cout<<"Initalizing file "<<XCDLFile<<" ..."<<std::endl;
#endif

try
  {
  XMLPlatformUtils::Initialize();
  pParser= XMLReaderFactory::createXMLReader();
  pParser->setContentHandler(pHandler);
  pParser->setErrorHandler(pHandler);

  #ifdef DEBUG_SAXINTERFACE
  std::cout<<"Indexing file: "<<XCDLFile<<"."<<std::endl;
  #endif
  pParser->parse(XCDLFile);
  }
catch(...)
  {
  pErr->message= "Error: SaxInterface: Parse error while trying to parse XCDL file \'" + std::string(XCDLFile) + "\' for indexing. Check input for validity.";
  pErr->registrateError(pErr->message);
  pErr->error=-1;
  }

delete pParser;

XMLPlatformUtils::Terminate();

return;
}


