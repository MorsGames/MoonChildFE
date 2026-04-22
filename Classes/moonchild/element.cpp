#include <frm_wrk.hpp>
#include <element.hpp>
#include <plof.hpp>
#include <diamond.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>



// If forced seq is NOT SEQ_DUMMY the anim-sequence will be forced

void element_init(UINT16 x, UINT16 y, INT16 spdx, INT16 spdy,
               ELEMENT_CB elementroutx, INT16 x1,  INT16 x2,  INT16 x3,
               ELEMENT_CB elementrouty, INT16 y1,  INT16 y2,  INT16 y3,
               ELEMENT_CB hoverrouty, INT16 hy1, INT16 hy2, INT16 hy3,
               ANIM *anim, UINT16 anmspd, UINT16 movdelay, UINT16 forcedseq, UINT16 bulletflg, UINT16 destroyable,
							 WAV_FN wavrout, UINT16 rnddelay, UINT16 rnddomain, WAV2_FN proximity)
{
  OBJECT *element;
  element = (OBJECT *) malloc (sizeof(OBJECT));
  element->x = x;
  element->y = y;
  element->live  = element_live;
  element->death = element_death;
  element->clear = element_clear;
  element->lethal = 1;     // energy drain

  element->animate = copy_anim(anim);
//  vgassert(element->animate);
  element->frame = anim_setsequence(element->animate, 0, SEQ_FORCE);

  element->sizex = element->frame->get_width();;
  element->sizey = element->frame->get_height();;
  element->blitstartx = 0;
  element->blitstarty = 0;
  element->blitsizex = 0;
  element->blitsizey = 0;

  element->coloffsetx = element->sizex/4;
  element->coloffsety = element->sizey/4;
  element->colwidth = element->sizex/2;
  element->colheight = element->sizey/2;


  element->blood = (void *) malloc(sizeof(ELEMENT_BLOOD));
//  vgassert(element->blood);
  ELEMENT_BLOOD *elementblood;
  elementblood = (ELEMENT_BLOOD *)element->blood;
  elementblood->x  = x;
  elementblood->y  = y;
  elementblood->spdx  = spdx;
  elementblood->spdy  = spdy;
  elementblood->timeout = 150;
  elementblood->moveroutx = elementroutx;
  elementblood->x1 = x1;
  elementblood->x2 = x2;
  elementblood->x3 = x3;
  elementblood->moverouty = elementrouty;
  elementblood->y1 = y1;
  elementblood->y2 = y2;
  elementblood->y3 = y3;
  elementblood->hoverrouty = hoverrouty;
  elementblood->hy1 = hy1;
  elementblood->hy2 = hy2;
  elementblood->hy3 = hy3;
  elementblood->forcedseq = forcedseq;
  elementblood->anmspd = anmspd;
  elementblood->curmov = 0;
  elementblood->movdelay = movdelay;
  elementblood->bulletflg = bulletflg;
  elementblood->destroyable = destroyable;

  elementblood->rnddelay = rnddelay;
  elementblood->rnddomain = rnddomain;
  elementblood->curdelay = 10;
  elementblood->wav_fn = wavrout;
  elementblood->wav2_fn = proximity;

  totalgenercnt++;

  object_add(element);
}

