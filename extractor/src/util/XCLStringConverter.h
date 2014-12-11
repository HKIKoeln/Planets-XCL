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
#ifndef XCLSTRINGCONVERTER_H
#define XCLSTRINGCONVERTER_H

#include "Defines.h"
#include <iostream>
#include <QVector>
#include <QTextStream>
#include "XCLException.h"
#include <QString>
#include <QHash>

/**
	@author Jan Schnasse <jan.schnasse@uni-koeln.de>
*/



class XCLStringConverter
{
public:
  XCLStringConverter() {};
  ~XCLStringConverter(){};
  //  template <class T>
  // static T string2Number(const QString& value, const QString& interpretation, BOOL isBigEndian);
  static const QString& decStringToHex(const QString& value);
  static const QString& decStringToUtf16(const QString& value);
  const QString& getBytesFromUInt(const QString& value, const QString& interpretation);
  static const QString& wrapData(const QString& data);
private:
  static void getUBytesFromStream(	QTextStream& in,
                                   QVector<_UINT8>& bytes,
                                   const int interpretation,
                                   BOOL isBigEndian);
  static void getSBytesFromStream(QTextStream& in,  QVector<_SINT8>& bytes, const int interpretation, BOOL isBigEndian);
};



inline const QString& XCLStringConverter::wrapData(const QString& data)
{
  //Modified, to make lines ca. 72 chars long and break them at nearest space char.
  if(data.size()<100)return data;
  QString* wrapped = new QString();

  for(int i=0, lcount=1;i<data.size();i++)
  {
    wrapped->append(data.at(i));
    if (lcount<72) lcount++;
    else
    {
      if (data.at(i)!=' ')
        for (i++;i<data.size() && data.at(i)!=' ';i++)
          wrapped->append(data.at(i));
      if (i<data.size()) wrapped->append("\n");
      while(i < data.size() && data.at(i)==' ') i++;
      i--;
      lcount=1;
    }
  }
  return *wrapped;
}

inline const QString& XCLStringConverter::getBytesFromUInt(const QString& value, const QString& interpretation)
{
  QHash <QString, int> bytenumbers;
  bytenumbers.insert("uint8", 1);
  bytenumbers.insert("uint16", 2);
  bytenumbers.insert("uint32", 4);
  bytenumbers.insert("uint64", 8);
  bytenumbers.insert("sint8", 1);
  bytenumbers.insert("sint16", 2);
  bytenumbers.insert("sint32", 4);
  bytenumbers.insert("sint64", 8);

  int intvalue = value.toInt();
  QVector<int> bytevalues;

  while(bytevalues.size()<bytenumbers[interpretation])
  {

    if (intvalue<256)
    {
      bytevalues.prepend(intvalue);

      while (bytevalues.size()<bytenumbers[interpretation])
        bytevalues.prepend(0);
    }
    else
    {
      bytevalues.prepend(intvalue%256);
      intvalue = intvalue >> 8;

      if(bytevalues.size()>=bytenumbers[interpretation])
        throw XCLException("error number is to big");
    }

  }
  QString* result = new QString();
  QTextStream in(result,QIODevice::ReadOnly);

  for(int i=0; i<bytevalues.size();i++)
  {
    in<<bytevalues[i];
    in<<" ";
  }
  result = in.string();

  return *result;
}


/*template <class T>
inline T XCLStringConverter::string2Number(const QString& value, const QString& interpretation, BOOL isBigEndian)
{
  QString* v=new QString(value);
  QTextStream in(v,QIODevice::ReadOnly);
 
  int byteno;
 
  if (interpretation == "uint8")
  {
    byteno = 1;
    QVector<_UINT8> bytes;
    getUBytesFromStream(in, bytes, byteno,isBigEndian);
    return bytes[0];
  }
  if (interpretation == "sint8")
  {
    byteno = 1;
    QVector<_SINT8> bytes;
    getSBytesFromStream(in, bytes, byteno,isBigEndian );
    return bytes[0];
  }
  if (interpretation == "uint16")
  {
    byteno = 2;
    QVector<_UINT8> bytes;
    getUBytesFromStream(in, bytes, byteno,isBigEndian );
    return BYTES2WORD(bytes[0],bytes[1]);
  }
 
  if (interpretation == "sint16")
  {
    byteno = 2;
    QVector<_SINT8> bytes;
    getSBytesFromStream(in, bytes, byteno,isBigEndian );
    return BYTES2WORD(bytes[0],bytes[1]);
  }
  else if (interpretation == "uint32")
  {
    byteno = 4;
    QVector<_UINT8> bytes;
    getUBytesFromStream(in, bytes, byteno,isBigEndian );
    return BYTES2LWORD(bytes[0],bytes[1],bytes[2],bytes[3]);
  }
  else if (interpretation == "sint32")
  {
    byteno = 4;
    QVector<_SINT8> bytes;
    getSBytesFromStream(in, bytes, byteno,isBigEndian );
    return BYTES2LWORD(bytes[0],bytes[1],bytes[2],bytes[3]);
  }
  else if (interpretation == "uint64")
  {
    byteno = 8;
    QVector<_UINT8> bytes;
    getUBytesFromStream(in, bytes, byteno,isBigEndian );
    return  BYTES2DWORD(bytes[0],bytes[1],bytes[2],bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
  }
 
  else if (interpretation == "sint64")
  {
    byteno = 8;
    QVector<_SINT8> bytes;
    getSBytesFromStream(in, bytes, byteno,isBigEndian );
    return  BYTES2DWORD(bytes[0],bytes[1],bytes[2],bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
  }
 
std::cout<<"ungültiger Datentyp!!!!!!!!!!!!!!!!!!!!\n";
  return 0;
}*/



