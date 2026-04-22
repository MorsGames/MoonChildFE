#ifndef _SEAMINE_H
#define _SEAMINE_H

#include <objects.hpp>

typedef struct SEAMINE_BLOOD SEAMINE_BLOOD;

struct SEAMINE_BLOOD {
  INT16      dummy;
};


OBJECT *seamine_init (UINT16 x, UINT16 y);
int  seamine_live (OBJECT *object, UINT32 param);
void seamine_death(OBJECT *object, UINT32 param);
void seamine_clear(OBJECT *object, UINT32 param);

#endif