int element_live (OBJECT *object, UINT32 param)
{
  ELEMENT_BLOOD *elementblood;
  UINT16       oldx,i;

  elementblood = (ELEMENT_BLOOD *)object->blood; /* in case of multiple elements */

  elementblood->x += elementblood->spdx;
  elementblood->y += elementblood->spdy;
  object->x += elementblood->spdx;
  object->y += elementblood->spdy;
  
  if ((elementblood->x < (player1.worldx -300)) || (elementblood->y < (player1.worldy -300)) || (elementblood->x > (player1.worldx +1000)) || (elementblood->y > (player1.worldy +1000)))
  {
	elementblood->timeout--;
	if (elementblood->timeout == 0)
	{
		return 1;
	}
  }
  else 
  {
	elementblood->timeout = 225;
  }
  
  for (i=0; i<elementblood->anmspd; i++)
  {
    object->frame = anim_nextframe(object->animate);
  }

  elementblood->curmov++;
  if (elementblood->curmov > elementblood->movdelay)
  {
    elementblood->curmov = 0;
  
    oldx = object->x;
    if (elementblood->moveroutx)
    {
      object->x = elementblood->moveroutx((INT16 *) &elementblood->x, &elementblood->x1, &elementblood->x2, &elementblood->x3);
    }

    object->y = elementblood->y;

    if (elementblood->moverouty)
    {
      object->y = elementblood->moverouty((INT16 *) &elementblood->y, &elementblood->y1, &elementblood->y2, &elementblood->y3);
    }

    if (elementblood->hoverrouty)
    {
      object->y = elementblood->moverouty((INT16 *) &object->y, &elementblood->hy1, &elementblood->hy2, &elementblood->hy3);
    }

    if (elementblood->forcedseq == SEQ_DUMMY)
    {
      if (oldx != object->x) // bewogen?
      { 
        if (oldx < object->x) //naar rechts?
        {
          anim_setsequence(object->animate, SEQ_FLYRIGHT, SEQ_NOFORCE);
        }
        else
        {
          anim_setsequence(object->animate, SEQ_FLYLEFT, SEQ_NOFORCE);
        }
      }
    }
    else
    {
      anim_setsequence(object->animate, elementblood->forcedseq, SEQ_NOFORCE);
    }
  }

	if (elementblood->wav_fn && object->visible)
	{
		elementblood->curdelay--;
		if (!elementblood->curdelay)
		{
			elementblood->curdelay = MC_RANDOM(elementblood->rnddomain) + elementblood->rnddelay;
			elementblood->wav_fn();
		}
	}

	if (elementblood->wav2_fn)
	{
		INT32 distxl,distyl;
		INT32 distxr,distyr;

		
		distxl = abs(hoi->x - (object->x-64));
		distyl = abs(hoi->y - object->y);
		if (distxl < distyl) distxl = distyl;    // de verste distance wint

		distxr = abs(hoi->x - (object->x+64));
		distyr = abs(hoi->y - object->y);
		if (distxr < distyr) distxr = distyr;    // de verste distance wint

		elementblood->wav2_fn(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
	}
  
	if (elementblood->bulletflg)
    {
      if (bullet_check(object))
	  {
        if (elementblood->destroyable)
		{
          elementblood->destroyable--;
		  if (!elementblood->destroyable)
	      {
	        plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 0, 0, 0, 0, 0);
   		    totalshootcnt++;

//			log_out("gen(%d) - shoot(%d) ",totalgenercnt,totalshootcnt);
            
			if(totalshootcnt == totalgenercnt)
			{
// hard coded level exceptions...

				if (player1.worldx > 6100 && ((diamcnt&4) == 0))
				{				
 				  diamond_init( object->x+(object->sizex/2), object->y+(object->sizey/2));
                  diamcnt|=4;
				}

				if (player1.worldx < 3100 && player1.worldx > 1700 && ((diamcnt&2) == 0))
				{				
 				  diamond_init( object->x+(object->sizex/2), object->y+(object->sizey/2));
                  diamcnt|=2;
				}

				if (player1.worldx < 1706 && ((diamcnt&1) == 0))
				{				
 				  diamond_init( object->x+(object->sizex/2), object->y+(object->sizey/2));
                  diamcnt|=1;
				}
		    }
			return 1;
	      }
	    else
	      {
	        plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 0, 0, 0, 0);
	      }
		}
	    else
        {
          plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 0, 0, 0, 0);
        }
	  }
    }      


	if (hoi->frame == 0) // hoi is dood... dus object vernietigt zichzelf
	{
      plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 0, 0, 0, 0, 0);
      totalshootcnt++;
	  return 1;
	}

  return 0;
}

void element_death(OBJECT *object, UINT32 param)
{
}

void element_clear(OBJECT *object, UINT32 param)
{
}

//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
UINT16 element_sin(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
{
  *(r1) += *(r2);
  *(r1) &= 1023;

  return (*(pos) + ((sinus512[*(r1)]**(r3))>>10));
}

//r1 = minx
//r2 = maxx
//r3 = speed
UINT16 element_pingpong(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
{
  *(pos) += *(r3);
  
  if (*(r3)>0)
  {
    if (*(r2) < *(pos)) *(r3) = -*(r3);
  }
  else
  {
    if (*(r1) > *(pos)) *(r3) = -*(r3);
  }

  return (*(pos));
}

//r1 = x1
//r2 = x2
//r3 = delay
UINT16 element_flipflap(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
{
  *(r3) -= 1;
  if (*(r3)) return (*(pos));
  
  *(r3) = 120;

  if (*(r1) == *(pos))
  {
    *(pos) = *(r2);
    return (*(r2));
  }

  *(pos) = *(r1);
  return (*(r1));
}


//r1 = yrestart
//r2 = gravy
//r3 = spd
UINT16 element_gravy(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
{
  *(pos) += *(r2)>>6;
  *(r2)  += *(r3);

  if (*(pos) > *(r1)+2000)
  {
    *(pos) = *(r1);
    *(r2)  = 0;
  }

  return (*(pos));
}



//r1 = checkx
//r2 = restartx
//r3 = speed
UINT16 element_flyby(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
{
  *(pos) += *(r3);
  
  if (*(r3)>0)
  {
    if (*(r1) < *(pos)) *(pos) = *(r2);
  }
  else
  {
    if (*(r1) > *(pos)) *(pos) = *(r2);
  }

  return (*(pos));
}







