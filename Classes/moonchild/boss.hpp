#ifndef _BOSS_H
#define _BOSS_H

#include <objects.hpp>

typedef struct BOSS_BLOOD BOSS_BLOOD;


struct BOSS_BLOOD {
  UINT16      srcx;
  UINT16      srcy;
  UINT16      destx;
  UINT16      desty;
  UINT16      movetimeout;     // if this hits 0 the factor goes in and the boss moves
  UINT16      movefactor;      // factor gives percentage of movement
  UINT16      movespeed;       // speed in which boss moves
  UINT16      minedelay;       // delayfactor for shooting
  UINT16      bossmode;        // boss mode
  UINT16      sinamp;
  UINT16      sinspd;
  UINT16      sincnt;
  UINT16      openup;          // how far open?
  UINT16      opendir;         // 1 = opening
  UINT16      opendelay;       // delay 'till dir switches
  UINT16      hit;             // nrofhits necesarry
  OBJECT     *topleft;
  OBJECT     *topright;
  OBJECT     *bottomleft;
  OBJECT     *bottomright;
  OBJECT     *heart;


  UINT16      plofamp;
  UINT16      plofsincnt1x;
  UINT16      plofsincnt2x;
  UINT16      plofsincnt1y;
  UINT16      plofsincnt2y;
  UINT16      madecnt;
  OBJECT     *plof[64];        // 64 diamonds voor plof effect!
};

  

OBJECT *boss_init (UINT16 x, UINT16 y);
int  boss_live (OBJECT *object, UINT32 param);
void boss_death(OBJECT *object, UINT32 param);
void boss_clear(OBJECT *object, UINT32 param);

#endif
