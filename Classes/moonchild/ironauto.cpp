#include <frm_wrk.hpp>
#include <ironauto.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

OBJECT *ironauto_init(UINT16 x, UINT16 y, UINT16 *movtab, UINT16 movflg, OBJECT *koppellift, UINT16 hyprsndflg)
{
  OBJECT *ironauto;
  ironauto = (OBJECT *) malloc (sizeof(OBJECT));
  ironauto->x = x;
  ironauto->y = y;
  ironauto->live  = ironauto_live;
  ironauto->death = ironauto_death;
  ironauto->clear = ironauto_clear;
  ironauto->sizex = 80;
  ironauto->sizey = 16;
  ironauto->blitstartx = 0;
  ironauto->blitstarty = 0;
  ironauto->blitsizex = 160;
  ironauto->blitsizey = 16;
  ironauto->lethal = 0; //harmless

  ironauto->animate = (ANIM *)copy_anim(orgironauto);
  ironauto->frame = anim_setsequence(ironauto->animate, 0, SEQ_FORCE);

  ironauto->blood = (void *) malloc(sizeof(IRONAUTO_BLOOD));
//  vgassert(ironauto->blood);
  IRONAUTO_BLOOD *ironautoblood;
  ironautoblood = (IRONAUTO_BLOOD *)ironauto->blood;
  ironautoblood->x        = x;
  ironautoblood->y        = y;
  ironautoblood->childcol    = 0;
  ironautoblood->movtab = movtab;
  ironautoblood->movpos = 0;
  ironautoblood->movdel = movtab[1]+1;
  ironautoblood->movflg = movflg;
  ironautoblood->onliftdel = 100;
  ironautoblood->koppellift = koppellift;
  ironautoblood->hyprsndflg = hyprsndflg;

  ironautoblood->dirx  = ((INT16)ironautoblood->movtab[ironautoblood->movpos]) >> 8;
  ironautoblood->diry  = (INT16)((INT8)((INT16)ironautoblood->movtab[ironautoblood->movpos]));

  object_add(ironauto);
  return ironauto;
}


int ironauto_live (OBJECT *object, UINT32 param)
{
  IRONAUTO_BLOOD *ironautoblood;
  UINT16 xgrootte;

  ironautoblood = (IRONAUTO_BLOOD *)object->blood; /* in case of multiple ironautos */

  if (ironautoblood->movflg == 2)   // if 2 then he's a slave of the kopple lift
  {
      return 0;
  }
//  ironautoblood->oldx = object->x;
  ironautoblood->oldy = object->y;

  if ((ironautoblood->movflg && ironautoblood->childcol) || (ironautoblood->movflg == 0) )
  {
    if (ironautoblood->movdel)
    {
      ironautoblood->movdel--;
      if (!ironautoblood->movdel)
      {
          ironautoblood->movpos += 2;
          if (ironautoblood->movtab[ironautoblood->movpos+1] == 0)
          {
                ironautoblood->movpos = 0;
          }

		  ironautoblood->movdel = ironautoblood->movtab[ironautoblood->movpos+1];
          ironautoblood->dirx  = ((INT16)ironautoblood->movtab[ironautoblood->movpos]) >> 8;
          ironautoblood->diry  = (INT16)((INT8)((INT16)ironautoblood->movtab[ironautoblood->movpos]));

          if ((ironautoblood->dirx != 00 || ironautoblood->diry != 00) && ironautoblood->hyprsndflg == 1) play_hyprlift(object->x,object->y);

	  }
    }
    object->x += ironautoblood->dirx;
    object->y += ironautoblood->diry;
    ironautoblood->onliftdel = 100;
    object->blitsizey = 16;

    if (ironautoblood->koppellift)
    {
      IRONAUTO_BLOOD *ironautoblood2;
      ironautoblood->koppellift->y = object->y;
      ironautoblood2 = (IRONAUTO_BLOOD *) ironautoblood->koppellift->blood;
      ironautoblood2->onliftdel = 100;
    }
  }
  

  
  ironautoblood->onliftdel--;

  if (ironautoblood->onliftdel < 16 && ironautoblood->onliftdel != 0 && (object->x != ironautoblood->x || object->y != ironautoblood->y))
  {
    object->blitsizey = ironautoblood->onliftdel;
    if (ironautoblood->koppellift) ironautoblood->koppellift->blitsizey = ironautoblood->onliftdel;
  }
  
  if (!ironautoblood->onliftdel)
  {
    object->x = ironautoblood->x;
    object->y = ironautoblood->y;
    object->blitsizey = 16;

    if (ironautoblood->koppellift) ironautoblood->koppellift->blitsizey = 16;
    if (ironautoblood->koppellift) ironautoblood->koppellift->y = ironautoblood->y;

    ironautoblood->onliftdel = 100;
    ironautoblood->movpos = 0;
    ironautoblood->movdel = ironautoblood->movtab[1]+1;

    ironautoblood->dirx  = ((INT16)ironautoblood->movtab[ironautoblood->movpos]) >> 8;
    ironautoblood->diry  = (INT16)((INT8)((INT16)ironautoblood->movtab[ironautoblood->movpos]));
  }

  xgrootte = 162;

  if (ironautoblood->koppellift) xgrootte = 322;
/* code for collision with hoi */
  
  if (ironautoblood->childcol)
    {
//      hoi->x -= ironautoblood->oldx - object->x;

      hoi->y -= ironautoblood->oldy - object->y;
    }

  if (ironautoblood->hyprsndflg == 0)
  {
     
	INT32 distxl,distyl;
	INT32 distxr,distyr;

    distxl = abs(hoi->x - (object->x-64));
	distyl = abs(hoi->y - object->y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (object->x+64));
	distyr = abs(hoi->y - object->y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	prox_slowlift_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume

  }


  if ((UINT16)(hoi->x - object->x + 32) < xgrootte)   /* x collision */
    {
      if ((UINT16)(hoi->y - object->y + hoi->sizey) <32)   /* y collision */
  {
    HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;

    if (hoiblood->gravy > 0)
      {
        hoi->y = object->y - hoi->sizey;

        if (!ironautoblood->childcol)
        {
          ironautoblood->childcol = 1;
          if (hoiblood->gravy > 76 )
	      {
	        hoiblood->kneelcnt = (hoiblood->gravy-76) >> 1;
          }
          if(ironautoblood->hyprsndflg == 1) play_hyprlift(object->x,object->y);
        }

        hoiblood->gravy = 0;
        hoiblood->onfloor = 2;
        hoiblood->onobject = 1;

      }
    else
      {
        ironautoblood->childcol = 0;
      }
  }
      else
  {
    ironautoblood->childcol = 0;
  }
    }
  else
    {
      ironautoblood->childcol = 0;
    }

  return 0;
}

void ironauto_death(OBJECT *object, UINT32 param)
{
}

void ironauto_clear(OBJECT *object, UINT32 param)
{
}


