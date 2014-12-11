#include <fstream>
#include <stdio.h>
#include "defines.h"
#include "Request2Input.h"
#include "Comparer.h"
#include "ComparatorOutput.h"
#include "ComparerResults.h"
#include "ComparatorError.h"
#include "InputParams.h"
#include "timestamp.h"


ComparatorOutput :: ComparatorOutput(InputParams *pInputParams)
{
pIP= pInputParams;
}

ComparatorOutput :: ComparatorOutput() : pIP(NULL)
{
}

ComparatorOutput :: ~ComparatorOutput()
{
pIP= NULL;
}

// writes the root tag of cpr file (INCOMPLETE!):
void ComparatorOutput :: createCPROutputFrame()
{
// create timestamped CPR name:
if(pIP->outFlag==false)
  {
  // set outFlag to indicate that file is already created:
  pIP->outFlag= true;
  std::string cpr="cpr.cpr";
  // in case option -i is set:
  if(pIP->CPRName!="\0") cpr= pIP->CPRName;

  // incase option -o is set, create path for cpr file:
  if(pIP->outDir.compare("\0")!=0) pIP->outDir= pIP->outDir + cpr;
  else  pIP->outDir= cpr;

  std::ofstream of;
  // create new cpr file:
  of.open(pIP->outDir.c_str());

  of<<"<?xml version='1.0' encoding='UTF-8'?>\n";
  of<<"<cpResponse xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n ";
  of<<"xmlns=\"http://www.planets-project.eu/xcl/schemas/xcl\" \n";
  of<<"xsi:schemaLocation=\"http://www.planets-project.eu/xcl/schemas/xcl  cprPP5.xsd\">\n";

  of.close();
  }
}

// opens existing CPR file and appends endtag:
void ComparatorOutput :: createCPROutputEnd()
{
std::ofstream of;
of.open(pIP->outDir.c_str(), std::ios::app);
of<<"</cpResponse>\n";
of.close();
}