inline void XCLStringConverter::getUBytesFromStream(QTextStream& in,  QVector<_UINT8>& bytes, const int interpretation,BOOL isBigEndian )
{
  int a;

  while(!in.atEnd())
  {
    in >> a;
    bytes.append(a);
  }

  if(isBigEndian)
  {

    for(int i=bytes.size();i<interpretation;i++)
    {
      bytes.prepend(0);
    }
    /* if (bytes.size()>interpretation)
     {
       std::cout<<"wrong encoding"<<std::endl;
       throw XCLException( "\n--Wrong encoding-------------\n");
     }*/
  }
  else
  {
    for(int i=bytes.size();i<interpretation;i++)
    {
      bytes.append(0);
    }
  }
}

inline void XCLStringConverter::getSBytesFromStream(QTextStream& in,  QVector<_SINT8>& bytes, const int interpretation ,BOOL isBigEndian)
{
  int a;

  while(!in.atEnd())
  {
    in >> a;
    bytes.append(a);
  }
  if(isBigEndian)
  {

    for(int i=bytes.size();i<interpretation;i++)
    {
      bytes.prepend(0);
    }
    /* if (bytes.size()>interpretation)
     {
       std::cout<<"wrong encoding"<<std::endl;
       throw XCLException( "\n--Wrong encoding-------------\n");
     }*/
  }
  else
  {
    for(int i=bytes.size();i<interpretation;i++)
    {
      bytes.append(0);
    }
  }
}



