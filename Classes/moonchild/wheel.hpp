#ifndef _WHEEL_H
#define _WHEEL_H

#include <objects.hpp>

typedef struct WHEEL_BLOOD WHEEL_BLOOD;

struct WHEEL_BLOOD {
  UINT32     x;
  UINT32     y;
  INT16      spdx;
  INT16      spdy;
  UINT16     boty;
};


void wheel_init(UINT16 x, UINT16 y, UINT16 boty);
int  wheel_live (OBJECT *object, UINT32 param);
void wheel_death(OBJECT *object, UINT32 param);
void wheel_clear(OBJECT *object, UINT32 param);

#endif
