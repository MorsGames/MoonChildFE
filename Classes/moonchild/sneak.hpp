#ifndef _SNEAK_H
#define _SNEAK_H

#include <objects.hpp>

typedef struct SNEAK_BLOOD SNEAK_BLOOD;

struct SNEAK_BLOOD {
  INT32 x;
  INT32 y;
  INT16 spdx;
  INT16 spdy;
};

void sneak_init (UINT16 x, UINT16 y);
int  sneak_live (OBJECT *object, UINT32 param);
void sneak_death(OBJECT *object, UINT32 param);
void sneak_clear(OBJECT *object, UINT32 param);

#endif
