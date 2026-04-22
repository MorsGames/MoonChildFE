#ifndef _MOUTH_H
#define _MOUTH_H

#include <objects.hpp>

typedef struct MOUTH_BLOOD MOUTH_BLOOD;

struct MOUTH_BLOOD {
  UINT16     x;
  UINT32     y;
  UINT16     delay;
  UINT16     ground;
  INT16      height;
  INT16      gravy;
  UINT16     framenr;
};


void mouth_init (UINT16 x, UINT16 y, INT16 height);
int  mouth_live (OBJECT *object, UINT32 param);
void mouth_death(OBJECT *object, UINT32 param);
void mouth_clear(OBJECT *object, UINT32 param);

#endif
