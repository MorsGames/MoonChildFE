#include <frm_wrk.hpp>
#include <asteroid.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>
#include <mine.hpp>
#include <plof.hpp>



void asteroid_init (UINT16 x, UINT16 y, UINT16 tx, UINT16 ty, UINT16 bx, UINT16 by, INT16 spdx, INT16 spdy, INT16 gravy, UINT16 minenr)
{
  OBJECT *asteroid;
  asteroid = (OBJECT *) malloc (sizeof(OBJECT));
  asteroid->x = x;
  asteroid->y = y;
  asteroid->live  = asteroid_live;
  asteroid->death = asteroid_death;
  asteroid->clear = asteroid_clear;
  asteroid->blitsizex = 0;
  asteroid->blitsizey = 0;
  asteroid->blitstartx = 0;
  asteroid->blitstarty = 0;


  asteroid->animate = copy_anim((ANIM *)orgasteroids);
  asteroid->frame = anim_setsequence(asteroid->animate, 0, SEQ_FORCE);
  asteroid->frame = anim_forceframe(asteroid->animate, minenr);

  asteroid->sizex = asteroid->frame->get_width();
  asteroid->sizey = asteroid->frame->get_height();

  asteroid->blitsizex = asteroid->sizex;
  asteroid->blitsizey = asteroid->sizey;
  asteroid->blitstartx = 0;
  asteroid->blitstarty = 0;
  asteroid->lethal = 1;
  asteroid->coloffsetx = asteroid->sizex/4;
  asteroid->coloffsety = asteroid->sizey/4;
  asteroid->colwidth = asteroid->sizex - asteroid->coloffsetx;
  asteroid->colheight= asteroid->sizey - asteroid->coloffsety;


  asteroid->blood = (void *) malloc(sizeof(ASTEROID_BLOOD));
//  vgassert(asteroid->blood);
  ASTEROID_BLOOD *asteroidblood;

  asteroidblood = (ASTEROID_BLOOD *)asteroid->blood;
  asteroidblood->x = x << 8;
  asteroidblood->y = y << 8;

  asteroidblood->tx = tx;
  asteroidblood->ty = ty;
  asteroidblood->bx = bx;
  asteroidblood->by = by;

  asteroidblood->minenr = minenr;
  if (asteroidblood->minenr == 0) asteroidblood->hitcnt = 30;
  if (asteroidblood->minenr == 1) asteroidblood->hitcnt = 10;
  if (asteroidblood->minenr == 2) asteroidblood->hitcnt = 1;

  asteroidblood->gravy = gravy;
  asteroidblood->spdx = spdx;
  asteroidblood->spdy = spdy;

  asteroidblood->shootdelay = 75;

  totalgenercnt++;
  
  object_add(asteroid);
}


