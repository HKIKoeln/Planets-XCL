#include <stdlib.h>
#include <cstdlib>
#include <utility>   // for std::pair
#include <algorithm>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include "Comparer.h"
#include "defines.h"
#include "ComparisonConfigurationData.h"
#include "ComparatorError.h"
#include "ComparerMeasure.h"
#include "ComparerMeasureDataRef.h"
#include "ComparerMeasureValueSet.h"
#include "ComparerResults.h"
#include "DataTagset.h"
#include "XCDLIndexes.h"

Comparer :: Comparer(ComparatorError *pCE, XCDLIndex* pIdxSrc, XCDLIndex* pIdxTar)
{
pErr= pCE;
pIndexSrc= pIdxSrc;
pIndexTar= pIdxTar;
pPropIndexSrc= NULL;
pPropIndexTar= NULL;
pCoco= NULL;
pRes= NULL;
pCompMeasure= new ComparerMeasure(pErr);
pCompMeasureValueSet= new ComparerMeasureValueSet(pErr);
pCompMeasureDataRef= new ComparerMeasureDataRef(pErr, pIndexSrc, pIndexTar);
pND1= NULL;
pND2= NULL;
dataAsSingleStringsA= NULL;
dataAsSingleStringsB= NULL;
dataAsSingleIntsA= NULL;
dataAsSingleIntsB= NULL;
}

Comparer :: ~Comparer()
{
//cleanUpAll();
pCoco= NULL;   // just set to NULL; will be deleted in called objects...
pErr= NULL;        // ...
pIndexSrc= NULL;   // ...
pIndexTar= NULL;   // ...
pRes= NULL;        // ...
pND1= NULL;        // ...
pND2= NULL;        // ...
}

