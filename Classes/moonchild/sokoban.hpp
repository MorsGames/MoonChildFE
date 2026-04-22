#ifndef _SOKOBAN_H
#define _SOKOBAN_H

#include <objects.hpp>

typedef struct SOKOBAN_POSSES SOKOBAN_POSSES;
typedef struct SOKOBAN_GROUP SOKOBAN_GROUP;
typedef struct SOKOBAN_BLOOD SOKOBAN_BLOOD;

struct SOKOBAN_POSSES {
  UINT16 seqnr;
  UINT16 moveability;             // bit 0 is x-dir ...bit 1 is y-dir
  UINT16 xpos[16];
  UINT16 ypos[16];
};

struct SOKOBAN_GROUP {
  UINT16          checkflg;         // 0 = no check, 1 = still to be checked, 2 = approved
  UINT16          movingflg;        // is this group moving?
  UINT16          seqnr;            // appearance
  UINT16          moveability;      // bit 0 is x-dir ...bit 1 is y-dir
  OBJECT         *block[16];        // max 16 blocks
  SOKOBAN_POSSES *orgpos;           // original posses
};

struct SOKOBAN_BLOOD {
  UINT16          movingcnt;
  UINT16          directionflg;     // in which direction are eventual blocks moving?
                                    // 0 = north, 1 = east, 2 = south, 3 = west
  SOKOBAN_GROUP  *groups[4];
};


OBJECT *sokoban_init(SOKOBAN_POSSES *group1,
         SOKOBAN_POSSES *group2,
         SOKOBAN_POSSES *group3,
         SOKOBAN_POSSES *group4);
int  sokoban_live (OBJECT *object, UINT32 param);
void sokoban_death(OBJECT *object, UINT32 param);
void sokoban_clear(OBJECT *object, UINT32 param);

void sokoban_reinit(OBJECT *soko);
int  sokoban_collision(OBJECT *soko, UINT16 mx, UINT16 my, UINT16 mw, UINT16 mh, UINT16 direction);
int  sokocollision(OBJECT *soko, UINT16 bx, UINT16 by, UINT16 group);

#endif
