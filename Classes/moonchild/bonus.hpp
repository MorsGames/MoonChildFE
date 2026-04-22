#ifndef _BONUS_H
#define _BONUS_H

#include <objects.hpp>

struct BONUS_BLOOD {
  INT16     lifespan;
};

void bonus_init(UINT16 x, UINT16 y, UINT16 sequencenr, UINT16 getpoints);
int  bonus_live (OBJECT *object, UINT32 param);
void bonus_death(OBJECT *object, UINT32 param);
void bonus_clear(OBJECT *object, UINT32 param);

#endif
