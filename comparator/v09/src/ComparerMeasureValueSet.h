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

#ifndef _COMPARERMEASUREVALUESET_H
#define _COMPARERMEASUREVALUESET_H

class PropertyIndex;
class ComparatorError;

class ComparerMeasureValueSet
{
    public:
    //-------   Constructors/ Destructor  ----
    ComparerMeasureValueSet(ComparatorError* pError)
        {
        pErr= pError;
        //set flags:
        FlagValueSetMatch= false;
        equalNumberOfValueSets= false;
        labValuesExist= true;
        consistentLabValType= true;
        // set metrics to default value:
        metric122= false;
        metric123= 0;
        metric124= 0;
        metric125= 0.0;
        metric126= 0.0;
        }
    ~ComparerMeasureValueSet() {pErr= NULL;}


    //----------   Metrics generation  ------
    int doValueSetStat_1(PropertyIndex* pPropIndexSrc);
    int doValueSetStat_2(PropertyIndex* pPropIndexTar);
    bool doValueSetMatch_1(PropertyIndex* pPropIndexSrc, PropertyIndex* pPropIndexTar);
    void doValueSetMatch_2_3_4_5_6(PropertyIndex* pPropIndexSrc, PropertyIndex* pPropIndexTar);
    void setValueSetIndicators(PropertyIndex* pPropIndexSrc, PropertyIndex* pPropIndexTar);

    //------    getters and setters  --------------
    inline void setEqualNumberOfValueSets(bool boolVal) {equalNumberOfValueSets= boolVal;}
    inline bool getEqualNumberOfValueSets() {return equalNumberOfValueSets;}
    inline void setLabValuesExist(bool boolVal) {labValuesExist= boolVal;}
    inline bool getLabValuesExist() {return labValuesExist;}
    inline void setConsistentLabValType(bool boolVal) {consistentLabValType= boolVal;}
    inline bool getConsistentLabValType() {return consistentLabValType;}
    inline bool getFlagValueSetMatch() {return FlagValueSetMatch;}
    inline bool getMetric122() {return metric122;}
    inline int getMetric123() {return metric123;}
    inline int getMetric124() {return metric124;}
    inline double getMetric125() {return metric125;}
    inline double getMetric126() {return metric126;}
    inline void resetFlags()
        {
        FlagValueSetMatch= false;
        equalNumberOfValueSets= false;
        labValuesExist= true;
        consistentLabValType= true;
        }
    inline void resetMetricVars()
        {
        metric122= false;
        metric123= 0;
        metric124= 0;
        metric125= 0.0;
        metric126= 0.0;
        }

    private:
    //----- valueSet indicators -------------
    bool equalNumberOfValueSets;
    bool labValuesExist;
    bool consistentLabValType;
     //------- Flags   ---------------------
    bool FlagValueSetMatch;  // is set if doValueSetMatch_2_3_4_5_6() is done
    //-------  metrics results  ------------
    bool metric122;
    int metric123;
    int metric124;
    double metric125;
    double metric126;

    // common class variables
    ComparatorError* pErr;
};
#endif
