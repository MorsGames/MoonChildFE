#include <frm_wrk.hpp>
#include <rocket.hpp>
#include <plof.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>


void rocket_init(UINT16 x, UINT16 y, UINT16 miny, INT16 accy)
{
  OBJECT *rocket;
  rocket = (OBJECT *) malloc (sizeof(OBJECT));
  rocket->x = x;
  rocket->y = y;
  rocket->live  = rocket_live;
  rocket->death = rocket_death;
  rocket->clear = rocket_clear;
  rocket->blitsizex = 0;
  rocket->blitsizey = 0;

  rocket->animate = copy_anim(orgrocketup);

  rocket->frame   = anim_setsequence(rocket->animate, 0, SEQ_FORCE);

  rocket->sizex = rocket->frame->get_width();
  rocket->sizey = rocket->frame->get_height();

  rocket->lethal = 3; // if hit then death!
  rocket->coloffsetx = 15;
  rocket->coloffsety = 16;
  rocket->colwidth  = 2;
  rocket->colheight = rocket->sizey - 32;
  
  rocket->blood = (void *) malloc(sizeof(ROCKET_BLOOD));
//  vgassert(rocket->blood);
  ROCKET_BLOOD *rocketblood;
  rocketblood = (ROCKET_BLOOD *)rocket->blood;

  rocketblood->x = x;
  rocketblood->y = y<<8;
  rocketblood->gravy = 0;
  rocketblood->accy = accy;
  rocketblood->miny = miny;
  rocketblood->anmcnt = 0;
  
  object_add(rocket);

  return;
}


int rocket_live (OBJECT *object, UINT32 param)
{

  ROCKET_BLOOD *rocketblood;
  rocketblood = (ROCKET_BLOOD *)object->blood; /* in case of multiple rockets */


  if (autoscrollspd)
  {
    object->x -= (autoscrollspd -1);
  }
  
  if (rocketblood->gravy != 0)
  {
    rocketblood->gravy += rocketblood->accy;
  }
  else
  {
    if ((UINT16)(object->x - hoi->x + 50) < 100)
    {
      if ((UINT16)(object->y - hoi->y) < 400)
      {
        rocketblood->gravy = -64;
        anim_setsequence(object->animate, 1, SEQ_FORCE);
        play_raket(object->x, object->y);
      }
    }
  }

  if (rocketblood->gravy != 0)
  {
    rocketblood->anmcnt++;
    rocketblood->anmcnt &= 3;
    object->frame = anim_forceframe(object->animate, rocketblood->anmcnt);
  }

  rocketblood->y += rocketblood->gravy;
  object->x = rocketblood->x;
  object->y = rocketblood->y >> 8;

  if ((object->y < rocketblood->miny || object->y < hoi->y) && rocketblood->gravy)
  {
    plof_init(object->x + (object->sizex/2), object->y + (object->sizey/2), 0, 1, 0, rocketblood->gravy >> 3, 0);
    return 1;
  }

  return 0;
}

void rocket_death(OBJECT *object, UINT32 param)
{
}

void rocket_clear(OBJECT *object, UINT32 param)
{
}


