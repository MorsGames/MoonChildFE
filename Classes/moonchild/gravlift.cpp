#include <frm_wrk.hpp>
#include <gravlift.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

void gravlift_init(UINT16 x, UINT16 y)
{
  OBJECT *gravlift;
  gravlift = (OBJECT *) malloc (sizeof(OBJECT));
  gravlift->x = x;
  gravlift->y = y;
  gravlift->live  = gravlift_live;
  gravlift->death = gravlift_death;
  gravlift->clear = gravlift_clear;

  gravlift->lethal = 0; // harmless

  gravlift->animate = (ANIM *)copy_anim(orgelevat);
  gravlift->frame = anim_setsequence(gravlift->animate, 0, SEQ_FORCE);

  gravlift->sizex = gravlift->frame->get_width();
  gravlift->sizey = gravlift->frame->get_height();
  gravlift->blitstartx = 0;
  gravlift->blitstarty = 0;
  gravlift->blitsizex = 0;
  gravlift->blitsizey = 0;

  gravlift->blood = (void *) malloc(sizeof(GRAVLIFT_BLOOD));
//  vgassert(gravlift->blood);
  GRAVLIFT_BLOOD *gravliftblood;
  gravliftblood = (GRAVLIFT_BLOOD *)gravlift->blood;
  gravliftblood->x = x;
  gravliftblood->y = y;
  gravliftblood->childcol = 0;
  object_add(gravlift);
}


int gravlift_live (OBJECT *object, UINT32 param)
{
  GRAVLIFT_BLOOD *gravliftblood;

  gravliftblood = (GRAVLIFT_BLOOD *)object->blood; /* in case of multiple gravlifts */

/* lets call the gravliftor user-routine first */

  gravliftblood->oldx = object->x;
  gravliftblood->oldy = object->y;


/* code for collision with hoi */
  
  if (gravliftblood->childcol)
    {
      object->y+=4;

      hoi->x -= gravliftblood->oldx - object->x;
      hoi->y -= gravliftblood->oldy - object->y;
    }
  else
    {
      if (object->y != gravliftblood->y) object->y--;
    }

  if ((UINT16)(hoi->x - object->x + 32) < 80)   /* x collision */
    {
      if ((UINT16)(hoi->y - object->y + hoi->sizey) <16)   /* y collsion */
	{
	  HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
	  
	  if (hoiblood->gravy > 0)
	    {
	      hoi->y = object->y - hoi->sizey;
	      
	      if (!gravliftblood->childcol)
		  {
			  gravliftblood->childcol = 1;
			  play_plntlft(object->x,object->y);
		  }
	      hoiblood->gravy = 0;
	      hoiblood->onfloor = 2;
	      hoiblood->onobject = 1;
	    }
	  else
	    {
	      gravliftblood->childcol = 0;
	    }
	}
      else
	{
	  gravliftblood->childcol = 0;
	}
    }
  else
    {
      gravliftblood->childcol = 0;
    }

  return 0;
}

void gravlift_death(OBJECT *object, UINT32 param)
{
}

void gravlift_clear(OBJECT *object, UINT32 param)
{
}










