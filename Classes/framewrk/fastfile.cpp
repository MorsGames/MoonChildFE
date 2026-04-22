/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       fastfile.c
 *  Content:    Fast file I/O for large numbers of files.
 *              Uses a single file built using FFCREATE.EXE; this file
 *              contains a directory + all the files.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTBILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/
//#include <io.h>
//#include <fcntl.h>
//#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fastfile.h"
#include "mydefs.hpp"

// strip the directory path of all files provided to open
#define USEBASENAME 1

#define TESTCASE

//#define LOG

#define FLOG

char cachename[128];
int cachesize;


extern char *FullPath2( char *a_File );

//#define LOG(x)  { \
//	  FILE *log; \
//	  log = fopen(FullPath2("log.txt"),"a"); \
//	  if(log) \
//{ \
//		  fwrite((x), strlen(x), 1, log); \
//		  fclose(log); \
//		  fflush(log); \
//} \
//} 

#define LOG(x)



#define GZIP
typedef unsigned char  Byte;
#include "zlib.h"

int Compare( LPFILEENTRY p1, LPFILEENTRY p2 );

char FASTFILE_FILENAME[256];




static int              LockCount;
static LPFILEENTRY      pFE;
static BYTE            *pBase;
static int              dwFECnt;	// nr of files
static LPFILEHANDLE     lpFH;		// fileentries
static int              dwFHCnt;
static long             lFileEnd;

static FILE *fHndl = 0;

unsigned char *DecrunchBuffer;
unsigned char *SourceBuffer;
/*
 * Compare 
 *
 * bsearch comparison routine
 */
int stricmp( char* s1, char* s2 );

int Compare( LPFILEENTRY p1, LPFILEENTRY p2 )
{
    int i;
    i = ( stricmp( (p1)->name,(p2)->name ) );
    return i;
    
} /* Compare */

int stricmp( char* s1, char* s2 )
{
	char* a = s1;
	char* b = s2;
	while ((*a) || (*b))
	{
		char c1 = *a;
		char c2 = *b;
		if ((c1 >= 'A') && (c1 <= 'Z')) c1 -= 'A' - 'a';
		if ((c2 >= 'A') && (c2 <= 'Z')) c2 -= 'A' - 'a';
		if (c1 == c2)
		{
			a++;
			b++;
		}
		else if (c1 < c2) return -1; else return 1; 
	}

	if((*a)==0 && (*b)==0) return 0;
	if((*a)!=0 && (*b)==0) return 1;
	if((*a)==0 && (*b)!=0) return -1;

	// hier kan ie niet komen
	return 0;
}


/*
 * FastFileInit
 *
 * Initialize for fast file access. The master file and maximum number
 * of open "files" are specified.
 */
int FastFileInit( char *fname, int max_handles )
{
//    HRSRC  h;

	cachename[0] = 0;
    LockCount = 0;
    FastFileFini();

	DecrunchBuffer = (unsigned char *)malloc(830000);
	SourceBuffer = (unsigned char *)malloc(400000);

	if(DecrunchBuffer)
	{
		LOG("decrunchbuffer allocated\n");
	}
	if(SourceBuffer)
	{
		LOG("sourcebuffer allocated\n");
	}

    /*
     * get a file handle array
     */
    lpFH = (FILEHANDLE *)malloc(max_handles * sizeof( FILEHANDLE ) );
    if( lpFH == NULL ) {

        return false;
    }
    memset(lpFH, 0, max_handles * sizeof(FILEHANDLE));
    dwFHCnt = max_handles;


	strcpy(FASTFILE_FILENAME, fname);
//		LOG("opening fastfile\n");

	fHndl = fopen(fname, "rb");
	if(fHndl)
	{
		/*
		 * get initial data from the memory mapped file
		 */
//		LOG("fastfileopen\n");

		fread(&dwFECnt, 1, 4, fHndl);

		pBase = (BYTE *) malloc(sizeof(FILEENTRY) * dwFECnt + 4);
		memcpy((void *)pBase, (void *)&dwFECnt, 4);

		pFE = (FILEENTRY *)(pBase+4);
		fread(pFE, 1, sizeof(FILEENTRY) * dwFECnt, fHndl);
		fseek(fHndl, 0, SEEK_SET );		// rewind
	    lFileEnd = pFE[dwFECnt-1].offset;

	}
	else
	{
//		LOG("fastfile open failed\n");
	    return false;
	}


    return true;

} /* FastFileInit */

