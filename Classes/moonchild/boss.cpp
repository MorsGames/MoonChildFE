#include <frm_wrk.hpp>
#include <boss.hpp>
#include <mine.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>

UINT16 bosspostab[16] = 
{
 2308-400, 1980,
 2308    , 1980,
 2308+400, 1980,

 2308-400, 2210,
 2308    , 2210,
 2308+400, 2210,

 1594    , 2050,
 2974    , 2050
};


OBJECT *boss_init(UINT16 x, UINT16 y)
{
  UINT16 i;
  OBJECT *boss;
  boss = (OBJECT *) malloc (sizeof(OBJECT));
  boss->x = x;
  boss->y = y;
  boss->live  = boss_live;
  boss->death = boss_death;
  boss->clear = boss_clear;
  boss->blitsizex = 0;
  boss->blitsizey = 0;
  boss->blitstartx = 0;
  boss->blitstarty = 0;

  boss->lethal = 0; //harmless

  boss->animate = 0;
  boss->frame = 0;


  boss->sizex = 0;
  boss->sizey = 0;

  boss->blitsizex = boss->sizex;
  boss->blitsizey = boss->sizey;
  boss->blitstartx = 0;
  boss->blitstarty = 0;
  boss->coloffsetx = 12;
  boss->coloffsety = 12;
  boss->colwidth = 64 - 24;
  boss->colheight= 64 - 24;


  boss->blood = (void *) malloc(sizeof(BOSS_BLOOD));
//  vgassert(boss->blood);
  BOSS_BLOOD *bossblood;

  bossblood = (BOSS_BLOOD *)boss->blood;
  bossblood->destx       = x;
  bossblood->desty       = y;

  bossblood->srcx        = x;
  bossblood->srcy        = y;
  bossblood->movetimeout = 50;
  bossblood->movefactor  = 0;

  bossblood->minedelay = 50;
  bossblood->bossmode = 1;

  bossblood->sinamp = 200;
  bossblood->sinspd = 5;
  bossblood->sincnt = 0;

  bossblood->openup = 0;
  bossblood->opendir = 0;
  bossblood->opendelay = 150;

  bossblood->hit  = 77;

  {               // heart
    OBJECT *temp;
    temp = (OBJECT *) malloc (sizeof(OBJECT));
    temp->x = x;
    temp->y = y;
    temp->live  = 0;
    temp->death = 0;
    temp->clear = 0;
    temp->blitsizex = 0;
    temp->blitsizey = 0;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    
    temp->lethal = 1; //harmless
    
    temp->animate = copy_anim((ANIM *)orgendoflevel);
    temp->frame = anim_setsequence(temp->animate, 4, SEQ_FORCE);
    
    
    temp->sizex = temp->frame->get_width();
    temp->sizey = temp->frame->get_height();
    
    temp->blitsizex = temp->sizex;
    temp->blitsizey = temp->sizey;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    temp->coloffsetx = 12;
    temp->coloffsety = 12;
    temp->colwidth = temp->sizex - 24;
    temp->colheight= temp->sizey - 24;
    
    temp->blood = 0;
    bossblood->heart = temp;
    object_add(temp);
  }
  {               // topleft
    OBJECT *temp;
    temp = (OBJECT *) malloc (sizeof(OBJECT));
    temp->x = x-32;
    temp->y = y-32;
    temp->live  = 0;
    temp->death = 0;
    temp->clear = 0;
    temp->blitsizex = 0;
    temp->blitsizey = 0;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    
    temp->lethal = 1; //harmless
    
    temp->animate = copy_anim((ANIM *)orgendoflevel);
    temp->frame = anim_setsequence(temp->animate, 0, SEQ_FORCE);
    
    
    temp->sizex = temp->frame->get_width();
    temp->sizey = temp->frame->get_height();
    
    temp->blitsizex = temp->sizex;
    temp->blitsizey = temp->sizey;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    temp->coloffsetx = 12;
    temp->coloffsety = 12;
    temp->colwidth = temp->sizex - 24;
    temp->colheight= temp->sizey - 24;
    
    temp->blood = 0;
    bossblood->topleft = temp;
    object_add(temp);
  }
  {               // top right
    OBJECT *temp;
    temp = (OBJECT *) malloc (sizeof(OBJECT));
    temp->x = x+32;
    temp->y = y-32;
    temp->live  = 0;
    temp->death = 0;
    temp->clear = 0;
    temp->blitsizex = 0;
    temp->blitsizey = 0;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    
    temp->lethal = 1; //harmless
    
    temp->animate = copy_anim((ANIM *)orgendoflevel);
    temp->frame = anim_setsequence(temp->animate, 1, SEQ_FORCE);
    
    
    temp->sizex = temp->frame->get_width();
    temp->sizey = temp->frame->get_height();
    
    temp->blitsizex = temp->sizex;
    temp->blitsizey = temp->sizey;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    temp->coloffsetx = 12;
    temp->coloffsety = 12;
    temp->colwidth = temp->sizex - 24;
    temp->colheight= temp->sizey - 24;
    
    temp->blood = 0;
    bossblood->topright = temp;
    object_add(temp);
  }
  {               // bottom left
    OBJECT *temp;
    temp = (OBJECT *) malloc (sizeof(OBJECT));
    temp->x = x-32;
    temp->y = y+32;
    temp->live  = 0;
    temp->death = 0;
    temp->clear = 0;
    temp->blitsizex = 0;
    temp->blitsizey = 0;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    
    temp->lethal = 1; //harmless
    
    temp->animate = copy_anim((ANIM *)orgendoflevel);
    temp->frame = anim_setsequence(temp->animate, 2, SEQ_FORCE);
    
    
    temp->sizex = temp->frame->get_width();
    temp->sizey = temp->frame->get_height();
    
    temp->blitsizex = temp->sizex;
    temp->blitsizey = temp->sizey;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    temp->coloffsetx = 12;
    temp->coloffsety = 12;
    temp->colwidth = temp->sizex - 24;
    temp->colheight= temp->sizey - 24;
    
    temp->blood = 0;
    bossblood->bottomleft = temp;
    object_add(temp);
  }
  {               // bottom right
    OBJECT *temp;
    temp = (OBJECT *) malloc (sizeof(OBJECT));
    temp->x = x+32;
    temp->y = y+32;
    temp->live  = 0;
    temp->death = 0;
    temp->clear = 0;
    temp->blitsizex = 0;
    temp->blitsizey = 0;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    
    temp->lethal = 1; //harmless
    
    temp->animate = copy_anim((ANIM *)orgendoflevel);
    temp->frame = anim_setsequence(temp->animate, 3, SEQ_FORCE);
    
    
    temp->sizex = temp->frame->get_width();
    temp->sizey = temp->frame->get_height();
    
    temp->blitsizex = temp->sizex;
    temp->blitsizey = temp->sizey;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    temp->coloffsetx = 12;
    temp->coloffsety = 12;
    temp->colwidth = temp->sizex - 24;
    temp->colheight= temp->sizey - 24;
    
    temp->blood = 0;
    bossblood->bottomright = temp;
    object_add(temp);
  }

  for (i=0; i<64; i++)
    {               // bottom right
    OBJECT *temp;
    temp = (OBJECT *) malloc (sizeof(OBJECT));
    temp->x = x+32;
    temp->y = y+32;
    temp->live  = 0;
    temp->death = 0;
    temp->clear = 0;
    temp->blitsizex = 0;
    temp->blitsizey = 0;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    
    temp->lethal = 0; //harmless
    
    temp->animate = copy_anim((ANIM *)orgdiamond);
    temp->frame = 0;
    
    temp->sizex = 32;
    temp->sizey = 32;
    
    temp->blitsizex = temp->sizex;
    temp->blitsizey = temp->sizey;
    temp->blitstartx = 0;
    temp->blitstarty = 0;
    temp->coloffsetx = 6;
    temp->coloffsety = 6;
    temp->colwidth = temp->sizex - 12;
    temp->colheight= temp->sizey - 12;
    
    temp->blood = 0;
    bossblood->plof[i] = temp;
    object_add(temp);
  }

  bossblood->plofamp = 600;
  bossblood->plofsincnt1x = 0;
  bossblood->plofsincnt2x = 0;
  bossblood->plofsincnt1y = 256;
  bossblood->plofsincnt2y = 256;

  bossblood->madecnt = 0;

  object_add(boss);

  return boss;
}


