#ifndef _SPIKE_H
#define _SPIKE_H

#include <objects.hpp>

struct SPIKE_BLOOD {
  UINT16     gravy;
  UINT16     x;
  UINT32     y;
  UINT16     lifespan;
  UINT32     maxy;
};

void spike_init (UINT16 x, UINT16 y, UINT16 maxy);
int  spike_live (OBJECT *object, UINT32 param);
void spike_death(OBJECT *object, UINT32 param);
void spike_clear(OBJECT *object, UINT32 param);

#endif
