#ifndef _PTOEI_H
#define _PTOEI_H

#include <objects.hpp>

typedef struct PTOEI_BLOOD PTOEI_BLOOD;
typedef void (*PTOEI_CB )(PTOEI_BLOOD *ptoei);

struct PTOEI_BLOOD {
  INT16      delay;
};


void ptoei_init (UINT16 x, UINT16 y);
int  ptoei_live (OBJECT *object, UINT32 param);
void ptoei_death(OBJECT *object, UINT32 param);
void ptoei_clear(OBJECT *object, UINT32 param);

#endif
