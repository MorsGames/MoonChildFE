/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       fastfile.h
 *  Content:    Definitions for fastfile access.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTBILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/

#include "ffent.h"
typedef void *HFASTFILE;

typedef struct {
    bool        inuse;
    int        pos;
    int        size;
    LPFILEENTRY pfe;
} FILEHANDLE, *LPFILEHANDLE;

extern int FastFileInit( char *fname, int max_handles );
extern void FastFileFini( void );
extern HFASTFILE FastFileOpen( char *name );
extern bool FastFileClose( HFASTFILE pfe );
extern bool FastFileRead( HFASTFILE pfh, void *ptr, int size );
extern bool FastFileSeek( HFASTFILE pfe, int off, int how );
extern long FastFileTell( LPFILEHANDLE pfh );
extern void *FastFileLock( LPFILEHANDLE pfh, int pos, int size );
extern bool FastFileUnlock( LPFILEHANDLE pfh, int pos, int size );

