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


class Request2;
class ValueSet;
class Results;
class PropertyTagset;
class ComparatorError;
class ComparerMeasure;
class NormData;

#include <vector>
#include <string>


class Comparer
{
      public:
             Comparer(Request2 *p, ComparatorError *pCE);
             ~Comparer();
             void cleanUpAll();
             void cleanUpSets();
             void cleanUpSet(unsigned short setnb);
             int compareProperties(PropertyTagset *p1, PropertyTagset *p2);
             int compareNormData(PropertyTagset *p1, NormData *pND1, NormData *pND2);

             bool checkSizeOfVals(ValueSet* pV1, ValueSet* pV2);
             std::map <std::string, std::string>* getMetricsList(std::string propName, Request2 *pR2);
             int storeValues(std::string labValString, std::vector <int> *setX);

             std::vector <int> *setA;
             std::vector <int> *setB;
             ComparerMeasure *pCompMeasure;
             Results *pRes;
             Request2 *pRequest2;
             ComparatorError *pErr;

};
#endif