/*
 * FastFileFini
 *
 * Clean up resources
 */
void FastFileFini( void )
{

	free(DecrunchBuffer);
	free(SourceBuffer);

   //
    //  dont unmap things if we have locks out standing
    //
    if (LockCount != 0)
        return;

    if( lpFH != NULL ) {
        free( lpFH );
        lpFH = NULL;
    }
	if(fHndl)
	{
		fclose(fHndl);
		fHndl = 0;
	}
    dwFHCnt = 0;
    pBase = NULL;
    dwFECnt = 0;
    pFE = NULL;

} /* FastFileFini */

/*
 * FastFileOpen
 *
 * Search the directory for the file, and return a file handle if found.
 */
HFASTFILE FastFileOpen( char * name )
{

    FILEENTRY   fe;
    LPFILEENTRY pfe;
#if USEBASENAME
    char *baseptr;
#endif // USEBASENAME

    
//		LOG("reading file: ");
//		LOG(name);
//		LOG("\n");

	if( pFE == NULL ) {
        return NULL;
    }
    if( name == NULL || name[0] == 0 ) {
        return NULL;
    }
//	return 0;

#if USEBASENAME
    baseptr = name + strlen(name)-1; // set at last char
    while (baseptr > name) {
        if (*baseptr == '/' || *baseptr == '\\' || *baseptr == ':') {
            baseptr++;
            break;
        }
        baseptr--;
    }
    strncpy( fe.name, baseptr,15 );
    fe.name[15] = 0;
#else // !USEBASENAME
    strncpy( fe.name, name,15 );
    fe.name[15] = 0;
#endif // USEBASENAME

	int idx;
	idx = strlen(fe.name);

	fe.name[idx-1]=0; //verander pcx extensie in rz extensie
	fe.name[idx-2]='z';
	fe.name[idx-3]='r';


		LOG("afterconversion: ");
		LOG(fe.name);
		LOG("\n");


#if 1
  {
        int i;

        pfe = NULL;
        for (i=0; i<dwFECnt; i++) {
            int rc;
            rc = stricmp(pFE[i].name, fe.name); 
            if((i%100)==0)
            {
                int t=0;
                t+=1;
            }
            if (rc == 0) {
                pfe = &pFE[i];
                break;
            }
        }
  }
#else        
    pfe = bsearch( &fe, pFE, dwFECnt, sizeof( FILEENTRY ), (LPVOID) Compare );
#endif
		LOG("searching file\n");
    
    if( pfe != NULL ) {
		LOG("filefound\n");
        int   i;
        for( i=0;i<dwFHCnt;i++ ) {
            if( !lpFH[i].inuse ) {
                lpFH[i].inuse = true;
                lpFH[i].pos = pfe->offset;
                lpFH[i].size = (pfe+1)->offset - pfe->offset;
                lpFH[i].pfe = pfe;

				//check if the last file was the same filename.
				//if so dont decrunch again!!


				if(strcmp(fe.name, cachename)==0)
				{
//					LOG("cachehit\n");
					lpFH[i].size = cachesize;
					lpFH[i].pos = 0;

					return &lpFH[i];
				}
				else
				{
					//decrunch de file into the decrunchbuffer
					char buffer[500];
					unsigned long destlen;

					fseek(fHndl, lpFH[i].pos, SEEK_SET);
					fread(SourceBuffer, 1, lpFH[i].size, fHndl);

//					LOG("readingfile to sourcebuf\n");
					sprintf(buffer, "sourcelength = %d bytes\n", lpFH[i].size);
//					LOG(buffer);

//					sprintf(buffer,"decbuffer = %d,  sourcebuffer = %d\n",DecrunchBuffer, SourceBuffer);
//					LOG(buffer);

					destlen=829990;
					int rc = uncompress(DecrunchBuffer, &destlen, SourceBuffer, lpFH[i].size);



					lpFH[i].size = destlen; //updating the length to the decrunched length!

//					LOG("file decrunched\n");
					sprintf(buffer, "decrunched %d bytes\n", (int)destlen);
//					LOG(buffer);

					sprintf(buffer, "return code rc = %d\n", rc);
//					LOG(buffer);


					lpFH[i].pos = 0;

					cachesize = destlen;
					strcpy(cachename, fe.name);

					return &lpFH[i];
				}
            }
        }
    } else {
    }


    return NULL;

} /* FastFileOpen */

