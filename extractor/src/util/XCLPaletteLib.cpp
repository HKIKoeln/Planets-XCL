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
#include "XCLPaletteLib.h"

static int preparePalette(UCHAR usePal[256][3], QString palette,bool planar,
   int depth);
static QByteArray *applyPalette(UCHAR usePal[256][3],QByteArray *filterMe,
   int maxpal, int width, int height);

QByteArray *XCLPalettePlanarSelector(
QByteArray                          *filterMe,
XCLFilter                                    &filter)
{
//return XCLPaletteChunkySelector(filterMe,filter);
//std::cout<<"CHOOSE PLANAR SELECTOR\n";

UCHAR usePal[256][3];
int maxpal,width,height,depth;

/*
#ifdef PlanetsDebug
XCLDebugDump("before lookup",filterMe);
#endif
*/
try
  {
  height=filter.getIntPar("imageHeight",true);
  width=filter.getIntPar("imageWidth",true);
    /* {
     QString hacking;
     int hacker;
     hacking=filter.getStringPar("bitsPerSample",true);
     if ((hacker=hacking.indexOf(QChar(' ')))>=0) hacking=hacking.left(hacker);
     printf("depth: **%s**\n",hacking.toAscii().data());
     depth=hacking.toInt();
     }*/
  depth=filter.getIntPar("bitsPerSample",true);

  }
catch(XCLException exception)
  {
//std::cout<<"DEPTH: "<<depth<<std::endl;
//  printf("caught!!\n");
  exception.message();
  }

maxpal=preparePalette(usePal,filter.getFilterValue(),true,depth);

return applyPalette(usePal,filterMe,maxpal,width,height);
}

static QByteArray *applyPalette(
UCHAR                           usePal[256][3],
QByteArray                                    *filterMe,
int                                                     maxpal,
int                                                            width,
int                                                                  height)
{
int limit;
QByteArray *result;
 
limit=width*height;
result=new QByteArray;
result->reserve(limit*3);

for (int i=0;i<limit;i++)
    {
    if ((UCHAR)(filterMe->data())[i] > maxpal)
       throw XCLException(QObject::tr("Pixel out of range of lookuptable"));
    for (int k=0;k<3;k++)
        result->append(usePal[(UCHAR)(filterMe->data()[i])][k]);
    }
/*
#ifdef PlanetsDebug
XCLDebugDump("after lookup",result);
#endif
*/

return result;
}

QByteArray *XCLPaletteChunkySelector(
QByteArray                          *filterMe,
XCLFilter                                    &filter)
{
UCHAR usePal[256][3];
int maxpal,width,height;

/*
#ifdef PlanetsDebug
//XCLDebugDump("before lookup",filterMe);
#endif
*/
try
  {
  height=filter.getIntPar("imageWidth",true);
  width=filter.getIntPar("imageHeight",true);
  }
catch(XCLException exception)
  {
 // printf("caught!!\n");
	exception.message();
  }

maxpal=preparePalette(usePal,filter.getFilterValue(),false,0);

return applyPalette(usePal,filterMe,maxpal,width,height);
}

static int preparePalette(
UCHAR                     usePal[256][3],
QString                                  palette,
bool                                             filterType,
int                                                         depth)
{
int value,maxpal;

QTextStream in(&palette,QIODevice::ReadOnly);
maxpal=0;

if (filterType==true)
   {
 //  printf("depth=%d\n",depth);
   maxpal=1<<depth;
 //  printf("max=%d\n",maxpal);
   for (int i=0;i<3;i++)
       for (int j=0;j<maxpal;j++)
           {
           in >> value;
           usePal[j][i]=value >> 8;
//printf("use %d / %d = %d\n",j,i,value);
           }
   }
else
   {
      for (;;)
      {
	  for (int j=0;j<3;j++)
	  {
	      in >> value;
	      usePal[maxpal][j]=value;
	      if (in.atEnd())
	      {
		  if (j==2)
		  {
		    break;
		  }
		  else if (!j)
		  {
		    maxpal--;
		    break;
		  }
		  else
		  {
		    throw XCLException(QObject::tr("Palette with missing color band(s)"));
		  }
		}
	  }
	  if (in.atEnd())
	  {
	      break;
	  }
	  if (++maxpal>256)
	  {
	      throw XCLException(QObject::tr("Palette with more than 256 entries"));
	  }
      }
   }
/*
for (int i=0;i<256;i++)
    {
printf("Val: %03d = {%02x, %02x, %02x}\n",i,usePal[i][0],usePal[i][1],usePal[i][2]);
    }*/

return maxpal;
}
