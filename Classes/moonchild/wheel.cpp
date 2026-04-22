#include <frm_wrk.hpp>
#include <wheel.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void wheel_init(UINT16 x, UINT16 y, UINT16 boty)
{
  OBJECT *wheel;
  wheel = (OBJECT *) malloc (sizeof(OBJECT));
  wheel->x = x;
  wheel->y = y;
  wheel->live  = wheel_live;
  wheel->death = wheel_death;
  wheel->clear = wheel_clear;
  wheel->blitsizex = 0;
  wheel->blitsizey = 0;
  wheel->blitstartx = 0;
  wheel->blitstarty = 0;

  wheel->lethal = 1; //harmless

  wheel->animate = copy_anim((ANIM *)orgwheel);
  wheel->frame = anim_setsequence(wheel->animate, 0, SEQ_FORCE);


  wheel->sizex = wheel->frame->get_width();
  wheel->sizey = wheel->frame->get_height();

  wheel->blitsizex = wheel->sizex;
  wheel->blitsizey = wheel->sizey;
  wheel->blitstartx = 0;
  wheel->blitstarty = 0;

  wheel->coloffsetx = 6;
  wheel->coloffsety = 6;
  wheel->colwidth = wheel->sizex - 12;
  wheel->colheight= wheel->sizey - 12;


  wheel->blood = (void *) malloc(sizeof(WHEEL_BLOOD));
//  vgassert(wheel->blood);
  WHEEL_BLOOD *wheelblood;

  wheelblood = (WHEEL_BLOOD *)wheel->blood;
  wheelblood->x          = x<<8;
  wheelblood->y          = y<<8;
  wheelblood->spdx       = 128+MC_RANDOM(256);
  wheelblood->spdy       = 0;
  wheelblood->boty       = boty;

  object_add(wheel);
}


int wheel_live (OBJECT *object, UINT32 param)
{
  UINT16 r;
  WHEEL_BLOOD *wheelblood;

  wheelblood = (WHEEL_BLOOD *)object->blood; /* in case of multiple wheels */

  wheelblood->y += wheelblood->spdy;

  wheelblood->spdy += 35;

  if (wheelblood->spdy > 0)
    {
      if (r=collision(object->x+(object->sizex/2), object->y - 8 + (object->sizey) ))
	{
	  //	  wheelblood->y += wheelblood->spdy;

	  wheelblood->y &= 0xfff000;
	  wheelblood->y += 0x800;;

        if (wheelblood->spdy > 100) play_wheel(object->x,object->y);

	  wheelblood->spdy = -(wheelblood->spdy - 650);
	  if (wheelblood->spdy > 0)
	    {
	      wheelblood->spdy = -200;
	    }

	  if (r == 6 || r == 7)
	    {
	      if (wheelblood->spdx > -500)
		{
		  wheelblood->spdx -= 300;
		}
	    }
	  if (r == 8 || r == 9)
	    {
	      if (wheelblood->spdx < 500)
		{
		  wheelblood->spdx += 300;
		}
	    }
	  if (r == 1)
	    {
	      if (wheelblood->spdx < 500)
		{
		  wheelblood->spdx += 50;
		}
	    }
	}
    }

  wheelblood->x += wheelblood->spdx;

  if ((wheelblood->x>>8) < 96)
    {
      wheelblood->x   -=  wheelblood->spdx;
      wheelblood->spdx = -wheelblood->spdx;
    }

  if ((wheelblood->x>>8) > 784)
    {
      wheelblood->x   -=  wheelblood->spdx;
      wheelblood->spdx = -wheelblood->spdx;
    }

  object->x = wheelblood->x >> 8;
  object->y = wheelblood->y >> 8;

  object->frame = anim_forceframe(object->animate, ((object->x/4)-1) & 15);

  if (object->y > wheelblood->boty)
    {
      return 1;
    }

  return 0;
}

void wheel_death(OBJECT *object, UINT32 param)
{
}

void wheel_clear(OBJECT *object, UINT32 param)
{
}



