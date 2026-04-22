#ifndef _BUMP_H
#define _BUMP_H

#include <objects.hpp>

typedef struct BUMP_BLOOD BUMP_BLOOD;
typedef void (*BUMP_CB )(BUMP_BLOOD *bump);

struct BUMP_BLOOD {
  INT16      delay;
  INT16      dirflg;
};


void bump_init (UINT16 x, UINT16 y, UINT16 dirflg);
int  bump_live (OBJECT *object, UINT32 param);
void bump_death(OBJECT *object, UINT32 param);
void bump_clear(OBJECT *object, UINT32 param);

#endif
