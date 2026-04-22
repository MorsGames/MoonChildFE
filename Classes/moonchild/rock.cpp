#include <frm_wrk.hpp>
#include <rock.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


OBJECT *rock_init(UINT16 x, UINT16 y, UINT16 minx, UINT16 maxx)
{
  OBJECT *rock;
  rock = (OBJECT *) malloc (sizeof(OBJECT));
  rock->x = x;
  rock->y = y;
  rock->live  = rock_live;
  rock->death = rock_death;
  rock->clear = rock_clear;
  rock->blitsizex = 0;
  rock->blitsizey = 0;
  rock->blitstartx = 0;
  rock->blitstarty = 0;

  rock->animate = copy_anim((ANIM *)orgbarrel);
  rock->frame = anim_setsequence(rock->animate, 0, SEQ_FORCE);


  rock->sizex = rock->frame->get_width();
  rock->sizey = rock->frame->get_height();

  rock->blitsizex = rock->sizex;
  rock->blitsizey = rock->sizey;
  rock->blitstartx = 0;
  rock->blitstarty = 0;
  rock->lethal = 4;              // collision notify
  rock->coloffsetx = 6;
  rock->coloffsety = 6;
  rock->colwidth = rock->sizex - 12;
  rock->colheight= rock->sizey - 12;

  rock->collided = 0;

  rock->blood = (void *) malloc(sizeof(ROCK_BLOOD));
//  vgassert(rock->blood);
  ROCK_BLOOD *rockblood;

  rockblood = (ROCK_BLOOD *)rock->blood;
  rockblood->minx = minx;
  rockblood->maxx = maxx;
  rockblood->spdx = 0;
  object_add(rock);

  return rock;
}


int rock_live (OBJECT *object, UINT32 param)
{
  ROCK_BLOOD *rockblood;

  rockblood = (ROCK_BLOOD *)object->blood; /* in case of multiple rocks */

  if (object->collided)
    {
      if (rockblood->spdx == 0)
	{
	  if (object->x == rockblood->minx) //left to right?
	    {
	      rockblood->spdx = 2;
	    }
	  else
	    {
	      rockblood->spdx = -2;
	    }
	}
    }

  if (rockblood->spdx > 0) // left to right?
    {
      rockblood->spdx++;
      object->x += rockblood->spdx >> 2;

      {
	INT32 distxl,distyl;
	INT32 distxr,distyr;

      distxl = abs(hoi->x - (object->x-64));
	distyl = abs(hoi->y - object->y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (object->x+64));
	distyr = abs(hoi->y - object->y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	prox_roltnlp_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
      }
  
  
  if (object->x > rockblood->maxx)
	{
	  object->x = rockblood->maxx;
	  rockblood->spdx = 0;
	}
    }

  
  if (rockblood->spdx < 0) // right to left?
    {
      rockblood->spdx--;
      object->x += rockblood->spdx >> 2;

      {
	INT32 distxl,distyl;
	INT32 distxr,distyr;

      distxl = abs(hoi->x - (object->x-64));
	distyl = abs(hoi->y - object->y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (object->x+64));
	distyr = abs(hoi->y - object->y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	prox_roltnlp_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
      }

      if (object->x < rockblood->minx)
	{
	  object->x = rockblood->minx;
	  rockblood->spdx = 0;
	}
    }

  object->collided = 0;

  object->frame = anim_forceframe(object->animate, ((object->x/8)-1) & 15);

  return 0;
}

void rock_death(OBJECT *object, UINT32 param)
{
}

void rock_clear(OBJECT *object, UINT32 param)
{
}



