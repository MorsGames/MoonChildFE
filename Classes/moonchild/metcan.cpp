#include <frm_wrk.hpp>
#include <metcan.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>
#include <metbal.hpp>


void metcan_init(UINT16 x, UINT16 y, INT16 dirx, UINT16 delay, UINT16 startdelay)
{
  OBJECT *metcan;
  metcan = (OBJECT *) malloc (sizeof(OBJECT));
  metcan->x = x;
  metcan->y = y;
  metcan->live  = metcan_live;
  metcan->death = metcan_death;
  metcan->clear = metcan_clear;
  metcan->blitsizex = 0;
  metcan->blitsizey = 0;
  metcan->blitstartx = 0;
  metcan->blitstarty = 0;

  metcan->lethal = 0; //harmless

  metcan->animate = copy_anim((ANIM *)orgmetcan);
  metcan->frame = anim_setsequence(metcan->animate, (dirx >= 1) ? 0 : 1, SEQ_FORCE);

  metcan->sizex = metcan->frame->get_width();
  metcan->sizey = metcan->frame->get_height();

  metcan->blitsizex = metcan->sizex;
  metcan->blitsizey = metcan->sizey;
  metcan->blitstartx = 0;
  metcan->blitstarty = 0;
  metcan->lethal = 0;
  metcan->coloffsetx = 6;
  metcan->coloffsety = 6;
  metcan->colwidth = metcan->sizex - 12;
  metcan->colheight= metcan->sizey - 12;


  metcan->blood = (void *) malloc(sizeof(METCAN_BLOOD));
//  vgassert(metcan->blood);
  METCAN_BLOOD *metcanblood;

  metcanblood = (METCAN_BLOOD *)metcan->blood;
  metcanblood->dirx       = dirx;
  metcanblood->initdelay  = delay;
  metcanblood->delay      = startdelay;

  object_add(metcan);
}


int metcan_live (OBJECT *object, UINT32 param)
{
  METCAN_BLOOD *metcanblood;

  metcanblood = (METCAN_BLOOD *)object->blood; /* in case of multiple metcans */


  metcanblood->delay--;

  if (metcanblood->delay == 10)
    {
      object->frame = anim_forceframe(object->animate, 1);
      if (metcanblood->dirx  >=  1)
	{
	  metbal_init(object->x+48, object->y+8, metcanblood->dirx );
        play_cannon(object->x+48, object->y+8);
      }
      else
	{
	  metbal_init(object->x-16, object->y+8, metcanblood->dirx );
        play_cannon(object->x-16, object->y+8);
	}
    }

  if (!metcanblood->delay)
    {
      object->frame = anim_forceframe(object->animate, 0);
      metcanblood->delay = metcanblood->initdelay;
    }

  return 0;
}

void metcan_death(OBJECT *object, UINT32 param)
{
}

void metcan_clear(OBJECT *object, UINT32 param)
{
}



