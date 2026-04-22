#include <frm_wrk.hpp>
#include <gumbal.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>

INT16 gumbalheight[22] = {0,0,0,0,0,-32,-56,-80,-102,-114,-120,
                           -120,-114,-102,-80,-56,-32,0,0,0,0,0};
UINT16 gumbalframe[22]  = {0,1,2,3,4,5,6,7,8,9,10,
                           10,9,8,7,6,5,4,3,2,1,0};


void gumbal_init(UINT16 x, UINT16 y, UINT16 framenr)
{
  OBJECT *gumbal;
  gumbal = (OBJECT *) malloc (sizeof(OBJECT));
  gumbal->x = x;
  gumbal->y = y;
  gumbal->live  = gumbal_live;
  gumbal->death = gumbal_death;
  gumbal->clear = gumbal_clear;
  gumbal->blitsizex = 0;
  gumbal->blitsizey = 0;

  gumbal->lethal = 1; // energy drain

  gumbal->animate = copy_anim(orggumbal);

  gumbal->frame   = anim_setsequence(gumbal->animate, 0, SEQ_FORCE);

  gumbal->sizex = gumbal->frame->get_width();
  gumbal->sizey = gumbal->frame->get_height();

  gumbal->coloffsetx = 20;
  gumbal->coloffsety = 20;
  gumbal->colwidth   = 40;
  gumbal->colheight  = 32;
  
  gumbal->blood = (void *) malloc(sizeof(GUMBAL_BLOOD));
//  vgassert(gumbal->blood);
  GUMBAL_BLOOD *gumbalblood;
  gumbalblood = (GUMBAL_BLOOD *)gumbal->blood;

  gumbalblood->framenr = framenr;
  gumbalblood->framedelay = 3;
  gumbalblood->y = y;
  object_add(gumbal);

  return;
}


int gumbal_live (OBJECT *object, UINT32 param)
{

  GUMBAL_BLOOD *gumbalblood;
  gumbalblood = (GUMBAL_BLOOD *)object->blood; /* in case of multiple gumbals */

  gumbalblood->framedelay--;
  if (!gumbalblood->framedelay)
  {
    gumbalblood->framedelay = 3;
    gumbalblood->framenr++;
    if (gumbalblood->framenr == 22) gumbalblood->framenr = 0;

    if (gumbalblood->framenr == 18)
      {
  play_ball(object->x,object->y);
      }
  }

  object->y = gumbalblood->y + gumbalheight[gumbalblood->framenr];
  object->frame = anim_forceframe(object->animate,gumbalframe[gumbalblood->framenr]);

  return 0;
}

void gumbal_death(OBJECT *object, UINT32 param)
{
}

void gumbal_clear(OBJECT *object, UINT32 param)
{
}


