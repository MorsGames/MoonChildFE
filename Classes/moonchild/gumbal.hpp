#ifndef _GUMBAL_H
#define _GUMBAL_H

#include <objects.hpp>

struct GUMBAL_BLOOD {
  INT16     framenr;
  INT16     framedelay;
  INT16     y;
};

void gumbal_init(UINT16 x, UINT16 y, UINT16 framenr);
int  gumbal_live (OBJECT *object, UINT32 param);
void gumbal_death(OBJECT *object, UINT32 param);
void gumbal_clear(OBJECT *object, UINT32 param);

#endif
