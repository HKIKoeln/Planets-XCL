#include <stdlib.h>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include "defines.h"
#include "PropertyTagset.h"
#include "Request2Input.h"
#include "ComparatorError.h"
#include "ComparerMeasure.h"
#include "Comparer.h"
#include "ComparerResults.h"
#include "DataTagset.h"



// constructor called while constructing XCDLHandler object for XCDL1 parsing:
Comparer :: Comparer(Request2 *p, ComparatorError *pCE) : setA(NULL), setB(NULL)
{
pErr= pCE;
pRes= new Results();
pCompMeasure= new ComparerMeasure(pErr);
pRequest2= p;

#ifdef DEBUG_COMPARER
std::cout<<"New Comparer object..."<<std::endl;
#endif
}

Comparer :: ~Comparer()
{
cleanUpAll();
pRequest2= NULL;   // just set to NULL; will be deleted in called object...
pErr= NULL;        // ...as well
}

void Comparer :: cleanUpAll()
{
cleanUpSets(); // free containers and delete allocated memory
if(pRes!=NULL) {delete pRes; pRes=NULL;}  // calls cleanUp method in this object...
if(pCompMeasure!=NULL) {delete pCompMeasure; pCompMeasure= NULL;} //  ... as well

#ifdef DEBUG_RELEASE
std::cout<<"Released Comparer obj ...\n"<<std::endl;
#endif
}


void Comparer :: cleanUpSet(unsigned short setnb)
{
if(setnb==1)
  {
  if(setA!=NULL)
    {
    if(!setA->empty()) setA->clear();
    delete setA; setA= NULL;
    }
  }
else if(setnb==2)
  {
  if(setB!=NULL)
    {
    if(!setB->empty()) setB->clear();
    delete setB; setB= NULL;
    }
  }
}

void Comparer :: cleanUpSets()
{
cleanUpSet(1);
cleanUpSet(2);
}


// search for the requested metrics of property X and return related metricSet:
std::map <std::string, std::string>* Comparer :: getMetricsList(std::string propName, Request2 *pR2)
{
MetricSet *pMSet;
std::list <MetricSet*> :: iterator i;
std::map <std::string, std::string> *metricSet= NULL;
std::string str;

for(i=pR2->pMR->sets->begin();i!=pR2->pMR->sets->end();++i)
   {
   pMSet=*i;
   if((pMSet->propertyName).compare(propName)==0)
     {
     metricSet= pMSet->getMetricSets();
     }
   }
if(metricSet==NULL)
  {
  #ifdef DEBUG_COMPARER
  std::cout<<"No metrics requested for property "<<propName<<"."<<std::endl;
  #endif
  }
return metricSet;
}

