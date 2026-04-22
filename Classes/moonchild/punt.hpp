#ifndef _PUNT_H
#define _PUNT_H

#include <objects.hpp>

struct PUNT_BLOOD {
  UINT16     wigglecnt;
  UINT16     gravy;
  UINT16     x;
  UINT16     y;
  UINT16     lifespan;
};

void punt_init(UINT16 x, UINT16 y);
int  punt_live (OBJECT *object, UINT32 param);
void punt_death(OBJECT *object, UINT32 param);
void punt_clear(OBJECT *object, UINT32 param);

#endif
