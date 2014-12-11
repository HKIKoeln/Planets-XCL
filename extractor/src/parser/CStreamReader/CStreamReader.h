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

#ifndef __CSTREAMREADER_H_
#define __CSTREAMREADER_H_

#include <fstream>
#include <string>

#ifdef __USE_EXTERNDEFINES_
#include "CStreamReaderDefines.h"
using namespace std;
#else
#include "Defines.h"
#endif


class CStreamReader {
	public:
							CStreamReader();		// Der Konstruktor
							~CStreamReader();		// Der Destruktor
		BOOL				close();				// Das Filehandling wird geschlossen
		BOOL				open(const char*);		// Ein File wird geoeffnet
		unsigned char*		getStream();			// Es wird ein Pointer auf den char-Buffer uebergeben.
		std::string			getStreamAsString();	// Der komplette Inhalt wird als String zurueckgegeben.
		std::string			getStreamName();		// Der Filename wird zurueckgegeben.
		LWORD				getStreamSize();		// Die Groesse des Files wird in Bytes zurueckgegeben.
	private:
		BOOL				buffer();				// Die Methode liesst das komplette File in den Buffer ein.
		std::ifstream		fin;					// Dieses Objekt wird zum Filehandling benoetigt.
		LWORD				lFileSize;				// Die Laenge der Datei wird in diesem Wert gespeichert.
		std::string			sFilename;				// Der Filename wird in diesem String gespeichert.
		unsigned char*		arrBuffer;				// Dieser Pointer zeigt auf das Array, in dem das gesamte File gespeichert ist.
		LWORD				lFilePosition;
};


#endif /* __CSTREAMREADER_H_ */