// type-based comparison of single properties:
int Comparer :: compareProperties(PropertyTagset *p1, PropertyTagset *p2)
{
MetricSet *pMSet;
std::list <MetricSet*> :: iterator i;
std::map <std::string, ValueSet*> :: iterator iter1, iter2;
ValueSet *pValSet1, *pValSet2;
std::map <std::string, std::string> *metricsList= NULL;
std::map <std::string, std::string> :: iterator iter3;
std::string propName, type, metric, result, tmp;
int error, intRes, n;
double ratRes;
char s[13];
bool  chk;
enum labValTypes {XCLLabel, _int, rat, string, ISO8601, none};
labValTypes LVtype;
labValTypes LVtype1=none;

// create new resultBox object:
if(pRes->ResultBox==NULL) pRes->ResultBox= new std::map <std::string, ResultSet*> ();

try
  {
   // get the metrics from the input (request2) that are to be applied to
   // the property:
   for(i=pRequest2->pMR->sets->begin();i!=pRequest2->pMR->sets->end();++i)
      {
      pMSet=*i;
      if((pMSet->propertyName).compare(p1->propName)==0)
        {
//   %%%      create new objects:          %%%                          //
        pRes->pRS= new ResultSet();
        pRes->pRS->propMetrics= new std::map <std::string, std::string> ();
        pRes->pRS->pV= new Values();
        // get list of requested metrics:
        metricsList= pMSet->getMetricSets();
        // set property ID and unit using request2in data:
        pRes->pRS->propId= pMSet->propertyID;
        pRes->pRS->propUnit= pMSet->propertyUnit;
        }
      }
    // return in case of unrequested metrics
    if(metricsList==NULL)
      {
      #ifdef DEBUG_COMPARER
      std::cout<<"No metrics requested for property "<<p1->propName<<"."<<std::endl;
      #endif
      return 0;
      }

   // get valueSet objects:
   iter1= p1->valueSets->begin();
   pValSet1= iter1->second;
   iter2= p2->valueSets->begin();
   pValSet2= iter2->second;

   // check if there are labelled values at all:
   if(pValSet1->labVal==false)       throw error=1;
   else if(pValSet2->labVal==false)  throw error=1;

   #ifdef DEBUG_TMP
   std::cout<<pValSet1->labValType<<"=Type1"<<std::endl;
   std::cout<<pValSet2->labValType<<"=Type2"<<std::endl<<std::endl;
   #endif

   // check if  types of labelled values are the same:
   //if(!(pValSet1->labValType).compare(pValSet2->labValType))  compTypeInd= true;
   //else                                                      compTypeInd= false;

   for(tmp= pValSet1->labValType, n=1;n<=2;tmp=pValSet2->labValType, n++)
      {
      // get type of labelled value:
     if(!(tmp).compare("fixedLabel"))      LVtype=XCLLabel;
     else if(!(tmp).compare("int"))        LVtype=_int;
     else if(!(tmp).compare("XCLLabel"))   LVtype=XCLLabel;
     else if(!(tmp).compare("uint32"))     LVtype=_int;
     else if(!(tmp).compare("uint8"))      LVtype=_int;
     else if(!(tmp).compare("uint16"))     LVtype=_int;
     else if(!(tmp).compare("int8"))       LVtype=_int;
     else if(!(tmp).compare("int16"))      LVtype=_int;
     else if(!(tmp).compare("int32"))      LVtype=_int;
     else if(!(tmp).compare("uint32Rational"))        LVtype=rat;
     else if(!(tmp).compare("string"))     LVtype=string;
     else if(!(tmp).compare("ASCII"))      LVtype=string;
     else if(!(tmp).compare("ISO8601"))    LVtype=ISO8601;
     else if(!(tmp).compare("XCLLabel"))   LVtype=XCLLabel;
      else if(!(tmp).compare("rational"))        LVtype=rat;
     else throw error=2;

     if(LVtype1==none) LVtype1= LVtype;
     }
   //std::cout<<LVtype<<" "<<LVtype1<<std::endl; system("PAUSE");

   if(LVtype1!=LVtype)  {throw error=3;}

   // store type:
   //sprintf(labValType,"%d",LVtype);
   if(LVtype==_int) pRes->pRS->pV->type= "int";
   else if(LVtype==XCLLabel) pRes->pRS->pV->type= "XCLLabel";
   else if(LVtype==string) pRes->pRS->pV->type= "string";
   else if(LVtype==rat) pRes->pRS->pV->type= "rational";
   else if(LVtype==ISO8601) pRes->pRS->pV->type= "ISO8601";

       //std::cout<<"Type:"<<" "<<pRes->pRS->pV->type<<std::endl; system("PAUSE");

   // store values:
   pRes->pRS->pV->srcValue=  pValSet1->labValVal;
   pRes->pRS->pV->tarValue=  pValSet2->labValVal;

  // start type-based comparison for all required metrics (from input request2):
  for(iter3=metricsList->begin();iter3!=metricsList->end();iter3++)
     {
     #ifdef DEBUG_COMPARER
     std::cout<<std::endl<<"Comparing Property \""<<p1->propName<<"\" with Metric: "<<iter3->first<<std::endl;
     #endif

     metric= iter3->first;

     if(LVtype==XCLLabel)
       {
       if(!metric.compare("1"))  //  =equal
         {
         if(!(pValSet1->labValVal).compare(pValSet2->labValVal))
           {
           pRes->pRS->propMetrics->insert(make_pair(metric, "true"));
           }
         else pRes->pRS->propMetrics->insert(make_pair(metric, "false"));
         }
       else throw error=5;
       }

     else if(LVtype==_int)
       {
       // check if input string is a set of values or a single value; in a XCDL labVal string
       // representing a set of values the single values are space seperated (0x32)   ;
       // if the string represents a set of values, store values in a container of type vector
       if(pValSet1->labValVal.find(" ")!=std::string::npos && pValSet2->labValVal.find(" ")!=std::string::npos)
         {
         setA= new std::vector <int> ();
         storeValues(pValSet1->labValVal, setA);
         setB= new std::vector <int> ();
         storeValues(pValSet2->labValVal, setB);

         // comparison of sets
         if(!metric.compare("11"))   // =hammingDistance
           {
           intRes= pCompMeasure->doHammingDistance(setA, setB);
           if(intRes==-1) {pErr->errorcode="ERRORCODE_110"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           else
             {
             // convert to string again:
             sprintf (s, "%d", intRes);
             result= std::string(s);
             }
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("12"))   // =simpleMatchCoefficientN
           {
           //input of n:
           std::string metricName= iter3->second;
           int pos1= metricName.rfind("#");
           if(metricName.rfind("#")!=std::string::npos)
             {
             std::string ngram=metricName.substr(pos1+1);
             n= atoi(ngram.c_str());
             ratRes= pCompMeasure->doSimpleMatchCoefficientN(setA, setB, n);
             if(ratRes==-1.0) {pErr->errorcode="ERRORCODE_111";result=pErr->errorcode; pRes->pRS->compStatus=1;}
             else
               {
               // convert to string again:
               sprintf (s, "%f", ratRes);
               result= std::string(s);
               }
             }
           else
             {
             pErr->registrateError("Error: Comparer:compareNormData(): Wrong syntax for metric simpleMatchCoefficientN in PCR file. Expected attribute 'param'.", "ERRORCODE_200");
             result= pErr->errorcode;
             }

           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           //std::cout<<"Simple Match coefficient for normData: "<<result<<std::endl;
           //system("PAUSE");
           }
         else if(!metric.compare("17"))   // =L2Distance, euclidDistance
           {
           ratRes= pCompMeasure->doEuclidDistance(setA, setB);
           if(ratRes==-1.0) {pErr->errorcode="ERRORCODE_112";result=pErr->errorcode; pRes->pRS->compStatus=1;}
           else
             {
             // convert to string again:
             sprintf (s, "%f", ratRes);
             result= std::string(s);
             }
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           //std::cout<<"L2Distance for normData: "<<ratRes<<std::endl;
           //system("PAUSE");
           }
         else if(!metric.compare("20"))   // =RMSE
           {
           ratRes= pCompMeasure->doRMSE(setA, setB);
           if(ratRes==-1.0) {pErr->errorcode="ERRORCODE_150"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           else
             {
             // convert to string again:
             sprintf (s, "%f", ratRes);
             result= std::string(s);
             }
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           //std::cout<<"RMSE for normData: "<<ratRes<<std::endl;
           //system("PAUSE");
           }
         }
       else
         {
         if(!metric.compare("1"))   // = Equal
           {
           if(!(pValSet1->labValVal).compare(pValSet2->labValVal))
             {
             pRes->pRS->propMetrics->insert(make_pair(metric, "true"));
             }
           else pRes->pRS->propMetrics->insert(make_pair(metric, "false"));
           }
         else if(!metric.compare("2"))  // =intDiff
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doIntDiff(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_201"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("4"))  // =intSum
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doIntSum(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_202"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("6"))  // =intRatio
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doIntRatio(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_203"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("8"))  // =intProduct
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doIntProduct(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_204"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
          else if(!metric.compare("10"))  // =percDeviation
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doPercDev(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_210"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else throw error=5;
         }
       }
     else if(LVtype==rat)
       {
       if(pValSet1->labValVal.find(" ")!=std::string::npos)
         {
         #ifdef DEBUG_COMPARER
         std::cout<<"Error in compare(): rational number processing"; system("PAUSE");exit(0);
         #endif
         }
       else
         {
         if(!metric.compare("1"))   // = Equal
           {
           if(!(pValSet1->labValVal).compare(pValSet2->labValVal))
             {
             pRes->pRS->propMetrics->insert(make_pair(metric, "true"));
             }
           else pRes->pRS->propMetrics->insert(make_pair(metric, "false"));
           }
         else if(!metric.compare("3"))  // =ratDiff
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doRatDiff(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_205"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("5"))  // =ratSum
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doRatSum(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_206"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("7"))  // =ratRatio
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doRatRatio(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_207"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else if(!metric.compare("9"))  // =ratProduct
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doRatProduct(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_208"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
          else if(!metric.compare("10"))  // =percDeviation
           {
           chk= checkSizeOfVals(pValSet1, pValSet2);
           if(chk)
             {
             result= pCompMeasure->doPercDev(pValSet1, pValSet2);
             }
           else {pErr->errorcode="ERRORCODE_210"; result=pErr->errorcode; pRes->pRS->compStatus=1;}
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else throw error=5;
         }
       }
     else if(LVtype==string)
       {
       if(!metric.compare("15")) // levenstheinDistance
         {
         intRes= pCompMeasure->doLevenstheinDistance(pValSet1->labValVal, pValSet2->labValVal);
         if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%i", intRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         //std::cout<<"Levensthein Distance for string: "<<result<<std::endl;
         //system("PAUSE");
         }
       else
         {
         tmp="Error: compareProperties(): Requested metric is not defined for property " + p1->propName + ".";
         pErr->registrateError(tmp, "ERRORCODE_300");
         }
       }
     else
       {
       std::cout<<"Error: Yet unsupported data type."<<std::endl;
       system("PAUSE");
       exit(0);
       }
     }
  }
catch(int error)
  {
  if(error==1) pErr->registrateError("Error: Comparer::compareProperties(): no labelled values");
  else if(error==2) pErr->registrateError("Error: Comparer::compareProperties(): undefined labValType");
  else if(error==3) pErr->registrateError("Error: Comparer::compareProperties(): yet unsupported feature :labValType comparison");
  else if(error==4) pErr->registrateError("Error: Comparer::compareProperties(): uint32's larger than 2.147.483.648: feature not supported yet");
  //else if(error==5) {std::cout<<LVtype1;system("PAUSE");pErr->registrateError("Error: Comparer::compareProperties(): Requested metric for datatype not supported");}

  return -1;
  }

// throw ResultSet for property x into ResultBox:
pRes->ResultBox->insert(make_pair(p1->propName, pRes->pRS));

#ifdef DEBUG_COMPARER
pRes->printComparerResults(); system("PAUSE");
#endif

return 0;
}

/*****************************************************************************
***************            normData comparison      **************************
*****************************************************************************/
int Comparer :: compareNormData(PropertyTagset *p1, NormData *pND1, NormData *pND2)
{
MetricSet *pMSet;
std::list <MetricSet*> :: iterator i;
std::map <std::string, std::string> *metricsList= NULL;
std::map <std::string, std::string> :: iterator iter3;
std::string  metric, result;
std::string normDataType= pND1->normDataType;
int intRes, n;
double ratRes;
char s[25];

// create new resultBox object:
if(pRes->ResultBox==NULL) pRes->ResultBox= new std::map <std::string, ResultSet*> ();

try
  {
   // get the metrics from the input (request2) that are to be applied to
   // the normData:
   for(i=pRequest2->pMR->sets->begin();i!=pRequest2->pMR->sets->end();++i)
      {
      pMSet=*i;
      if((pMSet->propertyName).compare(p1->propName)==0)
        {
        //create new objects:
        pRes->pRS= new ResultSet();
        pRes->pRS->propMetrics= new std::map <std::string, std::string> ();
        pRes->pRS->pV= new Values();
        // get list of requested metrics:
        metricsList= pMSet->getMetricSets();
        // set property ID and unit using request2in data:
        pRes->pRS->propId= pMSet->propertyID;
        pRes->pRS->propUnit= pMSet->propertyUnit;
        }
      }
    // return in case of unrequested metrics
    if(metricsList==NULL)
      {
      #ifdef DEBUG_COMPARER
      std::cout<<"No metrics requested for normData."<<std::endl;
      #endif
      return -1;
      }

   // get type of normData:
   if(!pND1->normDataType.compare("image")) pRes->pRS->pV->type= "hex";
   else  pRes->pRS->pV->type= "string";

    #ifdef _STDVERSION
    // store src and tar values
    pRes->pRS->pV->srcValue=pND1->normDataAsIs;
    pRes->pRS->pV->tarValue=pND2->normDataAsIs;
    #endif
    #ifdef _IFSERVICEVERSION
    pRes->pRS->pV->srcValue="";
    pRes->pRS->pV->tarValue="";
    #endif
    #ifdef _RELEASEVERSION
    pRes->pRS->pV->srcValue="";
    pRes->pRS->pV->tarValue="";
    #endif

  // start type-based comparison for all required metrics (from input request2):
  for(iter3=metricsList->begin();iter3!=metricsList->end();iter3++)
     {
     #ifdef DEBUG_COMPARER
     std::cout<<std::endl<<"Comparing normData with Metric: "<<iter3->first<<std::endl;
     system("PAUSE");
     #endif

     metric= iter3->first;

     // TODO: compare normData with respect to datatype; supported 31/03/2008: uint
     // TODO: if there are more than one normData tagsets, check length of both
     // and pre-pare for comparison; supported 31/03/2008: exactly one normData tagset

     if(pND1->normDataType.compare("image")==0)
       {
       // TODO: comparison of sets
       if(!metric.compare("11"))   // =hammingDistance
         {
         intRes= pCompMeasure->doHammingDistance(pND1->normDataAsUint, pND2->normDataAsUint);
         if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%d", intRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         //std::cout<<"Hamming distance for normData: "<<intRes<<std::endl;
         //system("PAUSE");
         }
       else if(!metric.compare("12"))   // =simpleMatchCoefficientN
         {
         //input of n:
         std::string metricName= iter3->second;
         int pos1= metricName.rfind("#");
         if(metricName.rfind("#")!=std::string::npos)
           {
           std::string ngram=metricName.substr(pos1+1);
           n= atoi(ngram.c_str());
           ratRes= pCompMeasure->doSimpleMatchCoefficientN(pND1->normDataAsUint, pND2->normDataAsUint, n);
           if(ratRes==-1.0) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
           else
             {
             // convert to string again:
             sprintf (s, "%f", ratRes);
             result= std::string(s);
             }
           }
         else
           {
           pErr->registrateError("Error: Comparer:compareNormData(): Wrong syntax for metric simpleMatchCoefficientN in PCR file. Expected attribute 'param'.", "ERRORCODE_200");
           result= pErr->errorcode;
           }

         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         //std::cout<<"Simple Match coefficient for normData: "<<result<<std::endl;
         //system("PAUSE");
         }
       else if(!metric.compare("17"))   // =L2Distance
         {
         ratRes= pCompMeasure->doEuclidDistance(pND1->normDataAsUint, pND2->normDataAsUint);
         if(ratRes==-1.0) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%f", ratRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         //std::cout<<"L2Distance for normData: "<<ratRes<<std::endl;
         //system("PAUSE");
         }
       else if(!metric.compare("20"))   // =RMSE
         {
         ratRes= pCompMeasure->doRMSE(pND1->normDataAsUint, pND2->normDataAsUint);
         if(ratRes==-1.0) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%f", ratRes);
           result= std::string(s);
           }

         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         //std::cout<<"RMSE for normData: "<<result<<std::endl;
         //system("PAUSE");
         }
       }
     else if(pND1->normDataType.compare("text")==0)
       {
       if(!metric.compare("15"))
         {
         intRes= pCompMeasure->doLevenstheinDistance(pND1->normDataAsIs, pND2->normDataAsIs);
         if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%i", intRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         //std::cout<<"Levensthein Distance for string: "<<result<<std::endl;
         //system("PAUSE");
         }
       else
         {
         pErr->registrateError("Error: compareNormData(): Requested metric is not defined for normData.", "ERRORCODE_301");
         }
       }
     else
       {
       std::cout<<"Error: Yet unsupported data type."<<std::endl;
       #ifdef DEBUG_COMPARER
       system("PAUSE");
       exit(0);
       #endif
       }
     }
  }
catch(int error)
  {
  }

// throw ResultSet for normData into ResultBox:
pRes->ResultBox->insert(make_pair(p1->propName, pRes->pRS));

return 0;
}

////////////////////////////////////////////////////////////////////////////

bool Comparer :: checkSizeOfVals(ValueSet* pV1, ValueSet* pV2)
{
unsigned long i1, i2;
bool result;

 i1= strtoul(pV1->labValVal.c_str(), NULL, 0);
 i2= strtoul(pV2->labValVal.c_str(), NULL, 0);
 if(i1<=2147483647 && i2<=2147483648)   result=true;  // convert to long
 else result=false; // TODO: do somthing else

 return result;
}


int Comparer :: storeValues(std::string labValString, std::vector <int>* setX)
{
std::string a;
std::istringstream iss (labValString);
std::vector <int> :: iterator i;

for(;iss.good();)
   {
   iss >> a >> std::ws;
   setX->push_back(atoi(a.c_str()));
   }

#ifdef DEBUG_COMPARER
for(i=setX->begin(); i!=setX->end(); ++i)
   {
   std::cout<<*i<<std::endl;
   }
   std::cout<<"storeValues()"<<std::endl;
   system("PAUSE");
#endif
  return 0;
}




