#ifndef _CLAW_H
#define _CLAW_H

#include <objects.hpp>

typedef struct CLAW_BLOOD CLAW_BLOOD;

struct CLAW_BLOOD {
  UINT16     x;
  UINT32     y;
  UINT16     xl;
  UINT16     xr;
  UINT16     spdx;
  UINT16     delay;
  UINT16     ground;
  INT16      height;
  INT16      gravy;
  UINT16     framenr;
  UINT16     topflg;
};


void claw_init(UINT16 x, UINT16 y, INT16 height, UINT16 xl, UINT16 xr, UINT16 topflg, INT16 spdx);
int  claw_live (OBJECT *object, UINT32 param);
void claw_death(OBJECT *object, UINT32 param);
void claw_clear(OBJECT *object, UINT32 param);

#endif
