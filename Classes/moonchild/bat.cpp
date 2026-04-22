#include <frm_wrk.hpp>
#include <bat.hpp>
#include <robyn.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>



// If forced seq is NOT SEQ_DUMMY the anim-sequence will be forced

void bat_init(UINT16 x, UINT16 y,
               BASIC_CB batroutx, UINT16 x1, UINT16 x2, UINT16 x3,
               BASIC_CB batrouty, UINT16 y1, UINT16 y2, UINT16 y3)
{
  OBJECT *bat;
  bat = (OBJECT *) malloc (sizeof(OBJECT));
  bat->x = x;
  bat->y = y;
  bat->live  = bat_live;
  bat->death = bat_death;
  bat->clear = bat_clear;
  bat->blitsizex = 0;
  bat->blitsizey = 0;
  bat->lethal = 1; //harmless

  bat->animate = copy_anim(orgbat);
//  vgassert(bat->animate);
  bat->frame = anim_setsequence(bat->animate, 0, SEQ_FORCE);

  bat->sizex = bat->frame->get_width();;
  bat->sizey = bat->frame->get_height();;

  bat->coloffsetx = 4;
  bat->coloffsety = 4;
  bat->colwidth  = bat->sizex - 4;
  bat->colheight = bat->sizey - 4;

  bat->blood = (void *) malloc(sizeof(BAT_BLOOD));
//  vgassert(bat->blood);
  BAT_BLOOD *batblood;
  batblood = (BAT_BLOOD *)bat->blood;
  batblood->x  = x;
  batblood->y  = y;
  batblood->moveroutx = batroutx;
  batblood->x1 = x1;
  batblood->x2 = x2;
  batblood->x3 = x3;
  batblood->moverouty = batrouty;
  batblood->y1 = y1;
  batblood->y2 = y2;
  batblood->y3 = y3;
  batblood->shootdelay = 100;
  batblood->shootoverride = 0;
  batblood->shootdirx = 0;
  object_add(bat);
}


int bat_live (OBJECT *object, UINT32 param)
{
  BAT_BLOOD *batblood;
  UINT16       oldx;

  batblood = (BAT_BLOOD *)object->blood; /* in case of multiple bats */

  object->frame = anim_nextframe(object->animate);
  object->frame = anim_nextframe(object->animate);

  oldx = object->x;
  if (batblood->moveroutx)
  {
    object->x = batblood->moveroutx((INT16 *)&batblood->x, &batblood->x1, &batblood->x2, &batblood->x3);
  }

  if (batblood->moverouty)
  {
    object->y = batblood->moverouty((INT16 *)&batblood->y, &batblood->y1, &batblood->y2, &batblood->y3);
  }

  if (batblood->shootoverride)
  {
    if (batblood->shootoverride == 4 )
    {
      robyn_init(object->x+16 + ((batblood->shootdirx) ? 10 : -10), object->y+18, (batblood->shootdirx) ? 40 : -40, -20);
    }
    anim_forceframe(object->animate, batblood->shootoverride / 3);
    batblood->shootoverride--;
  }
  else
  {
    if (oldx != object->x) // bewogen?
    { 
      if (oldx < object->x) //naar rechts?
      {
        batblood->lastdirx = 1;
        anim_setsequence(object->animate, SEQ_FLYRIGHT, SEQ_NOFORCE);
      }
      else
      {
        batblood->lastdirx = 0;
        anim_setsequence(object->animate, SEQ_FLYLEFT, SEQ_NOFORCE);
      }
    }
  }
  
  batblood->shootdelay--;
  if (!batblood->shootdelay)
  {
    batblood->shootdelay = MC_RANDOM(120)+40;

    batblood->shootoverride = 23;
    batblood->shootdirx = batblood->lastdirx;

    if (batblood->lastdirx) //naar rechts?
    {
      anim_setsequence(object->animate, SEQ_STANDRIGHT, SEQ_NOFORCE);
    }
    else
    {
      anim_setsequence(object->animate, SEQ_STANDLEFT, SEQ_NOFORCE);
    }

  }
  
  {
	INT32 distxl,distyl;
	INT32 distxr,distyr;

      distxl = abs(hoi->x - (object->x-64));
	distyl = abs(hoi->y - object->y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (object->x+64));
	distyr = abs(hoi->y - object->y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	prox_bat_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
  }
  return 0;
}

void bat_death(OBJECT *object, UINT32 param)
{
}

void bat_clear(OBJECT *object, UINT32 param)
{
}










