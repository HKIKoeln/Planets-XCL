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

#ifndef __CSTREAMMANAGER_H_
#define __CSTREAMMANAGER_H_

#include "CStreamReader.h"
#include "CStreamPath.h"
#include "CStreamUnzip.h"

#ifdef __USE_EXTERNDEFINES_
#include "CStreamManagerDefines.h"
using namespace std;
#else
#include "Defines.h"
#endif

#include <vector>
#include <string>
#include <stdio.h>
#include <ctime>
#include <dirent.h>
#include <cstdlib>


// ### Zu Debug-Zwecken sinnvoll

#include <iostream>



class CStreamManager {
	public:
								CStreamManager();					// Konstruktor
								~CStreamManager();					// Destruktor
		std::string				deflate(std::string, std::string);	// Entpackt ein ZIP-File in ein bestimmtes Verzeichnis
		std::string				deflate(std::string);				// Entpackt ein ZIP-File in ein zufaelliges Verzeichnis
		unsigned char*			getStream();						// Gibt einen Pointer auf den geoeffneten Char-Buffer zurueck.
		std::string				getStreamAsString();				// Gibt den kompletten Stream als String zurueck.
		LWORD					getStreamSize();					// Gibt die Laenge des gerade geoeffneten Streams zurueck.
		BOOL					open(const char*);					// Oeffnet das File, beruecksichtigt dabei NICHT den Pfad. Sollte spaeter private werden. (###)
		BOOL					open(std::string);					// Oeffnet das File, unter Beruecksichtigung des Pfades.
		BOOL					setPath(std::string);				// Setzt einen neuen Pfad, gibt den Erfolg zurueck.
		BOOL					setPathApp(std::string);			// Setzt einen neuen ROOT-Pfad der Applikation

/*
		BYTE getByte(LWORD byteNum)
		{
			return pStreamReader->getByte(byteNum);
		}

		BYTE getNextByte()
		{
			return pStreamReader->getNextByte();
		}
		BYTE getPrevByte()
		{
			return pStreamReader->getPrevByte();
		}
                
		*/

	private:
		CStreamReader*			pStreamReader;					// Ein Pointer auf den aktuellen StreamReader
		std::vector<CStreamReader*>	vec_sr;						// Der Vector, in dem die verschiedenen StreamReader gelesen werden.
		CStreamPath				pStreamPath;					// Ein Path-Objekt, mit dem die Pfade verwaltet werden.
		std::string				getRandomDirName(std::string);	// Gibt einen zufaelligen Namen fuer ein Verzeichnis zurueck, mit einem Praefix
		
};





#endif /* CSTREAMMANAGER_H_ */
