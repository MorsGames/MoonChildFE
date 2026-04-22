#ifndef _PAAL_H
#define _PAAL_H

#include <objects.hpp>

typedef struct PAAL_BLOOD PAAL_BLOOD;

struct PAAL_BLOOD {
  UINT16     y;
  INT16      sinspd;
  INT16      sincnt;
  INT16      sinamp;
};


OBJECT *paal_init(UINT16 x, UINT16 y, UINT16 seqnr, UINT16 sincnt, UINT16 sinamp, UINT16 sinspd);
int  paal_live (OBJECT *object, UINT32 param);
void paal_death(OBJECT *object, UINT32 param);
void paal_clear(OBJECT *object, UINT32 param);

#endif
