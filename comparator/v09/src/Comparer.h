/*
    * This module is part of the XCL software system
    *
    * Written by  Sebastian Beyl, Volker Heydegger, Jan Schnasse,
    *             Manfred Thaller
    *             2007 - 2009
    *
    * Copyright (C)  2007 - 2009
    * Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller
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
*/
#ifndef _COMPARER_H
#define _COMPARER_H

#include <vector>
#include <string>

class ComparisonConfiguration;
class Results;
class ComparatorError;
class ComparerMeasure;
class ComparerMeasureValueSet;
class ComparerMeasureDataRef;
class NormData;
class XCDLIndex;
class ValueSetIndex;
class PropertyIndex;
class RefParams;


class Comparer
{
public:
Comparer(ComparatorError *pCE, XCDLIndex* pIdxSrc, XCDLIndex* pIdxTar);
~Comparer();
///   --------   New    ------------   ///
// --------------  inline methods   ------------------------ //
inline void setNormData1Ptr(NormData *pNormData) {pND1= pNormData;}
inline void setNormData2Ptr(NormData *pNormData) {pND2= pNormData;}
inline void setResultsPtr(Results *pResults) {pRes= pResults;}
inline void setConfigFilePtr(ComparisonConfiguration *pConfig) {pCoco= pConfig;}
inline void cleanUpTempContainers(int type)
{
if(type==0)
  {
  if(dataAsSingleIntsA!=NULL) {delete dataAsSingleIntsA; dataAsSingleIntsA= NULL;}
  if(dataAsSingleIntsB!=NULL) {delete dataAsSingleIntsB; dataAsSingleIntsB= NULL;}
  }
else if(type==1)
  {
  if(dataAsSingleStringsA!=NULL) {delete dataAsSingleStringsA; dataAsSingleStringsA= NULL;}
  if(dataAsSingleStringsB!=NULL) {delete dataAsSingleStringsB; dataAsSingleStringsB= NULL;}
  }
return;
}
//    ----------------------------------------------------------------    //
void compareNormData();
void comparePropertyWithDataRef(std::string& property);
void comparePropertyWithoutDataRef(std::string property);


//private:
std::pair <float, float> doPositionMatchRatio(std::vector <RefParams*> *RefBoxSrc, std::vector <RefParams*> *RefBoxTar);
bool doSequenceMatchEqual(std::vector <RefParams*> *RefBoxSrc, std::vector <RefParams*> *RefBoxTar);

///----------    Sub Methods and variables    --------------------------  //
void setValueSetIndicators();
void applyStructuralMetric(std::string metric);
void applyBasicMetric(std::string metric);
std::map <std::string, std::string>* getMetricsListForPropertyX(std::map <std::string, std::string>* metricsList, std::string& property);
std::vector <RefParams*>*  getAllDataRefsFromPropertySets(std::vector <RefParams*>* RefBox, ValueSetIndex *pValSet, int src);
void loadDataAsSetOfValues(ValueSetIndex *pV1, ValueSetIndex *pV2, int type);
bool checkSizeOfVals(ValueSetIndex* pV1, ValueSetIndex* pV2);

public:
ComparatorError *pErr;
ComparerMeasure *pCompMeasure;
ComparerMeasureValueSet* pCompMeasureValueSet;
ComparerMeasureDataRef* pCompMeasureDataRef;
XCDLIndex *pIndexSrc;
XCDLIndex *pIndexTar;
PropertyIndex* pPropIndexSrc;
PropertyIndex* pPropIndexTar;
// private:
Results *pRes;
ComparisonConfiguration *pCoco;
NormData *pND1;
NormData *pND2;

std::vector <std::string> *dataAsSingleStringsA;
std::vector <std::string> *dataAsSingleStringsB;
std::vector <int> *dataAsSingleIntsA;
std::vector <int> *dataAsSingleIntsB;


///  --------------------------------- ///
};
#endif
