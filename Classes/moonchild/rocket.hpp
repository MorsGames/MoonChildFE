#ifndef _ROCKET_H
#define _ROCKET_H

#include <objects.hpp>

struct ROCKET_BLOOD {
  INT16     gravy;
  INT16     accy;
  UINT16     x;
  UINT32     y;
  UINT16     lifespan;
  UINT32     miny;
  UINT16     anmcnt;
};

void rocket_init (UINT16 x, UINT16 y, UINT16 miny, INT16 accy);
int  rocket_live (OBJECT *object, UINT32 param);
void rocket_death(OBJECT *object, UINT32 param);
void rocket_clear(OBJECT *object, UINT32 param);

#endif
