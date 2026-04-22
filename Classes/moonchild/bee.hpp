#ifndef _BEE_H
#define _BEE_H

#include <objects.hpp>

struct BEE_BLOOD {
  INT16     x;
  INT16     y;
  INT16     x1;
  INT16     x2;
  INT16     spdx;
  UINT16    framenr;
  UINT16    delay;
  UINT16    turncnt;
  UINT16    kamikaze;
};

void bee_init(UINT16 x, UINT16 y, UINT16 x1, UINT16 x2, UINT16 xspd);
int  bee_live (OBJECT *object, UINT32 param);
void bee_death(OBJECT *object, UINT32 param);
void bee_clear(OBJECT *object, UINT32 param);

#endif
