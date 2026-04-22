#ifndef _ROCK_H
#define _ROCK_H

#include <objects.hpp>

typedef struct ROCK_BLOOD ROCK_BLOOD;

struct ROCK_BLOOD {
  UINT16     minx;
  UINT16     maxx;
  INT16      spdx;
};


OBJECT *rock_init(UINT16 x, UINT16 y, UINT16 minx, UINT16 maxx);
int  rock_live (OBJECT *object, UINT32 param);
void rock_death(OBJECT *object, UINT32 param);
void rock_clear(OBJECT *object, UINT32 param);

#endif
