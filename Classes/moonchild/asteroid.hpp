#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <objects.hpp>

typedef struct ASTEROID_BLOOD ASTEROID_BLOOD;

struct ASTEROID_BLOOD {
  INT32  x;
  INT32  y;
  INT16  gravy;
  INT16  spdx;
  INT16  spdy;
  UINT16 tx;
  UINT16 ty;
  UINT16 bx;
  UINT16 by;
  UINT16 minenr;
  UINT16 hitcnt;
  UINT16 shootdelay;
};

void asteroid_init (UINT16 x, UINT16 y, UINT16 tx, UINT16 ty, UINT16 bx, UINT16 by, INT16 spdx, INT16 spdy, INT16 gravy, UINT16 minenr);
int  asteroid_live (OBJECT *object, UINT32 param);
void asteroid_death(OBJECT *object, UINT32 param);
void asteroid_clear(OBJECT *object, UINT32 param);

#endif
