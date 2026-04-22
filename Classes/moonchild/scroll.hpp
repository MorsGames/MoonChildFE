#ifndef _SCROLL_H
#define _SCROLL_H

#include <frm_wrk.hpp>
#include <editor.hpp>

struct VIEWPORT
{
  INT16      worldx;
  INT16      worldy;
  INT16      xoffsetcoarse;
  INT16      xoffsetfine;
  INT16      focusx;
  INT16      focusy;
  INT16      topx;
  INT16      topy;
  INT16      width;
  INT16      height;
  INT16      shakey;
  INT16      resbufnum;
  INT16      quakex;
  INT16      quakey;
  INT16      camx;        //override
  INT16      camy;        //override
  INT16      maxspd;      //override
  //  UINT32     score;       //real score
  //  UINT32     delayscore;  //score which lags behind
  UINT16     lives;
  UINT16     nrofblack;   //nr of black diamonds left
  UINT16     energy;      //begint met 33 loopt af!
  UINT16     newlife;     // start met 0... zodra 32 dan hebbie een nieuw leven erbij!
  MAP_DESCR *curmap;
  MAP_DESCR *loadedmap;
  BYTE      *orgsurface;
};

void camera_override(VIEWPORT *player, UINT16 camx, UINT16 camy, UINT16 maxspd);
void visual_updates(void);
void map_build(VIEWPORT *player);
void update(VIEWPORT *player);
void rebuild(VIEWPORT *player);
void blitpatcolumn(VIEWPORT *player, int x);
void scrolling(VIEWPORT *player);
void hoi_cam(VIEWPORT *player);
void gein_cam(void);
void restoredots(void);
void plotapix(UINT16 x, UINT16 y, UBYTE col);
void level4flash(VIEWPORT *player);

#endif
