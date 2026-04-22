#ifndef _STUKHOUT_H
#define _STUKHOUT_H

#include <objects.hpp>

typedef struct STUKHOUT_BLOOD STUKHOUT_BLOOD;

struct STUKHOUT_BLOOD {
  INT16      ystop;
  INT16      childcol;
  UINT16     oldx;
  UINT16     oldy;
};


void stukhout_init(UINT16 x, UINT16 y, UINT16 ystop);
int  stukhout_live (OBJECT *object, UINT32 param);
void stukhout_death(OBJECT *object, UINT32 param);
void stukhout_clear(OBJECT *object, UINT32 param);

#endif