void Comparer :: compareNormData()
{
int _case=0;
MetricSet *pMSet;
std::list <MetricSet*> :: iterator i;
std::map <std::string, std::string> *metricsList= NULL;
std::string  metric, result;
int intRes, n;
double ratRes;
char s[25];   // used for type-conversion

// create new resultBox object if not done so far:
if(pRes->ResultBox==NULL) pRes->ResultBox= new std::map <std::string, ResultSet*> ();
//create new objects:
pRes->pRS= new ResultSet();
pRes->pRS->propMetrics= new std::map <std::string, std::string> ();
pRes->pRS->pV= new Values();

try
  {
   // check if types of both normData is equal:
   if(pND1->normDataType.compare(pND2->normDataType)) {throw _case=1;}

   // get the metrics from the input (ComparisonConfiguration) that are to be applied to
   // the normData:
   for(i=pCoco->pCompSet->sets->begin();i!=pCoco->pCompSet->sets->end();++i)
      {
      pMSet=*i;
      if(!(pMSet->propertyName).compare("normData"))
        {
        // get list of requested metrics:
        metricsList= pMSet->getMetricSets();
        // set attribute values:
        pRes->pRS->propId= pMSet->propertyID;
        pRes->pRS->propName= "normData";
        }
      }
    // return in case of no request for any metrics:
    if(metricsList==NULL)
      {
      #ifdef DEBUG_COMPARER
      std::cout<<"Warning: No metrics requested for normData."<<std::endl;
      #endif
      pErr->registrateError("Warning: No metrics requested for normData.");
      pErr->error= -1;
      return;
      }

  // get type of normData and set type var for output:
  //if(!pND1->normDataType.compare("image")) pRes->pRS->pV->type= "hex";
  //else  pRes->pRS->pV->type= "string";

  // start type-based comparison for all required metrics (from input PCR):
  std::map <std::string, std::string> :: iterator i1;
  for(i1=metricsList->begin();i1!=metricsList->end();++i1)
     {
     #ifdef DEBUG_COMPARER
     std::cout<<std::endl<<"Comparing normData with Metric: "<<i1->first<<" (Name of metric:"<<i1->second<<")"<<std::endl;
     system("PAUSE");
     #endif

     metric= i1->first;

     if(pND1->normDataType.compare("image")==0)
       {
       // TODO: comparison of sets
       if(!metric.compare("11"))   // =hammingDistance
         {
         intRes= pCompMeasure->doHammingDistance(pND1->normDataAsInt, pND2->normDataAsInt);
         if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
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
         std::string metricName= i1->second;
         int pos1= metricName.rfind("#");
         if(metricName.rfind("#")!=std::string::npos)
           {
           std::string ngram=metricName.substr(pos1+1);
           n= atoi(ngram.c_str());
           ratRes= pCompMeasure->doSimpleMatchCoefficientN(pND1->normDataAsInt, pND2->normDataAsInt, n);
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
           pErr->registrateError("Error: Comparer:compareNormData(): Wrong syntax for metric simpleMatchCoefficientN in coco file.");
           pErr->errorcode= "code_B_34: Wrong syntax for metric 'simpleMatchCoefficientN' in configuration file";
           result= pErr->errorcode;
           }

         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         }
       else if(!metric.compare("17"))   // =euclidDistance
         {
         ratRes= pCompMeasure->doEuclidDistance(pND1->normDataAsInt, pND2->normDataAsInt);
         if(ratRes==-1.0) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%f", ratRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         }
       else if(!metric.compare("20"))   // =RMSE
         {
         ratRes= pCompMeasure->doRMSE(pND1->normDataAsInt, pND2->normDataAsInt);
         if(ratRes==-1.0) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%f", ratRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         }
       else
         {
         pErr->registrateError("Error: compareNormData(): Requested metric is not defined for normData of type 'image'.");
         pErr->errorcode= "code_B_37: Metric is not allowed for normalised data with information type 'image'.";
         pRes->pRS->compStatus=1;
         pRes->pRS->propMetrics->insert(make_pair(metric, std::string(pErr->errorcode)));
         }
       }
     else if(pND1->normDataType.compare("text")==0)
       {
       if(!metric.compare("11"))  // hammingDistance
         {
         intRes= pCompMeasure->doHammingDistance(pND1, pND2);

         if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%i", intRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         }
       if(!metric.compare("30"))  // hammingDistanceString
         {
         pND1->loadNormDataAsStrings();
         pND2->loadNormDataAsStrings();
         intRes= pCompMeasure->doHammingDistance(pND1->normDataAsStrings, pND2->normDataAsStrings);

         if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%i", intRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         }
       else if(!metric.compare("13"))  // simpleMatchCoefficient (ngram==1)
         {
         ratRes= pCompMeasure->doSimpleMatchCoefficient(pND1, pND2);

         if(ratRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
         else
           {
           // convert to string again:
           sprintf (s, "%f", ratRes);
           result= std::string(s);
           }
         pRes->pRS->propMetrics->insert(make_pair(metric, result));
         }
       else
         {
         pErr->registrateError("Error: compareNormData(): Requested metric is not defined for normData.");
         pErr->errorcode= "code_B_37: Metric is not allowed for normalised data with information type 'text'.";
         pRes->pRS->compStatus=1;
         pRes->pRS->propMetrics->insert(make_pair(metric, std::string(pErr->errorcode)));
         }
       }
     else
       {
       pErr->registrateError("Error: Information type (content of attribute: type) not supported by Comparator.");
       pRes->pRS->compStatus=2;
       }
     }
  }
catch(int _case)
  {
  if(_case==1)
    {
    pErr->registrateError("Error: compareNormData(): normData of compared XCDL have different types.");
    pRes->pRS->compStatus= 2;
    pErr->error= 0;
    }
  else
    {
    pErr->registrateError("Error: compareNormData(): unidentified error.");
    pErr->error= 0;
    }
  // throw ResultSet for normData into ResultBox:
  pRes->ResultBox->insert(std::make_pair("normData", pRes->pRS));
  return;
  }

// throw ResultSet for normData into ResultBox:
pRes->ResultBox->insert(std::make_pair("normData", pRes->pRS));
return;
}

// comparison of properties with data references (method is called for each property)
void Comparer :: comparePropertyWithDataRef(std::string& property)
{
std::map <std::string, std::string>* metricsList= NULL;

// create new object for result output of comparison set if not done before:
if(pRes->ResultBox==NULL) pRes->ResultBox= new std::map <std::string, ResultSet*> ();

// get list of all metrics requested for property x; if metrics are requested, initilize result objects (within that method):
metricsList= getMetricsListForPropertyX(metricsList, property);
if(pErr->error==-1) { pErr->error= 0; return;}  // no metrics requested for property; go on with next property

// search for property indexes in property boxes of objects, get them:
std::map <std::string, PropertyIndex*> :: iterator iPropIndex;

iPropIndex= pIndexSrc->pObjIndex->propertyIndexBox->find(property);// case not found: not possible
pPropIndexSrc= iPropIndex->second;  // = class variable

iPropIndex= pIndexTar->pObjIndex->propertyIndexBox->find(property);
pPropIndexTar= iPropIndex->second;   // = class variable


// set value set indicators (class variables) necessary for preselection of metrics support:
pCompMeasureValueSet->setValueSetIndicators(pPropIndexSrc, pPropIndexTar);

// compute metrics for propertyX:
std::map <std::string, std::string> :: iterator iMetric;  // pair: metricID, metricName

for(iMetric=metricsList->begin();iMetric!=metricsList->end();++iMetric)
   {
   #ifdef DEBUG_COMPARER
   std::cout<<std::endl<<"Comparing valueSet of property \""<<property<<"\" with Metric: "<<iMetric->first<<" "<<iMetric->second<<std::endl;
   #endif
   // check type of metric and apply it:
   if((iMetric->first).size()>=3) applyStructuralMetric(iMetric->first);  // !! comparison is based on metricID !!
   else applyBasicMetric(iMetric->first);
   }

// throw ResultSet for property into ResultBox:
pRes->ResultBox->insert(std::make_pair(property, pRes->pRS));
#ifdef DEBUG_COMPARER
std::cout<<"Comparison of property "<<property<<" finished."<<std::endl;system("PAUSE");
#endif

// reset variables:
if(metricsList!= NULL) metricsList= NULL;
pCompMeasureDataRef->resetFlags();
pCompMeasureValueSet->resetFlags();
pCompMeasureValueSet->resetMetricVars();

return;
}

// FROM:   void Comparer :: comparePropertyWithDataRef()
// PURPOSE: compute value for each metric from the metricsList for property X
void Comparer :: applyStructuralMetric(std::string metric)
{
char s[25];
std::string metricID="";
std::vector <ValueSetIndex*> :: iterator iSrcValSet;
std::vector <ValueSetIndex*> :: iterator iTarValSet;
int intResult, except;
bool boolResult;
double ratResult;


// get first char of metric ID for pre-selection:
std::string metricType = metric.substr(0,1);

// ============   structural metrics  ======================
// --------        <valueSet>      --------------------
// metric group:    1xx

try{

if(!metricType.compare("1"))  {

if(!metric.compare("101"))
  {
  intResult= pCompMeasureValueSet->doValueSetStat_1(pPropIndexSrc);
  sprintf (s, "%i", intResult);
  pRes->pRS->propMetrics->insert(std::make_pair("101", (std::string(s))));
  }
else if(!metric.compare("102"))
  {
  intResult= pCompMeasureValueSet->doValueSetStat_2(pPropIndexTar);
  sprintf (s, "%i", intResult);
  pRes->pRS->propMetrics->insert(std::make_pair("102", (std::string(s))));
  }
else if(!metric.compare("121"))
  {
  boolResult= pCompMeasureValueSet->doValueSetMatch_1(pPropIndexSrc, pPropIndexTar);
  if(boolResult) pRes->pRS->propMetrics->insert(std::make_pair("121", std::string("true")));
  else pRes->pRS->propMetrics->insert(std::make_pair("121", std::string("false")));
  }

// next three metrics use the same method (doValueSetMatch_2_3_4_5_6()); /*!results are stored in class variables in order to avoid unnecessary calls*/
else if(!metric.compare("122"))
  {
  // check constraints:
  // 1. metric 121 must be true (=equal number of value sets):
  if(!pCompMeasureValueSet->doValueSetMatch_1(pPropIndexSrc, pPropIndexTar))
    {
    throw except= 10;
    }
  //2. labValue exist + type equal in all valueSets
  if(!(pCompMeasureValueSet->getLabValuesExist()))
    {
    throw except= 11;
    }
  if(!(pCompMeasureValueSet->getConsistentLabValType()))
    {
    throw except= 12;
    }

  // now do metric 122 if constraints are ok:
  if(!pCompMeasureValueSet->getFlagValueSetMatch())
    {
    pCompMeasureValueSet->doValueSetMatch_2_3_4_5_6(pPropIndexSrc, pPropIndexTar);
    boolResult= pCompMeasureValueSet->getMetric122();
    }
  else boolResult= pCompMeasureValueSet->getMetric122();

  // store result:
  if(boolResult) pRes->pRS->propMetrics->insert(std::make_pair("122", std::string("true")));
  else pRes->pRS->propMetrics->insert(std::make_pair("122", std::string("false")));
  }
else if(!metric.compare("123"))
  {
  //check constraints:
  if(!(pCompMeasureValueSet->getLabValuesExist()))
    {
    throw except= 11;
    }
  if(!(pCompMeasureValueSet->getConsistentLabValType()))
    {
    throw except= 12;
    }

  if(pCompMeasureValueSet->getFlagValueSetMatch()) intResult= pCompMeasureValueSet->getMetric123();
  else
    {
    pCompMeasureValueSet->doValueSetMatch_2_3_4_5_6(pPropIndexSrc, pPropIndexTar);
    intResult= pCompMeasureValueSet->getMetric123();
    }

  sprintf (s, "%i", intResult);
  pRes->pRS->propMetrics->insert(std::make_pair(metric, (std::string(s))));
  }

else if(!metric.compare("124"))
  {
  //check constraints:
  if(!(pCompMeasureValueSet->getLabValuesExist()))
    {
    throw except= 11;
    }
  if(!(pCompMeasureValueSet->getConsistentLabValType()))
    {
    throw except= 12;
    }

  if(pCompMeasureValueSet->getFlagValueSetMatch())
         intResult= pCompMeasureValueSet->getMetric124();
  else
    {
    pCompMeasureValueSet->doValueSetMatch_2_3_4_5_6(pPropIndexSrc, pPropIndexTar);
    intResult= pCompMeasureValueSet->getMetric124();
    }
  sprintf (s, "%i", intResult);
  pRes->pRS->propMetrics->insert(std::make_pair(metric, (std::string(s))));
  }
else if(!metric.compare("125"))
  {
  //check constraints:
  if(!(pCompMeasureValueSet->getLabValuesExist()))
    {
    throw except= 11;
    }
  if(!(pCompMeasureValueSet->getConsistentLabValType()))
    {
    throw except= 12;
    }

  if(pCompMeasureValueSet->getFlagValueSetMatch())
         ratResult= pCompMeasureValueSet->getMetric125();
  else
    {
    pCompMeasureValueSet->doValueSetMatch_2_3_4_5_6(pPropIndexSrc, pPropIndexTar);
    ratResult= pCompMeasureValueSet->getMetric125();
    }
  sprintf (s, "%f", ratResult);
  pRes->pRS->propMetrics->insert(std::make_pair(metric, (std::string(s))));
  }
else if(!metric.compare("126"))
  {
  //check constraints:
  if(!(pCompMeasureValueSet->getLabValuesExist()))
    {
    throw except= 11;
    }
  if(!(pCompMeasureValueSet->getConsistentLabValType()))
    {
    throw except= 12;
    }

  if(pCompMeasureValueSet->getFlagValueSetMatch())
         ratResult= pCompMeasureValueSet->getMetric126();
  else
    {
    pCompMeasureValueSet->doValueSetMatch_2_3_4_5_6(pPropIndexSrc, pPropIndexTar);
    ratResult= pCompMeasureValueSet->getMetric126();
    }
  sprintf (s, "%f", ratResult);
  pRes->pRS->propMetrics->insert(std::make_pair(metric, (std::string(s))));

  }
} // end of metric group 1xx

//-----------        <dataRef>       -------------------
else if(!metricType.compare("2"))   {

  // make new preselection for subtype:
  metricType= metric.substr(0,2);
  if(!metricType.compare("25"))
    {
    // load reference boxes:
    if(pCompMeasureDataRef->refBoxLoadedFlag==false)
      {
      pCompMeasureDataRef->loadDataRefsForAllValueSets(pPropIndexSrc, pPropIndexTar);
      if(pErr->error==-2)
        {
        throw except= 21;
        }
      else pCompMeasureDataRef->refBoxLoadedFlag=true;
      }
    }
// get metric:
if(!metric.compare("251"))
  {
  if(!pCompMeasureDataRef->dataRefLoadedFlag)
    {
    pCompMeasureDataRef->loadDataRefStartAndEnd();
    pCompMeasureDataRef->dataRefLoadedFlag= true;
    }
  if(pCompMeasureDataRef->getResult1() == pCompMeasureDataRef->getResult2())
     pRes->pRS->propMetrics->insert(std::make_pair(metric, "true"));
  else pRes->pRS->propMetrics->insert(std::make_pair(metric, "false"));
  }
else if(!metric.compare("252"))
  {
  if(!pCompMeasureDataRef->dataRefLoadedFlag)
    {
    pCompMeasureDataRef->loadDataRefStartAndEnd();
    pCompMeasureDataRef->dataRefLoadedFlag= true;
    }
  intResult= pCompMeasureDataRef->getResult1() - pCompMeasureDataRef->getResult2();
  sprintf (s, "%i", intResult);
  pRes->pRS->propMetrics->insert(std::make_pair(metric, (std::string(s))));
  }
else if(!metric.compare("253"))
  {
  if(!pCompMeasureDataRef->dataRefLoadedFlag)
    {
    pCompMeasureDataRef->loadDataRefStartAndEnd();
    pCompMeasureDataRef->dataRefLoadedFlag= true;
    }
  // check constraints:
  if(!pCompMeasureValueSet->getEqualNumberOfValueSets()) throw except= 10;
  if(!pCompMeasureValueSet->getLabValuesExist()) throw except= 11;
  if(!pCompMeasureValueSet->getConsistentLabValType()) throw except= 12;

  boolResult= pCompMeasureDataRef->doDataRefMatch_11();
  if(boolResult) pRes->pRS->propMetrics->insert(std::make_pair("253", std::string("true")));
  else pRes->pRS->propMetrics->insert(std::make_pair("253", std::string("false")));
  }

else if(!metric.compare("254"))
  {
  if(!pCompMeasureDataRef->dataRefLoadedFlag)
    {
    pCompMeasureDataRef->loadDataRefStartAndEnd();
    pCompMeasureDataRef->dataRefLoadedFlag= true;
    }
  // check constraints:
  if(!pCompMeasureValueSet->getEqualNumberOfValueSets()) throw except= 10;

  if(pCompMeasureValueSet->getFlagValueSetMatch())
    {
    if(pCompMeasureValueSet->getMetric122())  // returns true if all values in valueSets of properties are equal!
      {
      boolResult= pCompMeasureDataRef->doDataRefMatch_11();
      if(boolResult) pRes->pRS->propMetrics->insert(std::make_pair("254", std::string("true")));
      else pRes->pRS->propMetrics->insert(std::make_pair("254", std::string("false")));
      }
    else
      {
      pRes->pRS->propMetrics->insert(std::make_pair("254", std::string("false")));
      }
    }
  else
    {
    pCompMeasureValueSet->doValueSetMatch_2_3_4_5_6(pPropIndexSrc, pPropIndexTar);
    if(pCompMeasureValueSet->getMetric122())
      {
      boolResult= pCompMeasureDataRef->doDataRefMatch_11();
      if(boolResult) pRes->pRS->propMetrics->insert(std::make_pair("254", std::string("true")));
      else pRes->pRS->propMetrics->insert(std::make_pair("254", std::string("false")));
      }
    else
      {
      pRes->pRS->propMetrics->insert(std::make_pair("254", std::string("false")));
      }
    }
  }
else
  {
  throw except=1;
  }
} // end of metric 2xx
} // end of try
catch (int except)
{
if(except==1)
  {
  pErr->registrateError("Unknown structural metric: ", metric);
  }
else if(except==10)
  {
  pErr->errorcode="code_S_10: Different number of value sets (node: valueSet) in src and tar.";
  pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
  }
else if(except==11)
  {
  pErr->errorcode="code_S_11: Lack of labelled value (node: labValue) in at least one value set of src or tar.";
  pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
  }
else if(except==12)
  {
  pErr->errorcode="code_S_12: Different types (node: type) in labelled values (node: labValue).";
  pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
  }
else if(except==21)
  {
  pErr->errorcode="code_S_21: Lack of data references (node: ref of node:propertySet) in both src and tar.";
  pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
  }

pErr->registrateError(pErr->errorcode);
pErr->error=0;
pRes->pRS->compStatus=1;  //  =partial

return;
}
return;
}


/// -------------------------------------------------------///
///  ----------------------------------------------------  ///
void Comparer :: comparePropertyWithoutDataRef(std::string property)
{
std::map <std::string, std::string>* metricsList= NULL;

// create new object for result output of comparison set if not done before:
if(pRes->ResultBox==NULL) pRes->ResultBox= new std::map <std::string, ResultSet*> ();

// get list of all metrics requested for property x; if metrics are requested, initilize result objects (within that method):
metricsList= getMetricsListForPropertyX(metricsList, property);
if(pErr->error==-1) { pErr->error= 0; return;}  // no metrics requested for property; go on with next property

// search for property indexes in property boxes of objects, get them:
std::map <std::string, PropertyIndex*> :: iterator iPropIndex;

iPropIndex= pIndexSrc->pObjIndex->propertyIndexBox->find(property);// case not found: not possible
pPropIndexSrc= iPropIndex->second;  // = class variable

iPropIndex= pIndexTar->pObjIndex->propertyIndexBox->find(property);
pPropIndexTar= iPropIndex->second;   // = class variable

// set value set indicators (class variables) necessary for preselection of metrics support:
pCompMeasureValueSet->setValueSetIndicators(pPropIndexSrc, pPropIndexTar);

// start comparison for all requested metrics (from input ComparisonConfiguration):
std::map <std::string, std::string> :: iterator iMetricList;
std::string metric;
for(iMetricList=metricsList->begin();iMetricList!=metricsList->end();++iMetricList)
   {
   #ifdef DEBUG_COMPARER
   std::cout<<std::endl<<"Comparing Property \""<<property<<"\" with Metric: "<<iMetricList->first<<std::endl;
   #endif
   // get metric:
   metric= iMetricList->first;
   // check type of metric and apply it:
   if((iMetricList->first).size()>=3) applyStructuralMetric(iMetricList->first);  // !! comparison is based on metricID !!
   else applyBasicMetric(iMetricList->first);
   }

// throw ResultSet for property into ResultBox:
pRes->ResultBox->insert(std::make_pair(property, pRes->pRS));
#ifdef DEBUG_COMPARER
std::cout<<"Comparison of property "<<property<<" finished."<<std::endl;system("PAUSE");
#endif

// reset variables:
if(metricsList!= NULL) metricsList= NULL;
pCompMeasureDataRef->resetFlags();
pCompMeasureValueSet->resetFlags();
pCompMeasureValueSet->resetMetricVars();

return;
}


// FROM:   void Comparer :: comparePropertyWithDataRef(), comparePropertyWithoutDataRef()
// PURPOSE: compute value for each basic metric from the metricsList for property X
void Comparer :: applyBasicMetric(std::string metric)
{
int intRes, error;
long longRes;
double ratRes;
std::string result;
char s[25];
std::vector <ValueSetIndex*> :: iterator iSrcValSet;
std::vector <ValueSetIndex*> :: iterator iTarValSet;

// check constraints:
// number of valueSets must be exactly on1(1) for basic metrics:
if((pPropIndexSrc->valueSetIndexBox->size())!=1 || pPropIndexTar->valueSetIndexBox->size()!=1)
  {
   pErr->errorcode="code_B_28: Metric is not applicable on properties with more than one value set (node: valueSet) or different numbers of value sets.";
   pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
   pErr->registrateError(pErr->errorcode);
   pErr->error=0;
   pRes->pRS->compStatus=1;
   return;
   }
// labValue exist + type equal in all valueSets
if(!(pCompMeasureValueSet->getLabValuesExist()))
  {
  pErr->errorcode="code_B_25: Lack of labelled value (node: labValue) in at least one value set of src or tar.";
  pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
  pErr->registrateError(pErr->errorcode);
  pErr->error=0;
  pRes->pRS->compStatus=1;
  return;
  }
if(!(pCompMeasureValueSet->getConsistentLabValType()))
  {
  pErr->errorcode="code_B_26: Different types (node: type) in labelled values (node: labValue).";
  pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
  pErr->registrateError(pErr->errorcode);
  pErr->error=0;
  pRes->pRS->compStatus=1;
  return;
  }

// if all pre-conditions apply, start with computation:
iSrcValSet=pPropIndexSrc->valueSetIndexBox->begin();
iTarValSet=pPropIndexTar->valueSetIndexBox->begin();

// metric: 1 = 'equal', allowed for all data types:
try
  {
  if(!metric.compare("1"))
    {
    if(!(*iSrcValSet)->labValVal.compare((*iTarValSet)->labValVal))
      {
      pRes->pRS->propMetrics->insert(make_pair(metric, "true"));
      }
    else pRes->pRS->propMetrics->insert(make_pair(metric, "false"));
    }
  else if(!metric.compare("2"))  // =intDiff
    {
    if(!((*iSrcValSet)->labValType).compare("int"))
      {
      if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
        {
        longRes= pCompMeasure->doIntDiff((*iSrcValSet), (*iTarValSet));
        sprintf(s, "%d", longRes);
        pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
        }
      else {throw error=1;}
      }
    else {throw error=2;}
    }
  else if(!metric.compare("3"))  // =ratDiff
         {
         if(!((*iSrcValSet)->labValType).compare("rational"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             ratRes= pCompMeasure->doRatDiff((*iSrcValSet), (*iTarValSet));
             sprintf(s, "%f", ratRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error=1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("4"))  // =intSum
         {
         if(!((*iSrcValSet)->labValType).compare("int"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             longRes= pCompMeasure->doIntSum((*iSrcValSet), (*iTarValSet));
             sprintf(s, "%d", longRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error=1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("5"))  // =ratSum
         {
         if(!((*iSrcValSet)->labValType).compare("rational"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             ratRes= pCompMeasure->doRatSum((*iSrcValSet), (*iTarValSet));
             sprintf(s, "%f", ratRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error=1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("6"))  // =intRatio
         {
         if(!((*iSrcValSet)->labValType).compare("int"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             ratRes= pCompMeasure->doIntRatio((*iSrcValSet), (*iTarValSet));
             if(pErr->error==-1) {throw error=3;}
             sprintf(s, "%f", ratRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error= 1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("7"))  // =ratRatio
         {
         if(!((*iSrcValSet)->labValType).compare("rational"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             ratRes= pCompMeasure->doRatRatio((*iSrcValSet), (*iTarValSet));
             if(pErr->error==-1) {throw error= 3;}
             sprintf(s, "%f", ratRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error= 1;}
           }
         else {throw error= 2;}
         }
       else if(!metric.compare("8"))  // =intProduct
         {
         if(!((*iSrcValSet)->labValType).compare("int"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             longRes= pCompMeasure->doIntProduct((*iSrcValSet), (*iTarValSet));
             if(pErr->error==-1) {throw error=0;}
             sprintf(s, "%d", longRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error=1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("9"))  // =ratProduct
         {
         if(!((*iSrcValSet)->labValType).compare("rational"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             ratRes= pCompMeasure->doRatProduct((*iSrcValSet), (*iTarValSet));
             if(pErr->error==-1) {error=0;}
             sprintf(s, "%d", ratRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error=1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("10"))  // =percDeviation
         {
         if(!((*iSrcValSet)->labValType).compare("int") || !((*iSrcValSet)->labValType).compare("rational"))
           {
           if(checkSizeOfVals((*iSrcValSet), (*iTarValSet)))
             {
             ratRes= pCompMeasure->doPercDeviation((*iSrcValSet), (*iTarValSet));
             if(pErr->error==-1) {error=0;}
             sprintf(s, "%d", ratRes);
             pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
             }
           else {throw error=1;}
           }
         else {throw error=2;}
         }
       else if(!metric.compare("11"))   // =hammingDistance
         {
         if(!((*iSrcValSet)->labValType).compare("int"))
           {
           loadDataAsSetOfValues((*iSrcValSet), (*iTarValSet), 0);
           intRes= pCompMeasure->doHammingDistance(dataAsSingleIntsA, dataAsSingleIntsB);
           cleanUpTempContainers(0);
           if(pErr->error==-1) {throw error=0;}
           }
         else if(!((*iSrcValSet)->labValType).compare("string"))
           {
           loadDataAsSetOfValues((*iSrcValSet), (*iTarValSet), 1);
           intRes= pCompMeasure->doHammingDistance(dataAsSingleStringsA, dataAsSingleStringsB);
           cleanUpTempContainers(1);
           if(pErr->error==-1) {throw error=0;}
           }
         else
           {
           throw error=1;
           }
         sprintf(s, "%d", intRes);
         pRes->pRS->propMetrics->insert(make_pair(metric, (std::string(s))));
         }
       else if(!metric.compare("15"))
         {
         // metric: 15 = 'levenstheinDistance, allowed for data types: string
         if(!((*iSrcValSet)->labValType).compare("string"))
           {
           intRes= pCompMeasure->doLevenstheinDistance((*iSrcValSet)->labValVal, (*iTarValSet)->labValVal);
           if(intRes==-1) {result=pErr->errorcode; pRes->pRS->compStatus=1;}
           else
             {
             // convert back to string:
             sprintf (s, "%i", intRes);
             result= std::string(s);
             }
           pRes->pRS->propMetrics->insert(make_pair(metric, result));
           }
         else
           {
           throw error=1;
           }
         }
       else
         {
         throw error=0;
         }
  }

catch(int error)
    {
    if(error==1)
      {
      pErr->errorcode="code_B_32: Out of size error. Value is not allowed exceeding 2.147.483.647 (content of node: value)";
      }
    else if(error==2)
      {
      pErr->errorcode="code_B_30: Metric is not defined for XCDL data type at hand (content of node: type)";
      }
    else if(error==3)
      {
      pErr->errorcode="code_B_33: Calculation not possible: division by zero. (content of node: value)";
      }
     else
      {
      pErr->errorcode="code_B_29: Common metric error";
      }
    pRes->pRS->propMetrics->insert(std::make_pair(metric, pErr->errorcode));
    pErr->registrateError(pErr->errorcode);
    pErr->error=0;
    pRes->pRS->compStatus=1;  //  =partial
    return;
    }
return;
}


///------------------------------------------------------------------------------
///------------------       Sub methods    --------------------------------------
///------------------------------------------------------------------------------
//  FROM:   void Comparer :: comparePropertyWithDataRef(), comparePropertyWithoutDataRef()
//  PURPOSE:    get metricsList for each property
std::map <std::string, std::string>* Comparer :: getMetricsListForPropertyX(std::map <std::string, std::string>* metricsList, std::string& property)
{
std::list <MetricSet*> :: iterator i;
bool propRequest= false;

// get metricsList and initialize objects for storage of results:
for(i=pCoco->pCompSet->sets->begin();i!=pCoco->pCompSet->sets->end();++i)
   {
   if(((*i)->propertyName).compare(property)==0)
     {
     propRequest= true;
     break;
     }
   }

if(propRequest)
  {
  // get list of requested metrics:
  metricsList= (*i)->getMetricSets();

  if(metricsList!=NULL)
    {
    pRes->pRS= new ResultSet();
    pRes->pRS->propMetrics= new std::map <std::string, std::string> ();
    pRes->pRS->pV= new Values();
    //  set property ID using coco data
    pRes->pRS->propId= (*i)->propertyID;
    pRes->pRS->propName= property;
    }
  else // return in case of not any metrics requested for property
       // only possible if coco is not validated!!
    {
    #ifdef DEBUG_COMPARER
    std::cout<<"No metrics requested for property "<<property<<"."<<std::endl;
    #endif
    pErr->error= -1;
    }
  }
else
  {
  //property not requested
  }
return metricsList;
}


// FROM::   void Comparer :: comparePropertyWithoutDataRef()
// PURPOSE:
void Comparer :: loadDataAsSetOfValues(ValueSetIndex *pV1, ValueSetIndex *pV2, int type)
{
std::istringstream issA (pV1->labValVal, std::istringstream::in);
std::istringstream issB (pV2->labValVal, std::istringstream::in);

if(type==0)
{
int val;

if(dataAsSingleIntsA==NULL) dataAsSingleIntsA= new std::vector <int> ();

for(int i =0;issA.good();++i)
   {
   issA >> std::dec >> val >> std::ws;
   dataAsSingleIntsA->push_back(val);
   }
if(dataAsSingleIntsB==NULL) dataAsSingleIntsB= new std::vector <int> ();

for(int i =0;issB.good();++i)
   {
   issB >> std::dec >> val >> std::ws;
   dataAsSingleIntsB->push_back(val);
   }
#ifdef DEBUG_COMPARER
std::cout<<"Data is loaded as single ints..."<<std::endl;
#endif
}

else if(type==1)
{
std::string str;

if(dataAsSingleStringsA==NULL) dataAsSingleStringsA= new std::vector <std::string> ();

for(int i =0;issA.good();++i)
   {
   issA >> str >> std::ws;
   dataAsSingleStringsA->push_back(str);
   }

if(dataAsSingleStringsB==NULL) dataAsSingleStringsB= new std::vector <std::string> ();

for(int i =0;issB.good();++i)
   {
   issB >> str >> std::ws;
   dataAsSingleStringsB->push_back(str);
   }
#ifdef DEBUG_COMPARER
std::cout<<"Data is loaded as single strings..."<<std::endl;
#endif
}
return;
}

// FROM: void Comparer :: comparePropertyWithoutDataRef()
// PURPOSE: Avoid wrong computation in case of large ints (TODO NEXT VERSION: better solution?)
bool Comparer :: checkSizeOfVals(ValueSetIndex* pV1, ValueSetIndex* pV2)
{
if(((strtoul(pV1->labValVal.c_str(), NULL, 0)) <= 2147483647) && ((strtoul(pV2->labValVal.c_str(), NULL, 0))<=2147483647))   return true;  // convert to long
else return false;
}









