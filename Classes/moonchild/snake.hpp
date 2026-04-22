#ifndef _SNAKE_H
#define _SNAKE_H

#include <objects.hpp>

struct SNAKETAB {
  UINT16     numelements;
  UINT16     sinx1cnt;
  INT16     sinx1spd;
  INT16     sinx1add;
  UINT16     siny1cnt;
  INT16     siny1spd;
  INT16     siny1add;
  UINT16     sinx2cnt;
  INT16     sinx2spd;
  INT16     sinx2add;
  UINT16     siny2cnt;
  INT16     siny2spd;
  INT16     siny2add;
};

struct SNAKE_BLOOD {
  UINT16      x;
  UINT16      y;
  UINT16      lastx;
  UINT16      lasty;
  SNAKETAB    snaketab;
  UINT16      shootdelay;
  UINT16      shootflg;
  UINT16      destroyable;
  UINT16      sinamp;
  OBJECT_CB   ready_fn;
  OBJECT     *elements[1];
};

void snake_init(UINT16 x, UINT16 y, ANIM *snakeanim, UINT16 seqnr, UINT16 sinamp,
		UINT16 shootflg, UINT16 destroyable, OBJECT_CB ready_fn, SNAKETAB *snaketab);
int  snake_live (OBJECT *object, UINT32 param);
void snake_death(OBJECT *object, UINT32 param);
void snake_clear(OBJECT *object, UINT32 param);

#endif
