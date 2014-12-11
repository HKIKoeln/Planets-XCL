/***************************************************
FPMUnit.cpp

DESCRIPTION: Core code for FPM processing

    * This module is part of the XCL software system
    *
    * Copyright (C) 2007 - 2008 University of Cologne,
                    Historisch-Kulturwissenschaftliche Informationsverarbeitung
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
****************************************************************************/
//#define DEBUG_CONSOLE
#ifdef DEBUG_CONSOLE
#include <iostream>
#endif

#include <string>
#include <list>
#include <fstream>
#include <dirent.h>
#include "boost/regex.hpp"
#include "FPMUnit.h"
#include "FPMUnitError.h"
#include "FileHandler.h"

FPMUnit :: FPMUnit()
{
            pFPMError= new FPMUnitError();
            pFH= new FileHandler();
            outputDir="\0";
            }

FPMUnit :: ~FPMUnit()
{
            delete pFPMError;
            delete pFH;
            }


std::string FPMUnit :: parseArgs(int argc, char *argv[])
{
bool outflag= false;
bool puidsFlag= false;
std::string puids="\0";

for(int c=1;c<argc;c++)
   {
   if(!strcmp(argv[c], "-o"))
     {
     if(argc==3)
       {
       pFPMError->registrateError("IN_0", "\0");
       createFPMUnitOutput(pFPMError->returnData);
       return "\0";
       }
     else
       {
       if(outflag==true)
         {
         pFPMError->registrateError("IN_0", "\0");
         createFPMUnitOutput(pFPMError->returnData);
         return "\0";
         }
       else
         {
         outflag= true;
         continue;
         }
       }
     }
   else
     {
     if(outflag == true)
       {
       outputDir= argv[c];
       }
     else
       {
       if(puidsFlag == false)
         {
         puids= argv[c];
         puidsFlag= true;
         }
       else
         {
         pFPMError->registrateError("IN_1", "\0");
         createFPMUnitOutput(pFPMError->returnData);
         return "\0";
         }
       }
     }
  }
return puids;
}

void FPMUnit :: executeFPMUnitCore(std::string puids)
{
  std::string puid;
  std::string FPMAnswer;
  std::string propList;
  bool ok;
  int action, pos1, except;
  std::list <std::string>* propLists;


  try
    {
    // check syntax of input PUID list:
    ok= checkInputSyntax(puids);
    if(!ok) {throw except=1;}

    // load list of all supported PUIDs into list 'supportedPuids':
    ok=loadPUIDList("res/PUIDList.txt");
    if(!ok) {throw except=2;}

    // get single puids, check if supported, load for output:
    for(unsigned int i=0;i<puids.size();)
       {
       pos1= puids.find(":",i);
       if(puids.find(":",i)!=std::string::npos)
         {
         puid=puids.substr(i,pos1-i);
         //std::cout<<"PUIDS ALL:"<<puids<<std::endl;
         //std::cout<<"PUID PARSED:"<<puid<<"\n";//std::cin>>stop;
         if(i==0) propLists= new std::list <std::string>;

         ok=checkPUID(puid);
         if(ok)
           {
           //get property list for format X from  'fpm'-directory
           propList=getPropertiesList(puid);//  std::cout<<propList<<"\n";
           if(propList.compare("\0")!=0)
             {
             propLists->push_back(propList);
             i= pos1+1;  //std::cout<<"I="<<i<<"\n";
             }
           else {throw except=3;}
           }
         else
           {
           propList="<format puid=\"" + puid +"\" status=\"unavailable\"/>";
           propLists->push_back(propList);
           i=pos1+1;
           continue;
           }
         }
       else break; // Fehler abfangen!!
       }
    ok= createFPMUnitOutput(propLists);
    if(!ok) throw except=3;
    }

  catch(int except)
      {
      if(except==1)
        {
        #ifdef DEBUG_CONSOLE
        std::cout<<"Input error: PUIDList syntax not correct\n";
        #endif
        pFPMError->registrateError("FPM_2", "\0");
        action= pFPMError->getAction();
        }
      else if(except==2)
        {
        #ifdef DEBUG_CONSOLE
        std::cout<<"Unable to load PUIDList.txt\n";
        #endif
        pFPMError->registrateError("FPM_1", "\0");
        action= pFPMError->getAction();
        }
      else if(except==3)
        {
        #ifdef DEBUG_CONSOLE
        std::cout<<"Unable to load property list file.\n";
        #endif
        if(!supportedPUIDs->empty()) supportedPUIDs->clear();
        delete supportedPUIDs;
        if(!propLists->empty()) propLists->clear();
        delete propLists;
        action= pFPMError->getAction();
        }
      // actions for returning to client:
      if(action==2)
        {
        FPMAnswer= pFPMError->returnData;
        FPMAnswer= createFPMUnitOutput(FPMAnswer);
        return;
        }
      }

   if(!supportedPUIDs->empty()) supportedPUIDs->clear();
   delete supportedPUIDs;

   return;
};



