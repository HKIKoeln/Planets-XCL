#include <string>
#include <map>
#include "ComparatorError.h"
#include "XCDLIndexes.h"
#include "ComparerMeasureDataRef.h"

void ComparerMeasureDataRef :: cleanUp()
{
pIndexSrc= NULL;
pIndexTar= NULL;
pErr= NULL;

if(SRC_Start!=NULL) {if(!SRC_Start->empty()) SRC_Start->clear(); delete SRC_Start; SRC_Start= NULL;}
if(SRC_End!=NULL) {if(!SRC_End->empty()) SRC_End->clear(); delete SRC_End; SRC_End= NULL;}
if(TAR_Start!=NULL) {if(!TAR_Start->empty()) TAR_Start->clear(); delete TAR_Start; TAR_Start= NULL;}
if(TAR_End!=NULL) {if(!TAR_End->empty()) TAR_End->clear(); delete TAR_End; TAR_End= NULL;}

if(referencesBoxSrc!=NULL)
  {
  if(!referencesBoxSrc->empty()) referencesBoxSrc->clear();
  delete referencesBoxSrc;
  referencesBoxSrc= NULL;
  }

if(referencesBoxTar!=NULL)
  {
  if(!referencesBoxTar->empty()) referencesBoxTar->clear();
  delete referencesBoxTar;
  referencesBoxTar= NULL;
  }
return;
}


// CALLED BY:
// PURPOSE: load data references (begin, end) for valueSets of property; sort them:
void ComparerMeasureDataRef :: loadDataRefStartAndEnd()
{
std::vector <RefParams*> :: iterator i1;
int startRef, endRef, count;

result1= 0;
result2= 0;

// allocate memory / clear objects:
if(SRC_Start!=NULL) {SRC_Start->clear();}
else SRC_Start= new std::vector <int> ();
if(SRC_End!=NULL) {SRC_End->clear();}
else SRC_End= new std::vector <int> ();
if(TAR_Start!=NULL) {TAR_Start->clear();}
else TAR_Start= new std::vector <int> ();
if(TAR_End!=NULL) {TAR_End->clear();}
else TAR_End= new std::vector <int> ();

// load references to vector containers:
if(referencesBoxSrc!=NULL)
  {
  if(!referencesBoxSrc->empty())
    {
    for(i1= referencesBoxSrc->begin();i1!=referencesBoxSrc->end(); ++i1)
       {
       endRef= strtoul(((*i1)->end).c_str(), NULL, 0);
       startRef= strtoul(((*i1)->begin).c_str(), NULL, 0);
       count= endRef - startRef;
       if(count>=0) result1+= count;
       result1+= (endRef - startRef);  // this is the number of SRC references in total (used for metric 201)
       SRC_Start->push_back(startRef);
       SRC_End->push_back(endRef);
       }
    }
  }

if(referencesBoxTar!= NULL)
  {
  if(!referencesBoxTar->empty())
    {
    for(i1= referencesBoxTar->begin();i1!=referencesBoxTar->end(); ++i1)
       {
       endRef= strtoul(((*i1)->end).c_str(), NULL, 0);
       startRef= strtoul(((*i1)->begin).c_str(), NULL, 0);
       count= endRef - startRef;
       if(count>=0) result2+= count;
       TAR_Start->push_back(startRef);
       TAR_End->push_back(endRef);
       }
    }
  }

//std::cout<<result1<<" "<<result2<<std::end

if(!SRC_Start->empty() || !SRC_End->empty() || !TAR_Start->empty() || !TAR_End->empty())
dataRefLoadedFlag= true;

return;
}

bool ComparerMeasureDataRef :: doDataRefMatch_11()
{
std::sort(SRC_Start->begin(), SRC_Start->end());
std::sort(TAR_Start->begin(), TAR_Start->end());
std::sort(SRC_End->begin(), SRC_End->end());
std::sort(TAR_End->begin(), TAR_End->end());

// ... compare (using STL algorithm):
if((equal (SRC_Start->begin(), SRC_Start->end(), TAR_Start->begin())) && (equal (SRC_End->begin(), SRC_End->end(), TAR_End->begin()))) return true;
else return false;
}

