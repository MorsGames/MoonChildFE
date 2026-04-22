#include <frm_wrk.hpp>
#include <mushroom.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void mushroom_init(UINT16 x, UINT16 y, UINT16 swingspd, UINT16 swingamp, UINT16 startheight, UINT16 endheight)
{
  OBJECT *mushroom;
  UINT16 i;

  mushroom = (OBJECT *) malloc (sizeof(OBJECT));
  mushroom->x = x;
  mushroom->y = y;
  mushroom->live  = mushroom_live;
  mushroom->death = mushroom_death;
  mushroom->clear = mushroom_clear;
  mushroom->blitsizex = 0;
  mushroom->blitsizey = 0;

  mushroom->animate = copy_anim(orgmushroom);

  mushroom->frame   = anim_setsequence(mushroom->animate, 1, SEQ_FORCE);

  mushroom->sizex = mushroom->frame->get_width();
  mushroom->sizey = mushroom->frame->get_height();

  mushroom->lethal = 0; // not lethal
  mushroom->coloffsetx = 15;
  mushroom->coloffsety = 0;
  mushroom->colwidth  = 2;
  mushroom->colheight = mushroom->sizey - 8;
  
  mushroom->blood = (void *) malloc(sizeof(MUSHROOM_BLOOD));
//  vgassert(mushroom->blood);
  MUSHROOM_BLOOD *mushroomblood;
  mushroomblood = (MUSHROOM_BLOOD *)mushroom->blood;

  mushroomblood->x = x;
  mushroomblood->y = y;
  mushroomblood->childcol = 0;
  mushroomblood->oldchildcol = 0;
  mushroomblood->oldx = x;
  mushroomblood->oldy = y;
  mushroomblood->swingswingcnt = 0;
  mushroomblood->swingswingspd = swingspd;
  mushroomblood->swingswingamp = swingamp;
  mushroomblood->startheight = startheight;
  mushroomblood->endheight = endheight;
  mushroomblood->curheight = startheight;
  
  for (i=0; i<64; i++)
  {
    mushroomblood->swingcnt[i] = 0;
  }

  object_add(mushroom);

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
      element->animate = copy_anim(orgmushroom);
      element->frame   = anim_setsequence(element->animate, 1, SEQ_FORCE);

      element->sizex = element->frame->get_width();
      element->sizey = element->frame->get_height();
    }
    else
    {
      element->animate = copy_anim(orgmushroom);
      element->frame   = anim_setsequence(element->animate, 0, SEQ_FORCE);
      
      element->sizex = element->frame->get_width();
      element->sizey = element->frame->get_height();
    }

    element->blood = 0;

    object_add(element);
    mushroomblood->elements[i] = element;
  }

  return;
}


int mushroom_live (OBJECT *object, UINT32 param)
{

  UINT16 i;
  UINT32 x,y;

  MUSHROOM_BLOOD *mushroomblood;
  mushroomblood = (MUSHROOM_BLOOD *)object->blood; /* in case of multiple mushrooms */

  mushroomblood->swingswingcnt += mushroomblood->swingswingspd;
  mushroomblood->swingswingcnt &= 1023;

  for (i=0; i<63; i++)
  {
    mushroomblood->swingcnt[i] = mushroomblood->swingcnt[i+1];
  }

  mushroomblood->swingcnt[63] = ((sinus512[mushroomblood->swingswingcnt]* mushroomblood->swingswingamp) >> 10);
  
  if (mushroomblood->oldchildcol != mushroomblood->childcol)
  {
    if (mushroomblood->childcol == 1) play_mushup (object->x, object->y);
    if (mushroomblood->childcol == 0) play_mushdwn(object->x, object->y);
    mushroomblood->oldchildcol = mushroomblood->childcol;
  }

  x = object->x<<10;
  y = object->y<<10;

  for (i=0; i<16; i++)
  {
    x = ((sinus512[ (mushroomblood->swingcnt[60 - (i*4)]+512)     &1023] * (mushroomblood->curheight)) )+x;
    y = ((sinus512[ (mushroomblood->swingcnt[60 - (i*4)]+256+512) &1023] * (mushroomblood->curheight)) )+y;
    mushroomblood->elements[i]->x = x>>10;
    mushroomblood->elements[i]->y = y>>10;
  }

  mushroomblood->elements[15]->x -= mushroomblood->elements[15]->sizex/2 - 8;

/* code for collision with hoi */

  object = mushroomblood->elements[15];  // ****** LOOK OUT CHANGING OBJECT!

  if (mushroomblood->childcol)
    {
      hoi->x -= mushroomblood->oldx - object->x;
      hoi->y -= mushroomblood->oldy - object->y;
    }

  mushroomblood->oldx = object->x;
  mushroomblood->oldy = object->y;
  
  if ((UINT16)(hoi->x - object->x + 32) < 88)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + hoi->sizey - 8) <16)   /* y collsion */
    {
      HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

      if (hoiblood->gravy > 0)
      {
        hoi->y = object->y - hoi->sizey + 8;

        if (!mushroomblood->childcol)
        {
          mushroomblood->childcol = 1;
        }

        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;

      }
      else
      {
        mushroomblood->childcol = 0;
      }
    }
    else
    {
      mushroomblood->childcol = 0;
    }
  }
  else
  {
    mushroomblood->childcol = 0;
  }

  if (mushroomblood->childcol)  // MC collisionde met mushroom
    {
      if (mushroomblood->curheight != mushroomblood->endheight)
	  {
	    if (mushroomblood->startheight < mushroomblood->endheight)
	    {
	      mushroomblood->curheight++;
	    }
	    else
	    {
	      mushroomblood->curheight--;
	    }
	  }
    }
  else
    {
      if (mushroomblood->curheight != mushroomblood->startheight)
	  {
	    if (mushroomblood->startheight < mushroomblood->endheight)
	    {
	      mushroomblood->curheight--;
	    }
	    else
	    {
	      mushroomblood->curheight++;
	    }
	  }
    }

  return 0;
}

void mushroom_death(OBJECT *object, UINT32 param)
{
}

void mushroom_clear(OBJECT *object, UINT32 param)
{
}


