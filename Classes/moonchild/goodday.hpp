#ifndef _GOODDAY_H
#define _GOODDAY_H

#include <objects.hpp>

typedef struct GOODDAY_BLOOD GOODDAY_BLOOD;

struct GOODDAY_BLOOD {
  INT16      dummy;
};


OBJECT *goodday_init (UINT16 x, UINT16 y);
int  goodday_live (OBJECT *object, UINT32 param);
void goodday_death(OBJECT *object, UINT32 param);
void goodday_clear(OBJECT *object, UINT32 param);

#endif
