#include <frm_wrk.hpp>
#include <plof.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>


void plof_init(UINT16 x, UINT16 y, UINT16 sequencenr, UINT16 lethal, INT16 spdx, INT16 spdy, UINT16 nosound)
{
  OBJECT *plof;
  plof = (OBJECT *) malloc (sizeof(OBJECT));
  plof->x = x;
  plof->y = y;
  plof->live  = plof_live;
  plof->death = plof_death;
  plof->clear = plof_clear;
  plof->blitsizex = 0;
  plof->blitsizey = 0;

	if (((UINT16)(x - hoi->x + 640) < 1280) && ((UINT16)(y - hoi->y + 320) < 640))
	{
		if (sequencenr == 0 && nosound == 0)
		{
			play_bigexp(x,y);
		}

		if (sequencenr == 1 && nosound == 0)
		{
			play_smexp(x,y);
		}

		if (sequencenr == 10 && nosound == 0)
		{
			play_cannon(x,y);
			sequencenr = 1;
		}
	}

  if (sequencenr == 10) sequencenr = 1;  // other sound for little plof

  plof->lethal = lethal;  // touch and very lethal

  plof->animate = copy_anim(orgplof);

  plof->frame   = anim_setsequence(plof->animate, sequencenr, SEQ_FORCE);

  plof->sizex = plof->frame->get_width();
  plof->sizey = plof->frame->get_height();

  plof->coloffsetx = 4;
  plof->coloffsety = 4;
  plof->colwidth  = plof->sizex-4;
  plof->colheight = plof->sizey-4;

  plof->x -= plof->sizex>>1;
  plof->y -= plof->sizey>>1;

  plof->blood = (void *) malloc(sizeof(PLOF_BLOOD));
//  vgassert(plof->blood);
  PLOF_BLOOD *plofblood;
  plofblood = (PLOF_BLOOD *)plof->blood;

  plofblood->lifespan = 21;

  if (sequencenr == 2) plofblood->lifespan = 24;
  if (sequencenr == 3) plofblood->lifespan = 8;
  if (sequencenr == 4) plofblood->lifespan = 12;
  if (sequencenr == 5) plofblood->lifespan = 12;

  plofblood->x = plof->x<<8;
  plofblood->y = plof->y<<8;
  plofblood->spdx = spdx;
  plofblood->spdy = spdy;



	object_add(plof);
}


int plof_live (OBJECT *object, UINT32 param)
{

  PLOF_BLOOD *plofblood;
  plofblood = (PLOF_BLOOD *)object->blood; /* in case of multiple plofs */

  object->frame = anim_nextframe(object->animate);
  object->frame = anim_nextframe(object->animate);

  plofblood->lifespan -= 1;

  plofblood->x += plofblood->spdx;
  plofblood->y += plofblood->spdy;

  object->x = plofblood->x>>8;
  object->y = plofblood->y>>8;

  if (!plofblood->lifespan) return 1;
  return 0;
}

void plof_death(OBJECT *object, UINT32 param)
{
}

void plof_clear(OBJECT *object, UINT32 param)
{
}


