#include <frm_wrk.hpp>
#include <switsj.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


unsigned char wallstuitertab[] = {
    0,1,1,1,2,2,2,3,3,4,4,5,6,7,8,
    10,12,14,16,19,22,25,29,33,38,43,49,56,
    64,73,83,94,105,116,126,120,115,112,110,109,109,108,108,108,108,109,109,110,
    112,115,120,126,122,121,120,120,120,121,122,126,125,125,127
    };

OBJECT *switsj_init(UINT16 x, UINT16 y, UINT16 wallx, UINT16 wally, UINT16 openflg)
{
  OBJECT *switsj;
  switsj = (OBJECT *) malloc (sizeof(OBJECT));
  switsj->x = x;
  switsj->y = y;
  switsj->live  = switsj_live;
  switsj->death = switsj_death;
  switsj->clear = switsj_clear;
  switsj->sizex = 32;
  switsj->sizey = 128;
  switsj->blitsizex = 0;
  switsj->blitsizey = 0;

  switsj->lethal = 0; //harmless

  switsj->blitstartx = 0;
  switsj->blitstarty = 0;

  switsj->animate = copy_anim((ANIM *)orgswitsj);
  switsj->frame = anim_setsequence(switsj->animate, 0, SEQ_FORCE);

  switsj->blood = (void *) malloc(sizeof(SWITSJ_BLOOD));
//  vgassert(switsj->blood);
  SWITSJ_BLOOD *switsjblood;

  switsjblood = (SWITSJ_BLOOD *)switsj->blood;

  switsjblood->dir  = 0;
    switsjblood->coll=0;

  if (wallx != 0 && wally != 0)
  {
    switsjblood->wall = wall_init(wallx, wally, openflg);
//    vgassert(switsjblood->wall);
  }
  else
  {
    switsjblood->wall = 0;
  }
  object_add(switsj);
  return switsj;
}


int switsj_live (OBJECT *object, UINT32 param)
{
  SWITSJ_BLOOD *switsjblood;

  switsjblood = (SWITSJ_BLOOD *)object->blood; /* in case of multiple switsjs */

/* code for collision with hoi */

  if ((UINT16)(hoi->x - object->x + 32) < 48)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + 64) <64)   /* y collision */
      {
        WALL_BLOOD *wallblood;
        HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
        
          if (shootkey && switsjblood->coll==0)
        {
            switsjblood->coll=1;


          if (switsjblood->wall == 0)
          {
            play_switch();
            switsjblood->dir ^= 1;  // welke kant schakelaar?
            object->frame = anim_forceframe(object->animate, switsjblood->dir);
          }
          else
          {
            wallblood = (WALL_BLOOD *)switsjblood->wall->blood;
            if (!wallblood->camdelay)
            {
              play_switch();
			  wallblood->camdelay = 1;
              wallblood->camdir = 0;
              switsjblood->dir ^= 1;  // welke kant schakelaar?
              object->frame = anim_forceframe(object->animate, switsjblood->dir);
            }
          }
        }
      }
      else
      {
          switsjblood->coll=0;
      }
  }
    else
    {
        switsjblood->coll=0;
    }

  return 0;
}

void switsj_death(OBJECT *object, UINT32 param)
{
}

void switsj_clear(OBJECT *object, UINT32 param)
{
}


OBJECT *wall_init(UINT16 x, UINT16 y, UINT16 openflg)
{
  OBJECT *wall;
  wall = (OBJECT *) malloc (sizeof(OBJECT));
  wall->x = x;
  wall->y = y;
  wall->live  = wall_live;
  wall->death = wall_death;
  wall->clear = wall_clear;
  wall->sizex = 32;
  wall->sizey = 128;
  wall->blitsizex = 32;
  wall->blitsizey = 128;
  wall->blitstartx = 0;
  wall->blitstarty = 0;

  wall->lethal = 0; //harmless


  wall->animate = copy_anim((ANIM *)orgwall);
  wall->frame = anim_setsequence(wall->animate, 0, SEQ_FORCE);

  wall->blood = (void *) malloc(sizeof(WALL_BLOOD));
//  vgassert(wall->blood);
  WALL_BLOOD *wallblood;

  wallblood = (WALL_BLOOD *)wall->blood;

  wallblood->camdelay = 0;
  wallblood->camdir = 0;
  wallblood->dir = !openflg;
  wallblood->stuit = 0;

  {
    UINT16 i;

    for (i=0; i<8; i++)
    {
      colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = (char) openflg;
      colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)+1] = (char) openflg;
      y+=16;
    }
  }

  object_add(wall);
  return wall;
}


int wall_live (OBJECT *object, UINT32 param)
{
  WALL_BLOOD *wallblood;

  wallblood = (WALL_BLOOD *)object->blood;

  if (wallblood->camdelay)
  {
    if (!wallblood->camdir)
    {
      UINT16 spd;
      spd = wallblood->camdelay/2 + 1;
      if (spd > 32) spd = 32;
      camera_override(&player1, object->x, object->y, spd);

      if (wallblood->camdelay < 120)
      {
        wallblood->camdelay++;
        if (wallblood->camdelay == 120)
        {
          wallblood->dir ^= 1;     // muur schuift omhoog/laag
          wallblood->stuit = 1;    // muur schuift omhoog/laag
        }
      }
    }
    else
    {
      if (wallblood->camdelay > 0)
      {
        UINT16 spd;
        spd = (120-wallblood->camdelay)/2 + 1;
        if (spd > 32) spd = 32;
        camera_override(&player1, player1.focusx+320, player1.focusy+240, spd);
        wallblood->camdelay--;
      }
    }
  }
  

  if (wallblood->stuit)
  {

    if (wallstuitertab[wallblood->stuit-1] == 126) play_door();
    if (!wallblood->dir)
    {
      object->blitsizey  = wallstuitertab[wallblood->stuit-1]+ 1;
    }
    else
    {
      object->blitsizey = 128 - wallstuitertab[wallblood->stuit-1];
    }
    object->blitstarty = 128 - object->blitsizey;

    wallblood->stuit++;

    if (wallblood->stuit == sizeof(wallstuitertab)/2)
    {
      UINT16 i,x,y,pat;

      pat = !wallblood->dir;
      x = object->x;
      y = object->y;

      for (i=0; i<8; i++)
      {
        colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = (char) pat;
        colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)+1] = (char) pat;
        y+=16;
      }
    }

    if (wallblood->stuit > sizeof(wallstuitertab))
    {
      wallblood->stuit = 0;
      wallblood->camdir = 1;
    }
  }
  

/* code for collision with hoi */

#if 0
  if ((UINT16)(hoi->x - object->x + 32) < 48)   /* x collision */
  {
    if ((UINT16)(hoi->y - object->y + 64) <64)   /* y collision */
      {
        HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood;
        
      }
  }
#endif


  return 0;
}

void wall_death(OBJECT *object, UINT32 param)
{
}

void wall_clear(OBJECT *object, UINT32 param)
{
}



