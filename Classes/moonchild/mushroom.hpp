#ifndef _MUSHROOM_H
#define _MUSHROOM_H

#include <objects.hpp>

struct MUSHROOM_BLOOD {
  UINT16     swingswingcnt;
  UINT16     swingswingspd;
  UINT16     swingswingamp;
  UINT16     x;
  UINT16     y;
  UINT16     childcol;
  UINT16     oldchildcol;
  UINT16     oldx;
  UINT16     oldy;
  UINT16     startheight;
  UINT16     endheight;
  UINT16     curheight;
    
  UINT16     swingcnt[64];
  OBJECT    *elements[16];
};

void mushroom_init (UINT16 x, UINT16 y, UINT16 swingspd, UINT16 swingamp, UINT16 startheight, UINT16 endheight);
int  mushroom_live (OBJECT *object, UINT32 param);
void mushroom_death(OBJECT *object, UINT32 param);
void mushroom_clear(OBJECT *object, UINT32 param);

#endif
