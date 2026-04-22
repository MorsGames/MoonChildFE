#ifndef _DIAMOND_H
#define _DIAMOND_H

#include <objects.hpp>

typedef struct DIAMOND_BLOOD DIAMOND_BLOOD;

struct DIAMOND_BLOOD {
  INT16      sincnt;
  INT16      y;
  UINT16     glim;
};


void diamond_init (UINT16 x, UINT16 y);
int  diamond_live (OBJECT *object, UINT32 param);
void diamond_death(OBJECT *object, UINT32 param);
void diamond_clear(OBJECT *object, UINT32 param);

#endif
