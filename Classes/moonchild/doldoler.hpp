#ifndef _DOOLDOLER_H
#define _DOOLDOLER_H

#include <objects.hpp>

typedef struct DOOLDOLER_BLOOD DOOLDOLER_BLOOD;

struct DOOLDOLER_BLOOD {
  UINT16 curdir;
  UINT16 shootdelay;
};

void dooldoler_init (UINT16 x, UINT16 y, UINT16 startdir);
int  dooldoler_live (OBJECT *object, UINT32 param);
void dooldoler_death(OBJECT *object, UINT32 param);
void dooldoler_clear(OBJECT *object, UINT32 param);

#endif
