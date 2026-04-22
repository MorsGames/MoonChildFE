#ifndef _MIXER_H
#define _MIXER_H

#include <objects.hpp>

struct MIXER_BLOOD {
  INT16     x;
  INT16     y;
  INT16     x1;
  INT16     x2;
  INT16     spdx;
  OBJECT   *mixer2;
  UINT16    delay;
  UINT16	framenr;		//####
  INT16		turncnt;	//####
  INT16		kamikaze;	// ####
};

void mixer_init(UINT16 x, UINT16 y, UINT16 x1, UINT16 x2, UINT16 xspd);
int  mixer_live (OBJECT *object, UINT32 param);
void mixer_death(OBJECT *object, UINT32 param);
void mixer_clear(OBJECT *object, UINT32 param);

struct MIXER2_BLOOD {
  INT16     x;
  INT16     y;
  INT16     x1;
  INT16     x2;
  INT16     spdx;
  
};

void mixer_init(UINT16 x, UINT16 y, UINT16 x1, UINT16 x2, UINT16 xspd);
int  mixer_live (OBJECT *object, UINT32 param);
void mixer_death(OBJECT *object, UINT32 param);
void mixer_clear(OBJECT *object, UINT32 param);
#endif