// FROM:
// PURPOSE:
void ComparerMeasureDataRef :: loadDataRefsForAllValueSets(PropertyIndex* pPropSrc, PropertyIndex* pPropTar)
{
std::vector <ValueSetIndex*> :: iterator iSrcValSet;
std::vector <ValueSetIndex*> :: iterator iTarValSet;

// make sure that references boxes objects are  not used:
if(referencesBoxSrc!=NULL)
  {
  if(!referencesBoxSrc->empty()) referencesBoxSrc->clear();
  }
if(referencesBoxTar!=NULL)
  {
  if(!referencesBoxTar->empty()) referencesBoxTar->clear();
  }
// generate new objects
if(referencesBoxSrc==NULL) {referencesBoxSrc= new std::vector <RefParams*> ();}
if(referencesBoxTar==NULL) {referencesBoxTar= new std::vector <RefParams*> ();}

// get data references of all valueSets for property X of source XCDL:
for(iSrcValSet=pPropSrc->valueSetIndexBox->begin();
                iSrcValSet!=pPropSrc->valueSetIndexBox->end();
                    ++iSrcValSet)
   {
   referencesBoxSrc= loadDataRefsForValueSetX(referencesBoxSrc, (*iSrcValSet), 1);
   }
// ... same for target XCDL:
for(iTarValSet=pPropTar->valueSetIndexBox->begin();
                iTarValSet!=pPropTar->valueSetIndexBox->end();
                    ++iTarValSet)
   {
   referencesBoxTar= loadDataRefsForValueSetX(referencesBoxTar, (*iTarValSet), 2);
   }

// indicate error for metrics output:
if(referencesBoxSrc->empty() && referencesBoxTar->empty()) pErr->error=-2;

return;
}


// FROM:    void Comparer :: loadDataRefBoxes()
// PURPOSE: method for getting all data references listed in a value set
std::vector <RefParams*>*  ComparerMeasureDataRef :: loadDataRefsForValueSetX(
                                            std::vector <RefParams*> *RefBox,
                                                        ValueSetIndex *pValSet, int src)
{
std::vector <DataRefIndex*>  :: iterator iDataRef;
std::map <std::string, PropertySetIndex*>  :: iterator iPropSet;
std::vector <RefParams*>  :: iterator iRefParams;

if(pValSet->dataRefIndexBox!=NULL)
  {
for(iDataRef= pValSet->dataRefIndexBox->begin();
        iDataRef!= pValSet->dataRefIndexBox->end();
            ++iDataRef)
   {
   if(!(*iDataRef)->ind.compare("normSpecific"))
     {
     // get propertySet id and search for that id in propertySetIndex:   /***   ***/ TODO case: can't find propSetId
     if(src==1) // source XCDL:
       {
       if(pIndexSrc->pObjIndex->propertySetIndexBox!=NULL)
         {
         iPropSet= pIndexSrc->pObjIndex->propertySetIndexBox->find((*iDataRef)->propSetId);
         }
       else continue;
       }
     else // target XCDL:
       {
       if( pIndexTar->pObjIndex->propertySetIndexBox!=NULL)
         {
         iPropSet= pIndexTar->pObjIndex->propertySetIndexBox->find((*iDataRef)->propSetId);
         }
       else continue;
       }

     // the value of this map is the pointer to the propertySetIndex; use this to load a local
     // copy of the referencesBox, which is filled with ALL RefParams objecs:
     if(RefBox==NULL) {RefBox= new std::vector <RefParams*>
          ((iPropSet->second)->pDataRefIndex->referencesBox->begin(), (iPropSet->second)->pDataRefIndex->referencesBox->end());}
     else  // in case of iteration append RefParams objects;
       {
       for(iRefParams= (iPropSet->second)->pDataRefIndex->referencesBox->begin();
                iRefParams!= (iPropSet->second)->pDataRefIndex->referencesBox->end();
                        ++iRefParams)
          {
          RefBox->push_back(*(iRefParams));
          }
        }
      }
    else { /* TODO next version: normAll ??? */}
    }
  }
else // no data references available for property: value set: value
  {
  return RefBox;  // do nothing
  }
return RefBox;
}

