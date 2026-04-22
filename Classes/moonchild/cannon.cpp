#include <frm_wrk.hpp>
#include <cannon.hpp>
#include <mine.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <plof.hpp>
#include <sneak.hpp>


void cannon_init(UINT16 x, UINT16 y, UINT16 shootrate, UINT16 shootspeed, UINT16 bulkflg,
		 ANIM *mineanim, UINT16 seqnr, UINT16 lifespan, UINT16 exploflg, UINT16 moveinflg, UINT16 cloudflg,
		 BASIC_CB basicroutx, INT16 x1,  INT16 x2,  INT16 x3,
		 BASIC_CB basicrouty, INT16 y1,  INT16 y2,  INT16 y3,
		 BASIC_CB hoverrouty, INT16 hy1, INT16 hy2, INT16 hy3,
		 UINT16 sinamp, UINT16 sinspd, UINT16 hitcnt, OBJECT_CB hitrout, WAV2_FN wav2_fn, UINT16 sneakflg
		 )
{
  OBJECT *cannon;
  cannon = (OBJECT *) malloc (sizeof(OBJECT));
  cannon->x = x;
  cannon->y = y;
  cannon->live  = cannon_live;
  cannon->death = cannon_death;
  cannon->clear = cannon_clear;
  cannon->blitsizex = 0;
  cannon->blitsizey = 0;

  cannon->animate = copy_anim((ANIM *)orgcannon);
  cannon->frame   = anim_setsequence(cannon->animate, 0, SEQ_FORCE);

  cannon->sizex = cannon->frame->get_width();
  cannon->sizey = cannon->frame->get_height();

  cannon->coloffsetx = 8;
  cannon->coloffsety = 8;
  cannon->colwidth  = 32;
  cannon->colheight = 32;

  cannon->lethal = 0 ; // harmless

  cannon->blood = (void *) malloc(sizeof(CANNON_BLOOD));
//  vgassert(cannon->blood);
  CANNON_BLOOD *cannonblood;
  cannonblood = (CANNON_BLOOD *)cannon->blood;

  cannonblood->shootspeed = shootspeed;
  cannonblood->shootrate = shootrate;
  cannonblood->shootdelay = shootrate;
  cannonblood->bulkflg = bulkflg;
  cannonblood->mineanim = mineanim;
  cannonblood->seqnr = seqnr;
  cannonblood->lifespan = lifespan;
  cannonblood->exploflg = exploflg;
  cannonblood->moveinflg = moveinflg;
  cannonblood->moveincnt = 0;
  cannonblood->cloudflg = cloudflg;
  cannonblood->sinamp = sinamp;
  cannonblood->sinspd = sinspd;
  cannonblood->sincnt = 0;
  cannonblood->hitcnt = hitcnt;
  cannonblood->hitdelay = 0;
  cannonblood->hitrout = hitrout;
  cannonblood->wav2_fn = wav2_fn;
  cannonblood->sneakflg = sneakflg;

// basic stuff

  cannonblood->x  = x;
  cannonblood->y  = y;
  cannonblood->moveroutx = basicroutx;
  cannonblood->x1 = x1;
  cannonblood->x2 = x2;
  cannonblood->x3 = x3;
  cannonblood->moverouty = basicrouty;
  cannonblood->y1 = y1;
  cannonblood->y2 = y2;
  cannonblood->y3 = y3;
  cannonblood->hoverrouty = hoverrouty;
  cannonblood->hy1 = hy1;
  cannonblood->hy2 = hy2;
  cannonblood->hy3 = hy3;

  object_add(cannon);
}