int boss_live (OBJECT *object, UINT32 param)
{
  BOSS_BLOOD *bossblood;
  INT32 destx,desty;
  INT32 x,y;
  INT16 xoffset,yoffset;
  UINT16 shootspeed;


  if (paraflg < 5) return 0;

  bossblood = (BOSS_BLOOD *)object->blood; /* in case of multiple bosss */

  if (bossblood->movetimeout)
    {
      bossblood->movetimeout--;
      if (!bossblood->movetimeout)
	{
	  UINT16 index;

	  bossblood->movefactor = MC_RANDOM(100)+200;
	  bossblood->movespeed = bossblood->movefactor;

	  bossblood->srcx = bossblood->destx;
	  bossblood->srcy = bossblood->desty;

	  index = MC_RANDOM(8);
	  index &= 7;
	  index <<= 1;

	  bossblood->destx = bosspostab[index  ];
	  bossblood->desty = bosspostab[index+1];

	}
    }

  if (bossblood->movefactor)
    {
      bossblood->movefactor--;
      if (!bossblood->movefactor)
	{
	  bossblood->movetimeout = MC_RANDOM(50)+10;
	}

      INT32 t;

      t = bossblood->srcx - bossblood->destx;
      t *= bossblood->movefactor;
      t /= bossblood->movespeed;
      object->x = t + bossblood->destx;

      t = bossblood->srcy - bossblood->desty;
      t *= bossblood->movefactor;
      t /= bossblood->movespeed;
      object->y = t + bossblood->desty;

    }

  bossblood->heart->x = object->x;
  bossblood->heart->y = object->y;
  
  bossblood->topleft->x = object->x-32-bossblood->openup;
  bossblood->topleft->y = object->y-32-bossblood->openup;
  
  bossblood->topright->x = object->x+32+bossblood->openup;
  bossblood->topright->y = object->y-32-bossblood->openup;
  
  bossblood->bottomleft->x = object->x-32-bossblood->openup;
  bossblood->bottomleft->y = object->y+32+bossblood->openup;
  
  bossblood->bottomright->x = object->x+32+bossblood->openup;
  bossblood->bottomright->y = object->y+32+bossblood->openup;

// open up gedeelte

  bossblood->opendelay--;
  if (!bossblood->opendelay)
    {
      bossblood->opendir^=1;
      if (bossblood->opendir)
	{
	  bossblood->opendelay = MC_RANDOM(150)+30;
	}
      else
	{
	  bossblood->opendelay = MC_RANDOM(100)+100;
	}
    }

  if (bossblood->opendir)
    {
      if (bossblood->openup <20)
	{
	  bossblood->openup++;
	}
    }
  else
    {
      if (bossblood->openup >0)
	{
	  bossblood->openup--;
	}
    }


  
  // shoot gedeelte

  xoffset = object->x + 32;
  yoffset = object->y + 32;
  
  destx = (hoi->x+(hoi->sizex/2));
  desty = (hoi->y+(hoi->sizey/2));
  
  if (bossblood->hit != 0)   // is ie al dood?
  {
    if (((UINT16)(xoffset - destx + 320) < 640) && ((UINT16)(yoffset - desty + 320) < 640))
    {
      if ((bossblood->srcx == bossblood->destx) && (bossblood->srcy == bossblood->desty))    // same then spray
	  {
	    bossblood->sincnt += bossblood->sinspd;
	    bossblood->sincnt &= 1023;
	  
	    if ((bossblood->movefactor & 15) == 15)
	    {
	      x = ((sinus512[ bossblood->sincnt          ] * bossblood->sinamp) >> 8);
	      y = ((sinus512[(bossblood->sincnt+256)&1023] * bossblood->sinamp) >> 8);
	      
	      destx = xoffset + x;
	      desty = yoffset + y;
	      
	      shootspeed = 100;
	      
	      mine_init(xoffset, yoffset, ((destx - xoffset)*256)/shootspeed, ((desty-yoffset)*256)/shootspeed, orgmine, 0, 140, 1, 2, 1);
	    }
	  }
      else
	  {
	    bossblood->minedelay--;
	    if (!bossblood->minedelay)
	    {
		  UINT16 hoek;
		  hoek = MC_RANDOM(1024);
		  hoek &= 1023;
	      bossblood->minedelay = MC_RANDOM(80)+80;

	      x = ((sinus512[ hoek          ] * bossblood->sinamp) >> 8);
	      y = ((sinus512[(hoek+256)&1023] * bossblood->sinamp) >> 8);
	      
	      destx = xoffset + x;
	      desty = yoffset + y;
	      
	      shootspeed = 100;
	      
	      mine_init(xoffset, yoffset, ((destx - xoffset)*256)/shootspeed, ((desty-yoffset)*256)/shootspeed, orgmine, 0, 140, 1, 2, 1);
		
		}
	  }
    }
  }


  // ploffing

  if (bossblood->hit == 0)
  {
    INT32 i,s1x, s2x, s1y, s2y, x, y;


    bossblood->madecnt++;

	mc_autorun = 2;   // dummy value so Mon Child can't move anymore!
    camera_override(&player1, object->x+32, object->y, 8);


	if (bossblood->madecnt == 150) play_madeit();

    if (bossblood->madecnt > 250)
	{
      bossblood->plofamp += 2;
      streepflg = 1;   // laat alle rommel op beeld staan  (unlimited bobs)
	}
	else
	{
      if (bossblood->plofamp >80) bossblood->plofamp -= 8;
	}

    if (bossblood->madecnt == 500) nextlvlflg = 1;

	bossblood->heart->lethal = 0;
    bossblood->topleft->lethal = 0;
    bossblood->topright->lethal = 0;
    bossblood->bottomleft->lethal = 0;
    bossblood->bottomright->lethal = 0;


    bossblood->plofsincnt1x += 9;
    bossblood->plofsincnt1y += 9;
    bossblood->plofsincnt2x += 15;
    bossblood->plofsincnt2y += 15;

	s1x = bossblood->plofsincnt1x;
	s2x = bossblood->plofsincnt2x;
	s1y = bossblood->plofsincnt1y;
	s2y = bossblood->plofsincnt2y;

	for (i=0; i<64; i++)
	{
      bossblood->plof[i]->frame = anim_forceframe(bossblood->plof[i]->animate, 0);

      x =  ((sinus512[s1x&1023] * (bossblood->plofamp)) );
      y =  ((sinus512[s1y&1023] * (bossblood->plofamp)) );
      x += ((sinus512[s2x&1023] * (bossblood->plofamp)) );
      y += ((sinus512[s2y&1023] * (bossblood->plofamp)) );

      s1x += 16;
      s1y += 16;
      s2x += 64;
      s2y += 64;

      bossblood->plof[i]->x = (x >> 10) + object->x + 16;
      bossblood->plof[i]->y = (y >> 10) + object->y + 16;
	}
  }
  return 0;
}

void boss_death(OBJECT *object, UINT32 param)
{
}

void boss_clear(OBJECT *object, UINT32 param)
{
}



