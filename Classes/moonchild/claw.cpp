#include <frm_wrk.hpp>
#include <claw.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

UINT16 clawwiggletab[36] = 
{
  0,0,0,0,
  3,3,3,3,
  0,0,0,3,
  3,3,0,0,
  3,0,0,3,
  3,0,0,3,
  3,0,3,0,
  3,0,3,0,
  3,0,3,0
};

UINT16 clawanimtab[8] = 
{
  0,0,1,1,1,2,2,3
};

void claw_init(UINT16 x, UINT16 y, INT16 height, UINT16 xl, UINT16 xr, UINT16 topflg, INT16 spdx)
{
  OBJECT *claw;
  claw = (OBJECT *) malloc (sizeof(OBJECT));
  claw->x = x;
  claw->y = y;
  claw->live  = claw_live;
  claw->death = claw_death;
  claw->clear = claw_clear;
  claw->blitsizex = 0;
  claw->blitsizey = 0;
  claw->blitstartx = 0;
  claw->blitstarty = 0;

  claw->lethal = 0; //harmless

  claw->animate = copy_anim((ANIM *)orgclaw);
  claw->frame = anim_setsequence(claw->animate, topflg, SEQ_FORCE);


  claw->sizex = claw->frame->get_width();
  claw->sizey = claw->frame->get_height();

  claw->blitsizex = 0;
  claw->blitsizey = 0;
  claw->blitstartx = 0;
  claw->blitstarty = 0;
  claw->lethal = 3;
  claw->coloffsetx = 6;
  claw->coloffsety = 6;
  claw->colwidth = 16;
  claw->colheight= 16;


  claw->blood = (void *) malloc(sizeof(CLAW_BLOOD));
//  vgassert(claw->blood);
  CLAW_BLOOD *clawblood;

  clawblood = (CLAW_BLOOD *)claw->blood;
  clawblood->x       = x;
  clawblood->y       = y<<8;
  clawblood->xl      = xl;
  clawblood->xr      = xr;
  clawblood->spdx    = 3;
  clawblood->ground  = y;
  clawblood->height  = height<<8;
  clawblood->delay   = 25;
  clawblood->framenr = 0;
  clawblood->topflg = topflg;
  clawblood->spdx = spdx;
  object_add(claw);
}


int claw_live (OBJECT *object, UINT32 param)
{
  CLAW_BLOOD *clawblood;

  clawblood = (CLAW_BLOOD *)object->blood; /* in case of multiple claws */


  if (!clawblood->delay)
    {
      clawblood->delay = 10000; // dummy value
      clawblood->gravy = clawblood->height;
      clawblood->framenr = 0;
    }


  clawblood->x += clawblood->spdx;
  if (clawblood->x > clawblood->xr) clawblood->spdx = -clawblood->spdx;
  if (clawblood->x < clawblood->xl) clawblood->spdx = -clawblood->spdx;


  if (clawblood->topflg == 0)
  {
    if (clawblood->delay > 1000)     // claw is in action apparently
    {
      clawblood->y +=clawblood->gravy;
      clawblood->gravy+=70;
      if ((clawblood->y>>8) > clawblood->ground)
	  {
	    clawblood->y = clawblood->ground<<8;
	    clawblood->gravy = (-clawblood->gravy)+700;
	    if (clawblood->gravy > 0)
	    {
	      clawblood->gravy = 0;
	      clawblood->y = clawblood->ground<<8;
	      clawblood->delay = MC_RANDOM(100)+50;
		}
	  }
    }
  }
  else
  {
    if (clawblood->delay > 1000)     // claw is in action apparently
    {
      clawblood->y -=clawblood->gravy;
      clawblood->gravy+=70;
      if ((clawblood->y>>8) < clawblood->ground)
	  {
	    clawblood->y = clawblood->ground<<8;
	    clawblood->gravy = (-clawblood->gravy)+700;
	    if (clawblood->gravy > 0)
	    {
	      clawblood->gravy = 0;
	      clawblood->y = clawblood->ground<<8;
	      clawblood->delay = MC_RANDOM(100)+50;
	    }
	  }
    }
  }


  if (clawblood->delay > 1000)
    {
      if (clawblood->gravy > -400 && clawblood->framenr < 1) clawblood->framenr = 1;
      if (clawblood->gravy > -350 && clawblood->framenr < 2) clawblood->framenr = 2;
      if (clawblood->gravy > -300 && clawblood->framenr < 3) clawblood->framenr = 3;
      if (clawblood->gravy > -250 && clawblood->framenr < 4)
			{
				clawblood->framenr = 4;
				play_tangclos(object->x, object->y);
			}
			if (clawblood->gravy > -200 && clawblood->framenr < 5) clawblood->framenr = 5;
      if (clawblood->gravy > -150 && clawblood->framenr < 6) clawblood->framenr = 6;
      if (clawblood->gravy > -100 && clawblood->framenr < 7) clawblood->framenr = 7;
    }

  object->frame = anim_forceframe(object->animate, clawanimtab[clawblood->framenr]);

  object->sizex = object->frame->get_width();
  object->sizey = object->frame->get_height();

  object->x = clawblood->x - (object->sizex/2);
  object->y = (clawblood->y>>8) - object->sizey;

  if (clawblood->delay < 1000 && clawblood->framenr > 0) clawblood->framenr--;


  if (clawblood->delay < 35)  // do little wiggle
    {

			if (clawblood->delay == 1) play_tangjmp(object->x, object->y);
      object->y += clawwiggletab[35-clawblood->delay];
    }

  clawblood->delay--;

  return 0;
}

void claw_death(OBJECT *object, UINT32 param)
{
}

void claw_clear(OBJECT *object, UINT32 param)
{
}



