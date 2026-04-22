#ifndef _SMLHEART_H
#define _SMLHEART_H

#include <objects.hpp>

typedef struct SMLHEART_BLOOD SMLHEART_BLOOD;

struct SMLHEART_BLOOD {
  INT16      srcx;
  INT16      srcy;
  UINT16     sincnt;
  UINT16     appearance;
  UINT16     scoreholdpos;
};


void smlheart_init (UINT16 srcx, UINT16 srcy, UINT16 appearance, UINT16 scoreholdpos);
int  smlheart_live (OBJECT *object, UINT32 param);
void smlheart_death(OBJECT *object, UINT32 param);
void smlheart_clear(OBJECT *object, UINT32 param);

#endif
