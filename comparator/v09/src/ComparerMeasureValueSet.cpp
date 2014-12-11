#include <string>
#include <vector>
#include "ComparerMeasureValueSet.h"
#include "XCDLIndexes.h"

// Metric: 101: Number of valueSets for property X in SRC. result type: int
int ComparerMeasureValueSet :: doValueSetStat_1(PropertyIndex* pPropIndexSrc)
{
return pPropIndexSrc->valueSetIndexBox->size();
}

// Metric: 102: Number of valueSets for property X in SRC. result type: int
int ComparerMeasureValueSet :: doValueSetStat_2(PropertyIndex* pPropIndexTar)
{
return pPropIndexTar->valueSetIndexBox->size();
}

// Metric: 121: Compare number of valueSets for property X in SRC and TAR for
// equal quantity of valueSets. Return type: bool
bool ComparerMeasureValueSet :: doValueSetMatch_1(PropertyIndex* pPropIndexSrc, PropertyIndex* pPropIndexTar)
{
if(pPropIndexSrc->valueSetIndexBox->size() == pPropIndexTar->valueSetIndexBox->size())
return true;

else return false;
}

void ComparerMeasureValueSet :: doValueSetMatch_2_3_4_5_6(PropertyIndex* pPropIndexSrc, PropertyIndex* pPropIndexTar)
{
std::vector <ValueSetIndex*> :: iterator iSrcValSet;
std::vector <ValueSetIndex*> :: iterator iTarValSet;

for(iSrcValSet=pPropIndexSrc->valueSetIndexBox->begin();
                iSrcValSet!=pPropIndexSrc->valueSetIndexBox->end();
                    ++iSrcValSet)
   {
   for(iTarValSet=pPropIndexTar->valueSetIndexBox->begin();
                iTarValSet!=pPropIndexTar->valueSetIndexBox->end();
                    ++iTarValSet)
      {
      if(!(*iSrcValSet)->labValVal.compare((*iTarValSet)->labValVal))
        {
        ++metric123;    // SRC valueSet matches (valueSetMatch_3)
        ++metric124;    // TAR valueSet matches (valueSetMatch_4)
        break;
        }
      }
   }

// calculate metric 122 (valueSetMatch_2):
if((metric123 == pPropIndexSrc->valueSetIndexBox->size()) && (metric124 == pPropIndexTar->valueSetIndexBox->size()))
  {
  metric122= true;
  }
else metric122= false;

// metric 125 (valueSetMatch_5):
metric125= ((double) metric123) / ((double) (pPropIndexSrc->valueSetIndexBox->size()));

// metric 126 (valueSetMatch_6):
metric126= ((double) metric124) / ((double) (pPropIndexSrc->valueSetIndexBox->size()));

// set flag to indicate that metric group is computed
FlagValueSetMatch= true;

return;
}

// CALLED BY:
// PURPOSE: set value set indicators (class variables) necessary for preselection of metrics support
void ComparerMeasureValueSet :: setValueSetIndicators(PropertyIndex* pPropIndexSrc, PropertyIndex* pPropIndexTar)
{
std::vector <ValueSetIndex*> :: iterator iSrcValSet;
std::vector <ValueSetIndex*> :: iterator iTarValSet;
std::set <std::string> labValType;

// first check, if there are the same number of value sets for property X in both XCDLs:
if(pPropIndexSrc->valueSetIndexBox->size() == pPropIndexTar->valueSetIndexBox->size())
        setEqualNumberOfValueSets(true);
else    setEqualNumberOfValueSets(false);


// also check if both have labelled values and if value types are consistent:
for(iSrcValSet=pPropIndexSrc->valueSetIndexBox->begin();
                iSrcValSet!=pPropIndexSrc->valueSetIndexBox->end();
                    ++iSrcValSet)
   {
   if((*iSrcValSet)->labValueFlag==false)
     {
     setLabValuesExist(false);
     }
   // throw labValType into set container; only different strings are stored!!!
   labValType.insert((*iSrcValSet)->labValType);
   }

for(iTarValSet=pPropIndexTar->valueSetIndexBox->begin();
                iTarValSet!=pPropIndexTar->valueSetIndexBox->end();
                    ++iTarValSet)
   {
   if((*iTarValSet)->labValueFlag==false)
     {
     setLabValuesExist(false);
     }
   labValType.insert((*iTarValSet)->labValType);
   }

// if there were different labValTypes, size must be >1!! (se above)
if(labValType.size()!=1) setConsistentLabValType(false);
return;
}

