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
#include "XCLPngLib.h"

enum iType {greyscale, truecolour, palette, greyscaleAlpha, truecolourAlpha};

static QByteArray *XCLPngAdaptive(QByteArray *filterMe, XCLFilter &filter);
static UCHAR *XCLPngFilter(UCHAR *ba, int fullWidth, int fullHeight, int depth,
   int interlace, QString imageType, int *bytes);
static int doPngPass(UCHAR *linein, UCHAR *lineout, int fullWidth,
   int fullHeight, int depth, int bytes, UCHAR *baseline, int woffset,
   int wincrement, int hoffset, int hincrement, int pass, int useType);
static void doPngFilter(UCHAR *linein, UCHAR *lineout, int width, int height,
   UCHAR *baseline, int bytes);
static int paethPredictor(int a, int b, int c);
static UCHAR *blowUp(UCHAR *ba, int fullWidth, int fullHeight, int depth,
   int useType);

static QByteArray *XCLPngAdaptive(
QByteArray                       *filterMe,
XCLFilter                                 &filter)
{
int height, width, depth,interlace,bytes;
QString imageType,interlaceType;
QByteArray *result;
UCHAR *prelim;
/*
#ifdef PlanetsDebug
XCLDebugDump("before adaptive",filterMe);
#endif
*/
/*
#ifdef PlanetsDebug
printf("==== Png ========\n");
QHash<QString, QString>::const_iterator iter = filter.getParams().constBegin();
 while (iter != filter.getParams().constEnd()) {
     printf("par: %s - %s\n",iter.key().toLatin1().data(),iter.value().toLatin1().data());
     ++iter;
 }
printf("==== Png ========\n");
#endif
*/
try
  {
  height=filter.getIntPar("imageHeight",true);
  width=filter.getIntPar("imageWidth",true);
  depth=filter.getIntPar("bitsPerSample",true);
  imageType=filter.getStringPar("colourSpaceName",true);
  interlaceType=filter.getStringPar("interlace",false);
  if (interlaceType=="adam7") interlace=1;
  else interlace=0;
  }
catch(XCLException exception)
  {
  exception.message();
  }

prelim=XCLPngFilter((UCHAR *)filterMe->data(),width,height,depth,interlace,imageType,&bytes);
if (prelim==NULL) return NULL;
result=new QByteArray((const char *)prelim,width*bytes*height);
delete prelim;
/*
#ifdef PlanetsDebug
XCLDebugDump("after adaptive",result);
#endif
*/
return result;
}

static UCHAR *XCLPngFilter(
UCHAR                     *ba,
int                           fullWidth,
int                                     fullHeight,
int                                                depth,
int                                                      interlace,
QString                                                            imageType,
int *bytes)
{
static QString types[] = { "greyscale", "truecolour", "palette", "greyscaleAlpha", "truecolourAlpha", NULL };
static int useBytes[5][2] = { { 1, 2 }, { 3, 6 }, { 1, 1 }, { 2, 4 } , { 4, 8 } };
int width,height,restart,useType,subType,capacity;
UCHAR *lineout, *baseline, *final;

for (useType=0; types[useType]!=NULL; useType++)
    if (imageType==types[useType]) break;
if (types[useType]==NULL) return NULL;

subType=depth>8 ? 1 : 0;
*bytes=useBytes[useType][subType];

height=fullHeight;
if (depth<8)
   {
   capacity=8/depth;
   width=fullWidth / capacity;
   if (fullWidth % capacity) width++;
   }
else
   {
   width=fullWidth;
   }
/*
#ifdef PlanetsDebug
for (int j=0,limit;j<height;j++)
    {
    int i;
    printf("filter:\n");
    i = j*((width*(*bytes))+1);
    limit=i+(width*(*bytes))+1;
    for (;i<limit;i++) printf("%03d ",ba[i]);
    printf("\n");
    }
#endif
*/
lineout=new UCHAR[fullWidth * fullHeight * (*bytes)];
baseline=new UCHAR[width*(*bytes)];
memset(baseline,0,width*(*bytes));

if (!interlace)
   {
   doPngFilter(ba,lineout,width,height,baseline,(*bytes));
   if (depth<8)
      {
      final=blowUp(lineout,fullWidth,fullHeight,depth,useType);
      delete lineout;
      lineout=final;
      }
   }
else
   {
   memset(lineout,0,fullWidth * fullHeight * (*bytes));

   restart=doPngPass(ba,lineout,fullWidth,fullHeight,depth,*bytes,baseline,0,8,0,8,1,useType);
   restart += doPngPass(ba+restart,lineout,fullWidth,fullHeight,depth,*bytes,baseline,4,8,0,8,2,useType);
   restart += doPngPass(ba+restart,lineout,fullWidth,fullHeight,depth,*bytes,baseline,0,4,4,8,3,useType);
   restart += doPngPass(ba+restart,lineout,fullWidth,fullHeight,depth,*bytes,baseline,2,4,0,4,4,useType);
   restart += doPngPass(ba+restart,lineout,fullWidth,fullHeight,depth,*bytes,baseline,0,2,2,4,5,useType);
   restart += doPngPass(ba+restart,lineout,fullWidth,fullHeight,depth,*bytes,baseline,1,2,0,2,6,useType);
   restart += doPngPass(ba+restart,lineout,fullWidth,fullHeight,depth,*bytes,baseline,0,1,1,2,7,useType);
   }
/*
#ifdef PlanetsDebug
for (int j=0;j<fullHeight;j++)
    {
    int i,limit;
    printf("filtered:\n");
    i = j*fullWidth*(*bytes);
    limit=i+fullWidth*(*bytes);
    for (;i<limit;i++) printf("%03d ",lineout[i]);
    printf("\n");
    }
#endif
*/
delete baseline;

return lineout;
}

