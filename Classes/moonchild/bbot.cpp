#include <frm_wrk.hpp>
#include <bbot.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <mine.hpp>
#include <hoi.hpp>
#include <plof.hpp>


void bbot_init(UINT16 x, UINT16 y, UINT16 sinxcnt, UINT16 sinxadd, UINT16 sinxamp, UINT16 shootflg, UINT16 leftx, UINT16 rightx, UINT16 spdx)
{
  OBJECT *bbot;
  UINT16 i;

  bbot = (OBJECT *) malloc (sizeof(OBJECT));
  bbot->x = x;
  bbot->y = y;
  bbot->live  = bbot_live;
  bbot->death = bbot_death;
  bbot->clear = bbot_clear;
  bbot->blitsizex = 0;
  bbot->blitsizey = 0;

  bbot->animate = 0;  // copy_anim(orgsegment);

  bbot->frame   = 0;  // anim_setsequence(bbot->animate, seqnr, SEQ_FORCE);

  bbot->sizex = 0;
  bbot->sizey = 0;

  bbot->lethal = 0; // not lethal
  bbot->coloffsetx = 16;
  bbot->coloffsety = 16;
  bbot->colwidth  = 16;
  bbot->colheight = 16;
  
  bbot->blood = (void *) malloc(sizeof(BBOT_BLOOD) + 16*sizeof(OBJECT *));
//  vgassert(bbot->blood);
  BBOT_BLOOD *bbotblood;
  bbotblood = (BBOT_BLOOD *)bbot->blood;

  bbotblood->x = x;
  bbotblood->y = y;
  bbotblood->shootflg = shootflg;
  bbotblood->shootdelay = 75;
  bbotblood->sinxcnt = sinxcnt;
  bbotblood->sinxamp = sinxamp;
  bbotblood->sinxadd = sinxadd;
  bbotblood->schuif = 64;
  bbotblood->leftx = leftx;
  bbotblood->rightx = rightx;
  bbotblood->spdx = spdx;

  for (i=0; i<32; i++)
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

    element->lethal = 1; // lethal  (NRG drain)

    element->animate = copy_anim(orgbbot);   // tussen elementje
    element->frame   = anim_setsequence(element->animate, 1, SEQ_FORCE);

    element->sizex = element->frame->get_width();
    element->sizey = element->frame->get_height();

    element->coloffsetx = 8;
    element->coloffsety = 8;
    element->colwidth  = 16;
    element->colheight = 16;

    element->blood = 0;

    switch (i)
	{
	case 0:
		element->frame   = anim_setsequence(element->animate, 2, SEQ_FORCE);
		break;
	case 31:
		element->frame   = anim_setsequence(element->animate, 0, SEQ_FORCE);
		element->x += 8;

        element->coloffsetx = 8;
        element->coloffsety = 8;
        element->colwidth  = 32;
        element->colheight = 32;
        break;
	default:
	    element->frame   = anim_setsequence(element->animate, 1, SEQ_FORCE);
	}
	
    object_add(element);
    bbotblood->elements[i] = element;
  }

  object_add(bbot);

  return;
}


int bbot_live (OBJECT *object, UINT32 param)
{
  UINT16 i;
  UINT16 sy;
  UINT16 sx;
  INT16 x;

  BBOT_BLOOD *bbotblood;
  bbotblood = (BBOT_BLOOD *)object->blood; /* in case of multiple bbots */

  object->x += bbotblood->spdx;
  if (object->x < bbotblood->leftx || object->x > bbotblood->rightx)
  {
    bbotblood->spdx = -bbotblood->spdx;
  }



  sy  = object->y<<5;
  bbotblood->sinxcnt += bbotblood->sinxadd;
  sx = bbotblood->sinxcnt;
  
  for (i=0; i<32; i++)
  {
    x =  ((sinus512[sx&1023] * (bbotblood->sinxamp)) >> 10);

    sx += bbotblood->sinxadd;

    bbotblood->elements[i]->x = object->x + x;
    bbotblood->elements[i]->y = sy>>5;
    sy -= bbotblood->schuif;

	if (i>0 && i <31)
	{
      bbotblood->elements[i]->x += 14;
	}

    if (i==31)
	{
      bbotblood->elements[i]->y -= 40;
	}

    if (bullet_check(bbotblood->elements[i]))
    {
      OBJECT * temp;
      temp = bbotblood->elements[i];
  	  bbotblood->lastx = temp->x+(temp->sizex/2);
	  bbotblood->lasty = temp->y+(temp->sizey/2);

	  if (bbotblood->schuif > 32) bbotblood->schuif-= 16;

      plof_init(bbotblood->lastx, bbotblood->lasty, 1, 0, 0, 0, 0);
    }
  }


  bbotblood->elements[0]->frame = anim_nextframe(bbotblood->elements[0]->animate);

  if (bbotblood->schuif < 256) bbotblood->schuif+= 1;
  
  if (bbotblood->elements[31]->visible && bbotblood->shootflg)
  {
	bbotblood->shootdelay--;
    if (!bbotblood->shootdelay)
    {
      bbotblood->shootdelay = 100;
      mine_init(bbotblood->elements[31]->x+24, bbotblood->elements[31]->y+16, ((hoi->x+32 - bbotblood->elements[31]->x)*256)/100, ((hoi->y+40-bbotblood->elements[31]->y)*256)/175, 0, 0, 250, 1, 1, 0);
    }
  }
  else
  {
    bbotblood->shootdelay = 100;
  }

  return 0;
}

void bbot_death(OBJECT *object, UINT32 param)
{
}

void bbot_clear(OBJECT *object, UINT32 param)
{
}


