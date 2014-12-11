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
#include "XCLDecompressorLib.h"

// static QByteArray *XCLDecompressorMuLaw(QByteArray *filterMe, XCLFilter &filter);
// 
// static QByteArray *XCLDecompressorMuLaw(QByteArray *filterMe, XCLFilter &filter)
// {
// //--ToDo: Mu-Law Algorithm
// }



static QByteArray *XCLDecompressorZlib(QByteArray *filterMe, XCLFilter &filter);

static QByteArray *XCLDecompressorZlib(QByteArray *filterMe, XCLFilter &filter)
{
  z_streamp zControl;
  QQueue<QByteArray *> queue;
  UCHAR *buffer;
  QByteArray *result, *fragment;
  int code;

#ifdef PlanetsDebug
  XCLDebugDump("before zlib",filterMe);
#endif

  zControl=new z_stream;
  zControl->next_in = (unsigned char *)filterMe->data();
  zControl->avail_in = filterMe->size();
  zControl->zalloc = Z_NULL;
  zControl->zfree = Z_NULL;
  zControl->opaque = NULL;
  zControl->total_in = zControl->total_out = 0;

  buffer = new UCHAR[128];

  //std::cout<<"HALLO "<<zControl->avail_in<<"\n";
  //std::cout<<"HALLO "<<zControl->next_in<<"\n";
  if (inflateInit(zControl) != Z_OK) return NULL;

  while(zControl->avail_in)
      {
      zControl->next_out = buffer;
      zControl->avail_out = 128;
      code = inflate(zControl,Z_SYNC_FLUSH);

      if (code == Z_STREAM_END)
	break;

      if (code != Z_OK)
	return NULL;

      fragment = new QByteArray((const char *)buffer,128-zControl->avail_out);
      queue.enqueue(fragment);
	  
      }

  result=new QByteArray;
  result->reserve(zControl->total_out);

  while (!queue.isEmpty())
	{
	fragment=queue.dequeue();
	result->append(*fragment);
	delete fragment;	
	}
  result->append(QByteArray((const char *)buffer,128-zControl->avail_out));

  inflateEnd(zControl);
  delete[] buffer;
  delete zControl;

  #ifdef PlanetsDebug
  XCLDebugDump("after zlib",result);
  std::cout<<"PAGE END\n";
  #endif

  return result;
  // Useless statement, but less compiler-warnings, because param IS in use!
  (void)filter;
}

static QByteArray *XCLDecompressorFlateDecode(QByteArray *filterMe, XCLFilter &filter);

static QByteArray *XCLDecompressorFlateDecode(
QByteArray                            *filterMe,
XCLFilter                                      &filter)
{
return XCLDecompressorZlib(filterMe,filter); 
}

//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
static QByteArray *XCLDecompressorMuLaw(QByteArray *filterMe, XCLFilter &filter);

