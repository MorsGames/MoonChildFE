#include <frm_wrk.hpp>
#include <bolt.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


void bolt_init(UINT16 x, UINT16 y, UINT16 xyflg, INT16 sincnt, INT16 sinspd, UINT16 sinamp, UINT16 curdelay, UINT16 totdelay, UINT16 shootdelay)
{
  OBJECT *bolt;
  bolt = (OBJECT *) malloc (sizeof(OBJECT));
  bolt->x = x;
  bolt->y = y;
  bolt->live  = bolt_live;
  bolt->death = bolt_death;
  bolt->clear = bolt_clear;
  bolt->sizex = 48;
  bolt->sizey = 48;
  bolt->blitsizex = 0;
  bolt->blitsizey = 0;
  bolt->blitstartx = 0;
  bolt->blitstarty = 0;

  bolt->lethal = 3; //zeer dodelijk

  bolt->animate = copy_anim((ANIM *)orgbolt);
  if (xyflg)
    {
      bolt->frame = anim_setsequence(bolt->animate, 0, SEQ_FORCE);
    }
  else
    {
      bolt->frame = anim_setsequence(bolt->animate, 1, SEQ_FORCE);
    }

  bolt->sizex = bolt->frame->get_width();
  bolt->sizey = bolt->frame->get_height();

  bolt->blood = (void *) malloc(sizeof(BOLT_BLOOD));
//  vgassert(bolt->blood);
  BOLT_BLOOD *boltblood;

  boltblood = (BOLT_BLOOD *)bolt->blood;
  boltblood->x      = x;
  boltblood->y      = y;
  boltblood->xyflg  = xyflg;
  boltblood->sincnt = sincnt;
  boltblood->sinspd = sinspd;
  boltblood->sinamp = sinamp;

  boltblood->curdelay   = curdelay;
  boltblood->totdelay   = totdelay;
  boltblood->shootdelay = shootdelay;

  if (xyflg)
    {
      bolt->coloffsetx = 16;
      bolt->coloffsety = 8;
      bolt->colwidth  =  96;
      bolt->colheight =  1;
      boltblood->x+= 32;
      boltblood->y+= 8;
      bolt->x+= 32;
      bolt->y+= 8;
    }
  else
    {
      bolt->coloffsetx = 8;
      bolt->coloffsety = 16;
      bolt->colwidth  =  1;
      bolt->colheight =  96;
      boltblood->x+= 32;
      boltblood->y+= 8;
      bolt->x+= 8;
      bolt->y+= 32;
    }

  {
    OBJECT *element;
    UINT16  j;

// the cathode

    element = (OBJECT *) malloc (sizeof(OBJECT));
    element->x = x;
    element->y = y;
    element->live  = 0;
    element->death = 0;
    element->clear = 0;
    element->blitsizex = 0;
    element->blitsizey = 0;

    element->lethal = 1; // lethal  (NRG drain)

    element->animate = copy_anim(orgbolt);
    j = 4;             // vertical   animations
    if (xyflg) j =2;   // horizontal animations

    element->frame   = anim_setsequence(element->animate, j, SEQ_FORCE);

    element->sizex = element->frame->get_width();
    element->sizey = element->frame->get_height();

    element->coloffsetx = 15;
    element->coloffsety = 15;
    element->colwidth  = 1;
    element->colheight = 1;

    element->blood = 0;

    object_add(element);
    boltblood->cathode = element;

// the anode

    element = (OBJECT *) malloc (sizeof(OBJECT));
    element->x = x;
    element->y = y;

    if (xyflg)
      {
	element->x+= 160;
      }
    else
      {
	element->y+= 160;
      }

    element->live  = 0;
    element->death = 0;
    element->clear = 0;
    element->blitsizex = 0;
    element->blitsizey = 0;

    element->lethal = 1; // lethal  (NRG drain)

    element->animate = copy_anim(orgbolt);
    j++;
    element->frame   = anim_setsequence(element->animate, j, SEQ_FORCE);

    element->sizex = element->frame->get_width();
    element->sizey = element->frame->get_height();

    element->coloffsetx = 15;
    element->coloffsety = 15;
    element->colwidth  = 1;
    element->colheight = 1;

    element->blood = 0;

    object_add(element);
    boltblood->anode = element;

  }

  object_add(bolt);
}


int bolt_live (OBJECT *object, UINT32 param)
{
  BOLT_BLOOD *boltblood;
  INT16       value;

  boltblood = (BOLT_BLOOD *)object->blood; /* in case of multiple bolts */

  boltblood->curdelay--;
  if (!boltblood->curdelay) boltblood->curdelay = boltblood->totdelay;

  if (boltblood->curdelay < boltblood->shootdelay)
    {
      object->frame   = anim_nextframe(object->animate);

  	{
		INT32 distxl,distyl;
		INT32 distxr,distyr;

		
		distxl = abs(hoi->x - (object->x-64));
		distyl = abs(hoi->y - object->y);
		if (distxl < distyl) distxl = distyl;    // de verste distance wint

		distxr = abs(hoi->x - (object->x+64));
		distyr = abs(hoi->y - object->y);
		if (distxr < distyr) distxr = distyr;    // de verste distance wint

		prox_stroom_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
	}

  
    }
  else
    {
      object->frame = 0;
    }

// handle animations of cathode and anode

  boltblood->cathode->frame = anim_nextframe(boltblood->cathode->animate);
  boltblood->anode->frame   = anim_nextframe(boltblood->anode->animate);

// handle movements

  boltblood->sincnt += boltblood->sinspd;
  boltblood->sincnt &= 1023;

  value = ((sinus512[(boltblood->sincnt) & 1023] * boltblood->sinamp) >> 8);

  if (boltblood->xyflg)
    {
      object->y  = boltblood->y+value+8;
      boltblood->cathode->y = boltblood->y+value;
      boltblood->anode->y   = boltblood->y+value;
    }
  else
    {
      object->x  = boltblood->x+value+8;
      boltblood->cathode->x = boltblood->x+value;
      boltblood->anode->x   = boltblood->x+value;
    }

  return 0;
}

void bolt_death(OBJECT *object, UINT32 param)
{
}

void bolt_clear(OBJECT *object, UINT32 param)
{
}



