#include "defines.h"
#include "InputParams.h"
#include "ComparatorError.h"
#include "ComparisonConfigurationData.h"
#include "ComparatorOutput.h"


InputParams :: InputParams(ComparatorError *pError)
{
pErr= pError;
applTarget= 1;
sourceXCDL= NULL;
targetXCDL= NULL;
copraName= "\0";
cocoFile= NULL;
outDir= "\0";
nameSourceXCDL= "\0";
nameTargetXCDL= "\0";
validationFlag= true;
outFlag= false;
logFileFlag= true;
outPropvalFlag= false;
outNormdataFlag= false;
compsetCount= 0;
}


InputParams :: ~InputParams()
{
//cleanUp();
}

void InputParams :: parseArgs(int argc, char* argv[])
{
bool optionsFlag= false;
int c, i;
std::string tmp, arg1, arg2;
char* buffer;


    if(argc==1)
      {
      printHelp();
      return;
      }
    else if(argc==2 || argc==3 || argc==4)
      {
      std::cout<<"Not enough arguments. Try again. "<<std::endl;
      pErr->registrateError("Error: parseArgs(): Not enough arguments.");
      pErr->error= -1;
      return;
      //printHelp();
      }
    else if(argc>=5)
      {
      applTarget= 1;

      for(c=1;c<argc;c++)
         {
         if(strcmp(argv[c], "-o") && strcmp(argv[c], "-c") && strcmp(argv[c], "-nolog") && strcmp(argv[c], "-novalidation") && strcmp(argv[c], "-outdata") && optionsFlag==false)
           {
           if(c==2)
             {
             targetXCDL= new char(strlen(argv[c]));
             targetXCDL= argv[c];
             targetXCDLs.push_back(targetXCDL);
             }
           else if(c==1)
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
             std::cout<<"Not enough or wrong order of arguments. Source and target XCDLs must be passed first. Try again. "<<std::endl;
             pErr->registrateError("Error: parseArgs(): Not enough or wrong order of arguments. Source and target XCDLs must be passed first.");
             pErr->error= -1;
             return;
             }
           else
             {
             if(!strcmp(argv[c], "-c"))
               {
               c++;
               if(c<argc)
                 {
                 cocoFile= new char(strlen(argv[c]));
                 cocoFile= argv[c];
                 }
               else
                 {
                 std::cout<<"Lacking argument. Expected indication of Comparator configuration file after -c. Try again. "<<std::endl;
                 pErr->registrateError("Error: parseArgs(): Lacking argument. Expected indication of Comparator configuration file after -c. Try again. ");
                 pErr->error= -1;
                 return;
                 }
               }
             else if(!strcmp(argv[c], "-nolog"))
               {
               logFileFlag= false;
               }
             else if(!strcmp(argv[c], "-novalidation"))
               {
               validationFlag= false;
               }
             else if(!strcmp(argv[c], "-outdata"))
               {
               c++;
               if(c<argc)
                 {
                 tmp= argv[c];
                 int pos1= tmp.find("|");
                 if(tmp.find("|")!=std::string::npos)
                   {
                   if(pos1!=0)   // in case separator is on first position
                     {
                     arg1= tmp.substr(0, pos1);
                     if(arg1.compare("PROPVAL") && arg1.compare("NORMDATA"))
                       {
                       std::cout<<"Unknown argument: "<<arg1<<" for option -outdata . Try again."<<std::endl;
                       pErr->registrateError("Error: parseArgs(): Unknown argument for option -outdata. Argument: ", arg1);
                       pErr->error= -1;
                       return;
                       }
                     }
                   else
                     {
                     std::cout<<"Wrong syntax for option -outdata. Use separator only once and only between the allowed arguments PROPVAL and NORMDATA. Your input: "<<tmp<<". Try again. "<<std::endl;
                     pErr->registrateError("Error: parseArgs(): Lacking arguments. Input: ", arg1);
                     pErr->error= -1;
                     return;
                     }
                   if(pos1!=(tmp.length()-1))  // in case separator is on last position
                     {
                     arg2= tmp.substr(pos1+1, tmp.length());
                     if(arg2.compare("PROPVAL") && arg2.compare("NORMDATA"))
                       {
                       std::cout<<"Unknown argument: "<<arg2<<" for option -outdata . Try again."<<std::endl;
                       pErr->registrateError("Error: parseArgs(): Unknown argument for option -outdata. Argument: ", arg2);
                       pErr->error= -1;
                       return;
                       }
                     }
                   else
                     {
                     std::cout<<"Lacking arguments for option -outdata. Expected second argument after separator. Try again. "<<std::endl;
                     pErr->registrateError("Error: parseArgs(): Lacking arguments. Input: ", arg1);
                     pErr->error= -1;
                     return;
                     }
                   }
                 else
                   {
                   arg1= tmp;
                   if(arg1.compare("PROPVAL") && arg1.compare("NORMDATA"))
                     {
                     std::cout<<"Unknown argument: "<<arg1<<" for option -outdata . Try again."<<std::endl;
                     pErr->registrateError("Error: parseArgs(): Unknown argument for option -outdata. Argument: ", arg1);
                     pErr->error= -1;
                     return;
                     }
                   }

                 // set flags:
                 if(arg1.compare("PROPVAL") || arg2.compare("PROPVAL")) outPropvalFlag= true;
                 if(arg1.compare("NORMDATA") || arg2.compare("NORMDATA")) outNormdataFlag= true;
                 }
               else
                 {
                 std::cout<<"Lacking argument. Expected indication of option argument after -outdata. Try again. "<<std::endl;
                 pErr->registrateError("Error: parseArgs(): Lacking argument. Expected indication of option argument after -outdata. ");
                 pErr->error= -1;
                 return;
                 }
               }
             else if(!strcmp(argv[c], "-o"))
               {
               c++;
               if(c<argc)
                 {
                 outDir= std::string(argv[c]);
                 }
               else
                 {
                 std::cout<<"Lacking argument. Expected indication of output directory after -o. Try again. "<<std::endl;
                 pErr->registrateError("Error: parseArgs(): Lacking argument. Expected indication of output directory after -o. Try again. ");
                 pErr->error= -1;
                 return;
                 }
               }
             else
               {
               std::cout<<"Unknown argument "<<std::string(argv[c])<<". Try again. "<<std::endl;
               pErr->registrateError("Error: parseArgs(): Unknown argument: ", std::string(argv[c]), 0);              //CompOut.createInputParamsErrorOutput();
               pErr->error= -1;
               return;
               }
             }
           }
         }
      if(cocoFile==NULL)
        {
        std::cout<<"Lacking arguments. Comparator configuration file must be passed. Try again. "<<std::endl;
        pErr->registrateError("Error: parseArgs(): Lacking arguments. Expected indication of Comparator configuration file. Try again. ");
        pErr->error= -1;
        return;
        }
      }