static int doPngPass( UCHAR *linein,
                      UCHAR *lineout,
                      int fullWidth,
		      int fullHeight,
		      int depth,
		      int bytes,
		      UCHAR *baseline,
		      int woffset,
		      int wincrement,
		      int hoffset,
		      int hincrement,
		      int pass,
		      int useType )
{
  int limit,size,w,h;
  UCHAR *fragment, *blownup;
  int prew,capacity;

  if (fullWidth<=woffset || fullHeight<=hoffset) return 0;

  w = (fullWidth) / wincrement;
  if (fullWidth%wincrement > woffset) w++;
  h = (fullHeight) / hincrement;
  if (fullHeight%hincrement > hoffset) h++;

  size = w*h*bytes;
  if (!size) return 0;

  fragment=new UCHAR[size];

  if (depth<8)
  {
    capacity=8/depth;
    prew=w / capacity;
    if (w % capacity) prew++;
  }
  else
  {
    prew=w;
  }
  doPngFilter(linein,fragment,prew,h,baseline,bytes);

  if (depth<8)
  {
    blownup=blowUp(fragment,w,h,depth,useType);
    limit=w*h;
    delete fragment;
    fragment=blownup;
  }
  else
    limit=size;

  if (bytes>1)
  {
    fullWidth *= bytes;
    woffset *= bytes;
    if (wincrement>0) wincrement = (wincrement-1) * bytes;
    for (int i=0, j=(hoffset*fullWidth) + woffset, k=0, l=hoffset;i<limit;)
    {
      for (int n=0;n<bytes;n++,j++,i++)
      {
	lineout[j]=fragment[i];
      }
      k++;
      if (k>=w)
      {
        k=0;
        l+=hincrement;
        j= (l*fullWidth) + woffset;
      }
      else j += wincrement;
    }
  }
  else
    for (int i=0, j=(hoffset*fullWidth)+woffset, k=0, l=hoffset;i<limit;i++)
    {
      lineout[j]=fragment[i];
      k++;
      if (k>=w)
      {
	k=0;
	l+=hincrement;
	j= (l*fullWidth) + woffset;
      }
      else
	j+=wincrement;
    }
  delete fragment;

/*
#ifdef PlanetsDebug
for (int j=0;j<fullHeight;j++)
    {
    int i;
    printf("pass %d:\n",pass);
    i = j*fullWidth*bytes;
    limit=i+fullWidth;
    for (;i<limit;i++) printf("%03d ",lineout[i]);
    printf("\n");
    }
#endif
*/
  return (prew*bytes+1)*h;
  
  // Unused vars, no compiler-warning, because "in use", USELESS BLOCK!!!
  {
      pass = 0;
  }

}

