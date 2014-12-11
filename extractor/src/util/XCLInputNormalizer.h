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
#ifndef __XCLINPUTNORMALIZER_H
#define __XCLINPUTNORMALIZER_H
#include <QString>
#include <QHash>
#include "XCLParsingItem.h"
#include "Defines.h"

class XCLInputNormalizer;
typedef QString* (XCLInputNormalizer::*PF)(QByteArray*,BOOL,const QString&);


class XCLInputNormalizer
{
public:
	XCLInputNormalizer();
	~XCLInputNormalizer();
	/*QString* normalizeValue(UCHAR* ba,_LENGTH length,
				const QString& interpret,
				BOOL isBigEndian);*/
      /*  QString* normalizeValues(const QString& str,
				const QString& interpret,
				BOOL isBigEndian,
				const QString& compression,
                                XCLParsingItem *parseTree);*/

        QString* normalizeValue(UCHAR* ba,_LENGTH length,
				const QString& interpret,
				BOOL isBigEndian);

        QString* normalizeValues(const QString& str,
				const QString& interpret,
				BOOL isBigEndian);


private:
        QString* normalizeUint8(UCHAR* ba,_LENGTH length);
	QString* normalizeUint16(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
        QString* normalizeUint24(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
	QString* normalizeUint32(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
	QString* normalizeUint32Rational(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
	QString* normalizeUint64(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);

       QString* normalizeSint8(UCHAR* ba,_LENGTH length);
	QString* normalizeSint16(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
	QString* normalizeSint32(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
	QString* normalizeSint64(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);

	QString* normalizeASCII(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);

	QString* normalizeLatin1(UCHAR* ba,_LENGTH length,
				BOOL isBigEndian);
      QString* normalizeUTF8(	UCHAR* ba,_LENGTH length,
					BOOL isBigEndian);

private:
	QHash<QString,QString*  (XCLInputNormalizer::*) (QByteArray*,BOOL,const QString&)> typeMap;	

  
};

#endif
