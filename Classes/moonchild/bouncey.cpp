#include <frm_wrk.hpp>
#include <bouncey.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <plof.hpp>
#include <boss.hpp>
#include <sound.hpp>


void bouncey_init(UINT16 x, UINT16 y, INT16 spdx, INT16 spdy)
{
  OBJECT *bouncey;
  bouncey = (OBJECT *) malloc (sizeof(OBJECT));
  bouncey->x = x;
  bouncey->y = y;
  bouncey->live  = bouncey_live;
  bouncey->death = bouncey_death;
  bouncey->clear = bouncey_clear;
  bouncey->blitsizex = 0;
  bouncey->blitsizey = 0;

  bouncey->coloffsetx = 6;
  bouncey->coloffsety = 6;
  bouncey->colwidth = 32 - 12;
  bouncey->colheight= 32 - 12;

  bouncey->animate = copy_anim((ANIM *)orgdiamond);
  bouncey->frame   = anim_setsequence(bouncey->animate, 0, SEQ_FORCE);

  bouncey->sizex = bouncey->frame->get_width();
  bouncey->sizey = bouncey->frame->get_height();

  bouncey->lethal = 0 ; // harmless

  bouncey->blood = (void *) malloc(sizeof(BOUNCEY_BLOOD));
//  vgassert(bouncey->blood);
  BOUNCEY_BLOOD *bounceyblood;
  bounceyblood = (BOUNCEY_BLOOD *)bouncey->blood;
  bounceyblood->realx = x << 4;
  bounceyblood->gravy = spdy;
  bounceyblood->spdx  = spdx;
  bounceyblood->lifespan = 0;

  play_dimndsht(x,y);

  object_add_first(bouncey);
}


int bouncey_live (OBJECT *object, UINT32 param)
{
  INT32 rememx,rememy;

  BOUNCEY_BLOOD *bounceyblood;
  bounceyblood = (BOUNCEY_BLOOD *)object->blood; /* in case of multiple bounceys */

/* handle gravity */
  bounceyblood->gravy += 2; /* let gravity prevail! */

  rememy = object->y;
  object->y += (bounceyblood->gravy >> 3);
  if (bounceyblood->gravy > 0)
    {
      if (collision(object->x+(object->sizex/2), object->y + (object->sizey) ))
	{
	  object->y = rememy;
	  
	  bounceyblood->gravy = -(bounceyblood->gravy - 30);

 	  play_blub(object->x, object->y);

	  
	  if (bounceyblood->spdx > 0)
	    {
	      bounceyblood->spdx -= 8;
	      if (bounceyblood->spdx < 0) bounceyblood->spdx = 0;
	    }
	  else
	    {
	      bounceyblood->spdx += 8;
	      if (bounceyblood->spdx > 0) bounceyblood->spdx = 0;
	    }
	}
    }
  
  if (bounceyblood->gravy > 126)
    {
      bounceyblood->gravy = 126;
    }


/* handle x-movement */

  rememx = bounceyblood->realx;
  bounceyblood->realx += bounceyblood->spdx;

  if (collision((bounceyblood->realx>>4) + (object->sizex/2), object->y + (object->sizey/2)))
    {
      bounceyblood->realx = rememx;
      bounceyblood->spdx = -bounceyblood->spdx;
    }

  object->x = bounceyblood->realx>>4;

  if (object->x< 100) return 1;
  if (object->x > player1.worldx+900) return 1;
  if (object->y > player1.worldy+600) return 1;
 
  if (object->y+CLIPMARGINY-64 > player1.curmap->mapsizey) return 1;

  // since this is only used in level 4 boss... let's collision check with him here

  if (boss)  // boss still out there somewhere?
    {
      BOSS_BLOOD *bossblood;

      bossblood = (BOSS_BLOOD *) boss->blood;

      if (bossblood->openup)
	{
	  INT16 x;
	  INT16 y;

	  
	  x = (INT16)(boss->x+boss->coloffsetx) - (INT16)(object->x+object->coloffsetx);
	  x += boss->colwidth;
	  if ((UINT16)x < object->colwidth+boss->colwidth)
	    {
	      y = (INT16)(boss->y+boss->coloffsety) - (INT16)(object->y+object->coloffsety);
	      y += boss->colheight;
	      if ((UINT16)y < object->colheight+boss->colheight)
		{
		  if (bossblood->hit) bossblood->hit--;
		  plof_init(object->x+16, object->y+16, 0, 0, 0, 0, 0);
		  return 1;
		}
	    }
	}
    }


  if (bounceyblood->lifespan)
    {
      bounceyblood->lifespan -= 1;
      
      if (!bounceyblood->lifespan)
	{
	  plof_init(object->x+16, object->y+16, 1, 0, 0, 0, 0);
	  return 1;
	}
    }
  else
    {
      if (bounceyblood->realx == rememx)
	{
	  if (object->y == rememy)
	    {
	      bounceyblood->lifespan = 50;
	    }
	}
    }


  return 0;
}

void bouncey_death(OBJECT *object, UINT32 param)
{
}

void bouncey_clear(OBJECT *object, UINT32 param)
{
}


