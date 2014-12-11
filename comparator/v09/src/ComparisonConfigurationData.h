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
/*****************************************************************************
ComparisonConfigurationData.h

Description: classes which handle comparison configuration file data
*****************************************************************************/
#ifndef _COMPARISONCONFIGURATIONDATA_H
#define _COMPARISONCONFIGURATIONDATA_H
#include <string>
#include <list>
#include <map>

class MetricSet
{
      public:
          MetricSet();
          ~MetricSet();
          void cleanUp();
          void printMetrics();
          inline std::map <std::string, std::string>* getMetricSets()
          {
          return metricSets;
          }
          std::string propertyName;
          std::string propertyID;
          std::string propertyUnit;
          std::map <std::string, std::string> *metricSets;  // key= metric ID, value= metric name
};

// takes up all input data from <compSet>:
class CompSet
{
    public:
         CompSet();
         ~CompSet();
         void cleanUp();
         inline std::list <MetricSet*>* deliverMRPropertySets()
         {
         return sets;
         }
         std::string source;
         std::string target;
         bool ok;
         std::list <MetricSet*> *sets;  // : MetricSet= requested Properties
         MetricSet *pMS;
};

class ComparisonConfiguration
{
 public:
       ComparisonConfiguration();
       ~ComparisonConfiguration();
       void cleanUp();
       void printComparisonConfigurationData();
       std::list <CompSet*> *compSets;
       std::list <CompSet*> :: iterator iterMR;
       CompSet *pCompSet;
};
#endif
