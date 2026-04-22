#include <frm_wrk.hpp>
#include <warp.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void warp_init(UINT16 x, UINT16 y, UINT16 destx, UINT16 desty, UINT16 restartflg)
{
  OBJECT *warp;
  warp = (OBJECT *) malloc (sizeof(OBJECT));
  warp->x = x;
  warp->y = y;
  warp->live  = warp_live;
  warp->death = warp_death;
  warp->clear = warp_clear;
  warp->blitsizex = 0;
  warp->blitsizey = 0;
  warp->blitstartx = 0;
  warp->blitstarty = 0;

  warp->lethal = 0; //harmless

  warp->animate = copy_anim((ANIM *)orgwarp);
  warp->frame = anim_setsequence(warp->animate, 0, SEQ_FORCE);


  warp->sizex = warp->frame->get_width();
  warp->sizey = warp->frame->get_height();

  warp->blitsizex = warp->sizex;
  warp->blitsizey = warp->sizey;
  warp->blitstartx = 0;
  warp->blitstarty = 0;
  warp->lethal = 0;
  warp->coloffsetx = 6;
  warp->coloffsety = 6;
  warp->colwidth = warp->sizex - 12;
  warp->colheight= warp->sizey - 12;


  warp->blood = (void *) malloc(sizeof(WARP_BLOOD));
//  vgassert(warp->blood);
  WARP_BLOOD *warpblood;

  warpblood = (WARP_BLOOD *)warp->blood;
  warpblood->destx      = destx;
  warpblood->desty      = desty;
  warpblood->restartflg = restartflg;
  object_add(warp);
}


int warp_live (OBJECT *object, UINT32 param)
{
  WARP_BLOOD *warpblood;

  warpblood = (WARP_BLOOD *)object->blood; /* in case of multiple warps */

/* code for collision with hoi */
  
  object->frame = anim_nextframe(object->animate);

  if ((UINT16)(hoi->x - object->x + 32) < 48)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + 64) <64)   /* y collision */
      {
        HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

        if (hoi->frame != 0 && editflg != 1)   // zijn we al aan het warpen?
        {
         
          play_warp();



#if 0
        hoi->x = warpblood->destx;
        hoi->y = warpblood->desty;
	hoiblood->xfloat = warpblood->destx<<8;
	hoiblood->yfloat = warpblood->desty<<8;
        hoiblood->spdx = 0;
        hoiblood->gravy = 0;

	hoiblood->xspd = 0;
	hoiblood->yspd = 0;
#endif
	if (warpblood->restartflg)
	  {
	    restartx = warpblood->destx;
	    restarty = warpblood->desty;
        hoiblood->flashcnt = 200;
	  }

        hoirise_init(hoi, hoi->x, hoi->y, warpblood->destx, warpblood->desty);
      }
    }
  }

  return 0;
}

void warp_death(OBJECT *object, UINT32 param)
{
}

void warp_clear(OBJECT *object, UINT32 param)
{
}



