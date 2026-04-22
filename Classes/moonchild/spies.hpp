#ifndef _SPIES_H
#define _SPIES_H

#include <objects.hpp>

typedef struct SPIES_BLOOD SPIES_BLOOD;
typedef void (*SPIES_CB )(SPIES_BLOOD *spies);

struct SPIES_BLOOD {
  INT16      sincnt;
  INT16      sinspd;
  INT16      rightflg;
  INT16      x;
  UINT16     soundflg;
  UINT16     sounddelay;
};


void spies_init(UINT16 x, UINT16 y, UINT16 rghtflg, UINT16 strtcnt,
		INT16 animspd, UINT16 soundflg);
int  spies_live (OBJECT *object, UINT32 param);
void spies_death(OBJECT *object, UINT32 param);
void spies_clear(OBJECT *object, UINT32 param);

#endif
