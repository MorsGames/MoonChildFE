#ifndef _CHAIN_H
#define _CHAIN_H

#include <objects.hpp>

struct CHAIN_BLOOD {
  UINT16     swingswingcnt;
  UINT16     swingswingspd;
  UINT16     swingswingamp;
  UINT16     x;
  UINT16     y;
  UINT16     childcol;
  UINT16     oldx;
  UINT16     oldy;
  UINT16     hookflg;
    
  UINT16     swingcnt[64];
  OBJECT    *elements[16];
};

void chain_init(UINT16 x, UINT16 y, UINT16 hookflg, UINT16 swingspd, UINT16 swingamp);
int  chain_live (OBJECT *object, UINT32 param);
void chain_death(OBJECT *object, UINT32 param);
void chain_clear(OBJECT *object, UINT32 param);

#endif
