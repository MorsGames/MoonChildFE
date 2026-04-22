#ifndef _PLANT_H
#define _PLANT_H

#include <objects.hpp>

struct PLANT_BLOOD {
  UINT16     swingswingcnt;
  UINT16     swingswingspd;
  UINT16     swingswingamp;
  UINT16     x;
  UINT16     y;
  UINT16     oldx;
  UINT16     oldy;

  UINT16     ampchangecnt;
  UINT16     ampchangedelay;
  INT16      ampchange;

  INT16      startamp;
  INT16      extraamp;
  INT16      shootdelay;
  INT16      fallspd;

  UINT16     headanimcnt;

  UINT16     swingcnt[128];
  OBJECT    *elements[16];

  OBJECT    *rock;
};

void plant_init(UINT16 x, UINT16 y, UINT16 swingspd, UINT16 swingamp, OBJECT *rock);
int  plant_live (OBJECT *object, UINT32 param);
void plant_death(OBJECT *object, UINT32 param);
void plant_clear(OBJECT *object, UINT32 param);

#endif
