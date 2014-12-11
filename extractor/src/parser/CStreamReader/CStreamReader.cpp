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

#include "CStreamReader.h"

#include <string>
#include <iostream>

/* Konstruktur des CStreamReader */
CStreamReader::CStreamReader() {
	// Es werden einige Variablen initialisiert und mit logischen Werten gefuellt.
	arrBuffer = NULL;
	sFilename = "";
	lFileSize = 0;
	lFilePosition = 0;
}

/* Destruktor des CStreamReader */
CStreamReader::~CStreamReader() 
{
	// Wenn das StreamObjekt noch geoeffnet ist, sollte es zuverlaessig wieder geschlossen werden.
	if (fin.is_open())
	{
		fin.close();
	}
}

/* Das File kann ganz explizit geschlossen werden. Diese Methode gibt als Rueckgabewert TRUE, wenn auch ein
   File geschlossen wurde, ansonsten gibt sie FALSE zurueck. */
BOOL CStreamReader::close() {
	if (fin.is_open())
	{
		// Der Buffer wird geloescht.
		delete arrBuffer;
		arrBuffer = NULL;
		// Der Filenamen wird zurueckgesetzt.
		sFilename = "";
		// Das Streamobjekt wird geschlossen.
		fin.close();
		return TRUE;
	}
	else
		return FALSE;

}


/* Es wird ein File geoeffnet und gibt entweder true oder false zurueck */
BOOL CStreamReader::open(const char* sFilenameToOpen) 
{
    	//Die Maskierungsbackslashes aus inputfilename werden entfernt, falls diese beim Aufrufen von Extractor hinzugefühgt worden sind.
        std::string s=sFilenameToOpen;

        int found = s.find_first_of("\\");
        while (found != -1)
        {
            s.erase(found, 1);
            found = s.find_first_of("\\",found+1);
        }
        sFilenameToOpen = s.c_str();
    
	fin.open(sFilenameToOpen, std::ios::binary);
	if (fin.is_open())
	{

		// Nach reichlichen Experimenten scheint dies Sinn zu machen: Es wird das File noch einmal als Filebuffer angepackt und dann
		// seiner Laenge her nach analysiert. Dieser Wert wird zwar so teuer erkauft, allerdings scheint dieser Weg auf den verschiedenen
		// Plattformen der einzig sinnvolle zu sein.
		std::fstream filestr(sFilenameToOpen);
		std::filebuf* pbuf;
		pbuf = filestr.rdbuf();
		// Die Laenge des Files wird bestimmt.
		lFileSize = pbuf->pubseekoff(0, std::ios_base::end);
		// Der Filebuffer wird geschlossen.
		filestr.close();

		// Ganz offensichtlich ist das Laden des Files erfolgreich, daher wird auch der Filename gespeichert.
		sFilename.assign(sFilenameToOpen);

		// Das Objekt wird nun dazu veranlasst, sich zu buffern. Damit wird der gesamte Inhalt in ein Char-Array gelesen.
		buffer();
		return TRUE;
	}	
	else
		return FALSE;
}

/* Das File wird durch diesen Befehl zuerst einmal komplett in einen Buffer eingelesen. */
BOOL CStreamReader::buffer() {
	if (fin.is_open())
	{
		// Der Buffer wird mit der richtigen Laenge initialisiert
		arrBuffer = new unsigned char[lFileSize + 1];
		
		// Die Laenge des Files ist bekannt. Daher kann auch daraufhin gelaufen werden.
		for (LWORD i = 0; i < lFileSize; i++)
		{
			// Das aktuelle Zeichen wird jetzt direkt auf das char-Array gelesen
			arrBuffer[i] = fin.get();
		}
		// An der letzten Stelle des Arrays wird noch ein Abschluss gesetzt.
		arrBuffer[lFileSize] = '\0';
		return TRUE;
	}
	else
		return FALSE;
}

/* Gibt einen Pointer auf den char-Buffer zurueck. Da dieser vom Start weg ohnehin NULL ist,
   muss eine entsprechende NULL-Behandlung hier nicht erfolgen. */
unsigned char* CStreamReader::getStream() {
	return arrBuffer;
}

/* Gibt des gesamten Fileinhalt als String zurueck */
std::string CStreamReader::getStreamAsString() {
	std::string sBuffer;
	sBuffer.assign((const char*)arrBuffer);
	return sBuffer;
}


/* Gibt den Namen des Files zurueck. */
std::string CStreamReader::getStreamName() {
	return sFilename;
}


/* Gibt die Laenge des Files zurueck, die beim Oeffnen ermittelt wurde. */
LWORD CStreamReader::getStreamSize() {
	return lFileSize;
}



