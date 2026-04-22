#include <frm_wrk.hpp>
#include <lift.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>


void lift_init(UINT16 x, UINT16 y, UINT16 ystop)
{
  OBJECT *lift;
  lift = (OBJECT *) malloc (sizeof(OBJECT));
  lift->x = x;
  lift->y = y;
  lift->live  = lift_live;
  lift->death = lift_death;
  lift->clear = lift_clear;
  lift->sizex = 88;
  lift->sizey = 16;
  lift->blitsizex = 0;
  lift->blitsizey = 0;
  lift->lethal = 0; // harmless

  lift->animate = (ANIM *)copy_anim(orglift);
  lift->frame = anim_setsequence(lift->animate, 0, SEQ_FORCE);
  lift->animate->delaycnt = 0;
  lift->animate->stepnr = 0;
  lift->frame = anim_nextframe(lift->animate);

  lift->blood = (void *) malloc(sizeof(LIFT_BLOOD));
//  vgassert(lift->blood);
  LIFT_BLOOD *liftblood;

  liftblood = (LIFT_BLOOD *)lift->blood;
  liftblood->childcol    = 0;
  liftblood->delay = 16;
  liftblood->ystop = ystop;
  object_add(lift);
}


int lift_live (OBJECT *object, UINT32 param)
{
  LIFT_BLOOD *liftblood;

  liftblood = (LIFT_BLOOD *)object->blood; /* in case of multiple lifts */

/* code for collision with hoi */
  
  if ((UINT16)(hoi->x - object->x + 32) < 112)   /* x collision */
    {
      if ((UINT16)(hoi->y - object->y + hoi->sizey) <16)   /* y collsion */
  {
    HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

    if (hoiblood->gravy > 0)
      {
        Cspr_frame *dummyframe;

        hoi->y = object->y - hoi->sizey;
        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;

        if (liftblood->ystop > object->y)
    {
      liftblood->delay--;
      if (!liftblood->delay) liftblood->delay = 12;
      if (liftblood->delay < 13)
        {
          dummyframe = object->frame;
          object->frame = anim_nextframe(object->animate);
          if (dummyframe != object->frame)
      {
        object->y += 1;
        hoi->y += 1;
      }
        }
    }
      }
    else
      {
        liftblood->childcol = 0;
      }
  }
      else
  {
    liftblood->childcol = 0;
  }
    }
  else
    {
      liftblood->childcol = 0;
    }

  return 0;
}

void lift_death(OBJECT *object, UINT32 param)
{
}

void lift_clear(OBJECT *object, UINT32 param)
{
}



