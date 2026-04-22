#ifndef _PLOF_H
#define _PLOF_H

#include <objects.hpp>

struct PLOF_BLOOD {
  INT32     x;
  INT32     y;
  INT16     spdx;
  INT16     spdy;
  INT16     lifespan;
};

void plof_init(UINT16 x, UINT16 y, UINT16 sequencenr, UINT16 lethal, INT16 spdx, INT16 spdy, UINT16 nosound);
int  plof_live (OBJECT *object, UINT32 param);
void plof_death(OBJECT *object, UINT32 param);
void plof_clear(OBJECT *object, UINT32 param);

#endif
