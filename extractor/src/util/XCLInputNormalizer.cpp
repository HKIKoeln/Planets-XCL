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

#include <iostream>
#include "XCLInputNormalizer.h"
#include "XCLException.h"
#include <QVector>
#include <QQueue>


XCLInputNormalizer::XCLInputNormalizer()
{
  //  typeMap.insert("uint8",&XCLInputNormalizer::normalizeUint8);
}

XCLInputNormalizer::~XCLInputNormalizer()
{}



/*QString* XCLInputNormalizer::normalizeValue(	UCHAR* ba,_LENGTH length,
						const QString& interpret,
						BOOL isBigEndian,
						const QString& compression)
{
 if(!typeMap.contains(interpret))throw new XCLException("XCLInputNormalizer: Unknown Type");
 
 QString*  (XCLInputNormalizer::*pf) (QByteArray*,const QString&,const QString&);
 pf=typeMap.value(interpret);
 return (*pf)(ba,isBigEndian);
}*/


/*
QString* XCLInputNormalizer::normalizeValues(	const QString& str,
						const QString& interpret,
						BOOL isBigEndian,
						const QString& compression,
                                                XCLParsingItem *parseTree)
{
  QString bStr=str;
 
  QVector<UCHAR> ba;
  QTextStream in(&bStr,QIODevice::ReadOnly);
  int a;
  while(!in.atEnd())
  {
    in >> a;
    ba.append(a);
  }
*/
/*int n = 0;
ba.prepend(123);
ba.prepend(58);
ba.prepend(2);
ba.prepend(n);
*/
/*
 
//QByteArray h=qUncompress((const char*)ba.data());
 return normalizeValue((UCHAR*)ba.data(),ba.size(),interpret,isBigEndian,parseTree);
//return new QString();
}*/
QString* XCLInputNormalizer::normalizeValue(	UCHAR* ba,_LENGTH length,
    const QString& interpret,
    BOOL isBigEndian)
{

  if(ba==NULL)throw XCLException("XCLInputNormalizer: ByteArray not instantiated");
  if(length==0)
    throw XCLException("XCLInputNormalizer: ByteArray is empty");

  if(interpret == "uint8")
  {
    return normalizeUint8(ba,length);
  }
  else if(interpret == "uint16")
  {
    return normalizeUint16(ba,length,  isBigEndian);
  }
  else if(interpret == "uint24")
  {
    return normalizeUint24(ba,length,  isBigEndian);
  }
  else if(interpret == "uint32")
  {
    return normalizeUint32(ba,length,  isBigEndian);
  }
  else if(interpret == "uint32Rational")
  {
    return normalizeUint32Rational(ba, length, isBigEndian);
  }
  else if(interpret == "uint64")
  {
    return normalizeUint64(ba,length,  isBigEndian);
  }
  else if(interpret == "sint8")
  {
    return normalizeSint8(ba,length);
  }
  else if(interpret == "sint16")
  {
    return normalizeSint16(ba,length,  isBigEndian);
  }
  else if(interpret == "sint32")
  {
    return normalizeSint32(ba,length,  isBigEndian);
  }
  else if(interpret == "sint64")
  {
    return normalizeSint64(ba,length,  isBigEndian);
  }
  else if(interpret == "ASCII")
  {
    return normalizeASCII(ba,length,  isBigEndian);
  }
  else if(interpret == "Latin1")
  {
    return normalizeLatin1(ba,length,  isBigEndian);
  }
  else if(interpret == "utf-8")
  {
    return normalizeUTF8(ba,length,  isBigEndian);
  }
  throw XCLException("XCLInputNormalizer: Unknown Type --"+interpret+"--");
}

