#include <frm_wrk.hpp>
#include <robyn.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <plof.hpp>
#include <cloud.hpp>
#include <sound.hpp>


void robyn_init(UINT16 x, UINT16 y, INT16 spdx, INT16 spdy)
{
  OBJECT *robyn;
  robyn = (OBJECT *) malloc (sizeof(OBJECT));
  robyn->x = x;
  robyn->y = y;
  robyn->live  = robyn_live;
  robyn->death = robyn_death;
  robyn->clear = robyn_clear;
  robyn->blitsizex = 0;
  robyn->blitsizey = 0;

  robyn->lethal = 0; //harmless untill they explode!

  robyn->animate = copy_anim((ANIM *)orgrobyn);
  robyn->frame   = anim_setsequence(robyn->animate, 0, SEQ_FORCE);

  robyn->sizex = robyn->frame->get_width();
  robyn->sizey = robyn->frame->get_height();

  robyn->blood = (void *) malloc(sizeof(ROBYN_BLOOD));
//  vgassert(robyn->blood);
  ROBYN_BLOOD *robynblood;
  robynblood = (ROBYN_BLOOD *)robyn->blood;
  robynblood->realx = x << 4;
  robynblood->gravy = spdy;
  robynblood->spdx  = spdx;
  robynblood->lifespan = 0;
  robynblood->bouncecnt = 0;

  object_add(robyn);
}


int robyn_live (OBJECT *object, UINT32 param)
{
  UINT32 rememx,rememy;

  ROBYN_BLOOD *robynblood;
  robynblood = (ROBYN_BLOOD *)object->blood; /* in case of multiple robyns */

/* handle gravity */
  robynblood->gravy += 5; /* let gravity prevail! */

  rememy = object->y;
  object->y += (robynblood->gravy >> 3);
  if (robynblood->gravy > 0)
    {
      if (collision(object->x+(object->sizex/2), object->y + (object->sizey) - 8 ))
  {
    object->y = rememy;

    if (robynblood->bouncecnt < 6)
    {
      robynblood->bouncecnt++;
      object->frame = anim_forceframe(object->animate, robynblood->bouncecnt);

      if (robynblood->bouncecnt < 6)
	{
	  play_blub(object->x, object->y);
	}
      if (robynblood->bouncecnt == 6) robynblood->lifespan = 1;
    }
    
    robynblood->gravy = -(robynblood->gravy - 30);

    if (robynblood->spdx > 0)
      {
        robynblood->spdx -= 8;
        if (robynblood->spdx < 0) robynblood->spdx = 0;
      }
    else
      {
        robynblood->spdx += 8;
        if (robynblood->spdx > 0) robynblood->spdx = 0;
      }
  }
    }

  if (robynblood->gravy > 126)
    {
      robynblood->gravy = 126;
    }


/* handle x-movement */

  rememx = robynblood->realx;
  robynblood->realx += robynblood->spdx;

  if (collision((robynblood->realx>>4) + (object->sizex/2), object->y + (object->sizey/2)))
    {
      robynblood->realx = rememx;
      robynblood->spdx = -robynblood->spdx;
    }

  object->x = robynblood->realx>>4;

  player2.focusx = object->x - 320;
  player2.focusy = object->y - 120;


  if (object->y+CLIPMARGINY-64 > player1.curmap->mapsizey) return 1;

  if (robynblood->lifespan)
    {
      robynblood->lifespan--;
//      object->frame = anim_nextframe(object->animate);
//      object->frame = anim_nextframe(object->animate);
      
      if (!robynblood->lifespan)
  {
    plof_init(object->x+6, object->y+4, 1, 1, 0, 0, 0);
    return 1;
  }
    }

  return 0;
}

void robyn_death(OBJECT *object, UINT32 param)
{
}

void robyn_clear(OBJECT *object, UINT32 param)
{
}


