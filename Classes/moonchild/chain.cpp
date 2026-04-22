#include <frm_wrk.hpp>
#include <chain.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void chain_init(UINT16 x, UINT16 y, UINT16 hookflg, UINT16 swingspd, UINT16 swingamp)
{
  OBJECT *chain;
  UINT16 i;

  chain = (OBJECT *) malloc (sizeof(OBJECT));
  chain->x = x;
  chain->y = y;
  chain->live  = chain_live;
  chain->death = chain_death;
  chain->clear = chain_clear;
  chain->blitsizex = 0;
  chain->blitsizey = 0;

  chain->animate = copy_anim(orgchainbal);

  chain->frame   = anim_setsequence(chain->animate, 0, SEQ_FORCE);

  chain->sizex = chain->frame->get_width();
  chain->sizey = chain->frame->get_height();

  chain->lethal = 0; // not lethal
  chain->coloffsetx = 15;
  chain->coloffsety = 0;
  chain->colwidth  = 2;
  chain->colheight = chain->sizey - 8;
  
  chain->blood = (void *) malloc(sizeof(CHAIN_BLOOD));
//  vgassert(chain->blood);
  CHAIN_BLOOD *chainblood;
  chainblood = (CHAIN_BLOOD *)chain->blood;

  chainblood->x = x;
  chainblood->y = y;
  chainblood->childcol = 0;
  chainblood->oldx = x;
  chainblood->oldy = y;
  chainblood->hookflg = hookflg;
  chainblood->swingswingcnt = 0;
  chainblood->swingswingspd = swingspd;
  chainblood->swingswingamp = swingamp;
  
  for (i=0; i<64; i++)
  {
    chainblood->swingcnt[i] = 0;
  }

  for (i=0; i<16; i++)
  {
    OBJECT *element;

    element = (OBJECT *) malloc (sizeof(OBJECT));
    element->x = x;
    element->y = y;
    element->live  = 0;
    element->death = 0;
    element->clear = 0;
    element->blitsizex = 0;
    element->blitsizey = 0;

    element->lethal = 0; // not lethal

    if (i!=15)
    {
      element->animate = copy_anim(orgchainbal);
      element->frame   = anim_setsequence(element->animate, 0, SEQ_FORCE);

      element->sizex = element->frame->get_width();
      element->sizey = element->frame->get_height();
    }
    else
    {
      if (hookflg)
      {
        element->animate = copy_anim(orghook);
        element->lethal = 1;                   // wel lethal

        element->frame   = anim_setsequence(element->animate, 0, SEQ_FORCE);

        element->sizex = element->frame->get_width();
        element->sizey = element->frame->get_height();

        element->coloffsetx = 4;
        element->coloffsety = 4;
        element->colwidth  = element->sizex - 8;
        element->colheight = element->sizey - 8;
      }
      else
      {
        element->animate = copy_anim(orgbiels);
        element->frame   = anim_setsequence(element->animate, 0, SEQ_FORCE);

        element->sizex = element->frame->get_width();
        element->sizey = element->frame->get_height();
      }
    }

    element->blood = 0;

    object_add(element);
    chainblood->elements[i] = element;
  }

  object_add(chain);

  return;
}


int chain_live (OBJECT *object, UINT32 param)
{

  UINT16 i;
  UINT32 x,y;

  CHAIN_BLOOD *chainblood;
  chainblood = (CHAIN_BLOOD *)object->blood; /* in case of multiple chains */

  chainblood->swingswingcnt += chainblood->swingswingspd;
  chainblood->swingswingcnt &= 1023;

  for (i=0; i<63; i++)
  {
    chainblood->swingcnt[i] = chainblood->swingcnt[i+1];
  }

  chainblood->swingcnt[63] = ((sinus512[chainblood->swingswingcnt]* chainblood->swingswingamp) >> 10);
  

  x = object->x<<10;
  y = object->y<<10;

  for (i=0; i<16; i++)
  {
    x = ((sinus512[ (chainblood->swingcnt[60 - (i*4)])     &1023] * (32)) )+x;
    y = ((sinus512[ (chainblood->swingcnt[60 - (i*4)]+256) &1023] * (32)) )+y;
    chainblood->elements[i]->x = x>>10;
    chainblood->elements[i]->y = y>>10;
  }


  if (((chainblood->swingswingcnt==296 || chainblood->swingswingcnt==297 || chainblood->swingswingcnt==298) || (chainblood->swingswingcnt==808 || chainblood->swingswingcnt==809 || chainblood->swingswingcnt==810)) )
  {
      play_ketting(object->x, object->y);
  }

  chainblood->elements[15]->x -= chainblood->elements[15]->sizex/2 - 8;

/* code for collision with hoi */

  object = chainblood->elements[15];  // ****** LOOK OUT CHANGING OBJECT!

  if (chainblood->childcol)
    {
      hoi->x -= chainblood->oldx - object->x;
      hoi->y -= chainblood->oldy - object->y;
    }

  chainblood->oldx = object->x;
  chainblood->oldy = object->y;
  
  if ((UINT16)(hoi->x - object->x + 32) < ((chainblood->hookflg) ? 40 : 112))   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + hoi->sizey) <16)   /* y collsion */
    {
      HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

      if (hoiblood->gravy > 0)
      {
        hoi->y = object->y - hoi->sizey;

        if (!chainblood->childcol)
        {
          chainblood->childcol = 1;
        }

        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;

      }
      else
      {
        chainblood->childcol = 0;
      }
    }
    else
    {
      chainblood->childcol = 0;
    }
  }
  else
  {
    chainblood->childcol = 0;
  }

  return 0;
}

void chain_death(OBJECT *object, UINT32 param)
{
}

void chain_clear(OBJECT *object, UINT32 param)
{
}


