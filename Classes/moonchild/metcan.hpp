#ifndef _METCAN_H
#define _METCAN_H

#include <objects.hpp>

typedef struct METCAN_BLOOD METCAN_BLOOD;

struct METCAN_BLOOD {
  INT16      dirx;
  INT16      delay;
  INT16      initdelay;
};


void metcan_init (UINT16 x, UINT16 y, INT16 dirx, UINT16 delay, UINT16 startdelay);
int  metcan_live (OBJECT *object, UINT32 param);
void metcan_death(OBJECT *object, UINT32 param);
void metcan_clear(OBJECT *object, UINT32 param);

#endif
