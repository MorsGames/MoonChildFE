#ifndef _BBOT_H
#define _BBOT_H

#include <objects.hpp>

struct BBOT_BLOOD {
  UINT16      x;
  UINT16      y;
  UINT16      sinxcnt;
  UINT16      sinxadd;
  UINT16      sinxamp;
  UINT16      shootdelay;
  UINT16      shootflg;
  UINT16      lastx;
  UINT16      lasty;
  UINT16      schuif;
  UINT16      leftx;
  UINT16      rightx;
  UINT16      spdx;
  OBJECT     *elements[32];
};

void bbot_init(UINT16 x, UINT16 y, UINT16 sinxcnt, UINT16 sinxadd, UINT16 sinxamp, UINT16 shootflg, UINT16 leftx, UINT16 rightx, UINT16 spdx);
int  bbot_live (OBJECT *object, UINT32 param);
void bbot_death(OBJECT *object, UINT32 param);
void bbot_clear(OBJECT *object, UINT32 param);

#endif
