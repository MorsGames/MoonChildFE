#include <frm_wrk.hpp>
#include <bump.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

void bump_init(UINT16 x, UINT16 y, UINT16 dirflg)
{
  OBJECT *bump;
  bump = (OBJECT *) malloc (sizeof(OBJECT));
  bump->x = x;
  bump->y = y;
  bump->live  = bump_live;
  bump->death = bump_death;
  bump->clear = bump_clear;
  bump->sizex = 80;
  bump->sizey = 64;
  bump->blitsizex = 0;
  bump->blitsizey = 0;
  bump->blitstartx = 0;
  bump->blitstarty = 0;

  bump->lethal = 0; //harmless

  bump->animate = copy_anim((ANIM *)orgbump);
  bump->frame = anim_setsequence(bump->animate, dirflg, SEQ_FORCE);

  bump->blood = (void *) malloc(sizeof(BUMP_BLOOD));
//  vgassert(bump->blood);
  BUMP_BLOOD *bumpblood;

  bumpblood = (BUMP_BLOOD *)bump->blood;
  bumpblood->delay     = 0;
  bumpblood->dirflg    = dirflg;
  object_add(bump);
}


int bump_live (OBJECT *object, UINT32 param)
{
  BUMP_BLOOD *bumpblood;

  bumpblood = (BUMP_BLOOD *)object->blood; /* in case of multiple bumps */

/* code for collision with hoi */
  
  switch(bumpblood->dirflg)
    {
    case 0:
      if ((UINT16)(hoi->x - object->x + 32) < 48)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + 64) <64)   /* y collision */
      {
        HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
        
        hoiblood->spdx = 16*13;
        hoiblood->gravy -= 32;
        bumpblood->delay = 23;
        object->frame = anim_forceframe(object->animate,1);
        play_spring(object->x, object->y);
      }
  }
      break;
    case 1:
      if ((UINT16)(hoi->x - object->x + 16) < 48)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + 64) <64)   /* y collision */
      {
        HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
        
        hoiblood->spdx = -(16*13);
        hoiblood->gravy -= 32;
        bumpblood->delay = 23;
        object->frame = anim_forceframe(object->animate,1);
        play_spring(object->x, object->y);
      }
  }
      break;
    case 2:
      if ((UINT16)(hoi->x - object->x + 16) < 48)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + 40) <16)   /* y collision */
      {
        HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
        
        hoiblood->gravy  = -96;
        bumpblood->delay = 23;
        object->frame = anim_forceframe(object->animate,1);
        play_spring(object->x, object->y);
      }
  }
      break;
    }


  if (bumpblood->delay)
    {
      bumpblood->delay--;
      if (!bumpblood->delay)
  {
    object->frame = anim_forceframe(object->animate,0);
  }
    }

  return 0;
}

void bump_death(OBJECT *object, UINT32 param)
{
}

void bump_clear(OBJECT *object, UINT32 param)
{
}