inline const QString& XCLStringConverter::decStringToHex(const QString& value)
{
  // bool ok;

  //std::cout<<"3\n";
  //Below: Version 3
  QString* hexString = new QString();
  static QString hexTable[] =
    {
      "00 ",
      "01 ",
      "02 ",
      "03 ",
      "04 ",
      "05 ",
      "06 ",
      "07 ",
      "08 ",
      "09 ",
      "0a ",
      "0b ",
      "0c ",
      "0d ",
      "0e ",
      "0f ",
      "10 ",
      "11 ",
      "12 ",
      "13 ",
      "14 ",
      "15 ",
      "16 ",
      "17 ",
      "18 ",
      "19 ",
      "1a ",
      "1b ",
      "1c ",
      "1d ",
      "1e ",
      "1f ",
      "20 ",
      "21 ",
      "22 ",
      "23 ",
      "24 ",
      "25 ",
      "26 ",
      "27 ",
      "28 ",
      "29 ",
      "2a ",
      "2b ",
      "2c ",
      "2d ",
      "2e ",
      "2f ",
      "30 ",
      "31 ",
      "32 ",
      "33 ",
      "34 ",
      "35 ",
      "36 ",
      "37 ",
      "38 ",
      "39 ",
      "3a ",
      "3b ",
      "3c ",
      "3d ",
      "3e ",
      "3f ",
      "40 ",
      "41 ",
      "42 ",
      "43 ",
      "44 ",
      "45 ",
      "46 ",
      "47 ",
      "48 ",
      "49 ",
      "4a ",
      "4b ",
      "4c ",
      "4d ",
      "4e ",
      "4f ",
      "50 ",
      "51 ",
      "52 ",
      "53 ",
      "54 ",
      "55 ",
      "56 ",
      "57 ",
      "58 ",
      "59 ",
      "5a ",
      "5b ",
      "5c ",
      "5d ",
      "5e ",
      "5f ",
      "60 ",
      "61 ",
      "62 ",
      "63 ",
      "64 ",
      "65 ",
      "66 ",
      "67 ",
      "68 ",
      "69 ",
      "6a ",
      "6b ",
      "6c ",
      "6d ",
      "6e ",
      "6f ",
      "70 ",
      "71 ",
      "72 ",
      "73 ",
      "74 ",
      "75 ",
      "76 ",
      "77 ",
      "78 ",
      "79 ",
      "7a ",
      "7b ",
      "7c ",
      "7d ",
      "7e ",
      "7f ",
      "80 ",
      "81 ",
      "82 ",
      "83 ",
      "84 ",
      "85 ",
      "86 ",
      "87 ",
      "88 ",
      "89 ",
      "8a ",
      "8b ",
      "8c ",
      "8d ",
      "8e ",
      "8f ",
      "90 ",
      "91 ",
      "92 ",
      "93 ",
      "94 ",
      "95 ",
      "96 ",
      "97 ",
      "98 ",
      "99 ",
      "9a ",
      "9b ",
      "9c ",
      "9d ",
      "9e ",
      "9f ",
      "a0 ",
      "a1 ",
      "a2 ",
      "a3 ",
      "a4 ",
      "a5 ",
      "a6 ",
      "a7 ",
      "a8 ",
      "a9 ",
      "aa ",
      "ab ",
      "ac ",
      "ad ",
      "ae ",
      "af ",
      "b0 ",
      "b1 ",
      "b2 ",
      "b3 ",
      "b4 ",
      "b5 ",
      "b6 ",
      "b7 ",
      "b8 ",
      "b9 ",
      "ba ",
      "bb ",
      "bc ",
      "bd ",
      "be ",
      "bf ",
      "c0 ",
      "c1 ",
      "c2 ",
      "c3 ",
      "c4 ",
      "c5 ",
      "c6 ",
      "c7 ",
      "c8 ",
      "c9 ",
      "ca ",
      "cb ",
      "cc ",
      "cd ",
      "ce ",
      "cf ",
      "d0 ",
      "d1 ",
      "d2 ",
      "d3 ",
      "d4 ",
      "d5 ",
      "d6 ",
      "d7 ",
      "d8 ",
      "d9 ",
      "da ",
      "db ",
      "dc ",
      "dd ",
      "de ",
      "df ",
      "e0 ",
      "e1 ",
      "e2 ",
      "e3 ",
      "e4 ",
      "e5 ",
      "e6 ",
      "e7 ",
      "e8 ",
      "e9 ",
      "ea ",
      "eb ",
      "ec ",
      "ed ",
      "ee ",
      "ef ",
      "f0 ",
      "f1 ",
      "f2 ",
      "f3 ",
      "f4 ",
      "f5 ",
      "f6 ",
      "f7 ",
      "f8 ",
      "f9 ",
      "fa ",
      "fb ",
      "fc ",
      "fd ",
      "fe ",
      "ff "
    } ;
  int oldSize=0;
  int newSize=0;

  int lineBreakLimit=70;
  //printf("gottcha in: %13.13s!\n",value.toAscii().data());
  hexString->reserve(value.size()*3);
  const QChar* dataArray = value.data(); //14.08.08
  for (int i=0,limit=value.size(),collect;i<limit;i++)
  {

    collect=0;
    /* //14.08.08
	for (;value.data()[i].toAscii()!=' '&& i<limit && value.data()[i].toAscii();i++)
     {
          collect = (collect*10) + value.data()[i].toAscii()-'0';
     }*/

    //14.08.08------------------
    char data =0;

    do
    {

      data=dataArray[i].toAscii();
      if(data != ' ' && i < limit && data)
      {
        collect = (collect*10) + data-'0';
        i++;
      }
      else
        break;
    }
    while (TRUE);
    //END------------------------------
    
    hexString->append(hexTable[collect]);
    newSize = hexString->size();

    while(newSize - oldSize >lineBreakLimit)
    {

      while(hexString->at(oldSize+lineBreakLimit) != ' ')
        oldSize++;

      hexString->insert(oldSize+lineBreakLimit+1,"\n"); // +1 behind the white space

      oldSize+=lineBreakLimit+1;
    }
  }
  //printf("gottcha out: %13.13s!\n",hexString->toAscii().data());

  /*
  //Below Version 2
   QString tmp;
   
   hexString->reserve(value.size()*3);
   
   for (int i=0,limit=value.size();i<limit;i++)
       {
       tmp.sprintf("%02x ",value.data()[i].toAscii());
       hexString->append(tmp);
       }
   
  */
  /*
  //Below Version 1:
   
   QString v=value;
   
   
   QTextStream in(&v,QIODevice::ReadOnly);
   
  int a;
   
   while(!in.atEnd())
   {
   
   in >> a;
   tmp.sprintf("%02x ",a);
   hexString->append(tmp);
   }
  */
  //std::cout<<"4\n";
  return *hexString;
}


inline const QString& XCLStringConverter::decStringToUtf16(const QString& value)
{
  // bool ok;
  QString* utf16String = new QString();
  QString tmp;
  QString v=value;
  QTextStream in(&v,QIODevice::ReadOnly);
  short a;
  while(!in.atEnd())
  {
    in >> a;
    utf16String->append(a);
  }
  /*std::cout<<"///////////////////////////////"<<std::endl;
  std::cout<<utf16String->toStdString()<<std::endl;
  std::cout<<"///////////////////////////////"<<std::endl;*/
  return *utf16String;
}



#endif