/*
 * FastFileClose
 *
 * Mark a fast file handle as closed
 */
bool FastFileClose( HFASTFILE pfh )
{
	LPFILEHANDLE _pfh;

	_pfh = (LPFILEHANDLE) pfh;
    if( _pfh == NULL || _pfh->inuse != true ) {
        return false;
    }
    _pfh->inuse = false;
    return true;

} /* FastFileClose */

/*
 * FastFileLock
 *
 * return a memory pointer into a fast file
 */
void *FastFileLock( LPFILEHANDLE pfh, int pos, int size )
{
	return 0;
} /* FastFileLock */

/*
 * FastFileUnlock
 *
 */
bool FastFileUnlock( LPFILEHANDLE pfh, int pos, int size )
{
	return true;

} /* FastFileUnlock */

/*
 * FastFileRead
 *
 * read from a fast file (memcpy!)
 */
bool FastFileRead( HFASTFILE pfh, void * ptr, int size )
{
	LPFILEHANDLE _pfh;
	_pfh = (LPFILEHANDLE) pfh;

    if( size < 0 ) {
        return false;
    }
    if( (_pfh->pos + size) > (_pfh->size )) {
        return false;
    }
	
	memcpy(ptr, DecrunchBuffer+_pfh->pos, size);
	_pfh->pos+=size;

	return true;
 
} /* FastFileRead */

/*
 * FastFileSeek
 *
 * Set to a new position in a fast file.  Uses standard SEEK_SET, SEEK_CUR,
 * SEEK_END definitions.
 */
bool FastFileSeek( HFASTFILE pfh, int off, int how )
{
	char buffer[500];
				LOG("fastfileseek\n");


 	LPFILEHANDLE _pfh;
	_pfh = (LPFILEHANDLE) pfh;

    if( how == SEEK_SET ) {
        if( off < 0 || off >= _pfh->size ) {
            return false;
        }
        _pfh->pos = off;
		sprintf(buffer, "seeking %d bytes from beginning\n", off);
//		LOG(buffer);
    } else if( how == SEEK_END ) {
        if( off >= _pfh->size ) {
            return false;
        }
        _pfh->pos = _pfh->size + off;      // RVV -=> changed to +!!!  otherwise not standard!
		sprintf(buffer, "seeking %d bytes from end\n", off);
//		LOG(buffer);
		sprintf(buffer, "offset is now: %d\n", _pfh->pos);
//		LOG(buffer);
    } else if( how == SEEK_CUR ) {
        _pfh->pos += off;
		sprintf(buffer, "seeking %d bytes from curpos\n", off);
//		LOG(buffer);
        if( _pfh->pos < 0 || _pfh->pos >= _pfh->size ) {
			_pfh->pos = 0;
			sprintf(buffer, "seeked too far!\n");
//			LOG(buffer);
            return false;
        }
    } else {
        return false;
    }
	
	return true;
} /* FastFileSeek */

/*
 * FastFileTell
 *
 * Get the current position in a fast file
 */
long FastFileTell( LPFILEHANDLE pfh )
{
	return pfh->pos;
	return 0;
} /* FastFileTell */
