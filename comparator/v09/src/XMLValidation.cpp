#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
//#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <string>
#include <fstream>
#include "defines.h"
#include "XercesString.h"
#include "InputParams.h"
#include "ComparatorError.h"
#include "XMLValidation.h"
#include "SaxErrorHandler.h"
#include "XCDLEntityResolver.h"

XMLValidation :: XMLValidation (ComparatorError *pError)
{
pErr= pError;
}

XMLValidation :: ~XMLValidation() {pErr= NULL;}


void XMLValidation :: validateXMLFile(InputParams *pIP, int choice)
{
std::ifstream file;
char* filename ="\0";
std::list <char*> :: iterator i1;
pErr->error= 0;

#ifdef DEBUG_XMLVALIDATION
std::cout<<"Initalizing validation of input files."<<std::endl;
#endif

XMLPlatformUtils::Initialize();
SAX2XMLReader* pParser= XMLReaderFactory::createXMLReader();
SaxErrorHandler* pSaxError= new SaxErrorHandler(pErr);
DefaultHandler handler;

try
  {
  //pParser->setProperty(XMLUni::fgXercesScannerName, (void *)XMLUni::fgSGXMLScanner);
  // Set the appropriate features on the parser:
  pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
  pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
  //pParser->setFeature(XMLUni::fgXercesDynamic, true);
  pParser->setFeature(XMLUni::fgXercesSchema, true);
  pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
  // Enable grammar caching feature:
  //pParser->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);

 // set error handler:
  pParser->setErrorHandler(pSaxError);

  //For XCDL validation, preprocess the schema and cache it:
  if(choice==1)
    {
    #ifdef _RELEASEVERSION
    pParser->loadGrammar("res/schemas/xcdl/XCDLCore.xsd", 1, true);
    #endif
    #ifdef _IFVERSION
    pParser->loadGrammar("/opt/comparator/res/schemas/xcdl/XCDLCore.xsd", 1, true);
    #endif
    // Instruct the parser to use the cached schema when processing XML documents:
    pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
    }
  }
catch(...)
  {
  if(choice==1)
    {
    pErr->registrateError("validateXMLFile(): Error during initialization for XML validation.(1)");
    }
  else  pErr->registrateError("validateXMLFile(): Error during initialization for XML validation.(2)");
  pErr->error=-1;
  delete pParser;
  delete pSaxError;
  XMLPlatformUtils::Terminate();
  return;
  }

XCDLEntityResolver *pXCDLER= NULL;

for(int c=0; c<=2 && pErr->error!=-1; c++)
   {
   if(choice==2) c=2;
   try
     {
     if(c==0)
       {
       pXCDLER= new XCDLEntityResolver();
       pParser->setEntityResolver(pXCDLER);
       filename= pIP->sourceXCDL;
       #ifdef DEBUG_XMLVALIDATION
       std::cout<<"Parsing file: "<<std::string(filename)<<" for validation."<<std::endl;
       #endif
       pParser->parse(pIP->sourceXCDL);
       pXCDLER->setEntityResolverState(0);       // reset state for TargetXCDLs
       }
     else if(c==1)
       {
       for(i1=pIP->targetXCDLs.begin();i1!=pIP->targetXCDLs.end() && pErr->error!=-1;++i1)
          {
          filename= *i1;
          #ifdef DEBUG_XMLVALIDATION
          std::cout<<"Parsing file: "<<filename<<" for validation."<<std::endl;
          #endif
          pParser->parse(filename);
          pXCDLER->setEntityResolverState(0);     // reset state for next targetXCDL
          }
       }
     else if(c==2 && choice==2)  // validate coco file
       {
       CocoEntityResolver *pPCRER= new CocoEntityResolver();
       pParser->setEntityResolver(pPCRER);
       filename= pIP->cocoFile;
       #ifdef DEBUG_XMLVALIDATION
       std::cout<<"Parsing file: "<<std::string(filename)<<" for validation."<<std::endl;
       #endif
       pParser->parse(pIP->cocoFile);
       delete pPCRER;
       }
     else break;
     }
   catch(const SAXException& e)
     {
     pErr->registrateError("validateXMLFile(): SAX error.");
     #ifdef DEBUG_XMLVALIDATION
     std::cout<<"validateXMLFile(): SAX error."<<std::endl;
     #endif
     pErr->error=-1;
     }
   catch(const XMLException& e)
     {
     pErr->registrateError("validateXMLFile(): XML error.");
     #ifdef DEBUG_XMLVALIDATION
     std::cout<<"validateXMLFile(): XML error."<<std::endl;
     #endif
     pErr->error=-1;
     }
   catch (std::string except)
     {
     except= except + " File: " + std::string(filename) + " .";
     pErr->registrateError(except);
     pErr->message= "XML validation failed. File: " + std::string(filename) + ".";

     #ifdef DEBUG_XMLVALIDATION
     std::cout<<except<<std::endl;
     #endif
     pErr->error=-1;
     }
   catch(...)
     {
     #ifdef DEBUG_XMLVALIDATION
     std::cout<<"unknown exception"<<std::endl;
     #endif
     pErr->registrateError("Exception occured while validating file. Please check input.");
     pErr->error=-1;
     }
   }

if(pXCDLER!=NULL) delete pXCDLER;
delete pParser;
delete pSaxError;
XMLPlatformUtils::Terminate();
return;
}

