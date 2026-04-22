#include <frm_wrk.hpp>
#include <bullet.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <plof.hpp>


void bullet_init(UINT16 x, UINT16 y, INT16 xspd_, INT16 yspd_)
{
  OBJECT *bullet;
  bullet = (OBJECT *) malloc (sizeof(OBJECT));
  bullet->x = x;
  bullet->y = y;
  bullet->live  = bullet_live;
  bullet->death = bullet_death;
  bullet->clear = bullet_clear;
  bullet->blitsizex = 0;
  bullet->blitsizey = 0;

  bullet->animate = copy_anim((ANIM *)orgbullet1);
  bullet->frame   = anim_setsequence(bullet->animate, 0, SEQ_FORCE);

  bullet->sizex = bullet->frame->get_width();
  bullet->sizey = bullet->frame->get_height();

  bullet->coloffsetx = 8;
  bullet->coloffsety = 8;
  bullet->colwidth  = 16;
  bullet->colheight = 16;

  bullet->lethal = 0 ; // harmless

  bullet->blood = (void *) malloc(sizeof(BULLET_BLOOD));
//  vgassert(bullet->blood);
  BULLET_BLOOD *bulletblood;
  bulletblood = (BULLET_BLOOD *)bullet->blood;
  bulletblood->xfloat = x << 8;
  bulletblood->yfloat = y << 8;
  bulletblood->xspd   = xspd_;
  bulletblood->yspd   = yspd_;
  bulletblood->lifespan = 250;
  bulletblood->remove = 0;

  object_add(bullet);
}

void bullet_dummy_init(void)
{
  return;

  OBJECT *bullet;
  bullet = (OBJECT *) malloc (sizeof(OBJECT));
  bullet->x = 0;
  bullet->y = 0;
  bullet->live  = 0;
  bullet->death = 0;
  bullet->clear = 0;
  bullet->blitsizex = 0;
  bullet->blitsizey = 0;

  bullet->animate = 0;
  bullet->frame   = 0;

  bullet->sizex = 4;
  bullet->sizey = 4;

  bullet->lethal = 0 ; // harmless

  bullet->blood = (void *) malloc(sizeof(BULLET_BLOOD));
//  vgassert(bullet->blood);
  BULLET_BLOOD *bulletblood;
  bulletblood = (BULLET_BLOOD *)bullet->blood;
  bulletblood->xfloat = 0;
  bulletblood->yfloat = 0;
  bulletblood->xspd   = 0;
  bulletblood->yspd   = 0;
  bulletblood->lifespan = 250;
  bulletblood->remove = 0;
}


int bullet_live (OBJECT *object, UINT32 param)
{

  UINT16 r;

  BULLET_BLOOD *bulletblood;
  bulletblood = (BULLET_BLOOD *)object->blood; /* in case of multiple bullets */

// handler which delayes removal of bullet from memory
// this is due to the latency in the collision handler...

  if (bulletblood->remove)
  {
    bulletblood->remove--;
    if (!bulletblood->remove)
    {
      return 1;
    }
    return 0;
  }


  bulletblood->xfloat += bulletblood->xspd;
  bulletblood->yfloat += bulletblood->yspd;

  object->x = bulletblood->xfloat >> 8;
  object->y = bulletblood->yfloat >> 8;

  if (r = collision(object->x+(object->sizex/2), object->y + (object->sizey/2) ))
  {

    if (r ==2)   // wegschietable blokje?
      {
	UINT16 x,y;
	x = object->x+(object->sizex/2);
	y = object->y+(object->sizey/2);
	x &= 0xffe0;
	y &= 0xffe0;
	
	player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = 0;
	colmap[((y)*player1.loadedmap->mapsizex/16/16)+((x)/16)] = 0;
	colmap[((y)*player1.loadedmap->mapsizex/16/16)+((x+16)/16)] = 0;
	colmap[((y+16)*player1.loadedmap->mapsizex/16/16)+((x)/16)] = 0;
	colmap[((y+16)*player1.loadedmap->mapsizex/16/16)+((x+16)/16)] = 0;

	plof_init(x+16, y+16, 0, 0, 0, 0, 0);
	object->frame = 0;
	bulletblood->remove = 2;
      }
    else
      {
	plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 0, 0, 0, 0);
	object->frame = 0;
	bulletblood->remove = 2;
      }

    return 0;
  }
  object->frame = anim_nextframe(object->animate);
  object->frame = anim_nextframe(object->animate);
  object->frame = anim_nextframe(object->animate);

  if (bulletblood->lifespan)
  {
    bulletblood->lifespan -= 1;

    if (!bulletblood->lifespan)
    {
      plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 0, 0, 0, 0);
      object->frame = 0;
      bulletblood->remove = 2;
      return 0;
    }
  }

  if ((UINT16) (object->x - player1.worldx + 50) > 690)
  {
    object->frame = 0;
    bulletblood->remove = 2;
    return 0;
  }

  bullet_add(object);

  return 0;
}

void bullet_death(OBJECT *object, UINT32 param)
{
}

void bullet_clear(OBJECT *object, UINT32 param)
{
}


