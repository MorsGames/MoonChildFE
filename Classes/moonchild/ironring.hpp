#ifndef _IRONRING_H
#define _IRONRING_H

#include <objects.hpp>
#include <elevat.hpp>

typedef struct IRONRING_BLOOD IRONRING_BLOOD;
typedef void (*IRONRING_CB )(OBJECT *ironring);

struct IRONRING_BLOOD {
  INT16      x;
  INT16      y;
  INT16      oldx;
  INT16      oldy;
  INT16      childcol;
  IRONRING_CB  ironringrout;
  INT16      mx;
  INT16      my;
  INT16      r1;
  INT16      r2;
  INT16      r3;
  INT16      r4;
};


void ironring_init(UINT16 x, UINT16 y, IRONRING_CB ironringrout,
     INT16 r1, INT16 r2, INT16 r3, INT16 r4);
int  ironring_live (OBJECT *object, UINT32 param);
void ironring_death(OBJECT *object, UINT32 param);
void ironring_clear(OBJECT *object, UINT32 param);

void ironring_sinx (OBJECT *ironring);
void ironring_siny (OBJECT *ironring);
void ironring_sinxy(OBJECT *ironring);
void ironring_flipflapx(OBJECT *ironring);
void ironring_flipflapy(OBJECT *ironring);

#endif
