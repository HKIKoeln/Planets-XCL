/*

    * This module is part of the XCL software system
    *
    * Written by Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 2006 - 2009
    *
    * Copyright (C) 2006 - 2009 Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 
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

#ifndef __CSTREAMPATH_H_
#define __CSTREAMPATH_H_

#ifdef __USE_EXTERNDEFINES_
#include "CStreamPathDefines.h"
using namespace std;
#else
#include "Defines.h"
#endif

#include <vector>
#include <string>

// Diese Includes koennen nachher gekippt werden
#include <stdio.h>
#include <iostream>

class CStreamPath {
	public:
		       CStreamPath();
		       ~CStreamPath();
		BOOL   setPath(std::string);
		BOOL   setPathApp(std::string);
		std::string getPath();
		std::string getPath(std::string);	// Die Funktion wird ueberladen. Statt nur den Pfad zurueckzugeben, wird die uebergebene Datei noch mitverarbeitet.
	private:
		std::string         sPathApp;
		std::vector<std::string> vPath;
};

#endif /* __CSTREAMPATH_H_ */
