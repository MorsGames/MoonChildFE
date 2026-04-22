#include <frm_wrk.hpp>
#include <plant.hpp>
#include <plof.hpp>
#include <mine.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

//tabel consists out of [how much to add/subtract, for how long]

INT16 ampchangetab[] = {
  1,50,     //  50
  -2,50,    // -50
  0,50,     // -50
  -2,50,    //-150
  3,50,     //   0
  1,25,     //  25
  5,30,     // 175
  0,50,     // 175
  -5,40,    // -25
  -1,75,    //-100
  2,50,     //   0
  -1,-1
};


void plant_init(UINT16 x, UINT16 y, UINT16 swingspd, UINT16 swingamp, OBJECT *rock)
{
  OBJECT *plant;
  UINT16 i;

  plant = (OBJECT *) malloc (sizeof(OBJECT));
  plant->x = x;
  plant->y = y;
  plant->live  = plant_live;
  plant->death = plant_death;
  plant->clear = plant_clear;
  plant->blitsizex = 0;
  plant->blitsizey = 0;

  plant->animate = copy_anim(orgplant);

  plant->frame   = anim_setsequence(plant->animate, 1, SEQ_FORCE);

  plant->sizex = plant->frame->get_width();
  plant->sizey = plant->frame->get_height();

  plant->lethal = 0; // not lethal
  plant->coloffsetx = 15;
  plant->coloffsety = 0;
  plant->colwidth  = 2;
  plant->colheight = plant->sizey - 8;
  
  plant->blood = (void *) malloc(sizeof(PLANT_BLOOD));
//  vgassert(plant->blood);
  PLANT_BLOOD *plantblood;
  plantblood = (PLANT_BLOOD *)plant->blood;

  plantblood->x = x;
  plantblood->y = y;
  plantblood->oldx = x;
  plantblood->oldy = y;
  plantblood->swingswingcnt = 0;
  plantblood->swingswingspd = swingspd;
  plantblood->swingswingamp = swingamp;

  plantblood->ampchange = 0;              // current amplitude
  plantblood->ampchangecnt = 0;           // position in table
  plantblood->ampchangedelay = 0;         // current delay status

  plantblood->ampchangedelay = ampchangetab[plantblood->ampchangecnt+1]*2;

  plantblood->startamp = 50;
  plantblood->extraamp = 0;
  plantblood->shootdelay = 25;
  plantblood->fallspd = 0;

  plantblood->headanimcnt = 0;

  plantblood->rock = rock;

  for (i=0; i<128; i++)
  {
    plantblood->swingcnt[i] = 0;
  }

  object_add(plant);

  for (i=0; i<16; i++)
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

    if (i!=15)
    {
      element->animate = copy_anim(orgplant);
      element->frame   = anim_setsequence(element->animate, 1, SEQ_FORCE);
      element->frame   = anim_forceframe(element->animate, !(i&1));
      element->sizex   = element->frame->get_width();
      element->sizey   = element->frame->get_height();
    }
    else
    {
      element->animate = copy_anim(orgplant);
      element->frame   = anim_setsequence(element->animate, 0, SEQ_FORCE);
      
      element->sizex = element->frame->get_width();
      element->sizey = element->frame->get_height();
    }

    element->lethal = 1; // not lethal
    element->coloffsetx = 8;
    element->coloffsety = 8;
    element->colwidth  = element->sizex - 16;
    element->colheight = element->sizey - 16;

    element->blood = 0;

    object_add(element);
    plantblood->elements[i] = element;
  }

  return;
}


