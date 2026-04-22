#ifndef _LIFT_H
#define _LIFT_H

#include <objects.hpp>

typedef struct LIFT_BLOOD LIFT_BLOOD;
typedef void (*LIFT_CB )(LIFT_BLOOD *lift);

struct LIFT_BLOOD {
  INT16      childcol;
  INT16      delay;
  INT16      ystop;
};


void lift_init(UINT16 x, UINT16 y, UINT16 ystop);
int  lift_live (OBJECT *object, UINT32 param);
void lift_death(OBJECT *object, UINT32 param);
void lift_clear(OBJECT *object, UINT32 param);

#endif