bool FPMUnit :: checkInputSyntax(std::string puids)
{
 std::string errorcode="FPM_0";

 try
   {
   boost::regex re("(((fmt/[0-9]+:)|(x-fmt/[0-9]+:))+)");
   if(!boost::regex_match(puids, re)) {errorcode="PUIDReq_2"; throw errorcode;}
   else  {return true;}
   }
 catch(std::string errorcode)
   {
   pFPMError->registrateError(errorcode, "\0");
   return false;
   }

return true;
};


bool FPMUnit :: loadPUIDList(std::string filename)
{
 std::string tmp, puid, errorcode;
 int pos1, pos2;

 try
   {
   tmp= pFH->loadFile(filename, pFPMError);
   if(tmp.compare("\0")==0) {throw false;}   //  std::cout<<tmp<<std::endl;

   pos1= tmp.find("#");
   if(tmp.find("#")!=std::string::npos)
     {
     tmp=tmp.substr(0,pos1);
     }
   else {errorcode= "FPM_1"; throw errorcode;}

   supportedPUIDs= new std::list <std::string> ();

   for(pos1=0;;)
      {
      pos2=tmp.find(":",pos1);
      if(tmp.find(":",pos1)!=std::string::npos)
        {
        puid=tmp.substr(pos1,pos2-pos1); // std::cout<<"puid:"<<puid<<"\n";
        supportedPUIDs->push_back(puid);
        pos1= pos2+1;
        }
      else break;
      }
   if(supportedPUIDs->empty()) { throw 1;}
   }

 catch(bool)
   {
   return false;
   }
 catch(std::string errorcode)
   {
   pFPMError->registrateError(errorcode, "\0");
   return false;
   }
 catch(...)
   {
   supportedPUIDs->clear();
   delete supportedPUIDs;
   errorcode="FPM_0";
   pFPMError->registrateError(errorcode, "\0");
   return false;
   }
return true;
};


bool FPMUnit :: checkPUID(std::string argvpuid)
{
 std::list <std::string> :: iterator iPUIDList;

    //std::cout<<" checkPUID: PUID1:"<<puid<<std::endl;
 for(iPUIDList=supportedPUIDs->begin();iPUIDList!=supportedPUIDs->end();iPUIDList++)
    {
    //std::cout<<"PUID2:"<<*iPUIDList<<"PUID2"<<std::endl;
    if(argvpuid.compare((*iPUIDList))==0) {/*std::cout<<"EQUAL"*/; return true;}
    }
 return false;
};


