#include <frm_wrk.hpp>
#include <spies.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void spies_init(UINT16 x, UINT16 y, UINT16 rghtflg, UINT16 strtcnt,
    INT16 animspd, UINT16 soundflg)
{
  OBJECT *spies;
  spies = (OBJECT *) malloc (sizeof(OBJECT));
  spies->x = x;
  spies->y = y;
  spies->live  = spies_live;
  spies->death = spies_death;
  spies->clear = spies_clear;
  spies->sizex = 64;
  spies->sizey = 16;
  spies->blitsizex = 64;
  spies->blitsizey = 16;
  spies->blitstartx = 0;
  spies->blitstarty = 0;

  spies->lethal = 1; //enrgy drain

  spies->coloffsetx = 0;
  spies->coloffsety = 0;
  spies->colwidth   = 64;    // moet nog beter gaan werken met in en uitschuiven!
  spies->colheight  = 16;
  

  spies->animate = (ANIM *)copy_anim(orgspies);
  spies->frame = anim_setsequence(spies->animate, 1+rghtflg, SEQ_FORCE);

  spies->blood = (void *) malloc(sizeof(SPIES_BLOOD));
//  vgassert(spies->blood);
  SPIES_BLOOD *spiesblood;

  spiesblood = (SPIES_BLOOD *)spies->blood;
  spiesblood->sincnt    = strtcnt;
  spiesblood->sinspd    = animspd;
  spiesblood->rightflg  = rghtflg;
  spiesblood->x         = x;

  spiesblood->sounddelay = 70;
  spiesblood->soundflg = soundflg;

  object_add(spies);
}


int spies_live (OBJECT *object, UINT32 param)
{
  SPIES_BLOOD *spiesblood;

  spiesblood = (SPIES_BLOOD *)object->blood; /* in case of multiple spiess */

//  if (object->visible)
    {
      if (spiesblood->soundflg)
	{
	  spiesblood->sounddelay--;
	  if (!spiesblood->sounddelay)
	    {
	      spiesblood->sounddelay=170;
	      play_houtpunt(object->x, object->y);
	    }
	}
    }

  spiesblood->sincnt += spiesblood->sinspd;
  spiesblood->sincnt &= 255;

  object->x = spiesblood->x + ((sinus128[spiesblood->sincnt]*
            63)>>8);

  if (spiesblood->rightflg == 0)
    {
      object->blitsizex  = spiesblood->x - (object->x - (object->sizex/2));
      object->blitstartx = 64 - object->blitsizex;
      object->x = spiesblood->x;
    }
  else
    {
      object->blitsizex = spiesblood->x - (object->x - (object->sizex/2));
    }

  object->coloffsetx = 20;
  object->colwidth   = (object->blitsizex<40) ? 0 : object->blitsizex-40;
  if (object->colwidth<0) object->colwidth = 0;

  return 0;
}

void spies_death(OBJECT *object, UINT32 param)
{
}

void spies_clear(OBJECT *object, UINT32 param)
{
}



