#include <frm_wrk.hpp>
#include <mouth.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

UINT16 mouthwiggletab[36] = 
{
  0,0,0,0,
  3,3,3,3,
  0,0,0,3,
  3,3,0,0,
  3,0,0,3,
  3,0,0,3,
  3,0,3,0,
  3,0,3,0,
  3,0,3,0
};

UINT16 mouthanimtab[8] = 
{
  0,1,2,3,4,5,4,3
};

void mouth_init(UINT16 x, UINT16 y, INT16 height)
{
  OBJECT *mouth;
  mouth = (OBJECT *) malloc (sizeof(OBJECT));
  mouth->x = x;
  mouth->y = y;
  mouth->live  = mouth_live;
  mouth->death = mouth_death;
  mouth->clear = mouth_clear;
  mouth->blitsizex = 0;
  mouth->blitsizey = 0;
  mouth->blitstartx = 0;
  mouth->blitstarty = 0;

  mouth->lethal = 0; //harmless

  mouth->animate = copy_anim((ANIM *)orgmouth);
  mouth->frame = anim_setsequence(mouth->animate, 0, SEQ_FORCE);


  mouth->sizex = mouth->frame->get_width();
  mouth->sizey = mouth->frame->get_height();

  mouth->blitsizex = 0;
  mouth->blitsizey = 0;
  mouth->blitstartx = 0;
  mouth->blitstarty = 0;
  mouth->lethal = 1;
  mouth->coloffsetx = 6;
  mouth->coloffsety = 6;
  mouth->colwidth = 48;
  mouth->colheight= 16;


  mouth->blood = (void *) malloc(sizeof(MOUTH_BLOOD));
//  vgassert(mouth->blood);
  MOUTH_BLOOD *mouthblood;

  mouthblood = (MOUTH_BLOOD *)mouth->blood;
  mouthblood->x       = x;
  mouthblood->y       = y<<8;
  mouthblood->ground  = y;
  mouthblood->height  = height<<8;
  mouthblood->delay   = 25;
  mouthblood->framenr = 0;
  object_add(mouth);
}


int mouth_live (OBJECT *object, UINT32 param)
{
  MOUTH_BLOOD *mouthblood;

  mouthblood = (MOUTH_BLOOD *)object->blood; /* in case of multiple mouths */


  if (!mouthblood->delay)
    {
      mouthblood->delay = 10000; // dummy value
      mouthblood->gravy = mouthblood->height;
      mouthblood->framenr = 0;
    }


  if (mouthblood->delay > 1000)     // mouth is in action apparently
    {
      mouthblood->y +=mouthblood->gravy;
      mouthblood->gravy+=70;
      if ((mouthblood->y>>8) > mouthblood->ground)
	{
	  mouthblood->y = mouthblood->ground<<8;
	  mouthblood->gravy = (-mouthblood->gravy)+700;
	  if (mouthblood->gravy > 0)
	    {
	      mouthblood->gravy = 0;
	      mouthblood->y = mouthblood->ground<<8;
	      mouthblood->delay = MC_RANDOM(100)+50;
	    }
	}
    }

  if (mouthblood->delay > 1000)
    {
      if (mouthblood->gravy > -700 && mouthblood->framenr < 1) mouthblood->framenr = 1;
      if (mouthblood->gravy > -600 && mouthblood->framenr < 2) mouthblood->framenr = 2;
      if (mouthblood->gravy > -500 && mouthblood->framenr < 3) mouthblood->framenr = 3;
      if (mouthblood->gravy > -400 && mouthblood->framenr < 4)
			{
				mouthblood->framenr = 4;
				play_tanden(object->x, object->y);
			}
			if (mouthblood->gravy > -300 && mouthblood->framenr < 5) mouthblood->framenr = 5;
      if (mouthblood->gravy > -200 && mouthblood->framenr < 6) mouthblood->framenr = 6;
      if (mouthblood->gravy > -100 && mouthblood->framenr < 7) mouthblood->framenr = 7;
    }

  object->frame = anim_forceframe(object->animate, mouthanimtab[mouthblood->framenr]);

  object->sizex = object->frame->get_width();
  object->sizey = object->frame->get_height();
  object->colwidth = object->sizex-6;

  object->x = mouthblood->x - (object->sizex/2);
  object->y = (mouthblood->y>>8) - object->sizey;

  if (mouthblood->delay < 1000 && mouthblood->framenr > 0) mouthblood->framenr--;


  if (mouthblood->delay < 35)  // do little wiggle
    {

			if (mouthblood->delay == 34) play_tandenm(object->x, object->y);
      object->y += mouthwiggletab[35-mouthblood->delay];
    }

  mouthblood->delay--;

  return 0;
}

void mouth_death(OBJECT *object, UINT32 param)
{
}

void mouth_clear(OBJECT *object, UINT32 param)
{
}



