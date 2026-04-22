#ifndef _CLOUD_H
#define _CLOUD_H

#include <objects.hpp>

struct CLOUD_BLOOD {
  INT16     lifespan;
  INT16     spdx;
  INT16     ytoggle;
};

void cloud_init(UINT16 x, UINT16 y, INT16 spdx, UINT16 sndflg);
int  cloud_live (OBJECT *object, UINT32 param);
void cloud_death(OBJECT *object, UINT32 param);
void cloud_clear(OBJECT *object, UINT32 param);

#endif
