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

#ifndef _COMPARATORENGINE_H
#define _COMPARATORENGINE_H

class ComparatorError;
class InputParams;
class XCDLIndex;
class NormData;
class SaxInterface;
class Comparer;
class Results;
class ComparisonConfiguration;
class ComparatorOutput;


class ComparatorEngine
{
  public:
  ComparatorEngine(ComparatorError *pError, InputParams *pIP, XCDLIndex *pIndexSource, XCDLIndex *pIndexTarget, ComparisonConfiguration *pPCR)
  {
  pErr= pError;
  pInParams= pIP;
  pIndexSrc= pIndexSource;
  pIndexTar= pIndexTarget;
  pConfig= pPCR;
  pNormData1= NULL;
  pNormData2= NULL;
  pSI= NULL;
  pComp= NULL;
  pResults= NULL;
  }

  ~ComparatorEngine() {};
  void compareXCDLs(char* sourceXCDL, char* targetXCDL);
  void serializeComparatorResults(int typeOfOutput, ComparatorOutput *pCO);

    // private:
  ComparatorError *pErr;
  XCDLIndex *pIndexSrc;
  XCDLIndex *pIndexTar;
  NormData *pNormData1;
  NormData *pNormData2;
  SaxInterface *pSI;
  Comparer *pComp;
  Results *pResults;
  ComparisonConfiguration *pConfig;
  InputParams *pInParams;
};
#endif
