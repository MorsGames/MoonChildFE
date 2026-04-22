#include <frm_wrk.hpp>
#include <mixer.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>

UINT16 mixerframes[14]     = {0,1,2,3,4,5,6,7,6,5,4,3,2,1};
UINT16 mixerturnframes[16] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};

void mixer_init(UINT16 x, UINT16 y, UINT16 x1, UINT16 x2, UINT16 xspd)
{
  OBJECT *mixer;
  mixer = (OBJECT *) malloc (sizeof(OBJECT));
  mixer->x = x;
  mixer->y = y;
  mixer->live  = mixer_live;
  mixer->death = mixer_death;
  mixer->clear = mixer_clear;
  mixer->blitsizex = 0;
  mixer->blitsizey = 0;

  mixer->animate = copy_anim(orgmixer);

  mixer->frame   = anim_setsequence(mixer->animate, 0, SEQ_FORCE);

  mixer->sizex = mixer->frame->get_width();
  mixer->sizey = mixer->frame->get_height();

  mixer->blood = (void *) malloc(sizeof(MIXER_BLOOD));
//  vgassert(mixer->blood);
  MIXER_BLOOD *mixerblood;
  mixerblood = (MIXER_BLOOD *)mixer->blood;

  mixerblood->x = x;
  mixerblood->y = y;
  mixerblood->x1 = x1;
  mixerblood->x2 = x2;
  mixerblood->spdx = xspd;
  mixerblood->delay = 2;
  mixerblood->framenr = 0;
  mixerblood->turncnt = 0;
  mixerblood->kamikaze = 0;
  object_add(mixer);

  return;
}


int mixer_live (OBJECT *object, UINT32 param)
{
  UINT16 remem;

  MIXER_BLOOD *mixerblood;
  mixerblood = (MIXER_BLOOD *)object->blood; /* in case of multiple mixers */

  object->x += mixerblood->spdx;
  if (mixerblood->spdx > 0)
  {
    if (object->x > mixerblood->x2)
    {
      mixerblood->spdx = -mixerblood->spdx;
      mixerblood->turncnt = 16;
    }
  }
  else
  {
    if (object->x < mixerblood->x1)
    {
      mixerblood->spdx = -mixerblood->spdx;
      mixerblood->turncnt = 16;
    }
  }
  

  if (!mixerblood->turncnt)
  {
    anim_setsequence(object->animate, mixerblood->spdx > 0 ? 1 : 0, SEQ_FORCE);
    mixerblood->delay--;
    if (mixerblood->delay == 0)
    {
      mixerblood->delay = 2;
      mixerblood->framenr++;
      if (mixerblood->framenr>=14) mixerblood->framenr = 0;
      object->frame = anim_forceframe(object->animate, mixerframes[mixerblood->framenr]);
    }
  }
  else
  {
    mixerblood->turncnt--;
    mixerblood->framenr = 0;
    anim_setsequence(object->animate, 2, SEQ_FORCE);
    object->frame = anim_forceframe(object->animate, mixerturnframes[mixerblood->spdx > 0 ? mixerblood->turncnt :  15-mixerblood->turncnt]);
  }

  if (mixerblood->spdx > 0)
  {
    if (hoi->x > object->x)
    {
      object->x += mixerblood->spdx;
      if (mixerblood->kamikaze < 100)
      {
        mixerblood->kamikaze+=2;
      }
    }
    else
    {
      if (mixerblood->kamikaze > 0)
      {
        mixerblood->kamikaze-=2;
      }
    }
  }
  else
  {
    if (hoi->x < object->x)
    {
      object->x += mixerblood->spdx;
      if (mixerblood->kamikaze < 100)
      {
        mixerblood->kamikaze+=2;
      }
    }
    else
    {
      if (mixerblood->kamikaze > 0)
      {
        mixerblood->kamikaze-=2;
      }
    }
  }

  object->y = mixerblood->y + mixerblood->kamikaze;
  
  return 0;
}

void mixer_death(OBJECT *object, UINT32 param)
{
}

void mixer_clear(OBJECT *object, UINT32 param)
{
}


