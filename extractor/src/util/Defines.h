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
#include <limits>
#ifndef __DEFINES_H_
#define __DEFINES_H_



// #define PlanetsDebug



/*Boolean enumeration values*/

#ifndef FALSE
#define FALSE                           0
#endif /*FALSE*/
#ifndef TRUE
#define TRUE                            1
#endif /*TRUE*/


#ifndef NULL
#define NULL                            ((void *) 0)
#endif /*NULL*/

#ifndef NIL
#define NIL                             ((void *) 0xeeeeeeeeuL)
#endif /*NIL*/



typedef unsigned short 			BOOL;
//32 Bit definitions
typedef unsigned char                   BYTE;
typedef unsigned short                  WORD;
typedef unsigned long int               LWORD;
typedef unsigned char			UCHAR;
typedef unsigned long long int          DWORD;
// 32BIT definitions end--------

//64Bit definitions
typedef unsigned char                   _UINT8;
typedef signed char                     _SINT8;

typedef signed short                    _SINT16;
typedef unsigned short                  _UINT16;

typedef signed int                	_SINT32;
typedef unsigned int              	_UINT32;

typedef signed long int                	_SINT64;
typedef unsigned long int              	_UINT64;

typedef unsigned long int		_LENGTH;
typedef signed long int			_SIZE;
typedef signed int			_VECTORLENGTH;

static const _UINT16 UINT16_MIN = std::numeric_limits<_UINT16>::min();
static const _UINT16 UINT16_MAX = std::numeric_limits<_UINT16>::max();
/*std::numeric_limits<_UINT32>::min()
std::numeric_limits<_UINT32>::max()
std::numeric_limits<_UINT64>::min()
std::numeric_limits<_UINT64>::max()
std::numeric_limits<_SINT16>::min()
std::numeric_limits<_SINT16>::max()
std::numeric_limits<_SINT32>::min()
std::numeric_limits<_SINT32>::max()
std::numeric_limits<_SINT64>::min()
std::numeric_limits<_SINT64>::max()*/
static const _LENGTH LENGTH_MIN = std::numeric_limits<_LENGTH>::min();
static const _LENGTH LENGTH_MAX = std::numeric_limits<_LENGTH>::max();


/*Some common byte/word transformations*/

#define LOBYTE(w)                       ((BYTE)          (w)            )
#define HIBYTE(w)                       ((BYTE)  (((WORD)(w)) >> 8)     )

#define LOWORD(l)                       ((WORD)          (l)            )
#define HIWORD(l)                       ((WORD) (((LWORD)(l)) >> 16)    )

#define BYTES2WORD(a, b)                (        (((WORD)(a)) <<  8) + \
                                                         (b)            )

#define BYTES2UINT24(a, b, c)                (   (((LWORD)(a)) << 16) + \
                                                 (((LWORD)(b)) <<  8) + \
                                                          (c)            )

#define BYTES2LWORD(a, b, c, d)         (       (((LWORD)(a)) << 24) + \
                                                (((LWORD)(b)) << 16) + \
                                                (((LWORD)(c)) <<  8) + \
                                                         (d)            )

#define BYTES2DWORD(a, b, c, d, e, f, g, h)    ( (((DWORD)(a)) << 56)+ \
                                                (((DWORD)(b)) <<  48) + \
                                                (((DWORD)(c)) <<  40) + \
                                                (((DWORD)(d)) <<  32) + \
                                                (((DWORD)(e)) <<  24) + \
                                                (((DWORD)(f)) <<  16) + \
                                                (((DWORD)(g)) <<  8)  + \
                                                          (h)              )

/*Other helper macros*/

#define DIM(a) (sizeof(a)/sizeof((a)[0]))

/*Bit field labels for readability*/

#define BIT0                                0x0001u
#define BIT1                                0x0002u
#define BIT2                                0x0004u
#define BIT3                                0x0008u
#define BIT4                                0x0010u
#define BIT5                                0x0020u
#define BIT6                                0x0040u
#define BIT7                                0x0080u
#define BIT8                                0x0100u
#define BIT9                                0x0200u
#define BIT10                               0x0400u
#define BIT11                               0x0800u
#define BIT12                               0x1000u
#define BIT13                               0x2000u
#define BIT14                               0x4000u
#define BIT15                               0x8000u
#define BIT16                           0x00010000uL
#define BIT17                           0x00020000uL
#define BIT18                           0x00040000uL
#define BIT19                           0x00080000uL
#define BIT20                           0x00100000uL
#define BIT21                           0x00200000uL
#define BIT22                           0x00400000uL
#define BIT23                           0x00800000uL
#define BIT24                           0x01000000uL
#define BIT25                           0x02000000uL
#define BIT26                           0x04000000uL
#define BIT27                           0x08000000uL
#define BIT28                           0x10000000uL
#define BIT29                           0x20000000uL
#define BIT30                           0x40000000uL
#define BIT31                           0x80000000uL


#endif /*DEFINES_H_*/
