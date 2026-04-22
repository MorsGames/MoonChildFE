#ifndef _PREFS_H
#define _PREFS_H

#include <frm_wrk.hpp>

/* Stuff which has to do with preferences */

void prefs_calcvals(void);

#define PREFS_LORES 0
#define PREFS_HIRES 1

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
};



#endif
