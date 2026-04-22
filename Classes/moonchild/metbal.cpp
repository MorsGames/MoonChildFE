#include <frm_wrk.hpp>
#include <metbal.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>
#include <plof.hpp>


void metbal_init(UINT16 x, UINT16 y, INT16 spdx)
{
  OBJECT *metbal;
  metbal = (OBJECT *) malloc (sizeof(OBJECT));
  metbal->x = x;
  metbal->y = y;
  metbal->live  = metbal_live;
  metbal->death = metbal_death;
  metbal->clear = metbal_clear;
  metbal->blitsizex = 0;
  metbal->blitsizey = 0;
  metbal->blitstartx = 0;
  metbal->blitstarty = 0;

  metbal->animate = copy_anim((ANIM *)orgmetbal);
  metbal->frame = anim_setsequence(metbal->animate, 0, SEQ_FORCE);


  metbal->sizex = metbal->frame->get_width();
  metbal->sizey = metbal->frame->get_height();

  metbal->blitsizex = metbal->sizex;
  metbal->blitsizey = metbal->sizey;
  metbal->blitstartx = 0;
  metbal->blitstarty = 0;
  metbal->lethal = 3;         // fast nrg drain
  metbal->coloffsetx = 6;
  metbal->coloffsety = 6;
  metbal->colwidth = metbal->sizex - 12;
  metbal->colheight= metbal->sizey - 12;

  metbal->visible = 1;

  metbal->blood = (void *) malloc(sizeof(METBAL_BLOOD));
//  vgassert(metbal->blood);
  METBAL_BLOOD *metbalblood;

  metbalblood = (METBAL_BLOOD *)metbal->blood;
  metbalblood->x         = x*2;
  metbalblood->spdx      = spdx;
  metbalblood->lifespan  = 300;
  object_add(metbal);
}


int metbal_live (OBJECT *object, UINT32 param)
{
  METBAL_BLOOD *metbalblood;

  metbalblood = (METBAL_BLOOD *)object->blood; /* in case of multiple metbals */

  metbalblood->x += metbalblood->spdx;
  object->x = metbalblood->x/2;

  if (collision(object->x+(object->sizex/2), object->y + (object->sizey) ))
    {
      plof_init(object->x, object->y, 1, 0, 0, 0, 0);
      return 1;
    }

  if (metbalblood->lifespan--) return 0;      // still visible

  return 1;    // remove ball since it's out of screen
}


void metbal_death(OBJECT *object, UINT32 param)
{
}

void metbal_clear(OBJECT *object, UINT32 param)
{
}



