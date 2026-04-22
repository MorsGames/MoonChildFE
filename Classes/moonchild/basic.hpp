#ifndef _BASIC_H
#define _BASIC_H

#include <objects.hpp>

typedef struct BASIC_BLOOD BASIC_BLOOD;
typedef UINT16 (*BASIC_CB )(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);


struct BASIC_BLOOD {
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
  UINT16      anmspd;
  UINT16      curmov;
  UINT16      movdelay;
  UINT16      forcedseq;
  UINT16      bulletflg;
  UINT16      destroyable;
	WAV2_FN      wav_fn;
	WAV2_FN     wav2_fn;        // proximity wav rout
	UINT16      rnddelay;
	UINT16      rnddomain;
	UINT16      curdelay;
};


void basic_init(UINT16 x, UINT16 y,
                BASIC_CB basicroutx, INT16 x1,  INT16 x2,  INT16 x3,
                BASIC_CB basicrouty, INT16 y1,  INT16 y2,  INT16 y3,
                BASIC_CB hoverrouty, INT16 hy1, INT16 hy2, INT16 hy3,
								ANIM *anim, UINT16 anmspd, UINT16 movdelay, UINT16 forcedseq, UINT16 bulletflg, UINT16 destroyable,
								WAV2_FN wavrout, UINT16 rnddelay, UINT16 rnddomain, WAV2_FN proximity);
int  basic_live (OBJECT *object, UINT32 param);
void basic_death(OBJECT *object, UINT32 param);
void basic_clear(OBJECT *object, UINT32 param);

UINT16 basic_sin(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 basic_pingpong(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 basic_flipflap(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 basic_gravy(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);
UINT16 basic_flyby(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3);

#endif
