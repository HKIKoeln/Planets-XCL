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
#include "Printer.h"
#include "XCELTreeBuilder.h"
#include "XCLXMLValidator.h"
#include "XCLDOMBuilder.h"
#include "XCELGrammarBuilder.h"
#include "XCELReader.h"
#include "XCELPreProcessor.h"
#include "XCLException.h"
#include "XCELReaderFactory.h"
#include "XCELAddFilterProcessingMethod.h"
#include "XCELDecompressProcessingMethod.h"
#include "XCELSetByteOrderProcessingMethod.h"
#include "XCELSetInterpretationProcessingMethod.h"
#include "XCELSetLengthProcessingMethod.h"
#include "XCELSetOptionalProcessingMethod.h"
#include "XCELSetNameProcessingMethod.h"
#include "XCELSetStartProcessingMethod.h"
#include "XCELIsEqualProcessingMethod.h"
#include "XCELSetSDNMProcessingMethod.h"
#include "XCELSetAddrSchemeProcessingMethod.h"
#include "XCELGoToLastAddressProcessingMethod.h"
#include "XCELGoToPreviousAddressProcessingMethod.h"
#include "XCELGoToNextAddressProcessingMethod.h"
#include "XCELGetItemNumberProcessingMethod.h"
#include "XCELSetIdentifierProcessingMethod.h"
#include "XCELSetValueProcessingMethod.h"
#include "XCELInterpretAsNumberMethod.h"
#include "XCELInterpretHexValuesMethod.h"
#include "XCELReaderDataObject.h"
#include "XCELPostProcessor.h"
#include "XCELSetExternalSourceProcessingMethod.h"
#include "XCELSetInternalSourceProcessingMethod.h"
#include "XCELExtractProcessingMethod.h"
#include "XCELUnzipProcessingMethod.h"
#include "XCELAddRelationProcessingMethod.h"
#include "XCELValueIsNotInIndexProcessingMethod.h"
#include "XCELIsNotEqualProcessingMethod.h"


XCELTreeBuilder::XCELTreeBuilder()
{
}


XCELTreeBuilder::~XCELTreeBuilder()
{
}


/*! \const XCLParsingItem& XCELTreeBuilder::build(QString xcelFileName, QString binaryFileName)
 *  \brief Builds the grammar for the file \a xcelFilename and parses the binary file \a binaryFileName into this strucutre. 
\sa XCELGrammarBuilder
 *  \param xcelFilename the name and location of the appropriate XCEL to process the binary file as QString.
 *  \param binFileName the name and location of the binary file as QString.
 *  \exception XCLException validation failed
 * \exception XCLException parsing error while reading binFileName with grammar xcelFileName
 *  \return the root element of the parsed binary file as an #XCLParsingItem.
 */
