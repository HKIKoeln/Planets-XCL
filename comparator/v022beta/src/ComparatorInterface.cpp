#include <fstream>
#include "defines.h"
#include "ComparatorInterface.h"
#include "SaxInterface.h"
#include "Request2InHandler.h"
#include "Request2Input.h"
#include "ComparatorError.h"
#include "InputParams.h"
#include "XMLValidation.h"
#include "ComparatorOutput.h"


ComparatorInterface :: ComparatorInterface(InputParams *pInPa)
{
pErr= new ComparatorError();
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

if(pInParams->applTarget==1)    // =Planets IF services usage
  {
  pXMLVal->validateXMLFile(pInParams, 2);      // validate PCR
  if(pErr->error==-1) {delete pXMLVal; return;}
  pXMLVal->validateXMLFile(pInParams, 1);      // validate XCDLs
  if(pErr->error==-1) {delete pXMLVal; return;}
  }
else {std::cout<<"Unknown application target.\n";} /* (TODO)  */

delete pXMLVal;
}


void ComparatorInterface :: startComparator()
{
Request2InHandler *pReq2In= NULL;
ComparatorOutput *pCO= NULL;
SaxInterface *pSI= NULL;
std::list <char*> :: iterator i1;
int error;
pErr->error=0;

// initialize variable for logfile output:
pErr->cprName= pInParams->CPRName;

// check if files can be opened:
testIfFileIs();
if(pErr->error==-1)
  {
  pErr->printLogfile();
  pCO= new ComparatorOutput(pInParams);
  pCO->createCPRErrorOutput(pErr->message);
  delete pCO;
  return;
  }

// validate input files:
doXMLValidation();
if(pErr->error==-1)
  {
  #ifdef DEBUG_COMPARATORINTERFACE
  std::cout<<"Validation error."<<std::endl;
  #endif
  pErr->printLogfile();
  pCO= new ComparatorOutput(pInParams);
  pCO->createCPRErrorOutput(pErr->message);
  delete pCO;
  return;
  }
else
  {
  #ifdef DEBUG_COMPARATORINTERFACE
  std::cout<<"Validation successful."<<std::endl;
  #endif
  }

// case: use Comparator for Planets IF Services
if(pInParams->applTarget==1)
  {
  try
    {
    // load PCR input:
    pSI= new SaxInterface(pErr);
    pReq2In= new Request2InHandler(pErr);
    pSI->startSaxParser(pInParams->PCRFile, pReq2In);

    #ifdef DEBUG_REQUEST2
    if(pErr->error!=0) pReq2In->pRequest2->printRequest2Input();
    system("PAUSE");
    #endif

    if(pErr->error==-1) {throw error=1;}

   // initialize new Sax Interface object if not done yet:
   if(pSI==NULL) pSI= new SaxInterface(pErr);
   std::list <char*> :: iterator i1;

   // for applTarget==1 (Planets IF Service): check if number of compSets and
   // number of target XCDL are equal else throw error:
   if(pReq2In->pRequest2->compSets->size()!= pInParams->targetXCDLs.size())
     {
     pCO= new ComparatorOutput(pInParams);
     pErr->message= "Number of compSets in PCR and given target files do not match. Check input.";
     pCO->createCPRCompsetErrorOutput(pErr->message, pReq2In->pRequest2->pMR);
     pErr->message.clear();
     delete pCO;
     }
   else    // get compSet and compare each targetXCDLs from list with sourceXCDL
     {
     for(pReq2In->pRequest2->iterMR= pReq2In->pRequest2->compSets->begin(),
                                         i1=pInParams->targetXCDLs.begin();
                i1!=pInParams->targetXCDLs.end(); //pReq2In->pRequest2->iterMR!= pReq2In->pRequest2->compSets->end();
                    ++pReq2In->pRequest2->iterMR, ++i1)
        {
        // set compSetCount for attribute id of <compset>:
        pInParams->compsetCount++;
        // assign class pointer to single <compSet> (=class MeasurementRequest):
        if(pReq2In->pRequest2->pMR!=NULL) pReq2In->pRequest2->pMR= NULL;
        pReq2In->pRequest2->pMR= (*(pReq2In->pRequest2->iterMR));
        //assign to variable and start comparison:
        pInParams->targetXCDL= *i1;
        //std::cout<<pInParams->targetXCDL;
        pSI->startSaxParser(pInParams, pReq2In->pRequest2);
        pInParams->targetXCDL= NULL;
        //if(pErr->error==-1) throw error=2;
        }
     }
    // finish CPR output for Planets Plato Tool:
    pCO= new ComparatorOutput(pInParams);
    pCO->createCPROutputEnd();
    delete pCO;
    }
  catch(int error)
    {
    if(error==1)
      {
      pErr->registrateError("Error: Couldn't parse Request 2 Input file.");
      }
    else if(error==2)
      {
      pErr->registrateError("Error: Error while comparing files.");
      }
    if(pReq2In!=NULL) {delete pReq2In; pReq2In= NULL;}
    if(pSI!=NULL) {delete pSI; pSI= NULL;}
    pErr->printLogfile();
    pErr->error= -1;
    return;
    }
  if(pReq2In!=NULL) {delete pReq2In;}
  if(pSI!=NULL) {delete pSI;}
  pErr->printLogfile();
  return;
  }

else
  {
  pErr->registrateError("Error: Unsupported application target code. Check input.\n");
  pErr->printLogfile();
  pErr->error= -1;
  return;
  }
}

void ComparatorInterface :: testIfFileIs()
{
std::ifstream file;
std::list <char*> :: iterator i;
std::list <char*> inputFiles(pInParams->targetXCDLs);

// insert sourceXCDL and PCR:
inputFiles.push_front(pInParams->sourceXCDL);
inputFiles.push_back(pInParams->PCRFile);

// test if files exist:
for(i= inputFiles.begin(); i!= inputFiles.end(); ++i)
   {
   //std::cout<<(*i);system("PAUSE");
   file.open(*i, std::ifstream::in);
   if(!file.good())
     {
     #ifdef DEBUG_COMPARATORINTERFACE
     std::cout<<"Fatal Error: Couldn't open file: "<<*i<<". File does not exist."<<std::endl;
     #endif
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
