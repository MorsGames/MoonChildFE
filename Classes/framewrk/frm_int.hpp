#ifndef _FRM_WRK_H
#define _FRM_WRK_H

//uses same define as frm_int.hpp!!! DON'T CHANGE!

//REMOVE LOG DEFINE FOR NO 'ERROR to FILE LOGGING'


#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <string.h>
#include <stdarg.h>

//#include <types.h>
#include "mydefs.hpp"
//#include "basedefs.hpp"
//#include "vgassert.hpp"
//#include "log.hpp"
#include "sprite.hpp"
#include "video.hpp"
#include "audio.hpp"
#include "timer.hpp"
#include "pcx.hpp"
#include "pcxff.hpp"
#include "movie.hpp"
#include "fastfile.h"

#define FW_KEYUP       (0)
#define FW_KEYDOWN     (1)
#define FW_LBUTTONDOWN (2)
#define FW_RBUTTONDOWN (3)
#define FW_LBUTTONUP   (4)
#define FW_RBUTTONUP   (5)
#define FW_MOUSEMOVE   (6)
#define FW_CDREADY     (7)

// Functions the user can call
void framework_util_SetMouse(int x, int y);
void framework_usefastfile(bool offon);
void framework_util_displayerror(char *errstring);

#define RASTCOL(r,g,b) _outp(0x3c8,0);   \
                       _outp(0x3c9,(r)); \
                       _outp(0x3c9,(g)); \
                       _outp(0x3c9,(b))


typedef void * (*HEARTBEAT_FN )(void);
void __cdecl Err( LPSTR fmt, ... );         // This is a generic error reporter. Just does a MsgBox

#ifdef _IN_MAIN

HEARTBEAT_FN             heartbeat;
Cvideo                  *lvideo;
Caudio                  *laudio;
Ctimer                  *ltimer;
Cmovie                  *lmovie;

// COM DirectDraw object

bool                     gbGameLoop;
bool                     frmwrk_usefastfile;
char                     szAppName[] = "Moon Child";
char                     szCaption[] = "Moon Child";


#else

extern HEARTBEAT_FN             heartbeat;
extern Cvideo                  *video;
extern Caudio                  *audio;
extern Ctimer                  *timer;
extern Cmovie                  *movie;

// COM DirectDraw object

extern bool                     gbGameLoop;
extern bool                     frmwrk_usefastfile;

extern char                     szAppName[];
extern char                     szCaption[];
#endif

#endif

