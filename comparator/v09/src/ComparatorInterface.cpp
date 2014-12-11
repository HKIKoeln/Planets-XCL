#include <fstream>
#include <list>
#include "defines.h"
#include "ComparatorInterface.h"
#include "ComparatorEngine.h"
#include "SaxInterface.h"
#include "ComparisonConfigurationHandler.h"
#include "ComparisonConfigurationData.h"
#include "XCDLIndexGenerator.h"
#include "XCDLIndexes.h"
#include "ComparatorError.h"
#include "InputParams.h"
#include "XMLValidation.h"
#include "ComparatorOutput.h"
#include "ComparatorEngine.h"


ComparatorInterface :: ComparatorInterface(InputParams *pInPa, ComparatorError* pError)
{
pErr= pError;
pInParams= pInPa;
}

ComparatorInterface :: ~ComparatorInterface()
{
if(pErr!=NULL) {delete pErr; pErr= NULL;}
pInParams= NULL;
}

void ComparatorInterface :: doXMLValidation()
{
XMLValidation *pXMLVal= new XMLValidation(pErr);

if(pInParams->applTarget==1)    // =common usage
  {
  pXMLVal->validateXMLFile(pInParams, 2);      // validate coco
  if(pErr->error==-1) {delete pXMLVal; return;}
  pXMLVal->validateXMLFile(pInParams, 1);      // validate XCDLs
  if(pErr->error==-1) {delete pXMLVal; return;}
  }
else
  {
  std::cout<<"Unknown application target.\n";
  }
delete pXMLVal;
}