int cannon_live (OBJECT *object, UINT32 param)
{
  INT32 x,y;
  INT32 destx,desty;
  UINT16 xyflg;
  UINT16 ratio;
  UINT16 hoek;

  CANNON_BLOOD *cannonblood;
  cannonblood = (CANNON_BLOOD *)object->blood; /* in case of multiple cannons */

// 'basic' move stuff

    if (cannonblood->moveroutx)
    {
      object->x = cannonblood->moveroutx((INT16 *) &cannonblood->x, &cannonblood->x1, &cannonblood->x2, &cannonblood->x3);
    }

    object->y = cannonblood->y;

    if (cannonblood->moverouty)
    {
      object->y = cannonblood->moverouty((INT16 *) &cannonblood->y, &cannonblood->y1, &cannonblood->y2, &cannonblood->y3);
    }

    if (cannonblood->hoverrouty)
    {
      object->y = cannonblood->moverouty((INT16 *) &object->y, &cannonblood->hy1, &cannonblood->hy2, &cannonblood->hy3);
    }

// calc anim angle
  
  xyflg = 0;
  hoek = 0;

  if (cannonblood->sinamp)
    {
      cannonblood->sincnt += cannonblood->sinspd;
      cannonblood->sincnt &= 1023;

      destx = (object->x+(object->sizex/2));
      desty = (object->y+(object->sizey/2));

      x = ((sinus512[ cannonblood->sincnt          ] * cannonblood->sinamp) >> 8);
      y = ((sinus512[(cannonblood->sincnt+256)&1023] * cannonblood->sinamp) >> 8);

      destx += x;
      desty += y;

    }
  else
    {
      destx = (hoi->x+(hoi->sizex/2));
      desty = (hoi->y+(hoi->sizey/2));

      x = ((destx - (object->x+(object->sizex/2))));
      y = ((desty - (object->y+(object->sizey/2))));
    }

  y *= 256;

  if (x < 0)
  {
    xyflg = 1;
    x = -x;
  }
  if (y < 0)
  {
    xyflg += 2;
    y = -y;
  }

  if (x == 0) x = 1;
  ratio = y/x;

  if (ratio < 51)
  {
    hoek = 4;
    goto endratio;
  }
  if (ratio < 171)
  {
    hoek = 3;
    goto endratio;
  }
  if (ratio < 383)
  {
    hoek = 2;
    goto endratio;
  }
  if (ratio < 1287)
  {
    hoek = 1;
    goto endratio;
  }
  hoek = 0;
endratio:

  if (xyflg & 2)
  {
    hoek  = 8-hoek;
    hoek &= 15;
  }

  if (xyflg & 1)
  {
    hoek  = 16-hoek;
    hoek &= 15;
  }


//hoeken zitten verkeerd om in anim

  hoek -= 4;
  hoek &= 15;

  object->frame = anim_forceframe(object->animate,hoek);


//shoot projectiles

  if (object->visible)
    {
      INT16 xoffset,yoffset;
      UINT16 shootspeed;

      cannonblood->shootdelay--;

      if (cannonblood->moveincnt < 90) cannonblood->moveincnt++;

      shootspeed= cannonblood->shootspeed + (30 - (cannonblood->moveincnt/3));

      xoffset = ((sinus512[((64 * hoek)+256) &1023] * 12) >> 8);
      yoffset = ((sinus512[((64 * hoek)+512) &1023] * 12) >> 8);

      xoffset+= (object->sizex/2);
      yoffset+= (object->sizey/2);

      xoffset += object->x;
      yoffset += object->y;

	  if (!cannonblood->sneakflg)
	  {
        if (cannonblood->bulkflg)
	    {
	      if (!cannonblood->shootdelay)
	      {
	        cannonblood->shootdelay = cannonblood->shootrate;
	      }
	      if (cannonblood->shootdelay < (cannonblood->shootrate/2))
	      {
	        if ((cannonblood->shootdelay & 7) == 7)
		    {
		      mine_init(xoffset, yoffset, ((destx - xoffset)*256)/shootspeed, ((desty-yoffset)*256)/shootspeed, cannonblood->mineanim, cannonblood->seqnr, cannonblood->lifespan, cannonblood->exploflg, 1, 0);
		      if (cannonblood->cloudflg) plof_init(xoffset, yoffset, 10, 0, 0, 0, 0);
		    }
	      }
	    }
        else
	    {
	      if (!cannonblood->shootdelay)
	      {
	        mine_init(xoffset, yoffset, ((destx - xoffset)*256)/shootspeed, ((desty-yoffset)*256)/shootspeed, cannonblood->mineanim, cannonblood->seqnr, cannonblood->lifespan, cannonblood->exploflg, 1, 0);
	        if (cannonblood->cloudflg) plof_init(xoffset, yoffset, 10, 0, 0, 0, 0);
	        cannonblood->shootdelay = cannonblood->shootrate;
	      }
	    }
	  }
      else
	  {
        if (!cannonblood->shootdelay)
        {
          cannonblood->shootdelay = cannonblood->shootrate;
	   	  sneak_init(xoffset-16, yoffset-16);
          if (cannonblood->cloudflg) plof_init(xoffset, yoffset, 10, 0, 0, 0, 0);
        }
	  }
    }
  else
    {
      cannonblood->shootdelay = cannonblood->shootrate;
      cannonblood->moveincnt = 0;
    }

  if (bullet_check(object))
    {
      cannonblood->hitcnt--;
      cannonblood->hitdelay = 6;
      if (!cannonblood->hitcnt)
	{
	  plof_init(object->x + (object->sizex/2), object->y + (object->sizey/2), 0, 0, 0, 0, 0);
	  //	  player1.score += 50;
	  return 1;
	}

      //      player1.score += 10;

      if (cannonblood->hitrout) cannonblood->hitrout(object,0);
    }
    

	if (cannonblood->wav2_fn)
	{
		INT32 distxl,distyl;
		INT32 distxr,distyr;

		
		distxl = abs(hoi->x - (object->x-64));
		distyl = abs(hoi->y - object->y);
		if (distxl < distyl) distxl = distyl;    // de verste distance wint

		distxr = abs(hoi->x - (object->x+64));
		distyr = abs(hoi->y - object->y);
		if (distxr < distyr) distxr = distyr;    // de verste distance wint

		cannonblood->wav2_fn(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
	}

  
	if (cannonblood->hitdelay)
    {
      cannonblood->hitdelay--;
      if (cannonblood->hitdelay)
	{
	  object->frame   = anim_setsequence(object->animate, 1, SEQ_FORCE);
	}
      else
	{
	  object->frame   = anim_setsequence(object->animate, 0, SEQ_FORCE);
	  object->frame   = anim_forceframe(object->animate,hoek);
	}
    }

  return 0;
}

void cannon_death(OBJECT *object, UINT32 param)
{
}

void cannon_clear(OBJECT *object, UINT32 param)
{
}


