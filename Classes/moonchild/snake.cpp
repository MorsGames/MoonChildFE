#include <frm_wrk.hpp>
#include <snake.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <mine.hpp>
#include <hoi.hpp>
#include <plof.hpp>


void snake_init(UINT16 x, UINT16 y, ANIM *snakeanim, UINT16 seqnr, UINT16 sinamp,
		UINT16 shootflg, UINT16 destroyable, OBJECT_CB ready_fn, SNAKETAB *snaketab)
{
  OBJECT *snake;
  UINT16 i;

  snake = (OBJECT *) malloc (sizeof(OBJECT));
  snake->x = x;
  snake->y = y;
  snake->live  = snake_live;
  snake->death = snake_death;
  snake->clear = snake_clear;
  snake->blitsizex = 0;
  snake->blitsizey = 0;

  snake->animate = 0;  // copy_anim(orgsegment);

  snake->frame   = 0;  // anim_setsequence(snake->animate, seqnr, SEQ_FORCE);

  snake->sizex = 0;
  snake->sizey = 0;

  snake->lethal = 0; // not lethal
  snake->coloffsetx = 16;
  snake->coloffsety = 16;
  snake->colwidth  = 16;
  snake->colheight = 16;
  
  snake->blood = (void *) malloc(sizeof(SNAKE_BLOOD) + snaketab->numelements*sizeof(OBJECT *));
//  vgassert(snake->blood);
  SNAKE_BLOOD *snakeblood;
  snakeblood = (SNAKE_BLOOD *)snake->blood;

  snakeblood->x = x;
  snakeblood->y = y;
  snakeblood->shootdelay = 50;

  snakeblood->sinamp = sinamp;
  snakeblood->shootflg = shootflg;
  snakeblood->destroyable = destroyable;

  snakeblood->ready_fn = ready_fn;

  for (i=0; i<snaketab->numelements; i++)
  {
    OBJECT *element;
    UINT16  j;

    element = (OBJECT *) malloc (sizeof(OBJECT));
    element->x = x;
    element->y = y;
    element->live  = 0;
    element->death = 0;
    element->clear = 0;
    element->blitsizex = 0;
    element->blitsizey = 0;

    element->lethal = 1; // lethal  (NRG drain)

    element->animate = copy_anim(snakeanim);
    element->frame   = anim_setsequence(element->animate, seqnr, SEQ_FORCE);

    for (j=0; j<i; j++)
    {
      element->frame   = anim_nextframe(element->animate);
      element->frame   = anim_nextframe(element->animate);
      element->frame   = anim_nextframe(element->animate);
    }

    element->sizex = element->frame->get_width();
    element->sizey = element->frame->get_height();

    element->coloffsetx = 8;
    element->coloffsety = 8;
    element->colwidth  = 16;
    element->colheight = 16;

    element->blood = 0;

    object_add(element);
    snakeblood->elements[i] = element;
  }

  memcpy (&snakeblood->snaketab, snaketab, sizeof(SNAKETAB));
  object_add(snake);

  return;
}


int snake_live (OBJECT *object, UINT32 param)
{
  UINT16 i;
  UINT32 ox = 0,oy = 0;
  UINT32 x = 0,y = 0;
  UINT16 s1x, s1y;
  UINT16 s2x, s2y;
  UINT16 visibleflg;
  UINT16 shootx,shooty;
  UINT16 alldeadflg = 1;

  SNAKE_BLOOD *snakeblood;
  snakeblood = (SNAKE_BLOOD *)object->blood; /* in case of multiple snakes */

  visibleflg = 0;

  snakeblood->snaketab.sinx1cnt += snakeblood->snaketab.sinx1spd;
  snakeblood->snaketab.sinx1cnt &= 1023;
  snakeblood->snaketab.siny1cnt += snakeblood->snaketab.siny1spd;
  snakeblood->snaketab.siny1cnt &= 1023;

  snakeblood->snaketab.sinx2cnt += snakeblood->snaketab.sinx2spd;
  snakeblood->snaketab.sinx2cnt &= 1023;
  snakeblood->snaketab.siny2cnt += snakeblood->snaketab.siny2spd;
  snakeblood->snaketab.siny2cnt &= 1023;


  s1x = snakeblood->snaketab.sinx1cnt;
  s1y = snakeblood->snaketab.siny1cnt;
  s2x = snakeblood->snaketab.sinx2cnt;
  s2y = snakeblood->snaketab.siny2cnt;

  ox = object->x << 10;
  oy = object->y << 10;

  for (i=0; i<snakeblood->snaketab.numelements; i++)
  {
    x =  ((sinus512[s1x&1023] * (snakeblood->sinamp)) );
    y =  ((sinus512[s1y&1023] * (snakeblood->sinamp)) );
    x += ((sinus512[s2x&1023] * (snakeblood->sinamp)) );
    y += ((sinus512[s2y&1023] * (snakeblood->sinamp)) );

    s1x += snakeblood->snaketab.sinx1add;
    s1y += snakeblood->snaketab.siny1add;
    s2x += snakeblood->snaketab.sinx2add;
    s2y += snakeblood->snaketab.siny2add;

    snakeblood->elements[i]->x = (ox+x) >> 10;
    snakeblood->elements[i]->y = (oy+y) >> 10;

//    ox = x;
//    oy = y;

    if (snakeblood->elements[i]->visible)
    {
      visibleflg = 1;   // at least 1 element is visible
      shootx = (ox+x) >> 10;
      shooty = (oy+y) >> 10;

    }

    if (snakeblood->elements[i]->frame)
    {
      alldeadflg = 0;
      snakeblood->elements[i]->frame = anim_nextframe(snakeblood->elements[i]->animate);
      snakeblood->elements[i]->frame = anim_nextframe(snakeblood->elements[i]->animate);

      if (bullet_check(snakeblood->elements[i]))
      {
        OBJECT * temp;
        temp = snakeblood->elements[i];
	snakeblood->lastx = temp->x+(temp->sizex/2);
	snakeblood->lasty = temp->y+(temp->sizey/2);

	if (snakeblood->destroyable)
	  {
	    temp->frame = 0;
	    plof_init(snakeblood->lastx, snakeblood->lasty, 0, 0, 0, 0, 0);
	  }
	else
	  {
	    plof_init(snakeblood->lastx, snakeblood->lasty, 1, 0, 0, 0, 0);
	  }
      }
    }
  }

  if (visibleflg && snakeblood->shootflg)
  {
    snakeblood->shootdelay--;
    if (!snakeblood->shootdelay)
    {
      snakeblood->shootdelay = 75;
      mine_init(shootx, shooty, ((hoi->x+32 - shootx)*256)/100, ((hoi->y+40-shooty)*256)/100, 0, 0, 250, 1, 1, 0);
    }
  }
  else
  {
    snakeblood->shootdelay = 75;
  }


  if (alldeadflg && snakeblood->ready_fn)
    {
//      log_out("snake ready with %d, %d = %x ", snakeblood->lastx, snakeblood->lasty, (((UINT32)snakeblood->lasty)<<16)+snakeblood->lastx);
      snakeblood->ready_fn(object, (((UINT32)snakeblood->lasty)<<16)+snakeblood->lastx);
      snakeblood->ready_fn = 0;
    }
  
  return 0;
}

void snake_death(OBJECT *object, UINT32 param)
{
}

void snake_clear(OBJECT *object, UINT32 param)
{
}


