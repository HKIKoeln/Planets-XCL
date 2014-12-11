#include "defines.h"
#include "InputParams.h"
#include "ComparatorError.h"
#include "Request2Input.h"
#include "ComparatorOutput.h"


InputParams :: InputParams() : applTarget(-1), sourceXCDL(NULL), targetXCDL(NULL), CPRName("\0"),
                             PCRFile(NULL), outDir("\0"), nameSourceXCDL("\0"), nameTargetXCDL("\0"),
                             outFlag(false), compsetCount(0)
{
}

InputParams :: ~InputParams()
{
cleanUp();
}

int InputParams :: parseArgs(int argc, char* argv[])
{
    ComparatorOutput CompOut;
    bool optionsFlag= false;
    int c;
    std::string tmp;

    if(argc==1 || argc==2)
      {
      /*if(argc==2 && !strcmp(argv[2], "-h")
        {
        printHelp();
        }
      else*/
      CompOut.createInputParamsErrorOutput();
      return -1;
      }
    else if(argc>=3)
      {
      applTarget= 1;

      for(c=1;c<argc;c++)
         {
         if(strcmp(argv[c], "-o") && strcmp(argv[c], "-c") && strcmp(argv[c], "-i") && optionsFlag==false)
           {
           if(c>1)
             {
             targetXCDL= new char(strlen(argv[c]));
             targetXCDL= argv[c];
             targetXCDLs.push_back(targetXCDL);
             }
           else
             {
             sourceXCDL= new char(strlen(argv[c]));
             sourceXCDL= argv[c];
             }
           }
         else
           {
           optionsFlag= true;
           if(c==1 || c==2)
             {
             CompOut.createInputParamsErrorOutput(); return -1;
             }
           else
             {
             if(!strcmp(argv[c], "-c"))
               {
               c++;
               if(c<argc)
                 {
                 PCRFile= new char(strlen(argv[c]));
                 PCRFile= argv[c];
                 }
               else {CompOut.createInputParamsErrorOutput(); return -1;}
               }
             /*else if(!strcmp(argv[c], "-i"))
               {
               c++;
               if(c<argc)
                 {
                 CPRName= std::string(argv[c]);
                 }
               else  {CompOut.createInputParamsErrorOutput(); return -1;}
               } */
             else if(!strcmp(argv[c], "-o"))
               {
               c++;
               if(c<argc)
                 {
                 outDir= std::string(argv[c]);
                 }
               else  {CompOut.createInputParamsErrorOutput();  return -1;}
               }
             else {CompOut.createInputParamsErrorOutput(); return -1;}
             }
           }
         }
      if(PCRFile==NULL)
        {
        CompOut.createInputParamsErrorOutput(); return -1;
        }
    /*  if(CPRName=="\0")
        {
        CompOut.createInputParamsErrorOutput(); return -1;
        } */
      }
return 0;
}

void InputParams :: printInputParams()
{
std::list <char*> :: iterator i1;
std::cout<<"--------InputParams:-------------------------"<<std::endl;
std::cout<<"sourceXCDL: "<<sourceXCDL<<std::endl;
std::cout<<"targetXCDL: "<<targetXCDL<<std::endl;
std::cout<<"PCRFile: "<<PCRFile<<std::endl;
std::cout<<"CPRName: "<<CPRName<<std::endl;
std::cout<<"outDir: "<<outDir<<std::endl;
std::cout<<"applTarget: "<<applTarget<<std::endl;
std::cout<<"nameSourceXCDL: "<<nameSourceXCDL<<std::endl;
std::cout<<"nameTargetXCDL: "<<nameTargetXCDL<<std::endl;

for(i1=targetXCDLs.begin(); i1!=targetXCDLs.end(); ++i1)
   {
   std::cout<<"targetXCDL from list: "<<*(i1)<<std::endl;
   }
std::cout<<"---------------------------------------------"<<std::endl;
}

void InputParams :: cleanUp()
{
std::list <char*> :: iterator i1;

if(!targetXCDLs.empty())
  {
  for(i1=targetXCDLs.begin(); i1!=targetXCDLs.end(); ++i1)
     {
     if(*i1!=NULL) delete [] *i1;
     }
   targetXCDLs.clear();
   }
if(sourceXCDL!=NULL) sourceXCDL= NULL;
if(PCRFile!=NULL) PCRFile= NULL;
if(targetXCDL!=NULL) targetXCDL= NULL;

#ifdef DEBUG_RELEASE
std::cout<<"deleted InputParams obj.\n ";
#endif
}
/*-----------------------------------------------------------------------------
method: checkForAppropriateCompSet(...)
checks if there is an appropriate compSet for Input source and target XCDLs:
compares source XCDL value stored in class InputParams variable 'sourceXCDL'
with each source XCDL value stored in class MeasurementRequest variable 'sourceXCDL';
compares target XCDL values stored in class InputParams container 'targetXCDLs'
with each target XCDL value stored in class MeasurementRequest variable 'targetXCDL';
*/

void InputParams :: checkForAppropriateCompSet(ComparatorError *pError, Request2 *pPCR)
{
std::list <char*> :: iterator i1;
bool partner= false;
// in case multiple comparisons:
// check if there is a suitable <compSet> for src/tar files:
pError->error= -1;


for(pPCR->iterMR= pPCR->compSets->begin();
            pPCR->iterMR!= pPCR->compSets->end();
              pPCR->iterMR++)
   {
   if((*(pPCR->iterMR))->source.compare(sourceXCDL)==0)
     {
     for(i1=targetXCDLs.begin();i1!=targetXCDLs.end(); ++i1)
       {
       if((*(pPCR->iterMR))->target.compare((std::string(*i1)))==0)
         {
         partner= true;
         break;
         }
       }
     if(partner==false)
       {
       (*(pPCR->iterMR))->ok= false;
       pError->message= "Warning: Could not find appropriate target XCDL in PCR for " + std::string(sourceXCDL);
       pError->registrateError(pError->message);
       pError->message.clear();
       }
     else partner= false;
     }
   else
     {
     (*(pPCR->iterMR))->ok= false;
     pError->message= "Warning: Could not find appropriate source XCDL in PCR for " + std::string(sourceXCDL);
     pError->registrateError(pError->message);
     pError->message.clear();
     }
   }
}
//-----------------------------------------------------------------------------