pErr->error= 1;
return;
}


void InputParams :: printInputParams()
{
std::list <char*> :: iterator i1;
std::cout<<"--------InputParams:-------------------------"<<std::endl;
std::cout<<"sourceXCDL: "<<sourceXCDL<<std::endl;
std::cout<<"targetXCDL: "<<targetXCDL<<std::endl;
std::cout<<"cocoFile: "<<cocoFile<<std::endl;
std::cout<<"copraName: "<<copraName<<std::endl;
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
if(cocoFile!=NULL) cocoFile= NULL;
if(targetXCDL!=NULL) targetXCDL= NULL;

pErr= NULL;

#ifdef DEBUG_RELEASE
std::cout<<"deleted InputParams obj.\n ";
#endif
}
/*------------------------------------------------------------------------------
method: printHelp()
prints short information on how to handle Comparator to the command line
-------------------------------------------------------------------------------*/
void InputParams :: printHelp()
{
std::cout<<std::endl<<"Comparator (version 0.9)"<<std::endl;
std::cout<<"Written by HKI, University of Cologne"<<std::endl<<std::endl<<std::endl;

std::cout<<"USAGE of Comparator:"<<std::endl<<std::endl;
std::cout<<"comparator  [pathToSourceXCDLFile] [pathToTargetXCDLFile] \n[-c pathToComparatorConfigFile]"<<std::endl<<std::endl;
std::cout<<"ADDITIONAL OPTIONS: "<<std::endl;
std::cout<<"Calling Comparator without any arguments evokes help menu."<<std::endl;
std::cout<<"-o  [myOutputDirectory] : print Comparator results file (copra.xml) to indicated directory"<<std::endl;
std::cout<<"-nolog  :  disable output of logging file"<<std::endl;
std::cout<<"-novalidation  :  disable validation of input files"<<std::endl;
std::cout<<"-outdata [<PROPVAL|NORMDATA>]  :  force output of original XCDL data;";
std::cout<<" option PROPVAL: output of property values; option NORMDATA: output of normalised data.";
std::cout<<" Note: Use separator (|) if both are wanted for output.  "<<std::endl;

std::cout<<std::endl<<std::endl;
}


