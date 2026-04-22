#ifndef _MYDEFS_H
#define _MYDEFS_H

#include <stdint.h>

typedef enum 
{
  VG_FALSE = 0,
  VG_TRUE  = 1
} 
VG_BOOLEAN;


typedef int s32;
typedef unsigned int u32;
typedef short s16;
typedef unsigned short u16;
typedef char s8;
typedef unsigned char u8;

typedef unsigned char  UBYTE;
typedef char  CHAR;
typedef unsigned char  BYTE;
typedef signed char    INT8;
typedef unsigned char  UINT8;
typedef short          INT16;
typedef unsigned short UINT16;
typedef int            INT32;
typedef unsigned int   UINT32;
typedef uintptr_t      UINTPTR;
typedef char * LPSTR;

typedef struct _mc_rect
{
    int left;
    int right;
    int top;
    int bottom;
} MC_RECT;

typedef struct _mc_paletteentry {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
} MC_PALETTEENTRY;

typedef struct _mc_point {
    int x;
    int y;
} MC_POINT, *PMC_POINT;

#endif /* _MYDEFS_H */
