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
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <qstring.h>
#include <time.h>

#include "XCLException.h"
#include "XCLDOMBuilder.h"
#include "Defines.h"


class FormatIdentifier
{
    public:
        FormatIdentifier();
       // FormatIdentifier(const QString& cf);
        ~FormatIdentifier();

        void identify(const QString& filename, const QString& xcelCurrentPath,  const QString& output);
       // void identify(const QString& filename, const QString& cf);
        const QString getCurrentFormat();
        const QString getXCELPath();
        const QString getXCDLPath();
        BOOL  getExtractionState();
        const QString askDroid(const QString& filename);
	
    private:
        QString puid;
        QString currentFormat; // ist LEER falls Flag extractionState true ist.
        QString xcelPath; // ist LEER falls Flag extractionState true ist.
        QString xcdlPath;
        BOOL extractionState; // ist TRUE, falls ein Script bzw. Tool die xcdl erzeugt hat.
        QString configFile; // !< The configFile.
        QDomDocument configDOM; // !< The DOM representation of the configuration file.
};

