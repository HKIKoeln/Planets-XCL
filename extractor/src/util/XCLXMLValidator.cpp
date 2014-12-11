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
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include "XCLXMLValidator.h"


#if defined(XERCES_NEW_IOSTREAMS)
#include <fstream>
#else
#include <fstream.h>
#endif


XCLXMLValidator::XCLXMLValidator():
    xmlFile(0),
    valScheme(AbstractDOMParser::Val_Auto),
    doNamespaces(true),
    doSchema(true),
    schemaFullChecking(true),
    errorOccured (false),
    recognizeNEL(false),
    printOutEncounteredEles(true)
{
try
  {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException& toCatch)
  {
    char* message = XMLString::transcode(toCatch.getMessage());
    std::cout << "Error during initialization! :\n"    << message << "\n";
    XMLString::release(&message);
  }
  errorHandler = new DOMXCLErrorHandler();
  buildValidator();
}


XCLXMLValidator::~XCLXMLValidator()
{
  if(errorHandler!=NULL) delete errorHandler;
  XMLPlatformUtils::Terminate();
}



void XCLXMLValidator::buildValidator()
{
  // Instantiate the DOM parser.
  static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
  DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
  parser = ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

  parser->setFeature(XMLUni::fgDOMNamespaces, doNamespaces);
  parser->setFeature(XMLUni::fgXercesSchema, doSchema);
  parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);

  if (valScheme == AbstractDOMParser::Val_Auto)
  {
    parser->setFeature(XMLUni::fgDOMValidateIfSchema, true);
  }
  else if (valScheme == AbstractDOMParser::Val_Never)
  {
    parser->setFeature(XMLUni::fgDOMValidation, false);
  }
  else if (valScheme == AbstractDOMParser::Val_Always)
  {
    parser->setFeature(XMLUni::fgDOMValidation, true);
  }


  // enable datatype normalization - default is off
  parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);

}

BOOL XCLXMLValidator::validate(QString xmlFile, bool printElements)
{

  const char* xmlfile = xmlFile.toAscii();

  // And create our error handler and install it

  parser->setErrorHandler(errorHandler);
  //
  //  Get the starting time and kick off the parse of the indicated
  //  file. Catch any exceptions that might propogate out of it.
  //
  unsigned long duration;

  XERCES_STD_QUALIFIER ifstream fin;

  //reset error count first
  errorHandler->resetErrors();

  XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc = 0;


  try
  {
    // reset document pool
    parser->resetDocumentPool();

    const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
    doc = parser->parseURI(xmlfile);
    const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
    duration = endMillis - startMillis;
  }
  catch (OutOfMemoryException)
  {
    errorOccured = TRUE;
    std::cout << "OutOfMemoryException" << std::endl;

  }
  catch (XMLException* toCatch)
  {
    errorOccured = TRUE;
    std::cout << "\nError during parsing: '" << xmlfile << "'\n"
    << "Exception message is:  \n"
    << StrX(toCatch->getMessage()) << "\n" <<std::endl;

  }
  catch (const DOMException& toCatch)
  {
    errorOccured = TRUE;
    const unsigned int maxChars = 2047;
    XMLCh errText[maxChars + 1];

    std::cout << "\nDOM Error during parsing: '" << xmlfile << "'\n"
    << "DOMException code is:  " << toCatch.code << std::endl;

    if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
      std::cout << "Message is: " << StrX(errText) << std::endl;


  }
  catch (...)
  {
    errorOccured = TRUE;
    std::cout << "\nUnexpected exception during parsing: '" << xmlfile << "'\n";

  }

  if (printElements)
  {
    //
    //  Extract the DOM tree, get the list of all the elements and report the
    //  length as the count of elements.
    //
    if (errorHandler->getSawErrors())
    {
      std::cout << "\nErrors occurred, no output available\n" << std::endl;
      errorOccured = TRUE;
    }
    else
    {
      if (doc)
      {
        printChildElements((DOMNode*)doc->getDocumentElement());
      }

      // Print out the stats that we collected and time taken.
      std::cout << xmlfile << ": " << duration << " ms ("     << " elems)." << std::endl;
    }
  }

  //
  //  Delete the parser itself.  Must be done prior to calling Terminate, below.
  //
  parser->release();

  // And call the termination method
  XMLPlatformUtils::Terminate();

  if (errorOccured)
    return FALSE;
  else
    return TRUE;

}

void XCLXMLValidator::printChildElements(DOMNode *n)
{
  DOMNode *child;
  if (n)
  {
    if (n->getNodeType() == DOMNode::ELEMENT_NODE)
    {

      char *name = XMLString::transcode(n->getNodeName());
      XERCES_STD_QUALIFIER cout <<"----------------------------------------------------------"<<XERCES_STD_QUALIFIER endl;
      XERCES_STD_QUALIFIER cout <<"Encountered Element : "<< name << XERCES_STD_QUALIFIER endl;

      XMLString::release(&name);

      if(n->hasAttributes())
      {
        // get all the attributes of the node
        DOMNamedNodeMap *pAttributes = n->getAttributes();
        int nSize = pAttributes->getLength();
        XERCES_STD_QUALIFIER cout <<"\tAttributes" << XERCES_STD_QUALIFIER endl;
        XERCES_STD_QUALIFIER cout <<"\t----------" << XERCES_STD_QUALIFIER endl;
        for(int i=0;i<nSize;++i)
        {
          DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
          // get attribute name
          char *name = XMLString::transcode(pAttributeNode->getName());

          XERCES_STD_QUALIFIER cout << "\t" << name << "=";
          XMLString::release(&name);

          // get attribute type
          name = XMLString::transcode(pAttributeNode->getValue());
          XERCES_STD_QUALIFIER cout << name << XERCES_STD_QUALIFIER endl;
          XMLString::release(&name);
        }
      }

    }
    for (child = n->getFirstChild(); child != 0; child=child->getNextSibling())
      printChildElements(child);
  }
}
