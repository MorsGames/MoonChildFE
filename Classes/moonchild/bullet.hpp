#ifndef _BULLET_H
#define _BULLET_H

#include <objects.hpp>

struct BULLET_BLOOD {
  INT32     xfloat;
  INT32     yfloat;
  INT16     xspd;
  INT16     yspd;
  UINT16    lifespan;
  UINT16    remove;
};

void bullet_init(UINT16 x, UINT16 y, INT16 xspd_, INT16 yspd_);
int  bullet_live (OBJECT *object, UINT32 param);
void bullet_death(OBJECT *object, UINT32 param);
void bullet_clear(OBJECT *object, UINT32 param);

#endif
