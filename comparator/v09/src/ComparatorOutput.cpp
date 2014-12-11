#include <fstream>
#include <stdio.h>
#include "defines.h"
#include "ComparisonConfigurationData.h"
#include "Comparer.h"
#include "XCDLIndexes.h"
#include "DataTagset.h"
#include "ComparatorOutput.h"
#include "ComparerResults.h"
#include "ComparatorError.h"
#include "InputParams.h"

ComparatorOutput :: ComparatorOutput(InputParams *pInputParams)
{
pIP= pInputParams;
}

ComparatorOutput :: ~ComparatorOutput()
{
}

// writes the root tag of copra file:
void ComparatorOutput :: createCopraOutputFrame()
{
if(pIP->outFlag==false)
  {
  // set outFlag to indicate that file is already created:
  pIP->outFlag= true;
  std::string copra="copra.xml";
  // in case option -i is set:
  if(pIP->copraName!="\0") copra= pIP->copraName;

  // incase option -o is set, create path for copra file:
  if(pIP->outDir.compare("\0")!=0) pIP->outDir= pIP->outDir + copra;
  else  pIP->outDir= copra;

  std::ofstream of;
  // create new copra file:
  of.open(pIP->outDir.c_str());

  of<<"<?xml version='1.0' encoding='UTF-8'?>\n";
  of<<"<copra xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n ";
  of<<"xmlns=\"http://www.planets-project.eu/xcl/schemas/xcl\" \n";
  of<<"xsi:schemaLocation=\"http://www.planets-project.eu/xcl/schemas/xcl  copra.xsd\">\n";

  of.close();
  }
}

// opens existing copra file and appends endtag:
void ComparatorOutput :: createCopraOutputEnd()
{
std::ofstream of;
of.open(pIP->outDir.c_str(), std::ios::app);
of<<"</copra>\n";
of.close();
}

