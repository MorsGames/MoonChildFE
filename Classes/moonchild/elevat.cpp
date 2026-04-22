#include <frm_wrk.hpp>
#include <elevat.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

void elevat_init(UINT16 x, UINT16 y, ELEVAT_CB elevatrout,
     INT16 r1, INT16 r2, INT16 r3)
{
  OBJECT *elevat;
  elevat = (OBJECT *) malloc (sizeof(OBJECT));
  elevat->x = x;
  elevat->y = y;
  elevat->live  = elevat_live;
  elevat->death = elevat_death;
  elevat->clear = elevat_clear;

  elevat->lethal = 0; // harmless

  elevat->animate = (ANIM *)copy_anim(orgelevat);
  elevat->frame = anim_setsequence(elevat->animate, 0, SEQ_FORCE);

  elevat->sizex = elevat->frame->get_width();
  elevat->sizey = elevat->frame->get_height();
  elevat->blitstartx = 0;
  elevat->blitstarty = 0;
  elevat->blitsizex = 0;
  elevat->blitsizey = 0;

  elevat->blood = (void *) malloc(sizeof(ELEVAT_BLOOD));
//  vgassert(elevat->blood);
  ELEVAT_BLOOD *elevatblood;
  elevatblood = (ELEVAT_BLOOD *)elevat->blood;
  elevatblood->x  = x;
  elevatblood->y  = y;
  elevatblood->weightextra = 0;
  elevatblood->weightto    = 0;
  elevatblood->weightamp   = 64;
  elevatblood->weightcnt   = 0;
  elevatblood->childcol    = 0;
  elevatblood->elevatrout  = elevatrout;
  elevatblood->mx = x;
  elevatblood->my = y;
  elevatblood->r1 = r1;
  elevatblood->r2 = r2;
  elevatblood->r3 = r3;
  object_add(elevat);
}


int elevat_live (OBJECT *object, UINT32 param)
{
  ELEVAT_BLOOD *elevatblood;
  UINT16 compsize;


  elevatblood = (ELEVAT_BLOOD *)object->blood; /* in case of multiple elevats */

  /* lets call the elevator user-routine first */

  if (elevatblood->elevatrout) elevatblood->elevatrout(elevatblood);

  elevatblood->oldx = object->x;
  elevatblood->oldy = object->y;

/* code for weight and aftershock */

  elevatblood->weightcnt += 16;
  elevatblood->weightcnt &= 255;

  if (elevatblood->weightamp) elevatblood->weightamp -= 2;
  if (elevatblood->weightamp < 0) elevatblood->weightamp = 0;
  
  if (elevatblood->weightextra != elevatblood->weightto)
    {
      if (elevatblood->weightextra > elevatblood->weightto)
  {
    elevatblood->weightextra--;
  }
      else
  {
    elevatblood->weightextra++;
  }
    }

  object->x = elevatblood->x;
  object->y = elevatblood->y + ((sinus128[elevatblood->weightcnt]*
         elevatblood->weightamp)>>8) 
                           + elevatblood->weightextra;

/* code for collision with hoi */
  
  if (elevatblood->childcol)
    {
      hoi->x -= elevatblood->oldx - object->x;
      hoi->y -= elevatblood->oldy - object->y;
    }

  compsize = 16;     //usually the thickness of an elevator = 16... except at high speeds

  if (elevatblood->r2 > 16)
    {
      compsize = 32;
    }

  elevatblood->weightto = 0;
  if ((UINT16)(hoi->x - object->x + 32) < 80)   /* x collision */
    {
      if ((UINT16)(hoi->y - object->y + hoi->sizey) <32)   /* y collsion */
  {
    HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
    
    if (hoiblood->gravy > 0)
      {
        hoi->y = object->y - hoi->sizey;
        
        if (!elevatblood->childcol)
        {
          elevatblood->weightamp = 32 + (hoiblood->gravy/2);
          elevatblood->weightcnt = 0;
          elevatblood->childcol = 1;
	    play_graspod(object->x, object->y);
          if (hoiblood->gravy > 76 )
	    {
	      hoiblood->kneelcnt = (hoiblood->gravy-76) >> 1;
          }
        }
        
        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;
        
        elevatblood->weightto = 16;
      }
    else
      {
        elevatblood->childcol = 0;
      }
  }
      else
  {
    elevatblood->childcol = 0;
  }
    }
  else
    {
      elevatblood->childcol = 0;
    }

  return 0;
}

void elevat_death(OBJECT *object, UINT32 param)
{
}

void elevat_clear(OBJECT *object, UINT32 param)
{
}


//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
void elevat_sinx(ELEVAT_BLOOD *elevatblood)
{
  elevatblood->r1 += elevatblood->r2;
  elevatblood->r1 &= 1023;

  elevatblood->x = elevatblood->mx + ((sinus512[elevatblood->r1]*
         elevatblood->r3)>>10);
}

//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
void elevat_siny(ELEVAT_BLOOD *elevatblood)
{
  elevatblood->r1 += elevatblood->r2;
  elevatblood->r1 &= 1023;

  elevatblood->y = elevatblood->my + ((sinus512[elevatblood->r1]*
         elevatblood->r3)>>10);
}



//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
void elevat_sinxy(ELEVAT_BLOOD *elevatblood)
{
  elevatblood->r1 += elevatblood->r2;
  elevatblood->r1 &= 1023;

  elevatblood->x = elevatblood->mx + ((sinus512[elevatblood->r1]*
         elevatblood->r3)>>10);

  elevatblood->y = elevatblood->my + ((sinus512[(elevatblood->r1 + 256) & 1023]*
         elevatblood->r3)>>10);
}








