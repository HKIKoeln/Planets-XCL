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

#include "CStreamPath.h"

/* Konstruktor, die aktuellen Pfadangaben werden gesetzt */
CStreamPath::CStreamPath()
{
	sPathApp = ".";
}

/* Destruktor, loescht alle Elemente, die nicht automatisch zerstoert werden */
CStreamPath::~CStreamPath()
{
}

/* setPath setzt einen neuen Pfad und gibt diesen aus. */
BOOL CStreamPath::setPath(std::string sPathRel)
{
	// Bevor ueberhaupt irgendetwas gemacht wird, ein ganz wichtiger Fall. Wenn der ganze Pfad mit einem
	// Slash beginnt, dann wird wieder im Application-Directory gestartet. Darueber kann man streiten,
	// und das sollte man auch noch.
	if (sPathRel.find("/", 0) == 0)
		while (vPath.size() > 0)
			vPath.pop_back();


	// Solange der String noch Inhalt hat, wird er weiter zerlegt.
	while (sPathRel.size() > 0)
	{
		// An dieser Stelle wird erst einmal der naechste Pfad ermittelt.
		std::string sNextDir;
		if (sPathRel.find("/", 0) != std::string::npos)
		{
			sNextDir = sPathRel.substr(0, sPathRel.find("/", 0));
			// Wenn dies der Fall ist, dann ist der Slash abschliessend. Ansonsten
			// folgen noch weitere Pfadangaben.
			if (sPathRel.find("/", 0) == sPathRel.size() - 1)
				sPathRel = "";
			else
				sPathRel = sPathRel.substr(sPathRel.find("/", 0) + 1);
		}
		else
		{
			// Es wurde kein Slash gefunden, aber auch kein abschliessender Slash
			sNextDir = sPathRel;
			sPathRel = "";
		}
		
		// Jetzt steht in der Variablen sNextDir das naechste Verzeichnis, dieses wird nun analysiert und es
		// werden die richtigen Aktionen daraus gezogen.
		if (sNextDir == "")
		{
			// Das Verzeichnis ist leer. Das kann beispielsweise passieren, wenn die Datei mit einem Slash beginnt.
			// Ueber diesen Fall muesste man eigentlich noch diskutieren, denn dann waere das root angesprungen.
			// Jetzt wird der Fall einfach ignoriert.
		} else if (sNextDir == ".") {
			// Es klingt zwar seltsam, aber hier passiert genau gar nichts.

		} else if (sNextDir == "..") {
			// Der eigentlich spannendste Fall. Solange ein Vektor-Path vorhanden ist, wird der letzte
			// rausgeschmissen. Ist das nicht der Fall, dann wird alles mit FALSE abgebrochen.
			if (vPath.size() > 0)
				vPath.pop_back();
			else
				return FALSE;
		} else {
			// Der Normalfall. Es wird ein neues Verzeichnis angehangen.
			vPath.push_back(sNextDir);
		}
	}

	return TRUE;
}

BOOL CStreamPath::setPathApp(std::string sPathAppNew)
{
	sPathApp = sPathAppNew;
	return TRUE;
}

/* getPath gibt den aktuellen Pfad aus */
std::string CStreamPath::getPath()
{
	std::string retPath;
	for (_LENGTH i = 0; i < vPath.size(); i++)
	{
		retPath = retPath + vPath[i] + "/";
	}
	retPath = sPathApp + "/" + retPath;
	return retPath;
}

/* getPath gibt den aktuellen Pfad aus, zusammen mit der uebergebenen Datei */
std::string CStreamPath::getPath(std::string sFile)
{
	// Es wird ein neuer Vektor erstellt, auf den das File angewendet wird, weil der Original-Pfad ja nicht durch
	// Dateioperationen verschoben werden soll.
	std::vector<std::string> vTempPath;
	
	// Da der Originalvektor nicht angepackt werden soll, werden nun alle Teile des Vektors kopiert. Das ist allerdings nur notwendig,
	// wenn die Datei nicht mit einem Slash beginnt. Ist das naemlich der Fall, bleibt der Vektor leer. Ansonsten wird der Originalpfad
	// in den Temporaeren umkopiert.
	if (sFile.find("/", 0) == 0)
	{}
	else
		for (_LENGTH i = 0; i < vPath.size(); i++)
			vTempPath.push_back(vPath[i]);


	// Das File wird nur weiter bearbeitet, wenn innerhalb noch ein Slash zu finden ist. Wenn das nicht der Fall ist,
	// dann wird davon ausgegangen, dass der Rest, der im File steht, die Datei ist, die bearbeitet werden soll.
	while (sFile.find("/", 0) != std::string::npos)
	{
		// Wenn direkt die erste Stelle ein Slash ist, dann wird diese Stelle einfach abgeschnitten. Denn wirklich
		// Information ist das nicht. Nur wenn der Slash ganz am Anfang gestanden haette, muesste das Auswirkungen haben.
		// Das ist aber schon bearbeitet.
		if (sFile.find("/", 0) == 0)
			sFile = sFile.substr(1);
		else
		{
			// Ganz offensichtlich folgt nun ein Pfad. Daher wird der String nun in zwei Teile zerlegt. Der erste BIS zum
			// gefundenen Slash, der zweite alles NACH dem gefundenen Slash.
			std::string sNewPath;
			sNewPath = sFile.substr(0, sFile.find("/", 0));
			sFile = sFile.substr(sFile.find("/", 0) + 1);
			// Es gibt nun ein paar Sonderfaelle.
			if (sNewPath == "")
			{
				// Der aktuelle Ordner koennte leer sein. Das passiert, wenn mehrere Slashes aufeinander folgen. Eigentlich kann
				// das gar nicht passieren, da dieser Fall schon durch die erste Bedingung in der while-Schleife angefangen wird.
				// Aber sollte an dieser Stelle mal etwas geaendert werden, dann ist es sicherer, den dann moeglichen Sonderfall
				// hier auch aufzunehmen.
			}
			else if (sNewPath == ".")
			{
				// Auch ein Punkt, der hier allerdings sehr wohl moeglich ist, hat gar keine Auswirkungen.
			}
			else if (sNewPath == "..")
			{
				// Wenn zwei Punkte als Pfad existieren, dann wird tatsaechlich zurueckgesprungen. Sollte sich dabei herausstellen,
				// dass das Ende des Pfades schon erreicht ist, wird ein "-1" ausgeschmissen.
				if (vTempPath.size() == 0)
					return "-1";
				else
					vTempPath.pop_back();
			}
			else
			{
				// Alle anderen Faelle, es kommt einfach ein Pfad. Der wird an den Vector angehangen.
				vTempPath.push_back(sNewPath);
			}
		}
	}

	std::string retPath;
	for (_LENGTH i = 0; i < vTempPath.size(); i++)
	{
		retPath = retPath + vTempPath[i] + "/";
	}
	retPath = sPathApp + "/" + retPath + sFile;
	return retPath;
}


