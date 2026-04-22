#ifndef _GLIM_H
#define _GLIM_H

#include <objects.hpp>

struct GLIM_BLOOD {
  INT16     lifespan;
};

void glim_init(UINT16 x, UINT16 y);
int  glim_live (OBJECT *object, UINT32 param);
void glim_death(OBJECT *object, UINT32 param);
void glim_clear(OBJECT *object, UINT32 param);

#endif
