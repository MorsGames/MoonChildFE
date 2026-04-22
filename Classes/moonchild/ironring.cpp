#include <frm_wrk.hpp>
#include <ironring.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void ironring_init(UINT16 x, UINT16 y, IRONRING_CB ironringrout,
     INT16 r1, INT16 r2, INT16 r3, INT16 r4)
{
  OBJECT *ironring;
  ironring = (OBJECT *) malloc (sizeof(OBJECT));
  ironring->x = x;
  ironring->y = y;
  ironring->live  = ironring_live;
  ironring->death = ironring_death;
  ironring->clear = ironring_clear;
  ironring->sizex = 80;
  ironring->sizey = 16;
  ironring->blitstartx = 0;
  ironring->blitstarty = 0;
  ironring->blitsizex = 80;
  ironring->blitsizey = 16;
  ironring->lethal = 0; //harmless

  ironring->animate = (ANIM *)copy_anim(orgironring);
  ironring->frame = anim_setsequence(ironring->animate, 0, SEQ_FORCE);

  ironring->blood = (void *) malloc(sizeof(IRONRING_BLOOD));
//  vgassert(ironring->blood);
  IRONRING_BLOOD *ironringblood;
  ironringblood = (IRONRING_BLOOD *)ironring->blood;
  ironringblood->x  = x;
  ironringblood->y  = y;
  ironringblood->childcol    = 0;
  ironringblood->ironringrout  = ironringrout;
  ironringblood->mx = x;
  ironringblood->my = y;
  ironringblood->r1 = r1;
  ironringblood->r2 = r2;
  ironringblood->r3 = r3;
  ironringblood->r4 = r4;
  object_add(ironring);
}


int ironring_live (OBJECT *object, UINT32 param)
{
  IRONRING_BLOOD *ironringblood;

  ironringblood = (IRONRING_BLOOD *)object->blood; /* in case of multiple ironrings */

  /* lets call the ironringor user-routine first */

  if (ironringblood->ironringrout) ironringblood->ironringrout(object);

//  ironringblood->oldx = object->x;
  ironringblood->oldy = object->y;


  object->x = ironringblood->x;
  object->y = ironringblood->y;

/* code for collision with hoi */
  
  if (ironringblood->childcol)
    {
//      hoi->x -= ironringblood->oldx - object->x;

      if (ironringblood->ironringrout != ironring_flipflapy)  //if it is flapy don't correct y coord
  {
    hoi->y -= ironringblood->oldy - object->y;
  }
    }

  if ((UINT16)(hoi->x - object->x + 32) < 80)   /* x collision */
    {
      if ((UINT16)(hoi->y - object->y + hoi->sizey) <32)   /* y collsion */
  {
    HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

    if (hoiblood->gravy > 0)
      {
        hoi->y = object->y - hoi->sizey;

        if (!ironringblood->childcol)
        {
          ironringblood->childcol = 1;
          if (hoiblood->gravy > 76 )
	    {
	      hoiblood->kneelcnt = (hoiblood->gravy-76) >> 1;
          }
        }

        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;

      }
    else
      {
        ironringblood->childcol = 0;
      }
  }
      else
  {
    ironringblood->childcol = 0;
  }
    }
  else
    {
      ironringblood->childcol = 0;
    }

  return 0;
}

void ironring_death(OBJECT *object, UINT32 param)
{
}

void ironring_clear(OBJECT *object, UINT32 param)
{
}


//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
void ironring_sinx(OBJECT *ironring)
{
  IRONRING_BLOOD *ironringblood;

  ironringblood = (IRONRING_BLOOD *)ironring->blood;

  ironringblood->r1 += ironringblood->r2;
  ironringblood->r1 &= 1023;

  ironringblood->x = ironringblood->mx + ((sinus512[ironringblood->r1]*
         ironringblood->r3)>>10);
}

//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
void ironring_siny(OBJECT *ironring)
{
  IRONRING_BLOOD *ironringblood;

  ironringblood = (IRONRING_BLOOD *)ironring->blood;

  ironringblood->r1 += ironringblood->r2;
  ironringblood->r1 &= 1023;

  ironringblood->y = ironringblood->my + ((sinus512[ironringblood->r1]*
         ironringblood->r3)>>10);
}

//r1 = x1
//r2 = x2
//r3 = delay
void ironring_flipflapx(OBJECT *ironring)
{
  IRONRING_BLOOD *ironringblood;

  ironringblood = (IRONRING_BLOOD *)ironring->blood;

  if (ironringblood->r3 == 32) play_pltfup (ironringblood->x, ironringblood->y);
  if (ironringblood->r3 == (ironringblood->r4-1)) play_pltfdwn(ironringblood->x, ironringblood->y);

  ironringblood->r3 -= 1;
  if (ironringblood->r3<32 && ironringblood->r3 > 1)
    {
      ironring->blitsizey = ironringblood->r3/2;
    }

  if (ironringblood->r3<ironringblood->r4 && ironringblood->r3 > ironringblood->r4-32)
    {
      ironring->blitsizey = ((ironringblood->r4 - ironringblood->r3)/2);
      if (ironring->blitsizey == 0) ironring->blitsizey = 1;
    }


  
  if (ironringblood->r3 == 0)
    {
      ironring->blitsizey = 1;
      ironringblood->r3 = ironringblood->r4;

      if (ironringblood->r1 == ironringblood->x)
      {
        ironringblood->x = ironringblood->r2;
        return; // (ironringblood->r2);
      }
      
      ironringblood->x = ironringblood->r1;
      return; // (ironringblood->r1);
    }

}


//r1 = y1
//r2 = y2
//r3 = delay
void ironring_flipflapy(OBJECT *ironring)
{
  IRONRING_BLOOD *ironringblood;

  ironringblood = (IRONRING_BLOOD *)ironring->blood;


  ironringblood->r3 -= 1;
  if (ironringblood->r3<32 && ironringblood->r3 > 1)
    {
      ironring->blitsizey = ironringblood->r3/2;
      return;
    }
  
  if (ironringblood->r3 == 0)
    {
      ironring->blitsizey = 16;
      ironringblood->r3 = ironringblood->r4;
      if (ironringblood->r1 == ironringblood->y)
  {
    ironringblood->y = ironringblood->r2;
    return; // (ironringblood->r2);
  }
      
      ironringblood->y = ironringblood->r1;
      return; // (ironringblood->r1);
    }
  else
    {
      return;
    }
}

//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
void ironring_sinxy(OBJECT *ironring)
{
  IRONRING_BLOOD *ironringblood;

  ironringblood = (IRONRING_BLOOD *)ironring->blood;

  ironringblood->r1 += ironringblood->r2;
  ironringblood->r1 &= 1023;

  ironringblood->x = ironringblood->mx + ((sinus512[(ironringblood->r1+512)&1023]*
         ironringblood->r3)>>10);
  ironringblood->y = ironringblood->my + ((sinus512[ironringblood->r1]*
         ironringblood->r3)>>10);
}









