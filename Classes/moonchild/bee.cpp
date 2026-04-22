#include <frm_wrk.hpp>
#include <bee.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>

UINT16 beeframes[14]     = {0,1,2,3,4,5,6,7,6,5,4,3,2,1};
UINT16 beeturnframes[16] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};

void bee_init(UINT16 x, UINT16 y, UINT16 x1, UINT16 x2, UINT16 xspd)
{
  OBJECT *bee;
  bee = (OBJECT *) malloc (sizeof(OBJECT));
  bee->x = x;
  bee->y = y;
  bee->live  = bee_live;
  bee->death = bee_death;
  bee->clear = bee_clear;
  bee->blitsizex = 0;
  bee->blitsizey = 0;

  bee->animate = copy_anim(orgbee);

  bee->frame   = anim_setsequence(bee->animate, 0, SEQ_FORCE);

  bee->sizex = bee->frame->get_width();
  bee->sizey = bee->frame->get_height();

  bee->lethal     = 1; // energy drain
  bee->coloffsetx = 8;
  bee->coloffsety = 8;
  bee->colwidth  = bee->sizex - 8;
  bee->colheight = bee->sizey - 8;

  bee->blood = (void *) malloc(sizeof(BEE_BLOOD));
//  vgassert(bee->blood);
  BEE_BLOOD *beeblood;
  beeblood = (BEE_BLOOD *)bee->blood;

  beeblood->x = x;
  beeblood->y = y;
  beeblood->x1 = x1;
  beeblood->x2 = x2;
  beeblood->spdx = xspd;
  beeblood->delay = 2;
  beeblood->framenr = 0;
  beeblood->turncnt = 0;
  beeblood->kamikaze = 0;
  object_add(bee);

  return;
}


int bee_live (OBJECT *object, UINT32 param)
{

  BEE_BLOOD *beeblood;
  beeblood = (BEE_BLOOD *)object->blood; /* in case of multiple bees */

  object->x += beeblood->spdx;
  if (beeblood->spdx > 0)
  {
    if (object->x > beeblood->x2)
    {
      beeblood->spdx = -beeblood->spdx;
      beeblood->turncnt = 16;
    }
  }
  else
  {
    if (object->x < beeblood->x1)
    {
      beeblood->spdx = -beeblood->spdx;
      beeblood->turncnt = 16;
    }
  }
  

  if (!beeblood->turncnt)
  {
    anim_setsequence(object->animate, beeblood->spdx > 0 ? 1 : 0, SEQ_FORCE);
    beeblood->delay--;
    if (beeblood->delay == 0)
    {
      beeblood->delay = 2;
      beeblood->framenr++;
      if (beeblood->framenr>=14) beeblood->framenr = 0;
      object->frame = anim_forceframe(object->animate, beeframes[beeblood->framenr]);
    }
  }
  else
  {
    beeblood->turncnt--;
    beeblood->framenr = 0;
    anim_setsequence(object->animate, 2, SEQ_FORCE);
    object->frame = anim_forceframe(object->animate, beeturnframes[beeblood->spdx > 0 ? beeblood->turncnt :  15-beeblood->turncnt]);
  }

	{
  	INT32 distxl,distyl;
	  INT32 distxr,distyr;

		
		distxl = abs(hoi->x - (object->x-64));
		distyl = abs(hoi->y - object->y);
		if (distxl < distyl) distxl = distyl;    // de verste distance wint

		distxr = abs(hoi->x - (object->x+64));
		distyr = abs(hoi->y - object->y);
		if (distxr < distyr) distxr = distyr;    // de verste distance wint

		prox_bee_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
	}

  if (beeblood->spdx > 0)
  {
    if (hoi->x > object->x)
    {
      object->x += beeblood->spdx;
      if (beeblood->kamikaze < 100)
      {
        beeblood->kamikaze+=2;
      }
    }
    else
    {
      if (beeblood->kamikaze > 0)
      {
        beeblood->kamikaze-=2;
      }
    }
  }
  else
  {
    if (hoi->x < object->x)
    {
      object->x += beeblood->spdx;
      if (beeblood->kamikaze < 100)
      {
        beeblood->kamikaze+=2;
      }
    }
    else
    {
      if (beeblood->kamikaze > 0)
      {
        beeblood->kamikaze-=2;
      }
    }
  }

  object->y = beeblood->y + beeblood->kamikaze;
  
  return 0;
}

void bee_death(OBJECT *object, UINT32 param)
{
}

void bee_clear(OBJECT *object, UINT32 param)
{
}


