#ifndef _ELEVAT_H
#define _ELEVAT_H

#include <objects.hpp>

typedef struct ELEVAT_BLOOD ELEVAT_BLOOD;
typedef void (*ELEVAT_CB )(ELEVAT_BLOOD *elevat);

struct ELEVAT_BLOOD {
  INT16      weightto;
  INT16      weightextra;
  INT16      weightamp;
  INT16      weightcnt;
  INT16      x;
  INT16      y;
  INT16      oldx;
  INT16      oldy;
  INT16      childcol;
  ELEVAT_CB  elevatrout;
  INT16      mx;
  INT16      my;
  INT16      r1;
  INT16      r2;
  INT16      r3;
};


void elevat_init(UINT16 x, UINT16 y, ELEVAT_CB elevatrout,
     INT16 r1, INT16 r2, INT16 r3);
int  elevat_live (OBJECT *object, UINT32 param);
void elevat_death(OBJECT *object, UINT32 param);
void elevat_clear(OBJECT *object, UINT32 param);

void elevat_sinx(ELEVAT_BLOOD *elevatblood);
void elevat_siny(ELEVAT_BLOOD *elevatblood);
void elevat_sinxy(ELEVAT_BLOOD *elevatblood);
#endif
