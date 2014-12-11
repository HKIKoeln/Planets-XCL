/*
    * This module is part of the XCL software system
    *
    * Written by  Sebastian Beyl, Volker Heydegger, Jan Schnasse,
    *             Manfred Thaller
    *             2007 - 2009
    *
    * Copyright (C)  2007 - 2009
    * Sebastian Beyl ,Volker Heydegger, Jan Schnasse, Manfred Thaller
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
/*****************************************************************************
ComparerOutput.h

Description:
*****************************************************************************/
#ifndef _COMPARATOROUTPUT_H
#define _COMPARATOROUTPUT_H

class Comparer;
class InputParams;
class MeassurementRequest;

class ComparatorOutput
{
  public:
         ComparatorOutput(InputParams *pInputParams);
         ComparatorOutput();
         ~ComparatorOutput();
         void outputForPlato(Comparer *pC); // specific output for use in PLANETS IF services
         void createCPROutputFrame();
         void createCPROutputEnd();
         void createCPRErrorOutput(std::string errorStatement);
         void createCPRCompsetErrorOutput(std::string errorStatement, MeasurementRequest *pMR);
         std::string extractXCDLFileName(char* dir);
         void createInputParamsErrorOutput();
         InputParams *pIP;
};

#endif