std::string FPMUnit :: getPropertiesList(std::string puid)
{
std::string tmp, newPUID, errorcode="FPM_0";

// replace slash in puid with _ :
int pos1= puid.find("/");
if(puid.find("/")!=std::string::npos) puid.replace(pos1,1,"_");
//std::cout<<puid<<std::endl;

std::string filename=puid+".fpm";

// tiff and png:
 if(!filename.compare("fmt_7.fpm")  ||
    !filename.compare("fmt_8.fpm")  ||
    !filename.compare("fmt_9.fpm")  ||
    !filename.compare("fmt_10.fpm") ||
    !filename.compare("fmt_152.fpm")  ||
    !filename.compare("fmt_153.fpm")  ||
    !filename.compare("fmt_154.fpm")  ||
    !filename.compare("fmt_155.fpm")  ||
    !filename.compare("fmt_156.fpm")  ||
    !filename.compare("x-fmt_387.fpm")  ||
    !filename.compare("x-fmt_388.fpm")  ||
    !filename.compare("x-fmt_399.fpm") )
   {
   try
     {
     tmp=pFH->loadFile("res/fpm/fmt_tiff.fpm", pFPMError);
     if(tmp.compare("\0")==0) {errorcode="FPM_3"; throw errorcode;}
     else
       {
       pos1= tmp.find("puid='fmt_10_tiff'");
       if(tmp.find("puid='fmt_10_tiff'")!=std::string::npos)
         {
         newPUID= "puid=\"" + puid + "\"";
         tmp.replace(pos1, 18, newPUID);
         //std::cout<<tmp;
         }
       }
     }
   catch(std::string errorcode)
     {
     pFPMError->registrateError(errorcode, "res/fpm/fmt_tiff.fpm");
     }
   }
 else if(!filename.compare("fmt_11.fpm") ||
         !filename.compare("fmt_12.fpm")  ||
         !filename.compare("fmt_13.fpm") )
   {
   try
     {
     tmp=pFH->loadFile("res/fpm/fmt_png.fpm", pFPMError);
     if(tmp.compare("\0")==0) {errorcode="FPM_3"; throw errorcode;}
     else
       {
       pos1= tmp.find("puid='fmt_13_png'");
       if(tmp.find("puid='fmt_13_png'")!=std::string::npos)
         {
         newPUID= "puid=\"" + puid + "\"";
         tmp.replace(pos1, 17, newPUID);
         //std::cout<<tmp;
         }
       }
     }
   catch(std::string errorcode)
     {
     pFPMError->registrateError(errorcode, "res/fpm/fmt_png.fpm");
     }
   }
 else
   {
   try
     {
     tmp=pFH->loadFile("res/fpm/fmt_imageMagick.fpm", pFPMError);
     if(tmp.compare("\0")==0) {errorcode="FPM_3"; throw errorcode;}
     else
       {
       pos1= tmp.find("puid='fmt_imageMagick'");
       if(tmp.find("puid='fmt_imageMagick'")!=std::string::npos)
         {
         newPUID= "puid=\"" + puid + "\"";
         tmp.replace(pos1, 22, newPUID);
         //std::cout<<tmp;
         }
       }
     }
   catch(std::string errorcode)
     {
     pFPMError->registrateError(errorcode, filename);
     }
   }
 return tmp;
};


bool FPMUnit :: createFPMUnitOutput(std::list <std::string>* propLists)
{
  std::list <std::string> :: iterator i;
  std::ofstream file;

  bool ok;
  try
    {
    if(!(outputDir.compare("\0")))  file.open("fpm.fpm",std::ios_base::out);
    else
      {
      ok= checkIfDirectoryIs(outputDir.c_str());
      if(ok)
        {
        outputDir= outputDir + "fpm.fpm";
        file.open(outputDir.c_str(),std::ios_base::out);
        }
      else
        {
        pFPMError->registrateError(std::string("Couldn't create fpm file. Directory does not exist."), "\0");
        }
      }
    }
  catch(std::string)
    {
    #ifdef DEBUG_CONSOLE
    std::cerr<<"Could not open fpm output file."<<std::endl;
    #endif
    pFPMError->registrateError("FPM_4", "fpm.fpm");
    return false;
    }

  file<<"<fpmResponse>";
  for(i=propLists->begin();i!=propLists->end();i++)
     {
     file<<*i;
     }
  file<<"</fpmResponse>";

  file.close();

  if(!propLists->empty()) propLists->clear();
  delete propLists;

  return true;
};


bool FPMUnit :: createFPMUnitOutput(std::string error)
{
  std::ofstream file;
  bool ok;

  try
    {
    if(!(outputDir.compare("\0")))   file.open("fpm.fpm",std::ios_base::out);
    else
      {
      ok= checkIfDirectoryIs(outputDir.c_str());
      if(ok)
        {
        outputDir= outputDir + "fpm.fpm";
        file.open(outputDir.c_str(),std::ios_base::out);
        }
      else
        {
        pFPMError->registrateError(std::string("Couldn't create fpm file. Directory does not exist."), "\0");
        }
      }
    }
  catch(std::string)
    {
    #ifdef DEBUG_CONSOLE
    std::cerr<<"Could not open fpm output file."<<std::endl;
    #endif
    pFPMError->registrateError("FPM_4", "fpm.fpm");
    return false;
    }

  file<<"<fpmResponse><fpmError>";
  file<<error;
  file<<"</fpmError></fpmResponse>";

  file.close();

  return true;
};

bool FPMUnit :: checkIfDirectoryIs(const char* dir)
{
DIR *pDir;
bool is= false;

pDir= opendir(dir);

if(pDir!= NULL)
  {
  is= true;
  closedir(pDir);
  }
return is;
}
