#ifndef _MINE_H
#define _MINE_H

#include <objects.hpp>

struct MINE_BLOOD {
  INT32     xfloat;
  INT32     yfloat;
  INT16     xspd;
  INT16     yspd;
  UINT16    lifespan;
  UINT16    exploflg;
  UINT16    animspd;
  UINT16    clipflg;
};

void mine_init(UINT16 x, UINT16 y, INT16 xspd_, INT16 yspd_, ANIM *anim, UINT16 seqnr, UINT16 lifespan, UINT16 exploflg, UINT16 animspd, UINT16 clipflg);
int  mine_live (OBJECT *object, UINT32 param);
void mine_death(OBJECT *object, UINT32 param);
void mine_clear(OBJECT *object, UINT32 param);

#endif
