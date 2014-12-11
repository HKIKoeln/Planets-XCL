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


#ifndef _COMPARERMEASUREDATAREF_H
#define _COMPARERMEASUREDATAREF_H

#include <vector>

class ComparatorError;
class RefParams;
class XCDLIndex;

class ComparerMeasureDataRef
{
    public:
    ComparerMeasureDataRef(ComparatorError *pError, XCDLIndex* pIdxSrc, XCDLIndex* pIdxTar)
        {
        pIndexSrc= pIdxSrc;
        pIndexTar= pIdxTar;
        referencesBoxSrc= NULL;
        referencesBoxTar= NULL;
        SRC_Start=NULL;
        SRC_End=NULL;
        TAR_Start=NULL;
        TAR_End=NULL;
        dataRefLoadedFlag= false;
        refBoxLoadedFlag= false;
        pErr= pError;
        }
    ~ComparerMeasureDataRef()
        {
        cleanUp();
        }

    void loadDataRefsForAllValueSets(PropertyIndex* pPropSrc, PropertyIndex* pPropTar);
    void loadDataRefStartAndEnd();
    std::vector <RefParams*>*  loadDataRefsForValueSetX(std::vector <RefParams*> *RefBox, ValueSetIndex *pValSet, int src);
    bool doDataRefMatch_11();
    //----- getters and setters   -----------
    inline bool getDataRefLoadedFlag() {return dataRefLoadedFlag;}
    inline bool getRefBoxLoadedFlag() {return refBoxLoadedFlag;}
    inline void resetFlags()
       {
       dataRefLoadedFlag= false;
       refBoxLoadedFlag= false;
       }
    void cleanUp();

    //
    ComparatorError* pErr;
    XCDLIndex *pIndexSrc;
    XCDLIndex *pIndexTar;

    //private:
    // ------  Containers used for metrics computation ----
    std::vector <RefParams*> *referencesBoxSrc;  // stores  dataRefs for all valueSets of a property for SRC
    std::vector <RefParams*> *referencesBoxTar;  //   ~
    std::vector <int>* SRC_Start;
    std::vector <int>* SRC_End;
    std::vector <int>* TAR_Start;
    std::vector <int>* TAR_End;
    // ------ variables for storing results temporarily ----
    int result1;
    int result2;
    inline int getResult1() {return result1;}
    inline int getResult2() {return result2;}
    // ------ Flags   ---------------------
    bool dataRefLoadedFlag;  // used for metrics group 2xx
    bool refBoxLoadedFlag;   // ~

};
#endif
