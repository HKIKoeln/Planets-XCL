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

#include "CStreamManager.h"

/* Konstruktor */
CStreamManager::CStreamManager() {
	// Es werden erst einmal alle Variablen, Pointer etc. initialisiert.
	pStreamReader = NULL;
	// Der Time fuer den Zufallsgenerator wird in Gang gesetzt
	std::srand((unsigned) time(NULL));
}

CStreamManager::~CStreamManager() {
	// Alle Files muessen wieder geschlossen werden
	for (_LENGTH i = 0; i < vec_sr.size(); i++)
	{
		if (vec_sr[i]->close() == TRUE)
		{
			delete vec_sr[i];
		}
	}
}

/* Entpackt eine ZIP-Datei in ein zufaelliges Verzeichnis, die Routine wird, nachdem ein Verzeichnis gefunden ist, durchgereicht an die Klasse CStreamZip */
std::string CStreamManager::deflate(std::string sFile)
{
	// Es wird solange ein Verzeichnis-Name erzeugt, bis das Definitiv nicht mehr existiert.
	// Dann wird dieser Verzeichnisname zum Entpacken genutzt.
	DIR *pDir;
	std::string sDirName;
	do
	{
		sDirName = getRandomDirName("unzip_");
		pDir = opendir(pStreamPath.getPath(sDirName).c_str());
	} while(pDir != NULL);

	// Das ganze wird an die Funktion, bei der der Pfadname fest ist, weitergereicht.
	return deflate(sFile, sDirName);
}


/* Entpackt eine ZIP-Datei in ein vorgegebenes Verzeichnis, die Routine wird durchgereicht an die Klasse CStreamZip */
std::string CStreamManager::deflate(std::string sFile, std::string sDir)
{
	// Die Datei, die entpackt werden soll, muss noch mit den Pfadangaben ergaenzt werden, das Verzeichnis ebenso.
	CStreamUnzip pStreamUnzip;
	pStreamUnzip.deflate(pStreamPath.getPath(sFile), pStreamPath.getPath(sDir));
	return sDir;
}


/* Diese Methode erzeugt einen Zufallsnamen mit acht Zeichen von a bis z und einem voranstehendem Praefix */
std::string CStreamManager::getRandomDirName(std::string sPraefix)
{
	// Zuerst einmal wird nur das Praefix gesetzt und ein Buchstabe definiert.
	std::string sDirName = sPraefix;
	char cLetter;
	// Nun wird achtmal ein Zufallsbuchstabe gefunden und an das Verzeichnis angehangen.
	for (int i = 0; i < 8; i++)
	{
		cLetter = 97 + (std::rand() % (26));
		sDirName = sDirName + cLetter;
	}
	return sDirName;
}


/* Wenn ein Stream geoeffnet ist, dann wird sein kompletter Inhalt als String zurueck gegeben. Ist das nicht der Fall, dann wird
   stattdessen ein leerer String zurueck gegeben. */
unsigned char* CStreamManager::getStream() {
	if (pStreamReader != NULL)
		return pStreamReader->getStream();
	else
		return NULL;
}


/* Wenn ein Stream geoeffnet ist, dann wird sein kompletter Inhalt als String zurueck gegeben. Ist das nicht der Fall, dann wird
   stattdessen ein leerer String zurueck gegeben. */
std::string CStreamManager::getStreamAsString() {
	if (pStreamReader != NULL)
		return pStreamReader->getStreamAsString();
	else
		return "";
}


/* Wenn ein Stream geoeffnet ist, dann wird die Laenge des Streams zurueck gegeben. Ist das nicht der Fall, dann wird
   stattdessen 0 zurueck gegeben. */
LWORD CStreamManager::getStreamSize() {
	if (pStreamReader != NULL)
		return pStreamReader->getStreamSize();
	else
		return 0;
}


/* Oeffnet bzw. setzt ein neues oder bereits bestehendes File. Gibt bei Erfolg TRUE zurueck, sonst FALSE */
BOOL CStreamManager::open(const char* sFilenameToOpen) {
	// Es wird geschaut, ob ueberhaupt ein Filename uebergeben wurde. Ist das nicht der Fall, wird
	// automatisch false zurueck gegeben.
	std::string sEmptyProof(sFilenameToOpen);
	if (sEmptyProof.size() < 1)
		return FALSE;

	// Es muss zuerst einmal geschaut werden, ob bereits ein File mit dem Namen existiert
	for (_LENGTH i = 0; i < vec_sr.size(); i++)
	{
		if (vec_sr[i]->getStreamName() == sFilenameToOpen)
		{
			pStreamReader = vec_sr[i];
			return true;
		}
	}
	// Ganz offensichtlich wurde das File noch nicht geoeffnet, dann wird das nun nachgeholt.
	pStreamReader = new CStreamReader();
	if (pStreamReader->open(sFilenameToOpen) == TRUE)
	{
		vec_sr.push_back(pStreamReader);
		return TRUE;
	}
	else
	{
		delete pStreamReader;
		pStreamReader = NULL;
		return FALSE;
	}
}

/* Oeffnet bzw. setzt ein neues oder bestehendes File. Bekommt als Argument einen String, benutzt die Pfadangaben des Pfad-Managers mit. */
BOOL CStreamManager::open(std::string sFilenameToOpen)
{
	// Es werden nun die beiden Pfadangaben zussammengebaut.
	return open(pStreamPath.getPath(sFilenameToOpen).c_str());
}


/* Setzt einen neuen Pfad innerhalb des Path-Objekts */
BOOL CStreamManager::setPath(std::string sNewPath) {
	return pStreamPath.setPath(sNewPath);
}

/* Setzt einen neuen AppPfad innerhalb des Path-Objekts */
BOOL CStreamManager::setPathApp(std::string sNewPathApp) {
	return pStreamPath.setPathApp(sNewPathApp);
}

