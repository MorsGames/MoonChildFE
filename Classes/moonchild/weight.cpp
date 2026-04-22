#include <frm_wrk.hpp>
#include <weight.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

unsigned char weightstuitertab[] = {
//     66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
//     76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
//     86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
//     96, 97, 98, 99,100,101,102,103,104,105,
//    106,107,108,109,110,111,112,113,114,115,
//    116,117,118,119,120,121,122,123,124,125,
//    126,127,128,129,130,131,132,133,134,135,
//    136,137,138,139,140,141,142,143,144,145,
//    146,147,148,149,150,151,152,153,154,155,
//    156,157,158,159,160,160,160,160,160,160,
//    160,160,160,160,160,160,160,160,160,160,
//    159,157,154,151,147,142,136,129,121,112,
//    102, 91, 79, 66, 66, 66, 66, 66, 66, 66,
//     66, 66, 66, 66, 66, 66

    160,160,160,160,160,160,160,160,160,160,
    160,160,160,160,160,160,160,160,160,160,
    160,160,160,160,160,160,160,160,160,160,
    160,159,158,157,156,155,154,153,152,151,
    150,149,148,147,146,145,144,143,142,141,
    140,139,138,137,136,135,134,133,132,131,
    130,129,128,127,126,125,124,123,122,121,
    120,119,118,117,116,115,114,113,112,111,
    110,109,108,107,106,105,104,103,102,101,
    100, 99, 98, 97, 96, 95, 94, 93, 92, 91,
     90, 89, 88, 87, 86, 85, 84, 83, 82, 81,
     80, 79, 78, 77, 76, 75, 74, 73, 72, 71,
     70, 69, 68, 67, 66, 66, 66, 66, 66, 66,
     66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
     66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
     66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
     66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
     67, 69, 72, 76, 81, 87, 94,102,111,121,
    132,144
    };



void weight_init(UINT16 x, UINT16 y, UINT16 startpos, UINT16 speed)
{
  OBJECT *weight;
  weight = (OBJECT *) malloc (sizeof(OBJECT));
  weight->x = x;
  weight->y = y;
  weight->live  = weight_live;
  weight->death = weight_death;
  weight->clear = weight_clear;

  weight->animate = copy_anim((ANIM *)orgweight);
  weight->frame = anim_setsequence(weight->animate, 0, SEQ_FORCE);

  weight->lethal = 0; //harmless    we do the collision internally!

  weight->sizex = weight->frame->get_width();
  weight->sizey = weight->frame->get_height();
  weight->blitstartx = 0;
  weight->blitstarty = 0;
  weight->blitsizex = weight->sizex;
  weight->blitsizey = weight->sizey;

  weight->blood = (void *) malloc(sizeof(WEIGHT_BLOOD));
//  vgassert(weight->blood);
  WEIGHT_BLOOD *weightblood;

  weightblood = (WEIGHT_BLOOD *)weight->blood;

  weightblood->stuit = startpos;
  weightblood->speed = speed;

  object_add(weight);
  return;
}


int weight_live (OBJECT *object, UINT32 param)
{
  WEIGHT_BLOOD *weightblood;

  weightblood = (WEIGHT_BLOOD *)object->blood;

  object->blitsizey   = weightstuitertab[weightblood->stuit];
  object->blitstarty  = object->sizey - weightstuitertab[weightblood->stuit];

  weightblood->stuit += weightblood->speed;

  if (weightblood->stuit > (sizeof(weightstuitertab)-12))
  {
    object->lethal = 2;
    object->coloffsetx = 20;
    object->coloffsety = 0;
    object->colwidth   = 36;
    object->colheight  = object->blitsizey;
  }
  else
  {
    object->lethal = 0;
  }

  if (weightblood->stuit == sizeof(weightstuitertab))
  {
    weightblood->stuit = 0;
    play_gewicht(object->x, object->y);
    if ((UINT16) (player1.worldy - object->y + 400) < 800)
    {
      if ((UINT16) (player1.worldx - object->x + 640) < 740)
      {
	  player1.shakey = -16;
      }
    }
  }
  
  return 0;
}

void weight_death(OBJECT *object, UINT32 param)
{
}

void weight_clear(OBJECT *object, UINT32 param)
{
}



