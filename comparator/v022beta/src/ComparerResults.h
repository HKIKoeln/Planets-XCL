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

#ifndef _COMPARERRESULTS_H
#define _COMPARERRESULTS_H

#include <map>
#include <string>
#include <list>

class Values
{
 public:
 Values();
 ~Values();

 void cleanUp();
 std::string type;
 std::string srcValue;
 std::string tarValue;
};

class ResultSet   // == <compSet> in request 2 output for PLATO
{
  public:
  ResultSet();
  ~ResultSet();
  inline std::map <std::string, std::string>* deliverPropMetrics()
   {
   return propMetrics;
   }
  void printResultSet();
  void cleanUp();

  Values *pV;
  std::string propId;
  std::string propUnit;
  int compStatus;  // variable for storing status of completeness of comparison
  std::map <std::string, std::string> *propMetrics; // key is id, value is result
  };


// for each set of XCDLs create a new ComparerResults object
class Results
{
 public:
  Results();
  ~Results();
  void cleanUp();
  void printComparerResults();
  std::string source;
  std::string target;
  ResultSet *pRS;
  std::map <std::string, ResultSet*> *ResultBox;  // key is propName


};

#endif