QString* XCLInputNormalizer:: normalizeUint8(   UCHAR* ba,_LENGTH length)
{
  const char *byteValues[] =
    {
      "0 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ",
      "8 ", "9 ", "10 ", "11 ", "12 ", "13 ", "14 ", "15 ",
      "16 ", "17 ", "18 ", "19 ", "20 ", "21 ", "22 ", "23 ",
      "24 ", "25 ", "26 ", "27 ", "28 ", "29 ", "30 ", "31 ",
      "32 ", "33 ", "34 ", "35 ", "36 ", "37 ", "38 ", "39 ",
      "40 ", "41 ", "42 ", "43 ", "44 ", "45 ", "46 ", "47 ",
      "48 ", "49 ", "50 ", "51 ", "52 ", "53 ", "54 ", "55 ",
      "56 ", "57 ", "58 ", "59 ", "60 ", "61 ", "62 ", "63 ",
      "64 ", "65 ", "66 ", "67 ", "68 ", "69 ", "70 ", "71 ",
      "72 ", "73 ", "74 ", "75 ", "76 ", "77 ", "78 ", "79 ",
      "80 ", "81 ", "82 ", "83 ", "84 ", "85 ", "86 ", "87 ",
      "88 ", "89 ", "90 ", "91 ", "92 ", "93 ", "94 ", "95 ",
      "96 ", "97 ", "98 ", "99 ", "100 ", "101 ", "102 ", "103 ",
      "104 ", "105 ", "106 ", "107 ", "108 ", "109 ", "110 ", "111 ",
      "112 ", "113 ", "114 ", "115 ", "116 ", "117 ", "118 ", "119 ",
      "120 ", "121 ", "122 ", "123 ", "124 ", "125 ", "126 ", "127 ",
      "128 ", "129 ", "130 ", "131 ", "132 ", "133 ", "134 ", "135 ",
      "136 ", "137 ", "138 ", "139 ", "140 ", "141 ", "142 ", "143 ",
      "144 ", "145 ", "146 ", "147 ", "148 ", "149 ", "150 ", "151 ",
      "152 ", "153 ", "154 ", "155 ", "156 ", "157 ", "158 ", "159 ",
      "160 ", "161 ", "162 ", "163 ", "164 ", "165 ", "166 ", "167 ",
      "168 ", "169 ", "170 ", "171 ", "172 ", "173 ", "174 ", "175 ",
      "176 ", "177 ", "178 ", "179 ", "180 ", "181 ", "182 ", "183 ",
      "184 ", "185 ", "186 ", "187 ", "188 ", "189 ", "190 ", "191 ",
      "192 ", "193 ", "194 ", "195 ", "196 ", "197 ", "198 ", "199 ",
      "200 ", "201 ", "202 ", "203 ", "204 ", "205 ", "206 ", "207 ",
      "208 ", "209 ", "210 ", "211 ", "212 ", "213 ", "214 ", "215 ",
      "216 ", "217 ", "218 ", "219 ", "220 ", "221 ", "222 ", "223 ",
      "224 ", "225 ", "226 ", "227 ", "228 ", "229 ", "230 ", "231 ",
      "232 ", "233 ", "234 ", "235 ", "236 ", "237 ", "238 ", "239 ",
      "240 ", "241 ", "242 ", "243 ", "244 ", "245 ", "246 ", "247 ",
      "248 ", "249 ", "250 ", "251 ", "252 ", "253 ", "254 ", "255 "
    };

  QString* normalizedString =  new QString();
  QString num;

  for(_UINT32 i=0;i<length;i++)
  {
    normalizedString->append(byteValues[ba[i]]);
  }

  normalizedString->remove(normalizedString->length()-1,1);
  return normalizedString;
}

QString* XCLInputNormalizer::normalizeUint16(	UCHAR* ba,_LENGTH length,
    BOOL isBigEndian)
{
  QString* normalizedString =  new QString();
  QString num;

  if(isBigEndian)
  {
    if(length>=2)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2WORD(ba[i],ba[i+1]));
        normalizedString->append(num+" ");
        i+=2;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else
    {
      normalizedString->setNum(ba[0]);
    }
  }
  else
  {
    if(length>=2)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2WORD(ba[i+1],ba[i]));
        normalizedString->append(num+" ");
        i+=2;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else
    {
      normalizedString->setNum(ba[0]);
    }
  }
  return normalizedString;
}