static void doPngFilter(
UCHAR                  *linein,
UCHAR                         *lineout,
int                                    width,
int                                          height,
UCHAR                                              *baseline,
int                                                          bytes)
{
UCHAR *lastline;
int filter,limit,last,lastlast;
/*
#ifdef PlanetsDebug
printf("doPng: %d / %d - %d\n",width,height,bytes);
for (int i=0;i<width*height;i++)
    {
    printf("%03d ",linein[i]);
    if (i && !(i%16)) printf("\n");
    }
printf("\n");
#endif
*/
for (int band=0;band<bytes;band++)
    {
    lastline=baseline;
    for (int i=band,j=band,k=0,l;k<height;k++)
        {
        i-=band;
        filter=linein[i];
        i+=(band+1);
        limit=i+(bytes*width);
        last=lastlast=0;
        l=band;
        for(;i<limit;i+=bytes,j+=bytes)
           {
           switch(filter)
              {
           case 0:
              lineout[j] = linein[i];
              break;
           case 1:
              lineout[j] = linein[i] + last;
              break;
           case 2:
              lineout[j] = linein[i] + lastline[l];
              l+=bytes;
              break;
           case 3:
              lineout[j] = linein[i] + ( ( last + lastline[l] ) >> 1 );
              l+=bytes;
              break;
           case 4:
              lineout[j] = linein[i] +
                 paethPredictor(last,lastline[l],lastlast);
              lastlast=lastline[l];
              l+=bytes;
              break;
           default:
              printf("no such filter: %d\n",filter);
              break;
              }
           last=lineout[j];
           }
        lastline=lineout+(k*width*bytes);
        }
    }
}

static int paethPredictor(
int                       a,
int                         b,
int                           c)
{
int p,pa,pb,pc;
/*
#ifdef PlanetsDebug
printf("Predict from: %d, %d, %d\n",a,b,c);
#endif
*/
p = a + b - c;
pa = abs(p - a);
pb = abs(p - b);
pc = abs(p - c);
if (pa <= pb && pa <= pc) return a;
else if (pb <= pc) return b;
return c;
}

static UCHAR *blowUp(
UCHAR               *ba,
int                     fullWidth,
int                               fullHeight,
int                                          depth,
int                                                useType)
{
int limit, cycle;
UCHAR maskset1[] = {'\x80','\x40','\x20','\x10','\x08','\x04','\x02','\x01'};
UCHAR maskset2[] = {'\xc0','\x30','\x0c','\x03'};
UCHAR maskset4[] = {'\xf0','\x0f'};
UCHAR *mask;
UCHAR *result;
int shiftset1[] = {7,6,5,4,3,2,1,0};
int shiftset2[] = {6,4,2,0};
int shiftset4[] = {4,0};
int *shift;
int table1[] = {0, 255};
int table2[] = {0, 85, 170, 255};
int table4[] = {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255};
int *table;

if (depth==1)
   {
   mask=maskset1;
   shift=shiftset1;
   table=table1;
   }
else if (depth==2)
   {
   mask=maskset2;
   shift=shiftset2;
   table=table2;
   }
else
   {
   mask=maskset4;
   shift=shiftset4;
   table=table4;
   }
cycle=8/depth;
result=new UCHAR[fullWidth * fullHeight];

limit=fullWidth * fullHeight;
for (int i=0,j=0,step=0,l=1;i<limit;i++)
    {
    result[i] = (ba[j] & mask[step]) >> shift[step];
    if (useType!=palette) result[i] = table[result[i]];
    if ( (++step) == cycle || l==fullWidth)
       {
       step=0;
       j++;
       if (l==fullWidth) l=1;
       else l++;
       }
    else l++;
    }
/*
#ifdef PlanetsDebug
for (int j=0;j<fullHeight;j++)
    {
    int i;
    printf("expanded:\n");
    i = j*fullWidth;
    limit=i+fullWidth;
    for (;i<limit;i++) printf("%03d ",result[i]);
    printf("\n");
    }
#endif
*/
return result;
}

QByteArray *XCLPngPrivateSelector(
QByteArray                       *filterMe,
XCLFilter                                 &filter)
{
static QStringList pngTrigger("adaptive");
static QByteArray *(*pngFunction[])
       (QByteArray* filterMe, XCLFilter &filter) =
   {
   XCLPngAdaptive
   };

for (int i=0;i<pngTrigger.size(); i++)
    {
    if (pngTrigger[i]==filter.getFilterValue())
       return (*pngFunction[i]) (filterMe,filter);
    }
throw new XCLException(QObject::tr("No filter '%1' in filter class '%2'").arg(filter.getFilterValue()).arg(filter.getFilterName()));
}