void ComparatorInterface :: startComparator()
{
ComparisonConfigurationHandler *pCocoHandler= NULL;
ComparatorOutput *pCO= NULL;
SaxInterface *pSI= NULL;
XCDLIndex *pIndexSource= NULL;
XCDLIndex *pIndexTarget= NULL;
XCDLIndexGenerator *pIndexGen= NULL;
ComparatorEngine *pCompEngine= NULL;

int error;
pErr->error=0;

// initialize variable for logfile output:
if(pInParams->logFileFlag) pErr->copraName= pInParams->copraName;

// check if files can be opened:
testIfFileIs();
if(pErr->error==-1)
  {
  if(pInParams->logFileFlag)  pErr->printLogfile();
  return;
  }

// validate input files:
if(pInParams->validationFlag)
  {
  doXMLValidation();
  if(pErr->error==-1)
    {
    std::cout<<"Validation error."<<std::endl;
    if(pInParams->logFileFlag) pErr->printLogfile();
    pCO= new ComparatorOutput(pInParams);
    pCO->createCopraErrorOutput(pErr->message);
    delete pCO;
    return;
    }
  else
    {
    #ifdef DEBUG_COMPARATORINTERFACE
    std::cout<<"Validation successful."<<std::endl;
    #endif
    }
  }

if(pInParams->applTarget==1)
  {
  try
    {
    // load coco input:
    pSI= new SaxInterface(pErr);
    pCocoHandler= new ComparisonConfigurationHandler(pErr);
    pSI->startSaxParser(pInParams->cocoFile, pCocoHandler);
    if(pErr->error==-1) {throw error=1;}

    #ifdef DEBUG_COMPARATORINTERFACE
    pCocoHandler->pCoco->printComparisonConfigurationData();
    system("PAUSE");
    #endif

    std::list <char*> :: iterator i1;

    // create Index for source XCDL:
    pIndexSource= new XCDLIndex();
    pIndexGen= new XCDLIndexGenerator(pErr, pCocoHandler->pCoco, pIndexSource);  // use this Constructor for applTarget==1
    pSI->startSaxParser(pInParams->sourceXCDL, pIndexGen);        // create Indexes
    if(pErr->error== -1) {throw error= 2;}
    // reset Index ptr for target XCDLs:
    pIndexGen->resetIndexPtr(NULL);
    //pIndexSource->print();

     for(pCocoHandler->pCoco->iterMR= pCocoHandler->pCoco->compSets->begin(),
                                         i1=pInParams->targetXCDLs.begin();
                i1!=pInParams->targetXCDLs.end();
                    ++pCocoHandler->pCoco->iterMR, ++i1)
        {
        // set compSetCount for attribute id of <compset> in outputfile:
        pInParams->compsetCount++;
        // assign class pointer to single <compSet> (=class CompSet):
        if(pCocoHandler->pCoco->pCompSet!=NULL) pCocoHandler->pCoco->pCompSet= NULL;
        pCocoHandler->pCoco->pCompSet= (*(pCocoHandler->pCoco->iterMR));
        //assign to variable:
        pInParams->targetXCDL= *i1;
        //std::cout<<pInParams->targetXCDL;
        // create indexes for target XCDL:
        pIndexTarget= new XCDLIndex();
        pIndexGen->resetIndexPtr(pIndexTarget);
        pIndexGen->resetParsingState();
        pSI->startSaxParser(pInParams->targetXCDL, pIndexGen);        // create Indexes
        if(pErr->error== -1) {throw error=3;}
        pIndexGen->resetIndexPtr(NULL);
        //pIndexTarget->pObjIndex->pDynIndex->print();

        // do comparison for each target XCDL:
        pCompEngine= new ComparatorEngine(pErr, pInParams, pIndexSource, pIndexTarget, pCocoHandler->pCoco);
        pCompEngine->compareXCDLs(pInParams->sourceXCDL, pInParams->targetXCDL);
        if(pErr->error!=0) {throw error=4;}

        // serialize results (2== copra output):
        if(pCO== NULL) pCO= new ComparatorOutput(pInParams);
        pCompEngine->serializeComparatorResults(2, pCO);
        if(pErr->error==-1) {throw error=5;}

        //clean up allocated memory:
        if(pInParams->targetXCDL!=NULL) pInParams->targetXCDL= NULL;
        if(pIndexTarget!=NULL) {delete pIndexTarget; pIndexTarget= NULL;}
        if(pCompEngine!=NULL) {delete pCompEngine; pCompEngine= NULL;}
        }

    if(pIndexSource!=NULL) delete pIndexSource;
    if(pIndexGen!=NULL) delete pIndexGen;
    if(pCO!=NULL) delete pCO;
    }
  catch(int error)
    {
    if(error==0)
      {
      std::cout<<"Parse error: Couldn't parse coco file. Check file for validity. Program terminates."<<std::endl;
      pErr->registrateError("Error: startComparator(), case 0: Couldn't parse coco file.");
      }
     else if(error==2)
      {
      std::cout<<"Parse error: Couldn't parse source XCDL file. Check file for validity. Program terminates."<<std::endl;
      pErr->registrateError("Error: startComparator(), case 2: Couldn't parse source XCDL for index creation.");
      }
    else if(error==3)
      {
      std::cout<<"Parse error: Couldn't parse target XCDL file. Check file for validity. Program terminates."<<std::endl;
      pErr->registrateError("Error: startComparator(), case 3: Couldn't parse target XCDL for index creation.");
      }
    else if(error==4)
      {
      std::cout<<"Error while comparing XCDL files. Check files for validity. Program terminates."<<std::endl;
      pErr->registrateError("Unspecified error: comparatorInterface(), case 4.");
      }
    else if(error==5)
      {
      std::cout<<"I/O error: Couldn't write copra file to specified out directory. Program terminates."<<std::endl;
      pErr->registrateError("Error: startComparator(), case 5: Error while writing copra file.");
      }

    if(pInParams->logFileFlag) pErr->printLogfile();
    pErr->error= -1;

    // free all local objects:
    if(pCocoHandler!=NULL) {delete pCocoHandler;}
    if(pSI!=NULL) {delete pSI;}
    if(pCO!=NULL) {delete pCO;}
    if(pIndexSource!=NULL) {delete pIndexSource;}
    if(pIndexTarget!=NULL) {delete pIndexTarget;}
    if(pIndexGen!=NULL) {delete pIndexGen;}
    if(pCompEngine!=NULL) {delete pCompEngine;}
    return;
    }

  if(pInParams->logFileFlag) pErr->printLogfile();
  if(pCocoHandler!=NULL) {delete pCocoHandler;}
  if(pSI!=NULL) {delete pSI;}
  pErr->error= 0;
  return;
  }
else
  {
  pErr->registrateError("Error: Unsupported application target code. Check input.\n");
  if(pInParams->logFileFlag) pErr->printLogfile();
  pErr->error= -1;
  return;
  }
}

void ComparatorInterface :: testIfFileIs()
{
std::ifstream file;
std::list <char*> :: iterator i;
std::list <char*> inputFiles(pInParams->targetXCDLs);

// insert sourceXCDL and coco:
inputFiles.push_front(pInParams->sourceXCDL);
inputFiles.push_back(pInParams->cocoFile);

// test if files exist:
for(i= inputFiles.begin(); i!= inputFiles.end(); ++i)
   {
   file.open(*i, std::ifstream::in);
   if(!file.good())
     {
     std::cout<<"I/O Error: Couldn't open file: "<<*i<<". File does not exist."<<std::endl;
     pErr->message= "File: " + std::string(*i) + " does not exist.";
     pErr->registrateError(pErr->message);
     pErr->error=-1;
     file.close();
     break;
     }
   else
     {
     #ifdef DEBUG_COMPARATORINTERFACE
     std::cout<<"File exists: "<<*i<<"."<<std::endl;
     #endif
     file.close();
     }
   }
return;
}


