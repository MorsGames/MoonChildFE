#include <frm_wrk.hpp>
#include <stukhout.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>


void stukhout_init(UINT16 x, UINT16 y, UINT16 ystop)
{
  OBJECT *stukhout;
  stukhout = (OBJECT *) malloc (sizeof(OBJECT));
  stukhout->x = x;
  stukhout->y = y;
  stukhout->live  = stukhout_live;
  stukhout->death = stukhout_death;
  stukhout->clear = stukhout_clear;

  stukhout->animate = (ANIM *)copy_anim(orgstukhout);
  stukhout->frame = anim_setsequence(stukhout->animate, 0, SEQ_FORCE);

  stukhout->sizex = stukhout->frame->get_width();
  stukhout->sizey = stukhout->frame->get_height();

  stukhout->blitsizex = stukhout->sizex;
  stukhout->blitsizey = 2;
  stukhout->blitstartx = 0;
  stukhout->blitstarty = stukhout->sizey-2;

  stukhout->lethal = 0; //harmless

  stukhout->blood = (void *) malloc(sizeof(STUKHOUT_BLOOD));
//  vgassert(stukhout->blood);
  STUKHOUT_BLOOD *stukhoutblood;

  stukhoutblood = (STUKHOUT_BLOOD *)stukhout->blood;

  stukhoutblood->ystop = ystop;
  stukhoutblood->childcol = 0;
  stukhoutblood->oldx = x;
  stukhoutblood->oldy = y;

  object_add_first(stukhout);
}


int stukhout_live (OBJECT *object, UINT32 param)
{
  STUKHOUT_BLOOD *stukhoutblood;

  stukhoutblood = (STUKHOUT_BLOOD *)object->blood; /* in case of multiple stukhouts */

  stukhoutblood->oldx = object->x;
  stukhoutblood->oldy = (object->y-object->blitstarty);

  object->frame = anim_nextframe(object->animate);

#if 1

  if (  object->blitsizey != object->sizey && object->blitstarty != 0)
  {
    object->blitsizey  += 1;
    object->blitstarty -= 1;
  }
  else
  {
      object->y += 1;
    if (object->y+object->sizey > stukhoutblood->ystop)
    {
      object->blitsizey = stukhoutblood->ystop - object->y + 1;
    }
  }

  if ((object->y+2) >= stukhoutblood->ystop) return 1;

#endif

/* code for collision with hoi */
  
  if (stukhoutblood->childcol)
  {
    hoi->x -= stukhoutblood->oldx - object->x;
    hoi->y -= stukhoutblood->oldy - (object->y - object->blitstarty);
  }

  if (object->blitsizey > 32)
  {
  if ((UINT16)(hoi->x - object->x + 32) < 128)   /* x collision */
  {
    if ((UINT16)(hoi->y - (object->y) - (32-object->blitstarty) + hoi->sizey) <16)   /* y collsion */
    {
      HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

      if (hoiblood->gravy > 0)
      {

        hoi->y = (object->y) - hoi->sizey + (32- object->blitstarty) ;

        stukhoutblood->childcol = 1;

        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;
      }
      else
      {
        stukhoutblood->childcol = 0;
      }
    }
    else
    {
    stukhoutblood->childcol = 0;
    }
  }
  else
  {
    stukhoutblood->childcol = 0;
  }
  }

  return 0;
}


void stukhout_death(OBJECT *object, UINT32 param)
{
}

void stukhout_clear(OBJECT *object, UINT32 param)
{
}



