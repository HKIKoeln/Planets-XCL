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
#ifndef _COMPARERMEASURE_H
#define _COMPARERMEASURE_H

#include <vector>
class ComparatorError;
class NormData;
class ValueSetIndex;
class RefParams;

class ComparerMeasure
{
public:
       ComparerMeasure(ComparatorError *pCompError);
       ~ComparerMeasure();
       void cleanUp();
       long doIntDiff(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2);
       long doIntSum(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2);
       double doIntRatio(ValueSetIndex *pV1, ValueSetIndex *pV2);
       long doIntProduct(ValueSetIndex *pV1, ValueSetIndex *pV2);
       double doRatDiff(ValueSetIndex *pV1, ValueSetIndex *pV2);
       double doRatSum(ValueSetIndex *pV1, ValueSetIndex *pV2);
       double doRatRatio(ValueSetIndex *pV1, ValueSetIndex *pV2);
       double doRatProduct(ValueSetIndex *pV1, ValueSetIndex *pV2);
       double doPercDeviation(ValueSetIndex *pV1, ValueSetIndex *pV2);
       int doHammingDistance(std::vector <int> *setA, std::vector <int> *setB);
       int doHammingDistance(std::vector <std::string> *pArray1, std::vector <std::string> *pArray2);
       int doHammingDistance(NormData *pND1, NormData *pND2);
       int doLevenstheinDistance(std::string stringA, std::string stringB);
       double doSimpleMatchCoefficientN(std::vector <int> *setA, std::vector <int> *setB, int ngram);
       double doSimpleMatchCoefficient(NormData *pND1, NormData *pND2);
       double doEuclidDistance(std::vector <int> *setA, std::vector <int> *setB);
       double doRMSE(std::vector <int> *setA, std::vector <int> *setB);

       ComparatorError *pCE;
};
#endif