void XMLValidation :: validateXMLFile(char* XMLFileName, char* XMLSchemaFileName)
{
pErr->error= 0;

#ifdef DEBUG_XMLVALIDATION
std::cout<<"Initalizing file "<<XMLFileName<<" for validation."<<std::endl;
#endif

XMLPlatformUtils::Initialize();
//XMLGrammarPool *grammarPool = new XMLGrammarPoolImpl(XMLPlatformUtils::fgMemoryManager);
//SAX2XMLReader* pParser= XMLReaderFactory::createXMLReader(XMLPlatformUtils::fgMemoryManager, grammarPool);
SAX2XMLReader* pParser= XMLReaderFactory::createXMLReader();
SaxErrorHandler* pSaxError= new SaxErrorHandler(pErr);

try
  {
  //pParser->setProperty(XMLUni::fgXercesScannerName, (void *)XMLUni::fgSGXMLScanner);
  pParser->setErrorHandler(pSaxError);
  // Set the appropriate features on the parser:
  pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
  //pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
  pParser->setFeature(XMLUni::fgXercesDynamic, true);
  //pParser->setFeature(XMLUni::fgXercesSchema, true);
  //pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
  // Enable grammar caching feature:
  //pParser->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);

  //Preprocess the XML Schema and cache it:
  //pParser->resetCachedGrammarPool();
  std::cout<<XMLSchemaFileName;
  pParser->loadGrammar(XMLSchemaFileName, 1, true);
  // Instruct the parser to use the cached schema when processing XML documents:
  pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
  // The object parser calls to find the schema and resolve schema imports/includes:
  //pParser->setEntityResolver(pValidator);
  }
catch(...)
  {
  pErr->registrateError("validateXMLFile(): Error during initialization for XML validation.(2)");
  pErr->error=-1;
  delete pParser;
  delete pSaxError;
  XMLPlatformUtils::Terminate();
  return;
  }

try
  {
  #ifdef DEBUG_XMLVALIDATION
  std::cout<<"Parsing file: "<<XMLFileName<<" for validation."<<std::endl;
  #endif
  pParser->parse(XMLFileName);
  }
catch (const SAXException& e)
  {
  pErr->registrateError("validateXMLFile(): SAX error.");
  #ifdef DEBUG_XMLVALIDATION
  std::cout<<"validateXMLFile(): SAX error."<<std::endl;
  #endif
  pErr->error= -1;
  }
catch (const XMLException& e)
  {
  pErr->registrateError("validateXMLFile(): XML error.");
  #ifdef DEBUG_XMLVALIDATION
  std::cout<<"validateXMLFile(): XML error."<<std::endl;
  #endif
  pErr->error= -1;
  }
catch (std::string except)
  {
  except= except + " File: " + std::string(XMLFileName) + ".";
  pErr->registrateError(except);
  pErr->message= "XML validation failed. File: " + std::string(XMLFileName) + ".";

  #ifdef DEBUG_XMLVALIDATION
  std::cout<<pErr->message<<std::endl;
  #endif
  pErr->error= -1;
  }
catch(...)
  {
  #ifdef DEBUG_XMLVALIDATION
  std::cout<<"unknown exception"<<std::endl;
  #endif
  pErr->registrateError("Exception occured while validating file. Please check input.");
  pErr->error= -1;
  }

delete pParser;
delete pSaxError;
XMLPlatformUtils::Terminate();
return;
}
