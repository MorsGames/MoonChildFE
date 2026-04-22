#include <frm_wrk.hpp>
#include <sneak.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>
#include <mine.hpp>
#include <plof.hpp>



void sneak_init (UINT16 x, UINT16 y)
{
  OBJECT *sneak;
  sneak = (OBJECT *) malloc (sizeof(OBJECT));
  sneak->x = x;
  sneak->y = y;
  sneak->live  = sneak_live;
  sneak->death = sneak_death;
  sneak->clear = sneak_clear;
  sneak->blitsizex = 0;
  sneak->blitsizey = 0;
  sneak->blitstartx = 0;
  sneak->blitstarty = 0;


  sneak->animate = copy_anim((ANIM *)orgheatsneaker);
  sneak->frame = anim_setsequence(sneak->animate, 0, SEQ_FORCE);


  sneak->sizex = sneak->frame->get_width();
  sneak->sizey = sneak->frame->get_height();

  sneak->blitsizex = sneak->sizex;
  sneak->blitsizey = sneak->sizey;
  sneak->blitstartx = 0;
  sneak->blitstarty = 0;
  sneak->lethal = 1;
  sneak->coloffsetx = 8;
  sneak->coloffsety = 8;
  sneak->colwidth = sneak->sizex - 16;
  sneak->colheight= sneak->sizey - 16;


  sneak->blood = (void *) malloc(sizeof(SNEAK_BLOOD));
//  vgassert(sneak->blood);
  SNEAK_BLOOD *sneakblood;

  sneakblood = (SNEAK_BLOOD *)sneak->blood;
  sneakblood->x = x << 8;
  sneakblood->y = y << 8;

  sneakblood->spdx = 0;
  sneakblood->spdy = 0;
  
  object_add(sneak);
}


int sneak_live (OBJECT *object, UINT32 param)
{
  SNEAK_BLOOD *sneakblood;

  INT32 x,y;
  INT32 destx,desty;
  UINT16 xyflg;
  UINT16 r;
  UINT16 ratio;
  UINT16 hoek;

  sneakblood = (SNEAK_BLOOD *)object->blood; /* in case of multiple sneaks */

  sneakblood->x += sneakblood->spdx;
  sneakblood->y += sneakblood->spdy;

  object->x = sneakblood->x >> 8;
  object->y = sneakblood->y >> 8;

  if (object->x < (hoi->x+32)) sneakblood->spdx += 32;
  if (object->x > (hoi->x+32)) sneakblood->spdx -= 32;
  if (object->y < (hoi->y+32)) sneakblood->spdy += 32;
  if (object->y > (hoi->y+32)) sneakblood->spdy -= 32;

  if (sneakblood->spdx >  (8*256)) sneakblood->spdx =  (5*256);
  if (sneakblood->spdx < -(8*256)) sneakblood->spdx = -(5*256);
  if (sneakblood->spdy >  (8*256)) sneakblood->spdy =  (5*256);
  if (sneakblood->spdy < -(8*256)) sneakblood->spdy = -(5*256);

// calc anim angle
  
  xyflg = 0;
  hoek = 0;

  destx = (hoi->x+(hoi->sizex/2));
  desty = (hoi->y+(hoi->sizey/2));

  x = ((destx - (object->x+(object->sizex/2))));
  y = ((desty - (object->y+(object->sizey/2))));



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

  if ((xyflg & 1) == 0)
  {
    hoek  = 16-hoek;
    hoek &= 15;
  }


//hoeken zitten verkeerd om in anim

  hoek += 8;
  hoek &= 15;

  object->frame = anim_forceframe(object->animate,hoek);


  if (r = collision(object->x+(object->sizex/2), object->y + (object->sizey/2) ))
  {

    if (r ==2)   // wegschietable blokje?
      {
	UINT16 x,y;
	x = object->x+(object->sizex/2);
	y = object->y+(object->sizey/2);
	x &= 0xffe0;
	y &= 0xffe0;
	
	player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = 0;
	colmap[((y)*player1.loadedmap->mapsizex/16/16)+((x)/16)] = 0;
	colmap[((y)*player1.loadedmap->mapsizex/16/16)+((x+16)/16)] = 0;
	colmap[((y+16)*player1.loadedmap->mapsizex/16/16)+((x)/16)] = 0;
	colmap[((y+16)*player1.loadedmap->mapsizex/16/16)+((x+16)/16)] = 0;

	plof_init(x+16, y+16, 0, 0, 0, 0, 0);
        return 1;
      }
    else
      {
	    plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 0, 0, 0, 0);
        return 1;
  	  }
  }

    {
	INT32 distxl,distyl;
	INT32 distxr,distyr;

      distxl = abs(hoi->x - (object->x-64));
	distyl = abs(hoi->y - object->y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (object->x+64));
	distyr = abs(hoi->y - object->y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	prox_heatsnkr_volume(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
  }

  return 0;
}

void sneak_death(OBJECT *object, UINT32 param)
{
}

void sneak_clear(OBJECT *object, UINT32 param)
{
}