const XCLParsingItem& XCELTreeBuilder::build(const QString& xcelfile,
    const QString& binfile)
{
  xcelFileName = xcelfile;
  binaryFileName = binfile;

  Printer::print("xcelFileName: "+xcelFileName+"\n");
  //std::cout<<"binaryFileName: "<<binaryFileName.toStdString()<<"\n";

  XCELReaderFactory readerFactory;
  XCELReaderDataObject dataObject;
  //------------------------Configuring--------------------------
  try
  {

    Printer::print("\tValidate XCEL...");

    if(! validate(xcelFileName))
      throw XCLException("XCEL is not valid");


    XCLDOMBuilder dombuilder;
    xcelDOM=dombuilder.getDOM(xcelFileName);
    Printer::print("ready\n");

    Printer::print("\tGenerate Grammar...");
    XCELGrammarBuilder grammarbuilder;
    dataObject.processingGrammar=new XCLSyntaxItem(grammarbuilder.buildGrammar(&xcelDOM));
    dataObject.preprocessingGrammar=new XCLSyntaxItem(grammarbuilder.buildPreProcessingGrammar(&xcelDOM));
    dataObject.postprocessingGrammar=new XCLSyntaxItem(grammarbuilder.buildPostProcessingGrammar(&xcelDOM));
    Printer::print("ready\n");


  }
  catch(XCLException exception)
  {
    throw XCLException(exception.getMessage());
  }


  try
  {
    Printer::print("\t Load Plugins...");


    readerFactory.registerMethod(new XCELSetLengthProcessingMethod());
    readerFactory.registerMethod(new XCELSetNameProcessingMethod());
    readerFactory.registerMethod(new XCELSetByteOrderProcessingMethod());
    readerFactory.registerMethod(new XCELSetStartProcessingMethod());
    readerFactory.registerMethod(new XCELSetInterpretationProcessingMethod());
    readerFactory.registerMethod(new XCELAddFilterProcessingMethod());
    readerFactory.registerMethod(new XCELSetOptionalProcessingMethod());
    //   readerFactory.registerMethod(new XCELSetMultipleProcessingMethod());
    readerFactory.registerMethod(new XCELDecompressProcessingMethod());
    readerFactory.registerMethod(new XCELIsEqualProcessingMethod());
    readerFactory.registerMethod(new XCELSetSDNMProcessingMethod());
    readerFactory.registerMethod(new XCELSetAddrSchemeProcessingMethod());
    readerFactory.registerMethod(new XCELGoToLastAddressProcessingMethod());
    readerFactory.registerMethod(new XCELGoToPreviousAddressProcessingMethod());
    readerFactory.registerMethod(new XCELGoToNextAddressProcessingMethod());
    readerFactory.registerMethod(new XCELGetItemNumberProcessingMethod());
    readerFactory.registerMethod(new XCELSetIdentifierProcessingMethod());
    readerFactory.registerMethod(new XCELSetValueProcessingMethod());
    readerFactory.registerMethod(new XCELInterpretAsNumberMethod());
    readerFactory.registerMethod(new XCELInterpretHexValuesMethod());
    readerFactory.registerMethod(new XCELSetExternalSourceProcessingMethod());
    readerFactory.registerMethod(new XCELSetInternalSourceProcessingMethod());
    readerFactory.registerMethod(new XCELExtractProcessingMethod());
    readerFactory.registerMethod(new XCELUnzipProcessingMethod());
    readerFactory.registerMethod(new XCELAddRelationProcessingMethod());
    readerFactory.registerMethod(new XCELValueIsNotInIndexProcessingMethod());
    readerFactory.registerMethod(new XCELIsNotEqualProcessingMethod());
    Printer::print("ready\n");
  }
  catch(XCLException exception)
  {
  }
  XCELReader* reader;
  //--------------------Pre Processing-----------------------
  try
  {
   Printer::print("\t Start pre processing...");
    
    XCELPreProcessor* preprocessor = readerFactory.preprocessor;

    reader = preprocessor->preprocess(*dataObject.processingGrammar,binaryFileName,
                                      *dataObject.preprocessingGrammar);
    Printer::print("ready\n");
  }
  catch(XCLException exception)
  {
    Printer::print("Preprocessing terminated with exception\n");
  }
  XCELMainProcessor* mainProcessor;
  //--------------------Main Processing---------------------------
  try
  {
    Printer::print("\t Start main processing...");
    mainProcessor = readerFactory.processor;

    root=mainProcessor->mainprocess(reader,*dataObject.processingGrammar);
    Printer::print("ready\n");
    debugTree=mainProcessor->getDebugTree();
    /*#ifdef PlanetsDebug
        
        std::cout<<"\t Print Debug Tree...";
        debugTree.print( );
    #endif*/

  }
  catch(XCLException exp)
  {
    exp.message();
    Printer::print("Exception: Parse "+ binaryFileName+" with Grammar "+xcelFileName+"\n");
#ifdef PlanetsDebug
    debugTree=mainProcessor->getDebugTree();
    Printer::print("\t Print Debug Tree...");
    debugTree.print( );
#endif
    throw XCLException(exp.getMessage());
  }
  //-----------------------Post Processing---------------------------
  try
  {

    Printer::print("\t Start post processing...");
    XCELPostProcessor* postProcessor = readerFactory.postprocessor;
    postProcessor->postprocess(dataObject.postprocessingGrammar,&root);
    Printer::print("ready\n");
  }
  catch( XCLException exception)
  {}
  /*readerFactory.registerDataType(new XCELUint32DataType());
  ...*/

  /*  XCELReader* preReader = readerFactory.createReader(XCELReaderFactory::PRE);
    XCELReader* reader = readerFactory.createReader(XCELReaderFactory::PRO);
    XCELReader* postReader = readerFactory.createReader(XCELReaderFactory::POST);

    return postReader.read(reader.read(preader.read()));*/
//root.print();
  return root;

}


BOOL XCELTreeBuilder::validate(const QString& xcelFileName)
{
  XCLXMLValidator validator;
  return validator.validate(xcelFileName);
}