// writes the root tag of cpr if not existing, appends compSet ( writes INCOMPLETE file!):
void ComparatorOutput :: createCopraOutputCore(Comparer *pC)
{
std::ofstream of;
std::map <std::string, ResultSet*> :: iterator i;
PropertyIndex* pProp= NULL;
std::vector <ValueSetIndex*> *pValueSets= NULL;
std::vector <ValueSetIndex*> :: iterator iVal;
ResultSet *ptrRS= NULL;
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

// create  root tag if not existing:
createCopraOutputFrame();

// add result for compSet:
of.open(pIP->outDir.c_str(), std::ios::app);

of<<"<set source=\""<<std::string(pIP->sourceXCDL)<<"\" ";
of<<"target=\""<<std::string(pIP->targetXCDL)<<"\">\n";

// add all comparison results:
if(pC->pRes->ResultBox!=NULL)
  {
for(i=pC->pRes->ResultBox->begin();i!=pC->pRes->ResultBox->end();++i)
   {
   ptrRS= i->second;

   // store propId to check for unsupported properties:
   IdOfpropertiesCompared.push_back(ptrRS->propId);

   // add property tag:
   of<<"<property id=\""<<ptrRS->propId<<"\" ";   // add attribute id
   of<<"name=\""<<i->first<<"\" ";                // add attribute name
   // add attribute state:
   if(ptrRS->compStatus==0) of<<"state=\"complete\">\n";
   else if(ptrRS->compStatus==1)  of<<"state=\"partial\">\n";
   else  // case: compStatus==2
     {
     of<<"state=\"missing\"/>\n";
     }

   if(ptrRS->compStatus!=2)
     {
// --------          <data>       ---------------------------------
     // add original data from XCDLs in case option 'outdata' is set:
     if(pIP->outPropvalFlag && (i->first).compare("normData")!=0)
       {
       of<<"<data>\n";
       // add source values:
       of<<"<src>\n";
       // add values of each valueSet
       pProp= pC->pIndexSrc->pObjIndex->getProperty(i->first);   //search for property X and get it
       if(pProp!=NULL)
         {
         pValueSets= pProp->getValueSetIndexBox();
         if(pValueSets!=NULL)
           {
           for(iVal=pValueSets->begin();iVal!=pValueSets->end();++iVal)
              {
              of<<"<value>"<<(*iVal)->getLabValVal()<<"</value>\n";
              }
           }
         }
       of<<"</src>\n";
       // add target values:
       of<<"<tar>\n";
       // add values of each valueSet
       pProp= pC->pIndexTar->pObjIndex->getProperty(i->first);   //search for property X (name) and get it
       if(pProp!=NULL)
         {
         pValueSets= pProp->getValueSetIndexBox();
         if(pValueSets!=NULL)
           {
           for(iVal=pValueSets->begin();iVal!=pValueSets->end();++iVal)
              {
              of<<"<value>"<<(*iVal)->getLabValVal()<<"</value>\n";
              }
           }
         }
       of<<"</tar>\n";
       of<<"</data>\n";
       }
     else if(pIP->outNormdataFlag && (i->first).compare("normData")==0)
       {
       of<<"<data>\n";
       // add source values:
       of<<"<src>\n";
       if(pC->pND1->normDataType.compare("image")==0)
         {
         of<<"<value>"<<pC->pND1->normDataAsIs<<"</value>\n";
         }
       else
         {
         if(pC->pND1->normDataAsIs!=NULL)
         of<<"<value>"<<pC->pND1->normDataAsStdStr<<"</value>\n";
         }
       of<<"</src>\n";
       // add target values:
       of<<"<tar>\n";
       if(pC->pND2->normDataType.compare("image")==0)
         {
         of<<"<value>"<<pC->pND2->normDataAsIs<<"</value>\n";
         }
       else
         {
         if(pC->pND2->normDataAsIs!=NULL)
         of<<"<value>"<<pC->pND2->normDataAsStdStr<<"</value>\n";
         }
       of<<"</tar>\n";
       of<<"</data>\n";
       }

//--------------   end of: <data>     ------------------------------------

//--------------     <metrics>      --------------------------------------
     // add metrics:
     of<<"<metrics>\n";
     propMetrics= ptrRS->deliverPropMetrics();
     propertySets= pC->pCoco->pCompSet->deliverMRPropertySets();
     for(i2=propMetrics->begin();i2!=propMetrics->end();++i2)
        {
        for(i4= propertySets->begin();i4!= propertySets->end();i4++)
           {
           pMetricSet= *i4;
           if(ptrRS->propName.compare(pMetricSet->propertyName)) {continue;}
           else // case: property name matched
             {
             matchedFlag=1;
             metricSets= pMetricSet->getMetricSets();
             for(i5=metricSets->begin();i5!=metricSets->end();i5++)
                {
                if(!(i2->first).compare(i5->first))
                  {
                  of<<"<metric id=\""<<i2->first<<"\" ";
                  of<<"name=\""<<i5->second<<"\">\n";
                  of<<"<result ";
                  // check string for prefix "code_", set state attribute and insert content:
                  tmp= i2->second;
                  pos1= tmp.find("code_", 0);
                  if(tmp.find("code_", 0)==std::string::npos)
                    {
                    of<<"state=\"ok\">";
                    }
                  else
                    {
                    of<<"state=\"failed\">";
                    }
                  of<<i2->second<<"</result>\n";
                  of<<"</metric>\n";
                  }
                else {/*not matched*/}
                }
             }
           if(matchedFlag!=0)  break;
           }
         }
      of<<"</metrics>\n";
// ---------------------    end of: <metrics>     ------------------------
      }
   // close property tag:
   of<<"</property>\n";
   }
  } // end of ResultBox check

//  ---------------     missing properties       ---------------------------
// check if there are any requested properties which were not comparable
// (not comparable, because they are not present in at least one of the files)

int flag=0;
propertySets= pC->pCoco->pCompSet->deliverMRPropertySets();
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
     of<<"compStatus=\"missing\"/>\n";
     }
   flag=0;
   }

// close compSet and request2:
of<<"</set>\n";

of.close();
}

// this methods writes a !COMPLETE! copra file:
void ComparatorOutput :: createCopraErrorOutput(std::string errorStatement)
{
createCopraOutputFrame();
std::ofstream of;
// create new copra file:
of.open(pIP->outDir.c_str());

of<<"<?xml version='1.0' encoding='UTF-8'?>\n";
of<<"<copra xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n ";
of<<"xmlns=\"http://www.planets-project.eu/xcl/schemas/xcl\"\n ";
of<<"xsi:schemaLocation=\"http://www.planets-project.eu/xcl/schemas/xcl  copra.xsd\">\n";
of<<"<error>"<<errorStatement<<"</error>\n";
of<<"</copra>\n";

of.close();
}

// extract src filename for Copra output in case of dir:
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
