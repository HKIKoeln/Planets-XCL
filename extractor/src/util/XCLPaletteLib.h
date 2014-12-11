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
#ifndef __XCLPALETTELIB_H
#define __XCLPALETTELIB_H
#include <QString>
#include <QHash>
#include <QByteArray>
#include <QVector>
#include <QQueue>
#include <QStringList>

#include "Defines.h"
#include "XCLException.h"
#include "XCLFilter.h"

QByteArray *XCLPalettePlanarSelector(QByteArray *filterMe, XCLFilter &filter);

QByteArray *XCLPaletteChunkySelector(
QByteArray                          *filterMe,
XCLFilter                                    &filter);

#ifndef XCLDebugTools
#define XCLDebugTools
void XCLDebugDump(QString prompt,QByteArray *ba);
#endif

#endif
