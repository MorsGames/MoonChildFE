#ifndef _BOUNCEY_H
#define _BOUNCEY_H

#include <objects.hpp>

struct BOUNCEY_BLOOD {
  INT32     realx;
  INT16     gravy;
  INT16     spdx;
  INT16     lifespan;
};

void bouncey_init(UINT16 x, UINT16 y, INT16 spdx, INT16 spdy);
int  bouncey_live (OBJECT *object, UINT32 param);
void bouncey_death(OBJECT *object, UINT32 param);
void bouncey_clear(OBJECT *object, UINT32 param);

#endif