// writes the root tag of cpr if not existing, appends compSet ( writes INCOMPLETE file!):
void ComparatorOutput :: outputForPlato(Comparer *pC)
{
std::ofstream of;
ResultSet *ptrRS= NULL;
std::map <std::string, ResultSet*> :: iterator i;
std::map <std::string, std::string> *propMetrics= NULL; // results of comparison from ComparerResults (first=id of metric, second= result)
std::map <std::string, std::string> :: iterator i2;
std::map <std::string, std::string> *metricSets= NULL; // requested Metrics (first= id of Metric, second= name of metric)
std::map <std::string, std::string> :: iterator i5;
std::list <std::string> IdOfpropertiesCompared;
std::list <std::string> :: iterator i3;
std::list <MetricSet*> *propertySets;
std::list <MetricSet*> :: iterator i4;
MetricSet *pMetricSet;
int matchedFlag=0, pos1;
std::string tmp;

// create timestamped CPR (only for stand-alone version!):
//std::string timestampedCPRName = "CPR_" + timestring() + ".cpr";

// create  root tag if not existing:
createCPROutputFrame();

// add result for compSet:
of.open(pIP->outDir.c_str(), std::ios::app);

of<<"<compSet id=\""<<pIP->compsetCount<<"\" source=\""<<std::string(pIP->sourceXCDL)<<"\" ";
of<<"target=\""<<std::string(pIP->targetXCDL)<<"\">\n";

// add all comparison results:
for(i=pC->pRes->ResultBox->begin();i!=pC->pRes->ResultBox->end();++i)
   {
   ptrRS= i->second;

   // store propId to check for unsupported properties:
   IdOfpropertiesCompared.push_back(ptrRS->propId);

   // add property tag:
   of<<"<property id=\"";
   of<<ptrRS->propId<<"\" name=\""<<i->first<<"\" ";
   if((ptrRS->propUnit).compare("-1")) of<<"unit=\""<<ptrRS->propUnit<<"\" ";
   // get compStatus from pRes->compStatus:
   if(ptrRS->compStatus==0) of<<"compStatus=\"complete\">\n";
   else if(ptrRS->compStatus==1)  of<<"compStatus=\"partial\">\n";
   else of<<"compStatus=\"failed\">\n";

   // now add the values:
   of<<"<values type=\""<<ptrRS->pV->type<<"\">\n";

   // add src and tar values:
   of<<"<src>"<<ptrRS->pV->srcValue<<"</src>\n";
   of<<"<tar>"<<ptrRS->pV->tarValue<<"</tar>\n";
   of<<"</values>\n";

   // add metrics:
   propMetrics= ptrRS->deliverPropMetrics();
   propertySets= pC->pRequest2->pMR->deliverMRPropertySets();
   for(i2=propMetrics->begin();i2!=propMetrics->end();++i2)
      {
      for(i4= propertySets->begin();i4!= propertySets->end();i4++)
         {
         //std::cout<<"Propertysets Schleife..."<<std::endl;
         pMetricSet= *i4;
         if(ptrRS->propId.compare(pMetricSet->propertyID)) {/*std::cout<<ptrRS->propId<<" "<<pMetricSet->propertyID;system("PAUSE")*/;continue;}
         else // case: property name matched
           {
           matchedFlag=1;
          // std::cout<<"Matched:"<<ptrRS->propId<<" "<<pMetricSet->propertyID;system("PAUSE");
           metricSets= pMetricSet->getMetricSets();
           for(i5=metricSets->begin();i5!=metricSets->end();i5++)
              {

              if(!(i2->first).compare(i5->first))
                {
                //std::cout<<i2->first<<" "<<i2->second<<" "<<i5->first<<" "<<i5->second;system("PAUSE");
                of<<"<metric id=\""<<i2->first<<"\" ";
                of<<"name=\""<<i5->second<<"\" ";

                // check string for prefix "ERRORCODE_" and set acc. attribute:
                tmp= i2->second;
                pos1= tmp.find("ERRORCODE_", 0);
                if(tmp.find("ERRORCODE_", 0)!=std::string::npos)
                  {
                  of<<"error=\""<<i2->second<<"\"/>\n";
                  }
                else
                  {
                  of<<"result=\""<<i2->second<<"\"/>\n";
                  }
                }
              else {/*std::cout<<"Not matched: "<<i2->first<<" "<<i2->second<<" "<<i5->first<<" "<<i5->second<<std::endl;system("PAUSE");*/}
             }
           if(matchedFlag!=0)  break;
           }
         }
       }
    // close set and property:
    of<<"</property>\n";
    }

// check if there are any requested properties which were not comparable
// (not comparable, because they are not present in at least one of the files)

int flag=0;
propertySets= pC->pRequest2->pMR->deliverMRPropertySets();
for(i4= propertySets->begin();i4!= propertySets->end();i4++)
   {
   pMetricSet= *i4;
   for(i3= IdOfpropertiesCompared.begin();i3!= IdOfpropertiesCompared.end();i3++)
      {
      if(!(pMetricSet->propertyID).compare((*i3))) flag=1;
      }
   if(flag==0)
     {
     of<<"<property id=\""<<pMetricSet->propertyID<<"\" name=\""<<pMetricSet->propertyName<<"\" ";
     of<<"compStatus=\"failed\"/>\n";
     }
   flag=0;
   }
// close compSet and request2:
of<<"</compSet>\n";

of.close();
}
// this methods writes a !COMPLETE! cpr file:
void ComparatorOutput :: createCPRErrorOutput(std::string errorStatement)
{
//std::string timestampedCPRName = "CPR_" + timestring() + ".cpr";
createCPROutputFrame();
std::ofstream of;
// create new cpr file:
of.open(pIP->outDir.c_str());

of<<"<?xml version='1.0' encoding='UTF-8'?>";
of<<"<cpResponse xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
of<<"xmlns=\"http://www.planets-project.eu/xcl/schemas/xcl\" ";
of<<"xsi:schemaLocation=\"http://www.planets-project.eu/xcl/schemas/xcl  cprPP5.xsd\">";
of<<"<error>"<<errorStatement<<"</error>";
of<<"</cpResponse>";

of.close();
}

