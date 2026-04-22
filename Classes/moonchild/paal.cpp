#include <frm_wrk.hpp>
#include <paal.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


OBJECT *paal_init(UINT16 x, UINT16 y, UINT16 seqnr, UINT16 sincnt, UINT16 sinamp, UINT16 sinspd)
{
  OBJECT *paal;
  paal = (OBJECT *) malloc (sizeof(OBJECT));
  paal->x = x;
  paal->y = y;
  paal->live  = paal_live;
  paal->death = paal_death;
  paal->clear = paal_clear;
  paal->sizex = 16;
  paal->sizey = 192;
  paal->blitsizex = 0;
  paal->blitsizey = 0;
  paal->blitstartx = 0;
  paal->blitstarty = 0;

  paal->coloffsetx = 14;
  paal->coloffsety = 8;

  if (seqnr == 1) paal->coloffsety=136;
  
  paal->colwidth  = 4;
  paal->colheight = 32;

  paal->lethal = 1;

  paal->animate = copy_anim((ANIM *)orgpaal);
  paal->frame = anim_setsequence(paal->animate, seqnr, SEQ_FORCE);

  paal->blood = (void *) malloc(sizeof(PAAL_BLOOD));
//  vgassert(paal->blood);
  PAAL_BLOOD *paalblood;

  paalblood = (PAAL_BLOOD *)paal->blood;
  paalblood->y          = y;
  paalblood->sincnt     = sincnt;
  paalblood->sinamp     = sinamp;
  paalblood->sinspd     = sinspd;
  object_add(paal);
  return paal;
}


int paal_live (OBJECT *object, UINT32 param)
{
  PAAL_BLOOD *paalblood;

  paalblood = (PAAL_BLOOD *)object->blood; /* in case of multiple paals */

  paalblood->sincnt += paalblood->sinspd;
  paalblood->sincnt &= 1023;

  object->y = paalblood->y + ((sinus512[paalblood->sincnt]* paalblood->sinamp)>>10);

  return 0;
}

void paal_death(OBJECT *object, UINT32 param)
{
}

void paal_clear(OBJECT *object, UINT32 param)
{
}



