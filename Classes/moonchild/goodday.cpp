#include <frm_wrk.hpp>
#include <goodday.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


OBJECT *goodday_init(UINT16 x, UINT16 y)
{
  OBJECT *goodday;
  goodday = (OBJECT *) malloc (sizeof(OBJECT));
  goodday->x = x;
  goodday->y = y;
  goodday->live  = goodday_live;
  goodday->death = goodday_death;
  goodday->clear = goodday_clear;
  goodday->sizex = 48;
  goodday->sizey = 48;
  goodday->blitsizex = 0;
  goodday->blitsizey = 0;
  goodday->blitstartx = 0;
  goodday->blitstarty = 0;

  goodday->coloffsetx = 8;
  goodday->coloffsety = 8;
  goodday->colwidth  = 32;
  goodday->colheight = 32;

  goodday->lethal = 1; //harmless

  goodday->animate = copy_anim((ANIM *)orggoodday);
  goodday->frame = anim_setsequence(goodday->animate, 0, SEQ_FORCE);

  goodday->blood = (void *) malloc(sizeof(GOODDAY_BLOOD));
//  vgassert(goodday->blood);
  GOODDAY_BLOOD *gooddayblood;

  gooddayblood = (GOODDAY_BLOOD *)goodday->blood;
  gooddayblood->dummy     = 0;
  object_add(goodday);
  return goodday;
}


int goodday_live (OBJECT *object, UINT32 param)
{
  GOODDAY_BLOOD *gooddayblood;

  gooddayblood = (GOODDAY_BLOOD *)object->blood; /* in case of multiple gooddays */


  return 0;
}

void goodday_death(OBJECT *object, UINT32 param)
{
}

void goodday_clear(OBJECT *object, UINT32 param)
{
}



