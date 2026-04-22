#include <frm_wrk.hpp>
#include <smlheart.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

// source in 'moonchildworld' coordinates. Destination in screen coordinates

// appearance:  0 = energy / 1 = newlife / 2 = blackdiamond

void smlheart_init(UINT16 srcx, UINT16 srcy, UINT16 appearance, UINT16 scoreholdpos)
{
  OBJECT *smlheart;
  smlheart = (OBJECT *) malloc (sizeof(OBJECT));
  smlheart->x = srcx;
  smlheart->y = srcy;
  smlheart->live  = smlheart_live;
  smlheart->death = smlheart_death;
  smlheart->clear = smlheart_clear;
  smlheart->blitsizex = 0;
  smlheart->blitsizey = 0;
  smlheart->blitstartx = 0;
  smlheart->blitstarty = 0;

  smlheart->lethal = 0; //harmless

  if (appearance == 2)    // black diamond
    {
      smlheart->animate = copy_anim((ANIM *)orgdiamond);
      smlheart->frame = anim_setsequence(smlheart->animate, 0, SEQ_FORCE);
    }
  else
    {
      smlheart->animate = copy_anim((ANIM *)orgsmallheart);
      smlheart->frame = anim_setsequence(smlheart->animate, 0, SEQ_FORCE);
    }


  smlheart->sizex = smlheart->frame->get_width();
  smlheart->sizey = smlheart->frame->get_height();

  smlheart->blitsizex = smlheart->sizex;
  smlheart->blitsizey = smlheart->sizey;
  smlheart->blitstartx = 0;
  smlheart->blitstarty = 0;
  smlheart->lethal = 0;
  smlheart->coloffsetx = 6;
  smlheart->coloffsety = 6;
  smlheart->colwidth = smlheart->sizex - 12;
  smlheart->colheight= smlheart->sizey - 12;


  smlheart->blood = (void *) malloc(sizeof(SMLHEART_BLOOD));
//  vgassert(smlheart->blood);
  SMLHEART_BLOOD *smlheartblood;

  smlheartblood = (SMLHEART_BLOOD *)smlheart->blood;
  smlheartblood->srcx         = srcx - player1.worldx - player1.quakex;
  smlheartblood->srcy         = srcy - player1.worldy - player1.quakey;
  smlheartblood->appearance   = appearance;
  smlheartblood->scoreholdpos = scoreholdpos;
  smlheartblood->sincnt = 0;
  object_add(smlheart);
}


int smlheart_live (OBJECT *object, UINT32 param)
{
  SMLHEART_BLOOD *smlheartblood;
  INT16 factor;
  UINT16 destx,desty;

  smlheartblood = (SMLHEART_BLOOD *)object->blood; /* in case of multiple smlhearts */

  destx = scoreposhold[ smlheartblood->scoreholdpos*2   ];
  desty = scoreposhold[(smlheartblood->scoreholdpos*2)+1];

  factor = sinus512[smlheartblood->sincnt & 1023];

  //  object->x = ((smlheartblood->srcx * factor) + (smlheartblood->destx * factor)) / 1024;
  //  object->y = ((smlheartblood->srcy * factor) + (smlheartblood->desty * factor)) / 1024;

  object->x = ((destx - smlheartblood->srcx)*factor) / 512;
  object->y = ((desty - smlheartblood->srcy)*factor) / 512;

  object->x += smlheartblood->srcx;
  object->y += smlheartblood->srcy;

  object->x += player1.worldx - player1.quakex;
  object->y += player1.worldy - player1.quakey;

  smlheartblood->sincnt+=4;

  if (smlheartblood->sincnt == 256)
    {
      if (smlheartblood->appearance == 1) player1.newlife++;

      if (smlheartblood->appearance == 0)
	{
	  if (player1.energy < 32) 
	    {
	      player1.energy+=2;
	      if (player1.energy > 32) player1.energy = 32;
	    }
	}

      if (smlheartblood->appearance == 2) player1.nrofblack--;

      return 1;
    }

  return 0;
}

void smlheart_death(OBJECT *object, UINT32 param)
{
}

void smlheart_clear(OBJECT *object, UINT32 param)
{
}



