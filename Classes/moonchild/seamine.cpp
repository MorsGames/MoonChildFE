#include <frm_wrk.hpp>
#include <seamine.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


OBJECT *seamine_init(UINT16 x, UINT16 y)
{
  OBJECT *seamine;
  seamine = (OBJECT *) malloc (sizeof(OBJECT));
  seamine->x = x;
  seamine->y = y;
  seamine->live  = seamine_live;
  seamine->death = seamine_death;
  seamine->clear = seamine_clear;
  seamine->sizex = 48;
  seamine->sizey = 48;
  seamine->blitsizex = 0;
  seamine->blitsizey = 0;
  seamine->blitstartx = 0;
  seamine->blitstarty = 0;

  seamine->coloffsetx = 8;
  seamine->coloffsety = 8;
  seamine->colwidth  = 32;
  seamine->colheight = 32;

  seamine->lethal = 1; //harmless

  seamine->animate = copy_anim((ANIM *)orgseamine);
  seamine->frame = anim_setsequence(seamine->animate, 0, SEQ_FORCE);

  seamine->blood = (void *) malloc(sizeof(SEAMINE_BLOOD));
//  vgassert(seamine->blood);
  SEAMINE_BLOOD *seamineblood;

  seamineblood = (SEAMINE_BLOOD *)seamine->blood;
  seamineblood->dummy     = 0;
  object_add(seamine);
  return seamine;
}


int seamine_live (OBJECT *object, UINT32 param)
{
  SEAMINE_BLOOD *seamineblood;

  seamineblood = (SEAMINE_BLOOD *)object->blood; /* in case of multiple seamines */


  return 0;
}

void seamine_death(OBJECT *object, UINT32 param)
{
}

void seamine_clear(OBJECT *object, UINT32 param)
{
}



