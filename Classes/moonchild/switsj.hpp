#ifndef _SWITSJ_H
#define _SWITSJ_H

#include <objects.hpp>

typedef struct SWITSJ_BLOOD SWITSJ_BLOOD;
typedef struct WALL_BLOOD   WALL_BLOOD;

struct SWITSJ_BLOOD {
  OBJECT    *wall;
  UINT16    dir;
  UINT16    coll;
};

struct WALL_BLOOD {
  UINT16    camdelay;
  UINT16    camdir;
  UINT16    dir;
  UINT16    stuit;
};


OBJECT *switsj_init (UINT16 x, UINT16 y, UINT16 wallx, UINT16 wally, UINT16 openflg);
int  switsj_live (OBJECT *object, UINT32 param);
void switsj_death(OBJECT *object, UINT32 param);
void switsj_clear(OBJECT *object, UINT32 param);

OBJECT *wall_init (UINT16 x, UINT16 y, UINT16 openflg);
int  wall_live (OBJECT *object, UINT32 param);
void wall_death(OBJECT *object, UINT32 param);
void wall_clear(OBJECT *object, UINT32 param);

#endif
