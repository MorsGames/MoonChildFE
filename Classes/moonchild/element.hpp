#ifndef _ELEMENT_H
#define _ELEMENT_H

#include <objects.hpp>

typedef struct ELEMENT_BLOOD ELEMENT_BLOOD;
typedef UINT16 (*ELEMENT_CB )(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);


struct ELEMENT_BLOOD {
  UINT16      x;
  UINT16      y;
  INT16       spdx;
  INT16       spdy;
  INT16       timeout;
  ELEMENT_CB  moveroutx;
  ELEMENT_CB  moverouty;
  ELEMENT_CB  hoverrouty;
  INT16       x1;
  INT16       x2;
  INT16       x3;
  INT16       y1;
  INT16       y2;
  INT16       y3;
  INT16       hy1;
  INT16       hy2;
  INT16       hy3;
  UINT16      anmspd;
  UINT16      curmov;
  UINT16      movdelay;
  UINT16      forcedseq;
  UINT16      bulletflg;
  UINT16      destroyable;
	WAV_FN      wav_fn;
	WAV2_FN     wav2_fn;        // proximity wav rout
	UINT16      rnddelay;
	UINT16      rnddomain;
	UINT16      curdelay;
};


void element_init(UINT16 x, UINT16 y, INT16 spdx, INT16 spdy,
                ELEMENT_CB elementroutx, INT16 x1,  INT16 x2,  INT16 x3,
                ELEMENT_CB elementrouty, INT16 y1,  INT16 y2,  INT16 y3,
                ELEMENT_CB hoverrouty, INT16 hy1, INT16 hy2, INT16 hy3,
								ANIM *anim, UINT16 anmspd, UINT16 movdelay, UINT16 forcedseq, UINT16 bulletflg, UINT16 destroyable,
								WAV_FN wavrout, UINT16 rnddelay, UINT16 rnddomain, WAV2_FN proximity);
int  element_live (OBJECT *object, UINT32 param);
void element_death(OBJECT *object, UINT32 param);
void element_clear(OBJECT *object, UINT32 param);

UINT16 element_sin(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 element_pingpong(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 element_flipflap(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 element_gravy(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 element_flyby(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);

#endif
