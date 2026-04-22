#ifndef _BOLT_H
#define _BOLT_H

#include <objects.hpp>

typedef struct BOLT_BLOOD BOLT_BLOOD;

struct BOLT_BLOOD {
  INT16      x;
  INT16      y;
  UINT16     xyflg;
  UINT16     sincnt;
  INT16     sinspd;
  UINT16     sinamp;
  UINT16     curdelay;
  UINT16     totdelay;
  UINT16     shootdelay;
  OBJECT    *cathode;
  OBJECT    *anode;
};


void bolt_init(UINT16 x, UINT16 y, UINT16 xyflg, INT16 sincnt, INT16 sinspd, UINT16 sinamp, UINT16 curdelay, UINT16 totdelay, UINT16 shootdelay);
int  bolt_live (OBJECT *object, UINT32 param);
void bolt_death(OBJECT *object, UINT32 param);
void bolt_clear(OBJECT *object, UINT32 param);

#endif
