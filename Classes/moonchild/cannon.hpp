#ifndef _CANNON_H
#define _CANNON_H

#include <objects.hpp>
#include <basic.hpp>

struct CANNON_BLOOD {
  UINT16    shootspeed;
  UINT16    shootrate;
  UINT16    shootdelay;
  UINT16    bulkflg;
  ANIM     *mineanim;
  UINT16    seqnr;
  UINT16    lifespan;
  UINT16    exploflg;
  UINT16    moveinflg;
  UINT16    moveincnt;
  UINT16    cloudflg;
  UINT16    sinamp;
  UINT16    sinspd;
  UINT16    sincnt;
  UINT16    hitcnt;
  UINT16    hitdelay;
  OBJECT_CB hitrout;

  // stuff for basic shit

  UINT16      x;
  UINT16      y;
  BASIC_CB    moveroutx;
  BASIC_CB    moverouty;
  BASIC_CB    hoverrouty;
  INT16       x1;
  INT16       x2;
  INT16       x3;
  INT16       y1;
  INT16       y2;
  INT16       y3;
  INT16       hy1;
  INT16       hy2;
  INT16       hy3;
  WAV2_FN     wav2_fn;
  UINT16      sneakflg;
};

void cannon_init(UINT16 x, UINT16 y, UINT16 shootrate, UINT16 shootspeed, UINT16 bulkflg, ANIM *mineanim,
		 UINT16 seqnr, UINT16 lifespan, UINT16 exploflg, UINT16 moveinflg, UINT16 cloudflg,
		 BASIC_CB basicroutx, INT16 x1,  INT16 x2,  INT16 x3,
		 BASIC_CB basicrouty, INT16 y1,  INT16 y2,  INT16 y3,
		 BASIC_CB hoverrouty, INT16 hy1, INT16 hy2, INT16 hy3,
		 UINT16 sinamp, UINT16 sinspd, UINT16 hitcnt, OBJECT_CB hitrout, WAV2_FN wav2_fn, UINT16 sneakflg
		 );
int  cannon_live (OBJECT *object, UINT32 param);
void cannon_death(OBJECT *object, UINT32 param);
void cannon_clear(OBJECT *object, UINT32 param);

#endif
