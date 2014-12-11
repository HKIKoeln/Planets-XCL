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
class PropertyTagset;

class ComparerMeasure
{
public:
       ComparerMeasure(ComparatorError *pCompError);
       ~ComparerMeasure();
       void cleanUp();
       std::string doIntDiff(ValueSet *pV1, ValueSet *pV2);
       std::string doIntSum(ValueSet *pV1, ValueSet *pV2);
       std::string doIntRatio(ValueSet *pV1, ValueSet *pV2);
       std::string doIntProduct(ValueSet *pV1, ValueSet *pV2);
       std::string doRatDiff(ValueSet *pV1, ValueSet *pV2);
       std::string doRatSum(ValueSet *pV1, ValueSet *pV2);
       std::string doRatRatio(ValueSet *pV1, ValueSet *pV2);
       std::string doRatProduct(ValueSet *pV1, ValueSet *pV2);
       std::string doPercDev(ValueSet *pV1, ValueSet *pV2);
       int doHammingDistance(std::vector <int> *setA, std::vector <int> *setB);
       int doLevenstheinDistance(std::string stringA, std::string stringB);
       double doSimpleMatchCoefficientN(std::vector <int> *setA, std::vector <int> *setB, int ngram);
       double doEuclidDistance(std::vector <int> *setA, std::vector <int> *setB);
       double doRMSE(std::vector <int> *setA, std::vector <int> *setB);
       ComparatorError *pCE;
};
#endif