// writes a a cpr with input error message (!COMPLETE! file):
void ComparatorOutput :: createInputParamsErrorOutput()
{
ComparatorError CompErr;

#ifdef DEBUG_GENERAL
std::cout<<"Invalid usage of Comparator. Usage: comparator [source-XCDL] [target-XCDL] [-c PCR] [options  <-o outputDir>]"<<std::endl;
system("PAUSE");
#endif

CompErr.registrateError("Invalid usage of Comparator. Usage: comparator [source-XCDL] [target-XCDL] [-c PCR]");
CompErr.printLogfile();
std::ofstream of;
of.open("cpr.cpr");
of<<"<?xml version='1.0' encoding='UTF-8'?>";
of<<"<cpResponse xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
of<<"xmlns=\"http://www.planets-project.eu/xcl/schemas/xcl\" ";
of<<"xsi:schemaLocation=\"http://www.planets-project.eu/xcl/schemas/xcl  cprPP5.xsd\">";
of<<"<error>Invalid usage of Comparator. Usage: comparator [source-XCDL] [target-XCDL] [-c PCR]</error>";
of<<"</cpResponse>";

of.close();
}

// creates a new cpr file if nozt existing, appends a compSet with error message (!INCOMPLETE! file):
void ComparatorOutput :: createCPRCompsetErrorOutput(std::string errorStatement, MeasurementRequest* pMR)
{
std::ofstream of;
if(pIP->outFlag==false)
  {
  createCPROutputFrame();
  pIP->outFlag=true;
  }

// append erroneous compSet:
of.open(pIP->outDir.c_str(), std::ios::app);

of<<"<compSet id=\""<<pIP->compsetCount<<"\" source=\""<<std::string(pIP->sourceXCDL)<<"\" ";
of<<"target=\""<<std::string(pIP->targetXCDL)<<"\">\n";
of<<"<error>"<<errorStatement<<"</error>\n";
of<<"</compSet>\n";

of.close();
}


// extract src filename for CPR output in case of dir:
std::string ComparatorOutput :: extractXCDLFileName(char* dir)
{
std::string XCDLFileName;
std::string inputXCDL= dir;
int pos1= inputXCDL.rfind("\\");
int pos2;

if(inputXCDL.rfind("\\")!=std::string::npos)
  {
  XCDLFileName= inputXCDL.substr(pos1+1, inputXCDL.length());
  pos2= inputXCDL.rfind(".");
  if(inputXCDL.rfind(".")!=std::string::npos)
    {
    inputXCDL= inputXCDL.substr(pos1+1, pos2-pos1-1);
    }
  else inputXCDL= inputXCDL.substr(pos1+1, inputXCDL.length()-pos1-1);
  }
else
  {
  pos1= inputXCDL.rfind("/");
  if(inputXCDL.rfind("/")!=std::string::npos)
    {
    XCDLFileName= inputXCDL.substr(pos1+1, inputXCDL.length());
    pos2= inputXCDL.rfind(".");
    if(inputXCDL.rfind(".")!=std::string::npos)
      {
      inputXCDL= inputXCDL.substr(pos1+1, pos2-pos1-1);
      }
    else inputXCDL= inputXCDL.substr(pos1+1, inputXCDL.length()-pos1-1);
    }
  else
    {
    XCDLFileName= inputXCDL;
    pos1=inputXCDL.rfind(".");
    if(inputXCDL.rfind(".")!=std::string::npos)
      {
      inputXCDL= inputXCDL.substr(0, pos1);
      }
    }
  }
return XCDLFileName;
}
