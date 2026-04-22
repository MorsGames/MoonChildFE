#ifndef _MAIN_H
#define _MAIN_H

#include <mydefs.hpp>
#include "Random.h"
//#include <conio.h>

/* Various defines */
#define ASCII_LEFTKEY  16
#define ASCII_RIGHTKEY 17
#define ASCII_UPKEY    18
#define ASCII_DOWNKEY  19

#define JUMPHEIGHT 54
#define CLIPMARGINX 0
#define CLIPMARGINY 64


//#define MC_RANDOM(scale) (((rand()>>16)*scale)/32768)
#define MC_RANDOM(scale) (PCG32Random(scale))
#define RASTCOL2(c,r,g,b)

//#define RASTCOL2(c,r,g,b) _outp(0x3c8,(c));   \
//                          _outp(0x3c9,(r)); \
//                          _outp(0x3c9,(g)); \
//                          _outp(0x3c9,(b))

#define PI 3.1415926535

/* Various typedefs */
typedef struct LEVEL_DESCR LEVEL_DESCR;
typedef struct VIEWPORT VIEWPORT;
typedef struct PREFS PREFS;
typedef struct OBJECT OBJECT;
typedef struct MAP_ITEM MAP_ITEM;
typedef struct MAP_DESCR MAP_DESCR;
typedef struct ANIM ANIM;
typedef struct SEQUENCE SEQUENCE;
typedef struct TRIGGER_ITEM TRIGGER_ITEM;
typedef struct SCR_TRIGGER_ITEM SCR_TRIGGER_ITEM;
typedef void (*LEVEL_INIT_FN)(void);
typedef void (*WAV_FN)(void);
typedef void (*WAV2_FN)(INT32, INT32);
typedef void (*TRIGGER_FN)(void);
typedef void (*SCR_TRIGGER_FN)(void);
typedef void (*MENU_FN)(void);
typedef void (*OBJECT_CB )(OBJECT *object, UINT32 param);
typedef int  (*OBJECT_CBR)(OBJECT *object, UINT32 param);

struct LEVEL_DESCR
{
  char *descr;          // filename of assets description file
  char *parallax;       // filename of parallax layer
  char *patspic;        // filename of pattern picture
  char *map;            // filename of mapdata
  char *colmap;         // filename of collisionmapdata
  UINT16         mapsizex;       // width of map  (in pixels)
  UINT16         mapsizey;       // height of map (in pixels)
  LEVEL_INIT_FN  level_init_fn;  // Level to init particular level
  TRIGGER_ITEM  *triglist;       // list with trigger descriptions for level
};


void framework_ExitGame(void);
HEARTBEAT_FN MC_heartbeat(void);
HEARTBEAT_FN framework_InitGame(Cvideo *newvideo);

int collision(UINT16 x, UINT16 y);
void bullet_swap(void);
OBJECT *bullet_check(OBJECT *bullet);
void bullet_add(OBJECT *bullet);
void bullet_init(void);


VG_BOOLEAN loadfile(char * fname, char *buffer, UINT32 length);
VG_BOOLEAN savefile(char * fname, char *buffer, UINT32 length);
VG_BOOLEAN loaddocfile(char * fname, char *buffer, UINT32 length);
VG_BOOLEAN savedocfile(char * fname, char *buffer, UINT32 length);

#endif