QString* XCLInputNormalizer::normalizeUint24(	UCHAR* ba,_LENGTH length,
    BOOL isBigEndian)
{

  QString* normalizedString =  new QString();
  QString num;

  if(isBigEndian)
  {
    if(length>=3)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2UINT24(ba[i],ba[i+1],ba[i+2]));
        normalizedString->append(num+" ");
        i+=3;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else if(length <= 2)
    {
      return normalizeUint16(ba,length,isBigEndian);
    }
  }
  else
  {
    if(length>=3)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2UINT24(ba[i+2],ba[i+1],ba[i]));
        normalizedString->append(num+" ");
        i+=3;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else if(length <= 2)
    {
      return normalizeUint16(ba,length,isBigEndian);
    }
  }
  return normalizedString;
}

QString* XCLInputNormalizer::normalizeUint32(	UCHAR* ba,_LENGTH length,
    BOOL isBigEndian)
{

  QString* normalizedString =  new QString();
  QString num;

  if(isBigEndian)
  {//std::cout<<"...Is BIG Endian...";
    if(length>=4)
    {//std::cout<<"...Size>=4...";
      for(_UINT32 i=0;i<length;)
      { //std::cout<<(unsigned int)ba[i]<<" "<<(unsigned int)ba[i+1]<<" "<<(unsigned int)ba[i+2]<<" "<<(unsigned int)ba[i+3]<<"\n";
        num.setNum(BYTES2LWORD((unsigned int)ba[i],(unsigned int)ba[i+1],(unsigned int)ba[i+2],(unsigned int)ba[i+3]));
        normalizedString->append(num+" ");
        i+=4;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else if(length <= 2)
    {
      return normalizeUint16(ba,length,isBigEndian);
    }
  }
  else
  {
    if(length>=4)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2LWORD(ba[i+3],ba[i+2],ba[i+1],ba[i]));
        normalizedString->append(num+" ");
        i+=4;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else if(length <= 2)
    {
      return normalizeUint16(ba,length,isBigEndian);
    }
  }

  return normalizedString;
}



QString* XCLInputNormalizer::normalizeUint32Rational(	UCHAR* ba,_LENGTH length,
    BOOL isBigEndian)
{
  QString* normalizedString =  new QString();
  QString* numerator =  new QString();
  QString* denumerator =  new QString();
  QString num;



  if(isBigEndian)
  {

    if(length>=8)
    {
      for(_UINT32 i=0;i<length;)
      {

        num.setNum(BYTES2LWORD((unsigned int)ba[i],(unsigned int)ba[i+1],(unsigned int)ba[i+2],(unsigned int)ba[i+3]));
        numerator->append(num+" ");

        num.setNum(BYTES2LWORD((unsigned int)ba[i+4],(unsigned int)ba[i+5],(unsigned int)ba[i+6],(unsigned int)ba[i+7]));
        denumerator->append(num+" ");
        i+=8;
      }
      numerator->remove(numerator->length()-1,1);
      denumerator->remove(denumerator->length()-1,1);

    }
  }
  else
  {

    if(length>=8)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2LWORD(ba[i+3],ba[i+2],ba[i+1],ba[i]));
        numerator->append(num+" ");

        num.setNum(BYTES2LWORD(ba[i+7],ba[i+6],ba[i+5],ba[i+4]));
        denumerator->append(num+" ");
        i+=8;
      }
      numerator->remove(numerator->length()-1,1);
      denumerator->remove(denumerator->length()-1,1);

    }

  }
  int temp=0;
  if(denumerator->toInt() != 0)
    temp = numerator->toInt()/denumerator->toInt();



  return &normalizedString->setNum(temp);
}



QString* XCLInputNormalizer::normalizeUint64(	UCHAR* ba,_LENGTH length,
    BOOL isBigEndian)
{
  QString* normalizedString =  new QString();
  QString num;

  if(isBigEndian)
  {
    if(length>=8)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2DWORD(ba[i],ba[i+1],ba[i+2],ba[i+3], ba[i+4], ba[i+5], ba[i+6], ba[i+7]));
        normalizedString->append(num+" ");
        i+=8;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else if(length <= 4)
    {
      return normalizeUint32(ba,length,isBigEndian);
    }
  }
  else
  {
    if(length>=8)
    {
      for(_UINT32 i=0;i<length;)
      {
        num.setNum(BYTES2DWORD(ba[i+7],ba[i+6],ba[i+5],ba[i+4], ba[i+3], ba[i+2], ba[i+1], ba[i]));
        normalizedString->append(num+" ");
        i+=8;
      }
      normalizedString->remove(normalizedString->length()-1,1);
    }
    else if(length <= 4)
    {
      return normalizeUint32(ba,length,isBigEndian);
    }
  }

  return normalizedString;
}


