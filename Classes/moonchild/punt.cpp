#include <frm_wrk.hpp>
#include <punt.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>

INT16 wiggletab[25] = { 4,0,-4,0,4,2,0,-2,-4,-2,0,2,4,
                        4,4,1,-1,-4,-4,-1,1,4,4,4,1};

void punt_init(UINT16 x, UINT16 y)
{
  OBJECT *punt;
  punt = (OBJECT *) malloc (sizeof(OBJECT));
  punt->x = x;
  punt->y = y;
  punt->live  = punt_live;
  punt->death = punt_death;
  punt->clear = punt_clear;
  punt->blitsizex = 0;
  punt->blitsizey = 0;

  punt->animate = copy_anim(orgpunt);

  punt->frame   = anim_setsequence(punt->animate, 0, SEQ_FORCE);

  punt->sizex = punt->frame->get_width();
  punt->sizey = punt->frame->get_height();

  punt->lethal = 3; // if hit then death!
  punt->coloffsetx = 15;
  punt->coloffsety = 0;
  punt->colwidth  = 2;
  punt->colheight = punt->sizey - 8;
  
  punt->blood = (void *) malloc(sizeof(PUNT_BLOOD));
//  vgassert(punt->blood);
  PUNT_BLOOD *puntblood;
  puntblood = (PUNT_BLOOD *)punt->blood;

  puntblood->x = x;
  puntblood->y = y<<3;
  puntblood->wigglecnt = 25;
  puntblood->gravy = 0;
  puntblood->lifespan = 80;
  
  object_add(punt);

  return;
}


int punt_live (OBJECT *object, UINT32 param)
{

  PUNT_BLOOD *puntblood;
  puntblood = (PUNT_BLOOD *)object->blood; /* in case of multiple punts */

  if (puntblood->wigglecnt)
  {
    puntblood->wigglecnt--;
    object->x = puntblood->x + wiggletab[puntblood->wigglecnt];
  }
  else
  {
    puntblood->gravy += 1;
    puntblood->y += puntblood->gravy;
    object->x = puntblood->x;
    object->y = puntblood->y>>3;
    puntblood->lifespan--;
    if (!puntblood->lifespan) return 1;
  }

  return 0;
}

void punt_death(OBJECT *object, UINT32 param)
{
}

void punt_clear(OBJECT *object, UINT32 param)
{
}


