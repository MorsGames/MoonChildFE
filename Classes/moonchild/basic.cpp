#include <frm_wrk.hpp>
#include <basic.hpp>
#include <plof.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>



// If forced seq is NOT SEQ_DUMMY the anim-sequence will be forced

void basic_init(UINT16 x, UINT16 y,
               BASIC_CB basicroutx, INT16 x1,  INT16 x2,  INT16 x3,
               BASIC_CB basicrouty, INT16 y1,  INT16 y2,  INT16 y3,
               BASIC_CB hoverrouty, INT16 hy1, INT16 hy2, INT16 hy3,
               ANIM *anim, UINT16 anmspd, UINT16 movdelay, UINT16 forcedseq, UINT16 bulletflg, UINT16 destroyable,
							 WAV2_FN wavrout, UINT16 rnddelay, UINT16 rnddomain, WAV2_FN proximity)
{
  OBJECT *basic;
  basic = (OBJECT *) malloc (sizeof(OBJECT));
  basic->x = x;
  basic->y = y;
  basic->live  = basic_live;
  basic->death = basic_death;
  basic->clear = basic_clear;
  basic->lethal = 1;     // energy drain

  basic->animate = copy_anim(anim);
//  vgassert(basic->animate);
  basic->frame = anim_setsequence(basic->animate, 0, SEQ_FORCE);

  basic->sizex = basic->frame->get_width();;
  basic->sizey = basic->frame->get_height();;
  basic->blitstartx = 0;
  basic->blitstarty = 0;
  basic->blitsizex = 0;
  basic->blitsizey = 0;

  basic->coloffsetx = basic->sizex/4;
  basic->coloffsety = basic->sizey/4;
  basic->colwidth = basic->sizex/2;
  basic->colheight = basic->sizey/2;


  basic->blood = (void *) malloc(sizeof(BASIC_BLOOD));
//  vgassert(basic->blood);
  BASIC_BLOOD *basicblood;
  basicblood = (BASIC_BLOOD *)basic->blood;
  basicblood->x  = x;
  basicblood->y  = y;
  basicblood->moveroutx = basicroutx;
  basicblood->x1 = x1;
  basicblood->x2 = x2;
  basicblood->x3 = x3;
  basicblood->moverouty = basicrouty;
  basicblood->y1 = y1;
  basicblood->y2 = y2;
  basicblood->y3 = y3;
  basicblood->hoverrouty = hoverrouty;
  basicblood->hy1 = hy1;
  basicblood->hy2 = hy2;
  basicblood->hy3 = hy3;
  basicblood->forcedseq = forcedseq;
  basicblood->anmspd = anmspd;
  basicblood->curmov = 0;
  basicblood->movdelay = movdelay;
  basicblood->bulletflg = bulletflg;
  basicblood->destroyable = destroyable;

	basicblood->rnddelay = rnddelay;
	basicblood->rnddomain = rnddomain;
	basicblood->curdelay = 10;
	basicblood->wav_fn = wavrout;
  basicblood->wav2_fn = proximity;

	object_add(basic);
}

int basic_live (OBJECT *object, UINT32 param)
{
  BASIC_BLOOD *basicblood;
  UINT16       oldx,i;

  basicblood = (BASIC_BLOOD *)object->blood; /* in case of multiple basics */

  for (i=0; i<basicblood->anmspd; i++)
  {
    object->frame = anim_nextframe(object->animate);
  }

  basicblood->curmov++;
  if (basicblood->curmov > basicblood->movdelay)
  {
    basicblood->curmov = 0;
  
    oldx = object->x;
    if (basicblood->moveroutx)
    {
      object->x = basicblood->moveroutx((INT16 *) &basicblood->x, &basicblood->x1, &basicblood->x2, &basicblood->x3);
    }

    object->y = basicblood->y;

    if (basicblood->moverouty)
    {
      object->y = basicblood->moverouty((INT16 *) &basicblood->y, &basicblood->y1, &basicblood->y2, &basicblood->y3);
    }

    if (basicblood->hoverrouty)
    {
      object->y = basicblood->moverouty((INT16 *) &object->y, &basicblood->hy1, &basicblood->hy2, &basicblood->hy3);
    }

    if (basicblood->forcedseq == SEQ_DUMMY)
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
      anim_setsequence(object->animate, basicblood->forcedseq, SEQ_NOFORCE);
    }
  }

	if (basicblood->wav_fn)
	{
		basicblood->curdelay--;
		if (!basicblood->curdelay)
		{
			basicblood->curdelay = MC_RANDOM(basicblood->rnddomain) + basicblood->rnddelay;
			basicblood->wav_fn(object->x, object->y);
		}
	}

	if (basicblood->wav2_fn)
	{
		INT32 distxl,distyl;
		INT32 distxr,distyr;

		
		distxl = abs(hoi->x - (object->x-64));
		distyl = abs(hoi->y - object->y);
		if (distxl < distyl) distxl = distyl;    // de verste distance wint

		distxr = abs(hoi->x - (object->x+64));
		distyr = abs(hoi->y - object->y);
		if (distxr < distyr) distxr = distyr;    // de verste distance wint

		basicblood->wav2_fn(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
	}
  
	if (basicblood->bulletflg)
    {
      if (bullet_check(object))
	{
	  if (basicblood->destroyable)
	    {
	      plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 0, 0, 0, 0, 0);
	      return 1;
	    }
	  else
	    {
	      plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 0, 0, 0, 0);
	    }
	}
    }      
  return 0;
}

void basic_death(OBJECT *object, UINT32 param)
{
}

void basic_clear(OBJECT *object, UINT32 param)
{
}

//r1 = sincnt
//r2 = sinspd
//r3 = amplitude
UINT16 basic_sin(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
{
  *(r1) += *(r2);
  *(r1) &= 1023;

  return (*(pos) + ((sinus512[*(r1)]**(r3))>>10));
}

//r1 = minx
//r2 = maxx
//r3 = speed
UINT16 basic_pingpong(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
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
UINT16 basic_flipflap(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
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
UINT16 basic_gravy(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
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
UINT16 basic_flyby(INT16 *pos, INT16 *r1, INT16 *r2, INT16 *r3)
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







