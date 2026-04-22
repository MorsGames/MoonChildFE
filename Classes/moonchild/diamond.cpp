#include <frm_wrk.hpp>
#include <diamond.hpp>
#include <bonus.hpp>
#include <smlheart.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>
#include <glim.hpp>


void diamond_init(UINT16 x, UINT16 y)
{
  OBJECT *diamond;
  diamond = (OBJECT *) malloc (sizeof(OBJECT));
  diamond->x = x;
  diamond->y = y;
  diamond->live  = diamond_live;
  diamond->death = diamond_death;
  diamond->clear = diamond_clear;
  diamond->sizex = 48;
  diamond->sizey = 48;
  diamond->blitsizex = 0;
  diamond->blitsizey = 0;
  diamond->blitstartx = 0;
  diamond->blitstarty = 0;


  diamond->coloffsetx = 15;
  diamond->coloffsety = 15;
  diamond->colwidth  =  2;
  diamond->colheight =  2;

  diamond->lethal = 4; // detect evt. collisions
  diamond->collided = 0;

  diamond->animate = copy_anim((ANIM *)orgdiamond);
  diamond->frame = anim_setsequence(diamond->animate, 0, SEQ_FORCE);

  diamond->blood = (void *) malloc(sizeof(DIAMOND_BLOOD));
//  vgassert(diamond->blood);
  DIAMOND_BLOOD *diamondblood;

  diamondblood = (DIAMOND_BLOOD *)diamond->blood;
  diamondblood->sincnt     = 0;
  diamondblood->y          = y;
  diamondblood->glim = 5;
  object_add(diamond);
}


int diamond_live (OBJECT *object, UINT32 param)
{
  DIAMOND_BLOOD *diamondblood;

  diamondblood = (DIAMOND_BLOOD *)object->blood; /* in case of multiple diamonds */

  diamondblood->sincnt += 20;
  diamondblood->sincnt &= 1023;

  object->y = diamondblood->y + ((sinus512[(diamondblood->sincnt) & 1023] * 6) >> 8);

  if (object->collided)
    { 
      bonus_init(object->x, object->y, 1, 1);
      smlheart_init(object->x, object->y, 2, 1 + (7-player1.nrofblack));
//      player1.nrofblack--;

      return 1;
    }

  diamondblood->glim--;
  if (!diamondblood->glim)
  {
    diamondblood->glim = MC_RANDOM(250)+25;
    glim_init(object->x - 4, object->y + 12);
  }

  return 0;
}

void diamond_death(OBJECT *object, UINT32 param)
{
}

void diamond_clear(OBJECT *object, UINT32 param)
{
}



