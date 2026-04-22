#ifndef _IRONAUTO_H
#define _IRONAUTO_H

#include <objects.hpp>
#include <elevat.hpp>

typedef struct IRONAUTO_BLOOD IRONAUTO_BLOOD;

struct IRONAUTO_BLOOD {
  UINT16      x;
  UINT16      y;
  INT16      oldx;
  INT16      oldy;
  INT16      childcol;
  UINT16    *movtab;
  UINT16     movpos;
  UINT16     movdel;
  UINT16     movflg;
  UINT16     onliftdel;
  UINT16     hyprsndflg;
  INT16      dirx;
  INT16      diry;
  OBJECT    *koppellift;
};


OBJECT *ironauto_init(UINT16 x, UINT16 y, UINT16 *movtab, UINT16 movflg, OBJECT *koppellift, UINT16 hyprsndflg);
int  ironauto_live (OBJECT *object, UINT32 param);
void ironauto_death(OBJECT *object, UINT32 param);
void ironauto_clear(OBJECT *object, UINT32 param);

void ironauto_sinx (OBJECT *ironauto);
void ironauto_siny (OBJECT *ironauto);
void ironauto_sinxy(OBJECT *ironauto);
void ironauto_flipflapx(OBJECT *ironauto);
void ironauto_flipflapy(OBJECT *ironauto);

#endif
