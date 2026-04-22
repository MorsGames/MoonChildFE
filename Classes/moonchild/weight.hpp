#ifndef _WEIGHT_H
#define _WEIGHT_H

#include <objects.hpp>

typedef struct WEIGHT_BLOOD   WEIGHT_BLOOD;

struct WEIGHT_BLOOD {
  UINT16    stuit;
  UINT16    speed;
};

void weight_init(UINT16 x, UINT16 y, UINT16 startpos, UINT16 speed);
int  weight_live (OBJECT *object, UINT32 param);
void weight_death(OBJECT *object, UINT32 param);
void weight_clear(OBJECT *object, UINT32 param);

#endif
