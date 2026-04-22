#ifndef _GRAVLIFT_H
#define _GRAVLIFT_H

#include <objects.hpp>

typedef struct GRAVLIFT_BLOOD GRAVLIFT_BLOOD;
typedef void (*GRAVLIFT_CB )(GRAVLIFT_BLOOD *gravlift);

struct GRAVLIFT_BLOOD {
  INT16      x;
  INT16      y;
  INT16      oldx;
  INT16      oldy;
  INT16      childcol;
};


void gravlift_init(UINT16 x, UINT16 y);
int  gravlift_live (OBJECT *object, UINT32 param);
void gravlift_death(OBJECT *object, UINT32 param);
void gravlift_clear(OBJECT *object, UINT32 param);

#endif
