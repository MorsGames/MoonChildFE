#ifndef _METBAL_H
#define _METBAL_H

#include <objects.hpp>

typedef struct METBAL_BLOOD METBAL_BLOOD;

struct METBAL_BLOOD {
  INT32      x;
  INT16      spdx;
  UINT16     lifespan;
};


void metbal_init (UINT16 x, UINT16 y, INT16 spdx);
int  metbal_live (OBJECT *object, UINT32 param);
void metbal_death(OBJECT *object, UINT32 param);
void metbal_clear(OBJECT *object, UINT32 param);

#endif