int plant_live (OBJECT *object, UINT32 param)
{
  UINT16 i;
  UINT32 x,y;
  UINT16 rot;

  PLANT_BLOOD *plantblood;
  plantblood = (PLANT_BLOOD *)object->blood; /* in case of multiple plants */

  if (plantblood->ampchangedelay) plantblood->ampchangedelay--;
  if (!plantblood->ampchangedelay)
    {
      plantblood->ampchangecnt += 2;
      if (ampchangetab[plantblood->ampchangecnt+1] == -1)
	{
	  plantblood->ampchangecnt = 0;
	}

      plantblood->ampchangedelay = ampchangetab[plantblood->ampchangecnt+1]*2;
    }

  plantblood->ampchange += ampchangetab[plantblood->ampchangecnt];


  plantblood->swingswingcnt += plantblood->swingswingspd;
  plantblood->swingswingcnt &= 1023;

  for (i=0; i<127; i++)
  {
    plantblood->swingcnt[i] = plantblood->swingcnt[i+1];
  }

  plantblood->swingcnt[127] = ((sinus512[plantblood->swingswingcnt]* plantblood->ampchange) >> 10);

  //  plantblood->swingcnt[127] = (hoi->x&255)-128;
  

  x = object->x<<10;
  y = object->y<<10;

  rot = 512;

  if (plantblood->startamp < 228) // if small don't collapse yet
    {
      for (i=0; i<16; i++)
	{
	  rot += plantblood->swingcnt[120 - (i*8)];
	  x = ((sinus512[ (rot)     &1023] * ((plantblood->startamp+plantblood->extraamp)/4)) )+x;
	  y = ((sinus512[ (rot+256) &1023] * ((plantblood->startamp+plantblood->extraamp)/4)) )+y;
	  plantblood->elements[i]->x = x>>10;
	  plantblood->elements[i]->y = y>>10;
	}
      plantblood->elements[15]->x -= plantblood->elements[15]->sizex/2 - 8;
    }
  else
    {
      UINT16 spd;
      plantblood->fallspd += 1;

      if (plantblood->fallspd == 2) play_segmexpl(hoi->x,hoi->y);
			
      camera_override(&player1, object->x+32, object->y, 8);
	mc_autorun = 2;   // dummy value so Mon Child can't move anymore!


      if (plantblood->fallspd == 198) play_madeit();
			if (plantblood->fallspd < 200)
	{
	  //	  player1.score += 25;    // so 200*25 is 5000 total score for ending the level!
	  
	  spd = plantblood->fallspd;
	  for (i=0; i<16; i++)
	    {
	      plantblood->elements[i]->x += ((i-8)/2);
	      plantblood->elements[i]->y += (spd>>3);
	      spd+=3;
	    }
	}      
      if (plantblood->fallspd > 450)
	{
				nextlvlflg = 1;
	}
    }

  if (plantblood->headanimcnt)
    {
      plantblood->headanimcnt--;

      plantblood->elements[15]->frame   = anim_forceframe(plantblood->elements[15]->animate, (plantblood->headanimcnt>10) ? 21-plantblood->headanimcnt : plantblood->headanimcnt);
    }

#if 1

  plantblood->shootdelay--;

  if (plantblood->shootdelay == 11) plantblood->headanimcnt = 22;


  if (!plantblood->shootdelay)
    {
      INT16 xoffset,yoffset;
      INT16 destx,desty;
      INT16 shootspeed;

      shootspeed = 100;

      plantblood->shootdelay = MC_RANDOM(50)+25;

      if (plantblood->elements[15]->visible)
	{

	  destx = (hoi->x+(hoi->sizex/2));
	  desty = (hoi->y+(hoi->sizey/2));
	  
	  xoffset = 16; //(plantblood->elements[15]->sizex/2);
	  yoffset = (plantblood->elements[15]->sizey/2);
	  
	  xoffset += plantblood->elements[15]->x;
	  yoffset += plantblood->elements[15]->y;
	  
      play_segmshot(xoffset, yoffset);
	  mine_init(xoffset, yoffset, ((destx - xoffset)*256)/shootspeed, ((desty-yoffset)*256)/shootspeed, orgmine, 0, 100, 1, 1, 0);
//	  plof_init(xoffset, yoffset, 1, 0, 0, 0, 0);
	}
    }

#endif


  if (!plantblood->fallspd)
    {
      if ((UINT16)(plantblood->rock->x + 20 - object->x) < 40)
	{
	  //	  player1.score += 50;
	  plantblood->startamp += 6;
	  plantblood->extraamp = 48;
      play_segmhit(plantblood->rock->x, plantblood->rock->y);
    }
    }

  if (plantblood->extraamp) plantblood->extraamp -= 4;

  return 0;
}

void plant_death(OBJECT *object, UINT32 param)
{
}

void plant_clear(OBJECT *object, UINT32 param)
{
}


