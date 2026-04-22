#ifndef _ROBYN_H
#define _ROBYN_H

#include <objects.hpp>

struct ROBYN_BLOOD {
  INT32     realx;
  INT16     gravy;
  INT16     spdx;
  INT16     lifespan;
  INT16     bouncecnt;
};

void robyn_init(UINT16 x, UINT16 y, INT16 spdx, INT16 spdy);
int  robyn_live (OBJECT *object, UINT32 param);
void robyn_death(OBJECT *object, UINT32 param);
void robyn_clear(OBJECT *object, UINT32 param);

#endif