static QByteArray *XCLDecompressorMuLaw(
                                              QByteArray                            *filterMe,
                                              XCLFilter                                      &filter)
{
    
//     std::cout<<"\n in *XCLDecompressorMuLaw \n ";
    QByteArray *result;
    
    static short MuLawDecompressTable[256] =
    {
        -32124,-31100,-30076,-29052,-28028,-27004,-25980,-24956,
        -23932,-22908,-21884,-20860,-19836,-18812,-17788,-16764,
        -15996,-15484,-14972,-14460,-13948,-13436,-12924,-12412,
        -11900,-11388,-10876,-10364, -9852, -9340, -8828, -8316,//32
        -7932, -7676, -7420, -7164, -6908, -6652, -6396, -6140,
        -5884, -5628, -5372, -5116, -4860, -4604, -4348, -4092,
        -3900, -3772, -3644, -3516, -3388, -3260, -3132, -3004,
        -2876, -2748, -2620, -2492, -2364, -2236, -2108, -1980,
        -1884, -1820, -1756, -1692, -1628, -1564, -1500, -1436,
        -1372, -1308, -1244, -1180, -1116, -1052,  -988,  -924,
        -876,  -844,  -812,  -780,  -748,  -716,  -684,  -652,
        -620,  -588,  -556,  -524,  -492,  -460,  -428,  -396,
        -372,  -356,  -340,  -324,  -308,  -292,  -276,  -260,
        -244,  -228,  -212,  -196,  -180,  -164,  -148,  -132,
        -120,  -112,  -104,   -96,   -88,   -80,   -72,   -64,
        -56,   -48,   -40,   -32,   -24,   -16,    -8,     0,
        32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956,
        23932, 22908, 21884, 20860, 19836, 18812, 17788, 16764,
        15996, 15484, 14972, 14460, 13948, 13436, 12924, 12412,
        11900, 11388, 10876, 10364,  9852,  9340,  8828,  8316,
        7932,  7676,  7420,  7164,  6908,  6652,  6396,  6140,
        5884,  5628,  5372,  5116,  4860,  4604,  4348,  4092,
        3900,  3772,  3644,  3516,  3388,  3260,  3132,  3004,
        2876,  2748,  2620,  2492,  2364,  2236,  2108,  1980,
        1884,  1820,  1756,  1692,  1628,  1564,  1500,  1436,
        1372,  1308,  1244,  1180,  1116,  1052,   988,   924,
        876,   844,   812,   780,   748,   716,   684,   652,
        620,   588,   556,   524,   492,   460,   428,   396,
        372,   356,   340,   324,   308,   292,   276,   260,
        244,   228,   212,   196,   180,   164,   148,   132,
        120,   112,   104,    96,    88,    80,    72,    64,
        56,    48,    40,    32,    24,    16,     8,     0
    }; 
    
    int  MuLawCompressedStream[10] =
    {
        0, 5,99,255, 78,55,5,11,125,222  
    };
//     std::cout<<" \n uncompressed Data:\n"<<sizeof(MuLawCompressedStream)<<"\n";
 
   
//      std::cout<<" \n uncompressed Data:\n"<<sizeof(MuLawCompressedStream)<<"\n";
     for(int j = 0; j < sizeof(MuLawCompressedStream)/sizeof(MuLawCompressedStream[0]); j++){
     
       
         short index=MuLawCompressedStream[j];
           short indexValue=MuLawDecompressTable[index];
     
//            std::cout<<"/"<<index<<"="<<indexValue;
       
   }
//    std::cout<<" \n\n";
    
    
    return XCLDecompressorZlib(filterMe,filter); 
    //returb XCLDecompressorMuLaw(filterMe,filter);
}

//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------



QByteArray *XCLCompressionSelector( QByteArray *filterMe, XCLFilter &filter)
{
  static QStringList decompressionTrigger("zlibDeflateInflate");
  decompressionTrigger.append("FlateDecode");
  decompressionTrigger.append("Mu-Law");
  
  static QByteArray *(*decompressionFunction[])
	(QByteArray* filterMe, XCLFilter &filter) =
    {
    XCLDecompressorZlib,
    XCLDecompressorFlateDecode,
    XCLDecompressorMuLaw
    };

  for (int i=0;i<decompressionTrigger.size(); i++)
  {
      if (decompressionTrigger[i]==filter.getFilterValue())
	return (*decompressionFunction[i]) (filterMe,filter);
  }
  throw new XCLException(QObject::tr("No filter '%1' in filter class '%2'").arg(filter.getFilterValue()).arg(filter.getFilterName()));
}



#ifdef PlanetsDebug
void XCLDebugDump(
QString           prompt,
QByteArray              *ba)
{
for (int i=0;i<ba->size();i++)
    {
    if (!(i%32)) printf("\n%s:\n",prompt.toLatin1().data());
    printf("%03d ",(unsigned char)(*ba)[i]);
    }
printf("\n");
}
#endif
