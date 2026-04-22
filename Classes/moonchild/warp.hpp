#ifndef _WARP_H
#define _WARP_H

#include <objects.hpp>

typedef struct WARP_BLOOD WARP_BLOOD;

struct WARP_BLOOD {
  INT16      destx;
  INT16      desty;
  UINT16     restartflg;
};


void warp_init (UINT16 x, UINT16 y, UINT16 destx, UINT16 desty, UINT16 restartflg);
int  warp_live (OBJECT *object, UINT32 param);
void warp_death(OBJECT *object, UINT32 param);
void warp_clear(OBJECT *object, UINT32 param);

#endif