QString* XCLInputNormalizer::normalizeSint8(UCHAR* ba, _LENGTH length)
{
  throw new XCLDataTypeException("DataType Sint8 is not supported\n");
  // useless statements for less compiler-warnings, params are not in use for NOW
  (void)ba;
  (void)length;
}

QString* XCLInputNormalizer::normalizeSint16(UCHAR* ba, _LENGTH length, BOOL isBigEndian)
{
  throw new XCLDataTypeException("DataType Sint16 is not supported\n");
  // useless statements for less compiler-warnings, params are not in use for NOW
  (void)ba;
  (void)length;
  (void)isBigEndian;
}
QString* XCLInputNormalizer::normalizeSint32(UCHAR* ba, _LENGTH length, BOOL isBigEndian)
{
  throw new XCLDataTypeException("DataType Sint32 is not supported\n");
  // useless statements for less compiler-warnings, params are not in use for NOW
  (void)ba;
  (void)length;
  (void)isBigEndian;
}
QString* XCLInputNormalizer::normalizeSint64(UCHAR* ba, _LENGTH length, BOOL isBigEndian)
{
  throw new XCLDataTypeException("DataType Sint64 is not supported\n");
  // useless statements for less compiler-warnings, params are not in use for NOW
  (void)ba;
  (void)length;
  (void)isBigEndian;
}

QString* XCLInputNormalizer::normalizeASCII(UCHAR* ba, _LENGTH length, BOOL isBigEndian)
{

  QString* normalizedString =  new QString();

  for(_LENGTH i=0; i<length;i++)
  {
    if(ba[i]=='\0') normalizedString->append("NULL");
    else if(ba[i] =='<') normalizedString->append("&lt;");
    else if(ba[i] =='>') normalizedString->append("&gt;");
    else if(ba[i] =='&') normalizedString->append("&amp;");
    else if(ba[i] =='\"') normalizedString->append("&qout;");
    else if(ba[i] == 223) normalizedString->append("&szlig;");
    else normalizedString->append(ba[i]);
    //normalizedString->append(" ");
  }
  return normalizedString;
  // useless statement for less compiler-warnings, param is not in use for NOW
  (void)isBigEndian;
}


QString* XCLInputNormalizer::normalizeUTF8( UCHAR* ba, _LENGTH length, BOOL isBigEndian)
{
  QString* normalizedString =  new QString();
  for(_LENGTH i=0; i<length; i++)
  {
    if(ba[i]=='\0') normalizedString->append("NULL");
    else if(ba[i] == '<') normalizedString->append("&lt;");
    else if(ba[i] == '>') normalizedString->append("&gt;");
    else if(ba[i] == '&') normalizedString->append("&amp;");
    else if(ba[i] == '\"') normalizedString->append("&qout;");
    else if(ba[i] == 223) normalizedString->append("&szlig;");
    else normalizedString->append(ba[i]);
  }
  return normalizedString;
  // useless statement for less compiler-warnings, param is not in use for NOW
  (void)isBigEndian;
}


QString* XCLInputNormalizer::normalizeLatin1( UCHAR* ba, _LENGTH length, BOOL isBigEndian)
{
  return normalizeASCII( ba, length, isBigEndian);
}


QString* XCLInputNormalizer::normalizeValues( const QString& str, const QString& interpret, BOOL isBigEndian)
{
  QString bStr=str;
  QString* asciiStr= new QString(str);
  // std::cout<<'str: '<<bStr.toStdString()<<std::endl;

  QVector<UCHAR> ba;
  QTextStream in(&bStr,QIODevice::ReadOnly);
  int a;
  if (interpret == "ASCII")
  {
    return asciiStr;
  }
  else
  {

    while(!in.atEnd())
    {
      in >> a;
      ba.append(a);
    }
    return normalizeValue((UCHAR*)ba.data(),ba.size(),interpret,isBigEndian);
  }
}
