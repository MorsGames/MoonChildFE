#include <frm_wrk.hpp>
#include <robyn.hpp>
#include <ptoei.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <mc.hpp>
#include <sound.hpp>


void ptoei_init(UINT16 x, UINT16 y)
{
  OBJECT *ptoei;
  ptoei = (OBJECT *) malloc (sizeof(OBJECT));
  ptoei->x = x;
  ptoei->y = y;
  ptoei->live  = ptoei_live;
  ptoei->death = ptoei_death;
  ptoei->clear = ptoei_clear;
  ptoei->sizex = 48;
  ptoei->sizey = 48;
  ptoei->blitsizex = 0;
  ptoei->blitsizey = 0;
  ptoei->blitstartx = 0;
  ptoei->blitstarty = 0;

  ptoei->lethal = 0;

  ptoei->animate = copy_anim((ANIM *)orgptoei);
  ptoei->frame = anim_setsequence(ptoei->animate, 0, SEQ_FORCE);

  ptoei->blood = (void *) malloc(sizeof(PTOEI_BLOOD));
//  vgassert(ptoei->blood);
  PTOEI_BLOOD *ptoeiblood;

  ptoeiblood = (PTOEI_BLOOD *)ptoei->blood;
  ptoeiblood->delay     = MC_RANDOM(200)+220;
  object_add(ptoei);
}


char ptoei_anim[50] = {
    1,1,1,1,1,1,2,2,2,2,
    2,2,1,1,0,0,0,0,0,0,
    0,0,0,1,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,1
};


int ptoei_live (OBJECT *object, UINT32 param)
{
  PTOEI_BLOOD *ptoeiblood;

  ptoeiblood = (PTOEI_BLOOD *)object->blood; /* in case of multiple ptoeis */

  ptoeiblood->delay--;
  if (ptoeiblood->delay == 0)
  {
    ptoeiblood->delay     = MC_RANDOM(200)+220;
    object->frame = anim_forceframe(object->animate,0);
  }

  if (ptoeiblood->delay < 50)
  {
    object->frame = anim_forceframe(object->animate,ptoei_anim[ptoeiblood->delay]);
  }

  if (ptoeiblood->delay == 10)
  {
    if (object->visible) play_ptoei(object->x, object->y);
    for (UINT16 i = 0; i<16; i++)
    {
      robyn_init(object->x+16, object->y+18, MC_RANDOM(96)-48, MC_RANDOM(64)-110);
    }
  }

  return 0;
}

void ptoei_death(OBJECT *object, UINT32 param)
{
}

void ptoei_clear(OBJECT *object, UINT32 param)
{
}



