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


#ifndef _COMPARATORINTERFACE_H
#define _COMPARATORINTERFACE_H

class SaxInterface;
class ComparatorError;
class InputParams;
class XMLValidation;

class ComparatorInterface
{
 public:
        ComparatorInterface(InputParams* PInPa);
        ~ComparatorInterface();

        void startComparator();  // default method for starting Comparator
        void doXMLValidation(); // start XML file validation
        void testIfFileIs(); // checks if file can be loaded

        InputParams *pInParams;
//private:
  ComparatorError *pErr;
};
#endif
