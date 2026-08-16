#ifndef _PREFS_H
#define _PREFS_H

#include <frm_wrk.hpp>

/* Stuff which has to do with preferences */

void prefs_calcvals(void);

#define PREFS_LORES 0
#define PREFS_HIRES 1

/* Widescreen support - 16:9 aspect ratio */
#define MC_BASE_WIDTH 640
#define MC_BASE_HEIGHT 480
/* Widescreen adds 224 pixels horizontally (640 + 224 = 864, which is 16:9 with 480 height) */
#define MC_WIDESCREEN_PADDING 112

struct PREFS
{
  UINT16 screentopx;
  UINT16 screentopy;
  UINT16 screenwidth;
  UINT16 screenheight;
  UINT16 reso;
  UINT16 leftkey;
  UINT16 rightkey;
  UINT16 upkey;
  UINT16 downkey;
  UINT16 jumpkey;
  UINT16 shootkey;
  
  /* Widescreen support - these are computed from base + padding */
  UINT16 basewidth;
  UINT16 baseheight;
  UINT16 widescreenpadding;
};



#endif