int asteroid_live (OBJECT *object, UINT32 param)
{
  ASTEROID_BLOOD *asteroidblood;
  OBJECT *bullet;


  asteroidblood = (ASTEROID_BLOOD *)object->blood; /* in case of multiple asteroids */

  asteroidblood->x += asteroidblood->spdx;
  asteroidblood->y += asteroidblood->spdy;

  object->x = asteroidblood->x >> 8;
  object->y = asteroidblood->y >> 8;

  if (asteroidblood->spdx >  (8*256)) asteroidblood->spdx =  (8*256);
  if (asteroidblood->spdx < -(8*256)) asteroidblood->spdx = -(8*256);
  if (asteroidblood->spdy >  (7*256)) asteroidblood->spdy =  (7*256);
  if (asteroidblood->spdy < -(7*256)) asteroidblood->spdy = -(7*256);

  asteroidblood->spdy += asteroidblood->gravy;

#if 0
  if (asteroidblood->minenr == 0) // de grote
  {
    if ((object->x+object->sizex/2)  < (hoi->x+32)) asteroidblood->spdx += 4;
    if ((object->x+object->sizex/2)  > (hoi->x+32)) asteroidblood->spdx -= 4;
    if ((object->y+object->sizey/2) < (hoi->y+32)) asteroidblood->spdy += 4;
    if ((object->y+object->sizey/2) > (hoi->y+32)) asteroidblood->spdy -= 4;

  }

  if (asteroidblood->minenr == 1) // de middel
  {
    if ((object->x+object->sizex/2)  < (hoi->x+32)) asteroidblood->spdx += 6;
    if ((object->x+object->sizex/2)  > (hoi->x+32)) asteroidblood->spdx -= 6;
    if ((object->y+object->sizey/2) < (hoi->y+32)) asteroidblood->spdy += 6;
    if ((object->y+object->sizey/2) > (hoi->y+32)) asteroidblood->spdy -= 6;

  }
#endif

  if (object->x < asteroidblood->tx)
  {
    asteroidblood->spdx = -asteroidblood->spdx;
	object->x = asteroidblood->tx;
    asteroidblood->x = object->x << 8;
  }
  if ((object->x+object->sizex) > asteroidblood->bx)
  {
    asteroidblood->spdx = -asteroidblood->spdx;
	object->x = asteroidblood->bx - object->sizex;
    asteroidblood->x = object->x << 8;
  }
  if (object->y < asteroidblood->ty)
  {
    asteroidblood->spdy = -asteroidblood->spdy;
	object->y = asteroidblood->ty;
    asteroidblood->y = object->y << 8;
  }
  if ((object->y+object->sizey) > asteroidblood->by)
  {
    asteroidblood->spdy = -asteroidblood->spdy;
	object->y = asteroidblood->by - object->sizey;
    asteroidblood->y = object->y << 8;
  }
  
  if (bullet = bullet_check(object))
  {
    asteroidblood->hitcnt--;

	if (!asteroidblood->hitcnt)
	  {
	    plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 0, 0, 0, 0, 0);

        
		if (asteroidblood->minenr == 0)
		{
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx-368, asteroidblood->spdy-668, 19, 1);
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx+468, asteroidblood->spdy-568, 20, 1);
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx-568, asteroidblood->spdy+468, 21, 1);
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx+668, asteroidblood->spdy+368, 22, 1);
	      plof_init(object->x+(object->sizex/2)-30, object->y+(object->sizey/2)-30, 0, 0, 0, 0, 0);
	      plof_init(object->x+(object->sizex/2)+30, object->y+(object->sizey/2)-30, 0, 0, 0, 0, 0);
	      plof_init(object->x+(object->sizex/2)-30, object->y+(object->sizey/2)+30, 0, 0, 0, 0, 0);
	      plof_init(object->x+(object->sizex/2)+30, object->y+(object->sizey/2)+30, 0, 0, 0, 0, 0);
		}
		
		if (asteroidblood->minenr == 1)
		{
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx-368, asteroidblood->spdy-668, 19, 2);
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx+468, asteroidblood->spdy-568, 20, 2);
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx-568, asteroidblood->spdy+468, 21, 2);
	      asteroid_init(object->x, object->y,  asteroidblood->tx, asteroidblood->ty,  asteroidblood->bx, asteroidblood->by, asteroidblood->spdx+668, asteroidblood->spdy+368, 22, 2);
		}
		
        totalshootcnt++;

		return 1;
	  }
	else
	  {
	    plof_init(bullet->x, bullet->y, 1, 0, 0, 0, 0);
	  }
  }
  
  if (object->visible && asteroidblood->minenr == 0)
  {
	asteroidblood->shootdelay--;
    if (!asteroidblood->shootdelay)
    {
      asteroidblood->shootdelay = 100;
      mine_init(object->x+48, object->y+48, ((hoi->x+32 - object->x)*256)/75, ((hoi->y+40-object->y)*256)/75, 0, 0, 100, 1, 1, 0);
    }
  }
  else
  {
    asteroidblood->shootdelay = 100;
  }

  
  return 0;
}

void asteroid_death(OBJECT *object, UINT32 param)
{
}

void asteroid_clear(OBJECT *object, UINT32 param)
{
}



