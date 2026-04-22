#include <frm_wrk.hpp>
#include <hoi.hpp>
#include <bouncey.hpp>
#include <cloud.hpp>
#include <bullet.hpp>
#include <plof.hpp>
#include <sokoban.hpp>
#include <bonus.hpp>
#include <smlheart.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <switsj.hpp>
#include <sound.hpp>


char idleanim[496] = {
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,3,3,3,3,2,2,2,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,
  3,3,3,3,2,2,2,2,1,1,1,1,2,2,2,2,
  3,3,3,3,2,2,2,2,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,3,3,3,3,2,2,2,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,3,3,3,3,2,2,2,2,1,1,1,1,
  2,2,2,2,3,3,3,3,2,2,2,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  4,4,4,4,5,5,5,5,4,4,4,4,1,1,1,1,
  4,4,4,4,5,5,5,5,4,4,4,4,1,1,1,1,
  4,4,4,4,5,5,5,5,4,4,4,4,1,1,1,1,
  4,4,4,4,5,5,5,5,4,4,4,4,1,1,1,1,
  4,4,4,4,1,1,1,1,2,2,2,2,3,3,3,3,
  2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
  4,4,4,4,5,5,5,5,4,4,4,4,1,1,1,1,
  4,4,4,4,5,5,5,5,4,4,4,4,1,1,1,1,
  4,4,4,4,4,4,4,1,1,1,1,1,1,1,1,1,
  2,2,2,2,3,3,3,3,2,2,2,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,3,3,3,3,2,2,2,2,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,
  1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3
};

char blinkanim2[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

  
/*
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
*/
 
};


UINT16 flashtab[200] = {
  1,0,0,0,0,0,1,1,1,1,
  1,0,0,0,0,0,1,1,1,1,
  1,0,0,0,0,0,1,1,1,1,
  1,0,0,0,0,0,1,1,1,1,
  1,0,0,0,0,1,1,1,1,0,
  0,0,0,1,1,1,1,0,0,0,
  0,1,1,1,1,0,0,0,0,1,
  1,1,1,0,0,0,0,1,1,1,
  1,0,0,0,1,1,1,0,0,0,
  1,1,1,0,0,0,1,1,1,0,
  0,0,1,1,1,0,0,0,1,1,
  1,0,0,0,1,1,1,0,0,0,
  1,1,0,0,1,1,0,0,1,1,
  0,0,1,1,0,0,1,1,0,0,
  1,1,0,0,1,1,0,0,1,1,
  0,0,1,1,0,0,1,1,0,0,
  1,0,1,0,1,0,1,0,1,0,
  1,0,1,0,1,0,1,0,1,0,
  1,0,1,0,1,0,1,0,1,0,
  1,0,1,0,1,0,1,0,1,0,
};


void reinithoi(void);


OBJECT *hoi_globinit(ANIM *appearance, HOI_CAPS* capabilities, HOI_AI intelligence, UINT16 lethal, OBJECT *pal)
{
  OBJECT *newhoi;
  HOI_BLOOD *hoiblood;

  newhoi = (OBJECT *) malloc (sizeof(OBJECT));
  newhoi->x = 0;
  newhoi->y = 0;
  newhoi->live  = NULL;
  newhoi->death = hoi_death;
  newhoi->clear = hoi_clear;
  newhoi->frame = 0;

  newhoi->animate = (ANIM *)copy_anim(appearance);
  newhoi->frame = anim_setsequence(newhoi->animate, 0, SEQ_FORCE);

  newhoi->sizex = newhoi->frame->get_width();
  newhoi->sizey = newhoi->frame->get_height();

  newhoi->blitsizex = newhoi->sizex;
  newhoi->blitsizey = newhoi->sizey;
  newhoi->blitstartx = 0;
  newhoi->blitstarty = 0;
  newhoi->lethal = lethal;
  newhoi->coloffsetx = 6;
  newhoi->coloffsety = 6;
  newhoi->colwidth = newhoi->sizex - 6;
  newhoi->colheight= newhoi->sizey - 6;
  newhoi->blood = (void *) malloc(sizeof(HOI_BLOOD));
//  vgassert(newhoi->blood);
  hoiblood = (HOI_BLOOD *)newhoi->blood;
  hoiblood->gravy = 0;
  hoiblood->spdx  = 0;
  hoiblood->curdirx  = 0;
  hoiblood->lastdirx = -1;
  hoiblood->onfloor  = 0;
  hoiblood->floor = 0;
  hoiblood->animover = 0;
  hoiblood->anmturn = 0;
  hoiblood->anmdir = 0;
  hoiblood->kneelcnt = 0;
  hoiblood->thudcnt = 0;
  hoiblood->thuddir = 0;
  hoiblood->onobject = 0;
  hoiblood->holddown = 0;
  hoiblood->leancnt = 0;
  hoiblood->leandir = 0;
  hoiblood->idlecnt = 0;
  hoiblood->idleanim = 0;
  hoiblood->xahead = 0;   /* when running camera focusses better */
  hoiblood->yahead = 0;   /* when jumping camera focusses better */
  hoiblood->hoi_caps = capabilities;
  hoiblood->hoi_ai = intelligence;
  hoiblood->pal = pal;

  hoiblood->hoi_caps->maxspd = 140;   // wonder shoes uitzetten


  hoiblood->leftkey=0;  /* if set we want to move left */
  hoiblood->rightkey=0; /* if set we want to move right */
  hoiblood->upkey=0;    /* if set we want to move up */
  hoiblood->downkey=0;  /* if set we want to move down */
  hoiblood->shootkey=0; /* if set we want to shoot */
  hoiblood->flashcnt=0; /* invincibility */
  hoiblood->xfloat = 0;   /* x-pos 256 times too big.*/
  hoiblood->yfloat = 0;   /* y-pos 256 times too big.*/
  hoiblood->xspd=0;     /* x acceleration */
  hoiblood->yspd=0;     /* y-acceleration */
  hoiblood->hoovercnt=0;/* current hover position */
  hoiblood->exhaustcnt=0; /* counter for exhaust */
  hoiblood->xcamxtra=0; /* extra add for camera in X */
  hoiblood->ycamxtra=0; /* extra add for camera in X */
  
  object_add(newhoi);
  return newhoi;
}


void hoi_init(OBJECT *newhoi, UINT16 x, UINT16 y)
{
  newhoi->x = x;
  newhoi->y = y;

  newhoi->live  = hoi_live;
  newhoi->frame = anim_setsequence(newhoi->animate, SEQ_WAITRIGHT, SEQ_FORCE);
  newhoi->frame = anim_forceframe(newhoi->animate, 1);
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)newhoi->blood;
  hoiblood->gravy = 0;
  hoiblood->spdx  = 0;
  hoiblood->curdirx  = 0;
  hoiblood->lastdirx = -1;
  hoiblood->onfloor  = 0;
  hoiblood->floor = 0;
  hoiblood->animover = 0;
  hoiblood->anmturn = 0;
  hoiblood->anmdir = 0;
  hoiblood->kneelcnt = 0;
  hoiblood->thudcnt = 0;
  hoiblood->thuddir = 0;
  hoiblood->onobject = 0;
  hoiblood->holddown = 0;
  hoiblood->leancnt = 0;
  hoiblood->leandir = 0;
  hoiblood->idlecnt = 510;
  hoiblood->idleanim = 7;
  hoiblood->yahead = 0;
  hoiblood->xahead = 0;
  hoiblood->blinkanim2cnt = 0;
  hoiblood->leftkey = 0;
  hoiblood->rightkey = 0;
  hoiblood->upkey = 0;
  hoiblood->downkey = 0;
  hoiblood->shootkey = 0;
//  hoiblood->flashcnt = 0;     this one not

  hoiblood->xfloat = x<<8;
  hoiblood->yfloat = y<<8;
  hoiblood->xspd = 0;
  hoiblood->yspd = 0;
  hoiblood->hoovercnt = 0;
  hoiblood->xcamxtra = 0;
  hoiblood->ycamxtra = 0;
}

void hoi_fly_init(OBJECT *newhoi, UINT16 x, UINT16 y)
{
  newhoi->x = x;
  newhoi->y = y;

  newhoi->live  = hoi_fly_live;

  newhoi->frame = anim_setsequence(newhoi->animate, SEQ_TURNLEFT, SEQ_FORCE);
  newhoi->frame = anim_forceframe(newhoi->animate, 1);

  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)newhoi->blood;
  hoiblood->gravy = 0;
  hoiblood->spdx  = 0;
  hoiblood->curdirx  = 0;
  hoiblood->lastdirx = -1;

  if (x & 1)   hoiblood->lastdirx = 1;
  
  hoiblood->onfloor  = 0;
  hoiblood->floor = 0;
  hoiblood->animover = 0;
  hoiblood->anmturn = 0;
  hoiblood->anmdir = 0;
  hoiblood->kneelcnt = 0;
  hoiblood->thudcnt = 0;
  hoiblood->thuddir = 0;
  hoiblood->onobject = 0;
  hoiblood->holddown = 0;
  hoiblood->leancnt = 0;
  hoiblood->leandir = 0;
  hoiblood->idlecnt = 0;
  hoiblood->idleanim = 0;
  hoiblood->yahead = 0;
  hoiblood->xahead = 0;
  hoiblood->blinkanim2cnt = 0;
  hoiblood->leftkey = 0;
  hoiblood->rightkey = 0;
  hoiblood->upkey = 0;
  hoiblood->downkey = 0;
  hoiblood->shootkey = 0;
//  hoiblood->flashcnt = 0;   this one not!!!

  hoiblood->xfloat = x<<8;
  hoiblood->yfloat = y<<8;
  hoiblood->xspd = 0;
  hoiblood->yspd = 0;
  hoiblood->hoovercnt = 0;
  hoiblood->xcamxtra = 0;
  hoiblood->ycamxtra = 0;
}


UINT16 hoi_ai(OBJECT *object)
{
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *) object->blood;

  if (sokomoved)
  {
    char x,y;

	x = sokomoved & 255;
	y = (sokomoved>>8) & 255;

	if (x>0) hoiblood->rightkey = 1;
	if (x<0) hoiblood->leftkey = 1;
	if (y>0) hoiblood->downkey = 1;
	if (y<0) hoiblood->upkey = 1;

  }
  else
  {
  
    if(mc_autorun)
    {
      hoiblood->leftkey  = 0;
      hoiblood->rightkey = 0;
      if (mc_autorun ==  1) hoiblood->rightkey = 1;
      if (mc_autorun == -1) hoiblood->leftkey  = 1;
      hoiblood->upkey = 0;
      hoiblood->downkey = 0;
      hoiblood->shootkey = 0;
    }
    else
    {
      hoiblood->leftkey = leftkey;
      hoiblood->rightkey = rightkey;
      if (object->live == hoi_fly_live)
        hoiblood->upkey = upkey;
      else
        hoiblood->upkey = jumpkey;
      hoiblood->downkey = downkey;
      hoiblood->shootkey = shootkey;
    }
  }
  sokomoved = 0;
  
  return 0;
}

UINT16 elf_ai(OBJECT *object)
{
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *) object->blood;

  if (hoi->y > object->y)
    {
      //    hoiblood->upkey = 1;
    }
  else
    {
      hoiblood->upkey = 0;
    }
  
  if (hoi->x > object->x) 
    {
      hoiblood->rightkey = 1;
    }
  else
    {
      hoiblood->rightkey = 0;
    }
  return 0;
}


UINT16 bomb_ai(OBJECT *object)
{
  HOI_BLOOD *hoiblood;
  SWITSJ_BLOOD *switsjblood;

  object->lethal = 1;
  
  
  hoiblood = (HOI_BLOOD *) object->blood;

  if (hoi->y > object->y)
    {
      //    hoiblood->upkey = 1;
    }
  else
    {
      hoiblood->upkey = 0;
    }
  
  hoiblood->rightkey = 0;
  hoiblood->leftkey = 1;
      
  switsjblood = (SWITSJ_BLOOD *) switch4->blood;
  if (object->y >= 2416)
  {
    if (switsjblood->dir == 1)
    {
      hoiblood->rightkey = 1;
      hoiblood->leftkey = 0;
    }
    else
    {
      hoiblood->rightkey = 0;
      hoiblood->leftkey = 1;
    }
  }


  switsjblood = (SWITSJ_BLOOD *) switch3->blood;
  if (object->y >= (2416+128))
  {
    if (switsjblood->dir == 1)
    {
      hoiblood->rightkey = 1;
      hoiblood->leftkey = 0;
    }
    else
    {
      hoiblood->rightkey = 0;
      hoiblood->leftkey = 1;
    }
  }

  switsjblood = (SWITSJ_BLOOD *) switch2->blood;
  if (object->y >= (2416+256))
  {
    if (switsjblood->dir == 1)
    {
      hoiblood->rightkey = 1;
      hoiblood->leftkey = 0;
    }
    else
    {
      hoiblood->rightkey = 0;
      hoiblood->leftkey = 1;
    }
  }


  switsjblood = (SWITSJ_BLOOD *) switch1->blood;
  if (object->y >= (2416+384) && object->y <= (2416+404))
  {
    if (switsjblood->dir == 1)
    {
      hoiblood->rightkey = 1;
      hoiblood->leftkey = 0;
    }
    else
    {
      hoiblood->rightkey = 0;
      hoiblood->leftkey = 1;
    }
  }
//  log_out("bombx = %d, bomby = %d",object->x, object->y);

if (object->y == 2880) // helmut zit in holletje
{
  if (object->x > 8287-10 && object->x < 8287+10)  // gat1?
  {
    if (gat1 == 0)
    {
      gat1 = 1;
    }
    else
    {
      return 1;
    }
  }

  if (object->x > 8448-10 && object->x < 8448+10)  // gat1?
  {
    if (gat2 == 0)
    {
      gat2 = 1;
    }
    else
    {
      return 1;
    }
  }

  if (object->x > 8671-10 && object->x < 8671+10)  // gat1?
  {
    if (gat3 == 0)
    {
      gat3 = 1;
    }
    else
    {
      return 1;
    }
  }

  if (object->x > 8862-10 && object->x < 8862+10)  // gat1?
  {
    if (gat4 == 0)
    {
      gat4 = 1;
    }
    else
    {
      return 1;
    }
  }

  if (object->x > 9023-10 && object->x < 9023+10)  // gat1?
  {
    if (gat5 == 0)
    {
      gat5 = 1;
    }
    else
    {
      return 1;
    }
  }

  if (object->x > 9184-10 && object->x < 9184+10)  // gat1?
  {
    if (gat6 == 0)
    {
      gat6 = 1;
    }
    else
    {
      return 1;
    }
  }

  if (object->x > 9344-10 && object->x < 9344+10)  // gat1?
  {
    if (gat7 == 0)
    {
      gat7 = 1;
    }
    else
    {
      return 1;
    }
  }
}

  //  if (hoi->x > object->x) 
//    {
//      hoiblood->rightkey = 1;
//    }
//  else
//    {
//      hoiblood->rightkey = 0;
//    }
return 0;
}


UINT16 helmut_ai(OBJECT *object)
{
  OBJECT *helmut;
  HOI_BLOOD *hoiblood;
  UINT16 *delaypos;
  UINT16 i;

  hoiblood = (HOI_BLOOD *) object->blood;

  helmut = hoiblood->pal;
  delaypos = (UINT16 *) helmut->blood;

  if (object->y < hoi->y+170 && object->y > hoi->y+70 && hoi->x < object->x+60 && hoi->x > object->x-60)
    {
      delaypos[8]++;
      hoiblood->upkey = 0;
      if (delaypos[8]>50)
	{
	  hoiblood->upkey = 1;
	}
    }
  else
    {
      delaypos[8] = 0;
      hoiblood->upkey = 0;
    }

  if (object->y < hoi->y-10 && object->y > hoi->y-70 && hoi->x < object->x+60 && hoi->x > object->x-60)
    {
      hoiblood->downkey = 1;
    }
  else
    {
      hoiblood->downkey = 0;
    }

  if (hoi->x > object->x+20 && hoi->x < object->x+260 && object->y < hoi->y+170 && object->y > hoi->y-100) 
    {
      hoiblood->rightkey = 1;
    }
  else
    {
      hoiblood->rightkey = 0;
    }

  if (hoi->x < object->x-20 && hoi->x > object->x-260 && object->y < hoi->y+170 && object->y > hoi->y-100 ) 
    {
      hoiblood->leftkey = 1;
    }
  else
    {
      hoiblood->leftkey = 0;
    }


  for (i=0; i<6; i++)
    {
      delaypos[i] = delaypos[i+2];
    };


  delaypos[0] = object->x;
  delaypos[7] = object->y-30 + ((sinus512[(object->x*8) & 1023] * 16) >> 10);

  helmut->x = delaypos[0];
  helmut->y = delaypos[1] + (hoiblood->kneelcnt*2);

  helmut->frame = anim_setsequence(helmut->animate, (hoiblood->curdirx == 1) ? 0 : 1, SEQ_FORCE);
  return 0;
}

UINT16 helmut2_ai(OBJECT *object)
{
  OBJECT *helmut;
  HOI_BLOOD *hoiblood;
  UINT16 *delaypos;
  UINT16 i;

  hoiblood = (HOI_BLOOD *) object->blood;

  helmut = hoiblood->pal;
  delaypos = (UINT16 *) helmut->blood;


  if (object->y < hoi->y+170 && object->y > hoi->y+70 && hoi->x < object->x+60 && hoi->x > object->x-60)
    {
      delaypos[8]++;
      hoiblood->upkey = 0;
      if (delaypos[8]>50)
	{
	  hoiblood->upkey = 1;
	}
    }
  else
    {
      delaypos[8] = 0;
      hoiblood->upkey = 0;
    }

  if (hoiblood->onfloor > 0 && hoiblood->onfloor < 8)
    {
      if (hoi->y - 20 < object->y)
	{
	  hoiblood->upkey = 1;
	}
    }
  

  if (object->y < hoi->y-10 && object->y > hoi->y-70 && hoi->x < object->x+60 && hoi->x > object->x-60)
    {
      hoiblood->downkey = 1;
    }
  else
    {
      hoiblood->downkey = 0;
    }

  if (hoi->x > object->x+20 && hoi->x < object->x+310 && object->y < hoi->y+220 && object->y > hoi->y-150) 
    {
      hoiblood->rightkey = 1;
    }
  else
    {
      hoiblood->rightkey = 0;
    }

  if (hoi->x < object->x-20 && hoi->x > object->x-310 && object->y < hoi->y+220 && object->y > hoi->y-150 ) 
    {
      hoiblood->leftkey = 1;
    }
  else
    {
      hoiblood->leftkey = 0;
    }


  for (i=0; i<6; i++)
    {
      delaypos[i] = delaypos[i+2];
    };


  delaypos[0] = object->x;
  delaypos[7] = object->y-30 + ((sinus512[(object->x*8) & 1023] * 16) >> 10);

  helmut->x = delaypos[0];
  helmut->y = delaypos[1] + (hoiblood->kneelcnt*2);

  helmut->frame = anim_setsequence(helmut->animate, (hoiblood->curdirx == 1) ? 0 : 1, SEQ_FORCE);
  return 0;
}




void helmut_init(int x, int y, int iq, HOI_CAPS *caps)
{
  OBJECT *obj,*helmut;
  UINT16 *helmblood;

  obj = (OBJECT *) malloc (sizeof(OBJECT));
  obj->x = 0;
  obj->y = 0;
  obj->live  = NULL;
  obj->death = NULL;
  obj->clear = NULL;

  obj->animate = (ANIM *)copy_anim(orghelmut);
  obj->frame = anim_setsequence(obj->animate, 0, SEQ_FORCE);

  obj->sizex = obj->frame->get_width();
  obj->sizey = obj->frame->get_height();

  obj->blitsizex = obj->sizex;
  obj->blitsizey = obj->sizey;
  obj->blitstartx = 0;
  obj->blitstarty = 0;

  obj->lethal = 1;

  obj->coloffsetx = 10;
  obj->coloffsety = 10;
  obj->colwidth = obj->sizex - 10;
  obj->colheight= obj->sizey - 10;

  obj->blood = (UINT16 *) calloc(16*2*(sizeof(UINT16)), sizeof(UINT16));

  if (iq == 0)
    {
      helmut = hoi_globinit(orgshoes, caps, helmut_ai, 0, obj);
    }
  else
    {
      helmut = hoi_globinit(orgshoes, caps, helmut2_ai, 0, obj);
    }

  helmblood = (UINT16 *)obj->blood;

  helmblood[9] = 13;  // extra hearts when dead!

  hoi_init(helmut, x, y);

  object_add(obj); // helm adden
}


int helmut_dwarrel(OBJECT *object, UINT32 param)
{
  UINT16 *helmblood;


  helmblood = (UINT16 *)object->blood; /* in case of multiple hois */

  if (editflg) return 0; /* if editor is on, mousecontrol takes Hoi over */

  if (!collision(object->x + ((object->sizex)/2) , object->y + (object->sizey) ))
    {
      object->y++;
      if ((object->y & 31) < 16)
	{
	  object->x--;
	}
      else
	{
	  object->x++;
	}
    }

  if (helmblood[9])
  {
    helmblood[9]--;

    if (helmblood[9] % 4 == 0) 
    {
      smlheart_init(object->x, object->y, 1, 8);
    }
  }
  
  return 0;
}


int hoi_live_warp (OBJECT *object, UINT32 param)
{
  HOI_BLOOD *hoiblood;

  hoiblood = (HOI_BLOOD *)  hoi->blood;
  
  if (hoiblood->xahead > 0)
  {
    hoiblood->xahead -= 4;
    if (hoiblood->xahead < 0) hoiblood->xahead = 0;
  }
  else
  {
    hoiblood->xahead += 4;
    if (hoiblood->xahead > 0) hoiblood->xahead = 0;
  }

  if (hoiblood->yahead > 0)
  {
    hoiblood->yahead -= 4;
    if (hoiblood->yahead < 0) hoiblood->yahead = 0;
  }
  else
  {
    hoiblood->yahead += 4;
    if (hoiblood->yahead > 0) hoiblood->yahead = 0;
  }

  hoiblood->xspd = 0;
  hoiblood->yspd = 0;

  if (hoiblood->holddown > 0)
  {
    hoiblood->holddown -= 4;
    if (hoiblood->holddown < 0) hoiblood->holddown = 0;
  }

  hoiblood->curdirx = 0;

  if (hoiblood->flashcnt)
    {
//      hoiblood->flashcnt = 0;
      object->blitsizey = object->sizey;
    }
  
  hoiblood->xfloat = object->x<<8;
  hoiblood->yfloat = object->y<<8;

  if (world == 2) set_backpak_volume(-2100);

  if (hoiblood->ycamxtra > 4 ) hoiblood->ycamxtra -= 4;
  if (hoiblood->ycamxtra < -4) hoiblood->ycamxtra += 4;
  if (hoiblood->xcamxtra > 4 ) hoiblood->xcamxtra -= 4;
  if (hoiblood->xcamxtra < -4) hoiblood->xcamxtra += 4;

  return 0;

}
  
  
int hoi_live (OBJECT *object, UINT32 param)
{
  HOI_BLOOD *hoiblood;
  short int y;
  int remem;
  UINT16 checkbounds;

  hoiblood = (HOI_BLOOD *)object->blood; /* in case of multiple hois */

  if (editflg) return 0; /* if editor is on, mousecontrol takes Hoi over */

  if (hoiblood->hoi_ai(object))   // als 1 terug dan dood
  {
    return 1;
  }

  hoiblood->blinkanim2cnt++;
  hoiblood->blinkanim2cnt&=511;

  /* handle gravity */
    
      hoiblood->gravy += 2; /* let gravity prevail! */

    if(mc_autorun)
    {
        if (object == hoi)    // MC dont run out of screen when lvel is made
        {
            if (object->x - (player1.curmap->mapsizex - player1.height-64)  > 500)
            {
                hoiblood->spdx = 0;
                object->x = (player1.curmap->mapsizex - player1.height-64)+500;
            }
        }
    }    

  if (hoiblood->gravy > 124) hoiblood->gravy = 124;

  object->y += (hoiblood->gravy >> 3);

  if (object == hoi)    // MC fall out of bounds (down the bottom of the screen)
    {
      if (object->y - (player1.curmap->mapsizey - player1.height-64)  > 460) player1.energy = 0;
    }


  if (hoiblood->onfloor) hoiblood->onfloor -= 1;

  hoiblood->floor = 0; /* when elevated in sky --> no floor present */

  if (hoiblood->gravy > 30)       hoiblood->idlecnt = 0;  // when we fall, we are NOT idle!
  
  
  if (hoiblood->gravy > 0)
    {
      int r;
      int t2 = (object->x & 15) ? (object->sizex>>4)+1 : (object->sizex>>4);
      for (int t=0; t< t2; t++)
	{
	  if (r = collision(object->x + (t<<4) , object->y + (object->sizey) ))
	    {
	      if (!hoiblood->onfloor)
		{

		  if (world == 3)
		    {
		      if (player1.loadedmap->map[(((object->y + (object->sizey))>>5)*levelmap->mapsizex>>5) + ((object->x + (t<<4))>>5)] == 331)
			  {
			    player1.loadedmap->map[(((object->y + (object->sizey))>>5)*levelmap->mapsizex>>5) + ((object->x + (t<<4))>>5)] = 300;
				play_lightwav(object->x,object->y);
			  }
		    }

		  if (hoiblood->gravy > 76 && r != 10 && r != 11  && r!= 13 && r!= 14 && r!=17 && r!= 19)
		    {
		      hoiblood->kneelcnt = (hoiblood->gravy-76) >> 1;
		      if (hoiblood->gravy > 96)
			{
			  player1.shakey = -16;
			  if (!mc_autorun) play_mcfall(object->x, object->y);
			  if (hoi == object)
			    {
			      if (!hoiblood->flashcnt)  // niet onverwoestbaar!
				{
				  if (player1.energy) player1.energy--;
				  if (player1.energy) player1.energy--;
				  if (player1.energy) player1.energy--;
				  if (player1.energy) player1.energy--;
				}
			    }
			}
		    }
		}
	      if (r!=6 && r!=7 && r!=8 && r!=9 && r!=10 && r!=11 && r!=13 && r!=14 && r!=17 && r!=19) //ramp? (schuine wand dus)
		{
		  //            object->y += 6;
		  //            object->y &= 0xfff0;
		  //            object->y -= 6;
		  //            hoiblood->gravy = 0;
		  //            hoiblood->onfloor = 8;
		  //            hoiblood->floor = r;
		  //            object->y -= (object->x & 31)/2;
		  //          }
		  //          else
		  //          {

		  if (r != 18)
		    {

		      object->y += object->sizey;
		      object->y &= 0xfff0;
		      object->y -= object->sizey;
		      hoiblood->gravy = 0;
		      hoiblood->onfloor = 8;
		      hoiblood->floor = r;
		    }
		}
	      if (r==18 && (t==0 || t==1 || t==2 || t==3))  // magically appearing floor
		{
		  UINT16 x,y;
		  x = object->x + (t<<4);
		  y = object->y + (object->sizey);
		  x &= 0xfff0;
		  y &= 0xfff0;
		  if (player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] != 1030)
		    {
		      player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] += 1;
			  if (player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] == 1024)
			  {
				play_glasblok(object->x,object->y);
			  }
		    }
		  if (t!=0)
		    {
		      object->y += object->sizey;
		      object->y &= 0xfff0;
		      object->y -= object->sizey;
		      hoiblood->gravy = 0;
		      hoiblood->onfloor = 8;
		      hoiblood->floor = 18;
		    }
		}
	      if (r==13)
		{
		  object->x -= 2;
		  hoiblood->spdx = 0;
		}
	      if (r==14)
		{
		  object->x += 2;
		  hoiblood->spdx = 0;
		}
	    }
	}
    }

  if (hoiblood->floor == 15) object->x--;
  if (hoiblood->floor == 16) object->x++;

  // ramp business schuin omhoog naar rechts

  if (hoiblood->gravy > -38)
    {
      if (!(hoiblood->kneelcnt && (hoiblood->floor == 1 )))
	{
	  for (y=0; y<3; y++)  // scan 3 patterns
	    {
	      if (collision(object->x + 32 , object->y + (object->sizey) - 16 +(y*16)  ) == 6)
		{
		  object->y += object->sizey;
		  object->y &= 0xfff0;
		  object->y -= object->sizey;
		  hoiblood->gravy = 0;
		  hoiblood->onfloor = 8;
		  hoiblood->floor = 6;
		  object->y -= (object->x & 15)/2;
		  object->y += (y*16);
		}

	      if (collision(object->x + 32 , object->y + (object->sizey) -16 +(y*16)) == 7)
		{
		  object->y += object->sizey;
		  object->y &= 0xfff0;
		  object->y -= object->sizey;
		  hoiblood->gravy = 0;
		  hoiblood->onfloor = 8;
		  hoiblood->floor = 6;
		  object->y -= ((object->x & 15)/2);
		  object->y -= 8;
		  object->y += (y*16);
		}
	    }
	}
    }

  // ramp business schuin omhoog naar links

#if 1
  if (hoiblood->gravy > -38)
    {
      if (!(hoiblood->kneelcnt && (hoiblood->floor == 1 )))
	{
	  for (y=0; y<3; y++)  // scan 3 patterns
	    {
	      if (collision(object->x + 32 , object->y + (object->sizey) - 16 +(y*16)  ) == 8)
		{
		  object->y += object->sizey;
		  object->y &= 0xfff0;
		  object->y -= object->sizey;
		  hoiblood->gravy = 0;
		  hoiblood->onfloor = 8;
		  hoiblood->floor = 8;
		  object->y += (object->x & 15)/2;
		  object->y -= 16;
		  object->y += (y*16);
		}

	      if (collision(object->x + 32 , object->y + (object->sizey) -16 +(y*16)) == 9)
		{
		  object->y += object->sizey;
		  object->y &= 0xfff0;
		  object->y -= object->sizey;
		  hoiblood->gravy = 0;
		  hoiblood->onfloor = 8;
		  hoiblood->floor = 8;
		  object->y -= 8;
		  object->y += ((object->x & 15)/2);
		  object->y += (y*16);
		}
	    }
	}
    }
#endif

  //  if (collision(object->x + 16 , object->y + (object->sizey) ) == 6)
  //  {
  //     object->y += 6;
  //     object->y &= 0xfff0;
  //     object->y -= 6;
  //     hoiblood->gravy = 0;
  //     hoiblood->onfloor = 8;
  //     hoiblood->floor = 6;
  //    object->y -= (object->x & 31)/2;
  //  }

  if (hoiblood->gravy < 0)
    {
      int r;
      int t2 = (object->x & 15) ? (object->sizex>>4)+1 : (object->sizex>>4);

      for (int t=0; t< t2; t++)
	{
	  if (r = collision(object->x + (t<<4) , object->y ))
	    {
	      if (r != 2 && r!=6 && r!=7 && r!=8 && r!=9 && r!=10 && r!=11 && r!=17 && r!=18 && r!=19)
		{
		  object->y += 15;
		  object->y += object->sizey;
		  object->y &= 0xfff0;
		  object->y -= object->sizey;
		  hoiblood->gravy = 0;
		}

	      if (r == 5)
		{
		  if (object == hoi)
		    {
		      player1.energy = 0;
		    }
		  else
		    {
		      if (hoiblood->hoi_caps->id == ID_HELMUT)
			{
			  hoiblood->pal->live = helmut_dwarrel;
                    play_helmdood(object->x, object->y);

                    //			  player1.score += 2500;
			}
		      if (hoiblood->hoi_caps->id == ID_BOMB)
			{

                    plof_init(object->x+32, object->y+32, 0, 0, 0, 0, 0);
//                    log_out("bomb is dood");
                  }
                  return 1;
		    }
		}
	    }
	}
    }


  if(hoiblood->onobject) hoiblood->floor = 2;
  hoiblood->onobject = 0;
  /* handle jump */

  if (hoiblood->upkey)
    {
        
      hoiblood->idlecnt = 0;
      if (hoiblood->onfloor)
	{   
	  hoiblood->gravy = -hoiblood->hoi_caps->jumpheight;
	}
    }


  if (hoiblood->shootkey)
    {
      if (world == 3 && paraflg == 7)   // (level 4 ... big boss is busy)
	{
	  hoiblood->idlecnt = 0;
	  keytab[prefs->shootkey] = 0;
	  if (!hoiblood->kneelcnt)
	    {
	      bouncey_init(object->x+20, object->y+20, -(hoiblood->lastdirx)*108 ,-68);
	    }
	  else
	    {
	      bouncey_init(object->x+20, object->y+40, -(hoiblood->lastdirx)*108 ,-44);
	    }
	}
    }


  checkbounds = 0;
  if (object->x <= player1.worldx)   // force moon child to the left
    {
      if (autoscrollspd)
	{
	  if (object == hoi)
	    {
	      if (!hoiblood->flashcnt)
		{
		  checkbounds = 1;
		  object->x = player1.worldx;
		  if (hoiblood->spdx < 0) hoiblood->spdx = 0;
		  hoiblood->leftkey = 0;
		}
	    }
	}
    }

  if (object->x >= (player1.worldx+640-64))   // force moon child to the left
    {
      if (autoscrollspd)
	{
	  if (object == hoi)
	    {
	      if (!hoiblood->flashcnt)
		{
		  object->x = player1.worldx+640-64;
		  if (hoiblood->spdx > 0) hoiblood->spdx = 0;
		  //		  hoiblood->rightkey = 0;
		}
	    }
	}
    }


  /* handle x-movement */

  if (hoiblood->leftkey && (!hoiblood->kneelcnt))
    {
      hoiblood->idlecnt = 0;
      if (hoiblood->spdx > 0)
	{
	  if (hoiblood->floor == 3)
	    {
	      hoiblood->spdx -= 1;
	    }
	  else
	    {
	      hoiblood->spdx -= 2;
	    }

	  if (hoiblood->floor)
	    {
	      anim_setsequence(object->animate, SEQ_SKIDRIGHT, SEQ_FORCE);
	      hoiblood->animover = 12;

	      if (hoiblood->spdx > 80)
		{
              if (hoiblood->hoi_caps->id != ID_BOMB)
              {
                cloud_init(object->x + 48 + (hoiblood->spdx >> 4),object->y + object->sizey -6, hoiblood->spdx >> 5, 1);
              }
            }
	    }

	  if (hoiblood->spdx > 128)
	    {
	      if (hoiblood->onfloor)
		{
		  object->y -= 6;
		  if (hoiblood->spdx > 192) object->y -= 6;
		  hoiblood->onfloor = 0;
		}
	    }

	  if (hoiblood->spdx > 80)
	    {
	      object->frame = anim_forceframe(object->animate, 1);
	    }
	  else
	    {
	      object->frame = anim_forceframe(object->animate, 0);        
	    }
	}
      else
	{
	  if (hoiblood->floor == 3)
	    {
	      hoiblood->spdx -= 2;
	    }
	  else
	    {
	      hoiblood->spdx -= 5;
	    }
    
	  anim_setsequence(object->animate, SEQ_WALKLEFT, SEQ_NOFORCE);
	  object->frame = anim_nextframe(object->animate);
      }
    } else

      if (hoiblood->rightkey && (!hoiblood->kneelcnt)) 
	{
	  hoiblood->idlecnt = 0;
	  if (hoiblood->spdx < 0)
	    {
	      if (hoiblood->floor == 3)
		{
		  hoiblood->spdx += 1;
		}
	      else
		{
		  hoiblood->spdx += 2;
		}

	      if (hoiblood->floor)
		{
		  anim_setsequence(object->animate, SEQ_SKIDLEFT, SEQ_FORCE);
		  hoiblood->animover = 12;

		  if (hoiblood->spdx < -80)
		    {
                  if (hoiblood->hoi_caps->id != ID_BOMB)
                  {
	              cloud_init(object->x + 4 + (hoiblood->spdx >> 4),object->y + object->sizey -6, hoiblood->spdx >> 5, 1);
		      }
                }
            }

	      if (hoiblood->spdx < -128)
		{
		  if (hoiblood->onfloor)
		    {
		      object->y -=6;
		      if (hoiblood->spdx < -192) object->y -= 6;
		      hoiblood->onfloor = 0;
		    }
		}
	      if (hoiblood->spdx < -80)
		{
		  object->frame = anim_forceframe(object->animate, 1);
		}
	      else
		{
		  object->frame = anim_forceframe(object->animate, 0);        
		}
	    }
	  else
	    {
	      if (hoiblood->floor == 3)
		{
		  hoiblood->spdx += 2;
		}
	      else
		{
		  hoiblood->spdx += 5;
		}

	      //HACK
	      if (hoiblood->spdx > 0)
		{
		  if (hoiblood->spdx <16) hoiblood->spdx += 16;
		}

	      //    if (hoiblood->onfloor)
	      //      {

            anim_setsequence(object->animate, SEQ_WALKRIGHT, SEQ_NOFORCE);
	      object->frame = anim_nextframe(object->animate);
            //      }
	    }
	}
      else
	{
	  if(hoiblood->lastdirx == 1)
	    {
	      if (hoiblood->spdx < -16)
		{
		  anim_setsequence(object->animate, SEQ_WALKLEFT, SEQ_NOFORCE);
		  object->frame = anim_nextframe(object->animate);
            } 
	      else
		{
              UINT16 voetje;
              voetje = 0;
              anim_setsequence(object->animate, SEQ_STANDLEFT, SEQ_FORCE);
//		  object->frame = anim_forceframe(object->animate,blinkanim2[hoiblood->blinkanim2cnt>>1]);

              if (hoiblood->floor == 6 || hoiblood->floor == 7)
              {
                voetje = 1;
              }
              if (hoiblood->floor == 8 || hoiblood->floor == 9)
              {
                voetje = 2;
              }
		  object->frame = anim_forceframe(object->animate, voetje);
            
            }
	    }
	  else
	    {
	      if (hoiblood->spdx > 32)
		{
		  anim_setsequence(object->animate, SEQ_WALKRIGHT, SEQ_NOFORCE);
		  object->frame = anim_nextframe(object->animate);
            }
	      else
		{
              UINT16 voetje;
              voetje = 0;

              anim_setsequence(object->animate, SEQ_STANDRIGHT, SEQ_FORCE);
//		  object->frame = anim_forceframe(object->animate,blinkanim2[hoiblood->blinkanim2cnt>>1]);

              if (hoiblood->floor == 6 || hoiblood->floor == 7)
              {
                voetje = 2;
              }
              if (hoiblood->floor == 8 || hoiblood->floor == 9)
              {
                voetje = 1;
              }
		  object->frame = anim_forceframe(object->animate, voetje);

            }

	    }
	}

// if shoes then clouds
   if (hoiblood->hoi_caps->maxspd == 250 && (hoiblood->leftkey || hoiblood->rightkey) && hoiblood->floor && (hoiblood->downkey == 0) && (hoiblood->leancnt == 0))
   {
      static UINT16 cldcnt;

      cldcnt += 1;
      cldcnt &= 3;
      
      if (cldcnt == 0)
      {
            cloud_init(object->x + ((hoiblood->lastdirx == -1) ? 10 : 42) + (hoiblood->spdx >> 4),object->y + object->sizey -6, (hoiblood->lastdirx == -1) ? -1 : 1 ,0);
      }
   }

// scheet laat geintje

   {
      static UINT16 scheet;

      if (object == hoi)
      {
        if (hoiblood->downkey == 0)
        {
          scheet = 0;
        }
        else
        {
          scheet++;
          if (scheet > 500)
          {
            scheet = 0;
            cloud_init(object->x + ((hoiblood->lastdirx == -1) ? -6 : 58) + (hoiblood->spdx >> 4),object->y + object->sizey -20, (hoiblood->lastdirx == -1) ? -1 : 1, 0);
            play_mcfart(object->x, object->y);
          }
        }
     }
   }

  if (hoiblood->floor != 0)
    {
      hoiblood->hoi_caps->jumpheight = hoiblood->hoi_caps->orgheight;
    }

  if (hoiblood->floor != 3)
    {
      if (hoiblood->floor == 4)
	{
	  hoiblood->hoi_caps->jumpheight = hoiblood->hoi_caps->orgheight/2;
	  if (hoiblood->spdx != 0)
	    {
	      if (hoiblood->spdx > 0)
		{
		  hoiblood->spdx -= 7;
		  if (hoiblood->spdx < 0) hoiblood->spdx = 1;
		}
	      else
		{
		  hoiblood->spdx += 7;
		  if (hoiblood->spdx > 0) hoiblood->spdx = -1;
		}
	    }

	  if (hoiblood->spdx > -18 && hoiblood->spdx < 0) hoiblood->spdx=-1;
	  if (hoiblood->spdx < 18 && hoiblood->spdx > 0) hoiblood->spdx=1;

	}
      else
	{
	  if (hoiblood->spdx > 0)
	    {
	      if (hoiblood->spdx > 48)
		{
		  hoiblood->spdx -= 3;
		}
	      else
		{
		  hoiblood->spdx -= 2;
		}
	      if (hoiblood->spdx < 0) hoiblood->spdx = 0;
	    }
          else
	    {
	      if (hoiblood->spdx < -48)
		{
		  hoiblood->spdx += 3;
		}
	      else
		{
		  hoiblood->spdx += 2;
		}
	      if (hoiblood->spdx > 0) hoiblood->spdx = 0;
	    }
	}
    }
  else
    {
      if (hoiblood->spdx != 0)
	{
	  if (hoiblood->spdx > 0)
	    {
	      hoiblood->spdx -= 1;
	    }
	  else
	    {
	      hoiblood->spdx += 1;
	    }
	}
    }


  if (hoiblood->spdx > 240)
    {
      hoiblood->spdx = 240;
    }

  if (hoiblood->spdx < -240)
    {
      hoiblood->spdx = -240;
    }

  if (hoiblood->spdx > hoiblood->hoi_caps->maxspd && hoiblood->spdx < hoiblood->hoi_caps->maxspd+6)
    {
      hoiblood->spdx = hoiblood->hoi_caps->maxspd;
    }

  if (hoiblood->spdx < -hoiblood->hoi_caps->maxspd && hoiblood->spdx > -hoiblood->hoi_caps->maxspd - 6)
    {
      hoiblood->spdx = -hoiblood->hoi_caps->maxspd;
    }

  INT16 destvalue;

  destvalue = hoiblood->spdx;
  if (destvalue < 48 && destvalue > -48) destvalue = 0;

  if (destvalue > hoiblood->xahead)
    {
      hoiblood->xahead += 2;
      if (destvalue < hoiblood->xahead) hoiblood->xahead = destvalue;
    }
  else
    {
      hoiblood->xahead -= 2;
      if (destvalue > hoiblood->xahead) hoiblood->xahead = destvalue;
    }
  

  // DO JUMP FRAMES UNLESS OVERRIDE IS ACTIVE (IN WHICH CASE WE SKID)


  if (hoiblood->animover)
    {
      hoiblood->animover--;
    }
  else
    {
      if (!hoiblood->floor)
	{
    
	  if (hoiblood->lastdirx == 1)
	    {
	      anim_setsequence(object->animate, SEQ_JUMPLEFT, SEQ_FORCE);
	    }
	  else
	    {
	      anim_setsequence(object->animate, SEQ_JUMPRIGHT, SEQ_FORCE);
	    }
    
	  if (hoiblood->gravy <0)
	    {
	      if (hoiblood->gravy < - 16)
		{
		  object->frame = anim_forceframe(object->animate, 2);        
		}
	      else
		{
		  object->frame = anim_forceframe(object->animate, 3);        
		}
	    }
	  else
	    {
	      if (hoiblood->gravy > 16)
		{
		  object->frame = anim_forceframe(object->animate, 5);        
		}
	      else
		{
		  object->frame = anim_forceframe(object->animate, 4);        
		}
	    }
	}
    }

  if (hoiblood->downkey && hoiblood->floor)
    {
      hoiblood->idlecnt = 0;

      if (hoiblood->holddown < 112) hoiblood->holddown += 4;
      if (hoiblood->kneelcnt<7) hoiblood->kneelcnt+=1;
    }
  else
    {
      if (hoiblood->holddown > 0) hoiblood->holddown -= 4;
      if (hoiblood->kneelcnt) hoiblood->kneelcnt -= 1;
    }

  if (hoiblood->kneelcnt)
    {
      object->colheight = object->sizey - 48;
      object->coloffsety= 48;
    }
  else
    {
      object->colheight= object->sizey - 6;
      object->coloffsety= 6;
    }

  UINT16 oldint;
  INT16  tempspd;

  oldint = object->x;
  tempspd = hoiblood->spdx;
  if (tempspd > -14 && tempspd < 14) tempspd = 0;
  tempspd >>= hoiblood->hoi_caps->xspddelay;


  // scalen speed
  extern int g_CurDeltaX;
  extern int g_MouseFlg;

  int	scalespd;
  int   scalefactor;

  if(g_MouseFlg)
  {
	  scalefactor = g_CurDeltaX;
	  if(scalefactor<0) scalefactor = -scalefactor;
	  if(scalefactor>64) scalefactor=64;
	  scalespd = tempspd*scalefactor;
	  scalespd /= 64;
	  tempspd = scalespd;
  }

  object->x += tempspd;
 
  
  
  if (hoiblood->hoi_caps->id == ID_BOMB && tempspd> 0) object->x +=1;



  hoiblood->curdirx = 0;
  //  if (!(object->x == oldint))
  {
    remem = hoiblood->lastdirx;
    if (oldint!=object->x)
      {
	hoiblood->lastdirx = (oldint>object->x) ? 1 : -1;
      }
    else
      {
	if (hoiblood->spdx != 0)
	  {
	    hoiblood->lastdirx = (hoiblood->spdx < 0) ? 1 : -1;
	  }
      }
    hoiblood->curdirx  = hoiblood->lastdirx;

    if(remem != hoiblood->lastdirx)
      {
	if (!hoiblood->anmturn)
	  {
	    hoiblood->anmturn = 15;
	    if (hoiblood->lastdirx == 1)
	      {
		hoiblood->anmdir  = 1;
	      }
	    else
	      {
		hoiblood->anmdir  = 0;
	      }
	  }
      }
    else
      {
      }
  }


#if 0
  if(hoiblood->onobject)  // Is he standing on an elevator platform?
    {
      hoiblood->onobject = 0;
      if(hoiblood->lastdirx == 1)
	{
	  anim_setsequence(object->animate, SEQ_STANDLEFT, SEQ_NOFORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_STANDRIGHT, SEQ_NOFORCE);
	}
      object->frame = anim_nextframe(object->animate);
    }
#endif


  if(hoiblood->anmturn)
    {
      hoiblood->anmturn--;
      
      if (hoiblood->anmdir==1)
	{
	  anim_setsequence(object->animate, SEQ_TURNRIGHT, SEQ_FORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_TURNLEFT, SEQ_FORCE);
	}

      object->frame = anim_forceframe(object->animate, hoiblood->anmturn/4);
    }


#if 1
  if (hoiblood->curdirx == -1)
    {
      int r;
      int t2 = ((object->y+(object->sizey&15)) & 15) ? (object->sizey>>4)+1 : (object->sizey>>4);
      for (int t=0; t< t2; t++)
	{
	  if (r = collision(object->x + (object->sizex), object->y+(object->sizey&15) + (t<<4) ))
	    {
	      if (r != 2 && r!=6 && r!=7 && r!=8 && r!=9 && r!=10 && r!=11 && r!=17 && r!=18 && r!=19)
		{
		  hoiblood->curdirx = 0;
		  if (hoiblood->spdx > 128)
		    {
		      cloud_init(object->x + 20, object->y + 58, 0, 1);

		      hoiblood->thudcnt = 15 + ((hoiblood->spdx - 128)/4);
		      hoiblood->thuddir = 0;

		      hoiblood->spdx = -64 - ((hoiblood->spdx - 128)/2);
		      //          hoiblood->spdx = -128;
		      object->x &= 0xfff0;
		      object->x--;
		    }
		  else
		    {
		      hoiblood->spdx = 0;
		      object->x &= 0xfff0;
		    }
		}
	    }
	}
    }

  for (int x2=0; x2<3; x2++)
    {
      int r;
      int t2 = ((object->y+(object->sizey&15)) & 15) ? (object->sizey>>4)+1 : (object->sizey>>4);
      for (int t=0; t< t2; t++)
	{
	  if (r = collision(object->x + 16 + (x2*16), object->y+(object->sizey&15) + (t<<4) ))
	    {
	      if (r == 10)
		{
		  UINT16 x,y;
		  x = object->x + 16 +(x2*16);
		  y = object->y+(object->sizey&15) + (t<<4);
		  x &= 0xfff0;
		  y &= 0xfff0;
		  player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] = 0 ;36;
		  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
		  if (object == hoi)
		    {
		      smlheart_init(x, y, 1, 8);
		      bonus_init(x,y,0,1);
                }
		  else
		    {
		      bonus_init(x,y,0,0);
		    }
		}
	      else if (r==5)
		{
		  if (object == hoi)
		    {
		      
		      player1.energy = 0;
		      
		    }
		}
	      else if (r == 11)
		{
		  UINT16 x,y;
		  x = object->x + 16 +(x2*16);
		  y = object->y+(object->sizey&15) + (t<<4);
		  x &= 0xfff0;
		  y &= 0xfff0;
                 
            int mapOffset = (y*player1.loadedmap->mapsizex/32/32)+(x/32);
            int mapColOffset = (y*player1.loadedmap->mapsizex/16/16)+(x/16);
            
		  player1.loadedmap->map[mapOffset] = 0 ;36;
		  colmap[mapColOffset] = 0;

            //remember blackdiamond pos. so if level is restarted, we can remove blackdiamond already
//            blackdiamondlocations[world*4+level][(7-player1.nrofblack)] = mapOffset;
//            blackdiamondcollocations[world*4+level][(7-player1.nrofblack)] = mapColOffset;
            
		  if (object == hoi)
		    {
		      bonus_init(x,y,1,1);
		      smlheart_init(x, y, 2, 1 + (7-player1.nrofblack));
		    }
		  else
		    {
		      bonus_init(x,y,1,0);
		    }
		}
	      else if (r == 17)
		{
		  if (object == hoi)
		    {
		      UINT16 x,y;
		      x = object->x + 16 +(x2*16);
		      y = object->y+(object->sizey&15) + (t<<4);
		      x &= 0xfff0;
		      y &= 0xfff0;
		      player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] = 0 ;36;
		      colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
		      hoiblood->hoi_caps->maxspd = 250;
                  play_bonus(object->x, object->y);
		    }
		}
	      else if (r == 19)
		{
		  UINT16 x,y;
		  x = object->x + 16 +(x2*16);
		  y = object->y+(object->sizey&15) + (t<<4);
		  x &= 0xfff0;
		  y &= 0xfff0;
		  player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] = 0 ;36;
		  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
		  if (object == hoi)
		    {
		      smlheart_init(x, y, 0, 0);
		      bonus_init(x,y,2,1);
		    }
		  else
		    {
		      bonus_init(x,y,2,0);
		    }
		}
	    }
	}
    }

#endif

#if 1
  if (hoiblood->curdirx == 1)
    {
      int r;
      int t2 = (object->y+(object->sizey&15) & 15) ? (object->sizey>>4)+1 : (object->sizey>>4);
      for (int t=0; t< t2; t++)
	{
	  if (r = collision(object->x , object->y+(object->sizey&15) + (t<<4) ))
	    {
	      if (r != 2 && r!=6 && r!=7 && r!=8 && r!=9 && r!=10 && r!=11 && r!=17 && r!=18 && r!=19)
		{
		  hoiblood->curdirx = 0;
		  if (hoiblood->spdx < -128)
		    {
		      cloud_init(object->x + 4, object->y + 58, 0, 1);
		      hoiblood->thudcnt = 15 + ((-hoiblood->spdx - 128)/4);
		      hoiblood->thuddir = 1;

		      hoiblood->spdx = 64 + ((-hoiblood->spdx - 128)/2);
		      object->x += 15;
		      object->x &= 0xfff0;
		    }
		  else
		    {
		      hoiblood->spdx = 0;
		      object->x += 15;
		      object->x &= 0xfff0;
		    }
		}
	    }
	}
    }
#endif


  if (checkbounds)
    {
      if ((object->x) < player1.worldx)  // moon child still more left than screen? than die!
	{
	  if (autoscrollspd)
	    {
	      if (object == hoi)
		{
		  if (!hoiblood->flashcnt)
		    {
		      player1.energy = 0;
		    }
		}
	    }
	}
    }

  //  if (hoiblood->curdirx == 0 && (hoiblood->leftkey || hoiblood->rightkey) && (hoiblood->floor != 0) )
  if (object->x == oldint && (hoiblood->leftkey || hoiblood->rightkey) && (hoiblood->floor != 0) )
    {
      if (hoiblood->floor != 4)  //  not on mud
	{
	  if (hoiblood->leancnt < 27) hoiblood->leancnt++;
	  if (hoiblood->leancnt == 1) hoiblood->leandir = hoiblood->lastdirx;
	}
    }
  else
    {
      if (hoiblood->leancnt) hoiblood->leancnt--;
    }


  if ((hoiblood->leancnt>4) && hoiblood->floor != 4)  // not on mud
    {
      if (hoiblood->leandir == 1)
	{
	  anim_setsequence(object->animate, SEQ_LEANLEFT, SEQ_FORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_LEANRIGHT, SEQ_FORCE);
	}

      object->frame = anim_forceframe(object->animate, (hoiblood->leancnt-4) / 6);
    }


  if (hoiblood->kneelcnt)
    {
      UINT16 voetje;

      voetje = 0;
      hoiblood->leancnt = 0;
      if (hoiblood->lastdirx == 1)
	{
	  anim_setsequence(object->animate, SEQ_KNEELLEFT, SEQ_FORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_KNEELRIGHT, SEQ_FORCE);
	}

      if (hoiblood->floor == 6 || hoiblood->floor == 7)
      {
        voetje = 1;
      }
      if (hoiblood->floor == 8 || hoiblood->floor == 9)
      {
        voetje = 2;
      }
//      object->frame = anim_forceframe(object->animate, (hoiblood->kneelcnt>>2) > 1 ? 1: (hoiblood->kneelcnt>>2));
//      object->frame = anim_forceframe(object->animate, (hoiblood->kneelcnt) ? 1: 0);
      object->frame = anim_forceframe(object->animate, voetje);
    }

  if (hoiblood->thudcnt)
    {
      hoiblood->thudcnt--;
      if (hoiblood->thuddir)
	{
	  anim_setsequence(object->animate, SEQ_BUMPLEFT, SEQ_FORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_BUMPRIGHT, SEQ_FORCE);
	}
      
      object->frame = anim_forceframe(object->animate, hoiblood->thudcnt>4 ? 1 : 0);
    }

  if (hoiblood->floor == 5)
    {
      if (object == hoi)
	{

	  player1.energy = 0;

	}
      else
	{
	  if (hoiblood->hoi_caps->id == ID_HELMUT)
	    {
	      hoiblood->pal->live = helmut_dwarrel;
            play_helmdood(object->x, object->y);
	      //	      player1.score += 2500;
	    }
        if (hoiblood->hoi_caps->id == ID_BOMB)
          {
            plof_init(object->x+32, object->y+32, 0, 0, 0, 0, 0);
          }
	  return 1;
	}

      //    return 1;  // property 5 is dood
    }


  if (object == hoi && hoiblood->floor != 0)
  {
	int r;
	int t2 = (object->x & 15) ? (object->sizex>>4)+1 : (object->sizex>>4);
	for (int t=0; t< t2; t++)
	{
	  if (r = collision(object->x + (t<<4) , object->y + (object->sizey) ) == 12) 
	  {
	    if ((restarty != ((hoi->y+15) & 0xfff0)-6) || (restartx != ((hoi->x+(t<<4)) & 0xffc0)))
	    {
	      restartx = ((hoi->x+(t<<4)) & 0xffc0);
	      restarty = ((hoi->y+15) & 0xfff0)-6;
	      restartpara = paraflg;
	      hoiblood->flashcnt = 200;
		  play_restart();
	    }
	  }
	}
  }


#if 0
  if (hoiblood->floor == 12)
  {

    if (object == hoi)
	{
	  if ((restarty != ((hoi->y+15) & 0xfff0)-6) || (restartx != ((hoi->x) & 0xffc0)+64))
	  {
	    restartx = ((hoi->x) & 0xffc0)+64;
	    restarty = ((hoi->y+15) & 0xfff0)-6;
	    restartpara = paraflg;
	    hoiblood->flashcnt = 200;
		play_restart();
	  }
	}
    else
	{
	}

      //    return 1;  // property 12 is restartpoint
  }

#endif
  
  hoiblood->idlecnt++;

  if (hoiblood->idlecnt < 16)
    {

      if (hoiblood->idleanim > 8) hoiblood->idleanim = 7;
      if (hoiblood->idleanim)
	{
	  hoiblood->anmturn = 0;
	  hoiblood->idleanim--;
	}
    }
 
  if (hoiblood->idlecnt > 500)
    {
      hoiblood->idlecnt = 510;
      hoiblood->anmturn = 0;
      if (hoiblood->idleanim < 990) hoiblood->idleanim++;
    }

  if (hoiblood->idleanim)
    {
      if (hoiblood->lastdirx == 1)
	{
	  anim_setsequence(object->animate, SEQ_WAITLEFT, SEQ_FORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_WAITRIGHT, SEQ_FORCE);
	}
      
      object->frame = anim_forceframe(object->animate, idleanim[hoiblood->idleanim>>1]);
    }


  if (hoiblood->flashcnt)
    {
      hoiblood->flashcnt--;
      object->blitsizey = flashtab[hoiblood->flashcnt] == 0 ?  1 : object->sizey;
    }

  // soundeffects for running

  UINT16 sapflg;

  sapflg = 0;

  if (hoiblood->floor == 4)
    {
      sapflg = 1;
    }


  if (hoiblood->hoi_caps->id == ID_HELMUT)
    {
      sapflg = 2;
    }


  if (object->animate->cursequence == SEQ_WALKLEFT || object->animate->cursequence == SEQ_WALKRIGHT)
    {
      if (object->animate->stepnr == 0 && object->animate->delay==0)
	{
	  if (!mc_autorun)play_mcloop(sapflg, object->x, object->y);
	}
      if (object->animate->stepnr == 5 && object->animate->delay==0)
	{
	  if (!mc_autorun)play_mcloop(sapflg, object->x, object->y);
	}
    }


  if ((object->blitsizey != 1) && object->frame)  // calculate current frame height
  {
    object->blitsizey = object->frame->get_height();
  }

  return 0;
}


#define SPD_ACCELERATE   (110)
#define SPD_BRAKE        (90)
#define SPD_SLOWDOWN     (60)
#define SPD_MAXSPD       (3072)
#define SPD_BOUNCE       (1300)

UINT16 exhaustadd[] = {  50,66,
                         52,60,
                         54,56,
                         16,66,
                         14,60,
                         12,56,

                         16,66,
                         18,66,
                         20,66,
                         23,66,
                         26,66,
                         30,66,
                         34,66,
                         38,66,
                         41,66,
                         44,66,
                         46,66,
                         48,66,

                         48,66,
                         46,66,
                         44,66,
                         41,66,
                         38,66,
                         34,66,
                         30,66,
                         26,66,
                         23,66,
                         20,66,
                         18,66,
                         16,66
};

INT16 shootadd[] = { 32-46, 35,
                     32-42, 40,
                     32-38, 45,

                     46+12, 35,
                     42+12, 40,
                     38+12, 45
};


int hoi_fly_live (OBJECT *object, UINT32 param)
{
  HOI_BLOOD *hoiblood;
  UINT16     oldx,oldy;
  INT32      volume;
  INT16      temp,temp2;
  UINT16     basespdx;

  hoiblood = (HOI_BLOOD *)object->blood; /* in case of multiple hois */

  if (editflg) return 0; /* if editor is on, mousecontrol takes Hoi over */

  hoiblood->hoi_ai(object);

  if (autoscrollspd)
  {
    basespdx = (autoscrollspd -1) <<8;
  }
  else
  {
    basespdx = 0;
  }

  // handle y-movement

  volume = -1600;
  
  if (hoiblood->upkey)
    {
      volume = -1100;
      if (hoiblood->yspd > 0)
	{
	  hoiblood->yspd -= SPD_BRAKE;
	}
      else
	{
	  hoiblood->yspd -= SPD_ACCELERATE;
	}
    }

  if (hoiblood->downkey)
    {
	volume = -2100;
	if (hoiblood->yspd < 0)
	{
	  hoiblood->yspd += SPD_BRAKE;
	}
      else
	{
	  hoiblood->yspd += SPD_ACCELERATE;
	}
    }

  set_backpak_volume(volume);

  if (hoiblood->yspd > 0)
    {
      hoiblood->yspd -= SPD_SLOWDOWN;
      if (hoiblood->yspd < 0) hoiblood->yspd = 0;
    }
  else
    {
      hoiblood->yspd += SPD_SLOWDOWN;
      if (hoiblood->yspd > 0) hoiblood->yspd = 0;
    }

  // handle x-movement

  if (hoiblood->leftkey)
    {
      if (hoiblood->xspd > basespdx)
	{
	  hoiblood->xspd -= SPD_BRAKE;
	}
      else
	{
	  hoiblood->xspd -= SPD_ACCELERATE;
	}
    }

  if (hoiblood->rightkey)
    {
      if (hoiblood->xspd < basespdx)
	{
	  hoiblood->xspd += SPD_BRAKE;
	}
      else
	{
	  hoiblood->xspd += SPD_ACCELERATE;
	}
    }

  if (hoiblood->xspd > basespdx)
    {
      hoiblood->xspd -= SPD_SLOWDOWN;
      if (hoiblood->xspd < basespdx) hoiblood->xspd = basespdx;
    }
  else
    {
      hoiblood->xspd += SPD_SLOWDOWN;
      if (hoiblood->xspd > basespdx) hoiblood->xspd = basespdx;
    }

  //
  if (hoiblood->xspd > SPD_MAXSPD)
    {
      hoiblood->xspd = SPD_MAXSPD;
    }

  if (hoiblood->xspd < -SPD_MAXSPD)
    {
      hoiblood->xspd = -SPD_MAXSPD;
    }

  //
  if (hoiblood->yspd > SPD_MAXSPD)
    {
      hoiblood->yspd = SPD_MAXSPD;
    }

  if (hoiblood->yspd < -SPD_MAXSPD)
    {
      hoiblood->yspd = -SPD_MAXSPD;
    }

  oldx = (hoiblood->xfloat>>8);
  oldy = (hoiblood->yfloat>>8);


// y collision

    hoiblood->yfloat += hoiblood->yspd;

  hoiblood->hoovercnt += 18;
  hoiblood->hoovercnt &= 1023;

  // Vertical movement pattern checking

  if (hoiblood->yspd < 0)
    {
      int r;
      int t2 = (((hoiblood->xfloat >> 8)+8) & 15) ? (object->sizex>>4)+1 : (object->sizex>>4);
      
      for (int t=0; t< t2-1; t++)
	{
	  if (r = collision((hoiblood->xfloat >> 8) + 8 + (t<<4) , (hoiblood->yfloat >> 8) + 8))
	    {
	      if (r != 0)
		{
		  
		  if (hoiblood->yspd < -(8 << 8))
		    {
		      cloud_init((hoiblood->xfloat >> 8) + 20, (hoiblood->yfloat >> 8) + 48, 0, 1);
		    }
		  else
		    {
		    }
		  hoiblood->yspd = (-hoiblood->yspd)-SPD_BOUNCE;
		  if (hoiblood->yspd < 0) hoiblood->yspd = 0;
		  
		  hoiblood->yfloat += 0xf00;
		  hoiblood->yfloat += 0x800;
		  (hoiblood->yfloat) &= 0xfffff000;
		  hoiblood->yfloat -= 0x800;
		}
	      goto endchecky;
	    }
	}
    }
  else
    {
      int r;
      int t2 = (((hoiblood->xfloat >> 8)+8) & 15) ? (object->sizex>>4)+1 : (object->sizex>>4);
      
      for (int t=0; t< t2-1; t++)
	{
	  if (r = collision((hoiblood->xfloat >> 8) + (t<<4) + 8, (hoiblood->yfloat >> 8) + (object->sizey&0xfff0) - 8))
	    {
	      if (r != 0)
		{
		  if (hoiblood->yspd > (8 << 8))
		    {
		      cloud_init((hoiblood->xfloat >> 8) + 20, (hoiblood->yfloat >> 8) + 48, 0, 1);
		    }
		  else
		    {
		    }
		  
		  hoiblood->yspd = (-hoiblood->yspd)+SPD_BOUNCE;
		  if (hoiblood->yspd > 0) hoiblood->yspd = 0;
		  
		  hoiblood->yfloat += 0x800;
		  (hoiblood->yfloat) &= 0xfffff000;  // 16 pixels
		  hoiblood->yfloat -= 0x800;
		  
		}
	      goto endchecky;
	    }
	}
    }

  temp  = sokoban_collision(soko1, (hoiblood->xfloat >> 8)+8, (hoiblood->yfloat >> 8)+8, object->sizex-16, object->sizey-24,  (hoiblood->yspd > 0) ? 2 : 0);
  temp |= sokoban_collision(soko2, (hoiblood->xfloat >> 8)+8, (hoiblood->yfloat >> 8)+8, object->sizex-16, object->sizey-24,  (hoiblood->yspd > 0) ? 2 : 0);
  temp |= sokoban_collision(soko3, (hoiblood->xfloat >> 8)+8, (hoiblood->yfloat >> 8)+8, object->sizex-16, object->sizey-24,  (hoiblood->yspd > 0) ? 2 : 0);

  // check for puzzle in world 3

  if (world == 2 && level && 1)
  {
    UINT16 tempie;
    if ((hoiblood->xfloat >> 8) > 1440 && (hoiblood->xfloat >> 8) < 2080 && (hoiblood->yfloat >> 8) > 1332 && (hoiblood->yfloat >> 8) < 1974)
	{
      tempie = ((((hoiblood->yfloat >> 8)-1332) / 128) * 5) + (((hoiblood->xfloat >> 8)-1440) / 128);
	  if (tempie == lastcolumnr) temp |= 1;
	}
  }

  if (temp)
    {
      if (hoiblood->yspd < 0)
	{
	  if (hoiblood->yspd < -(8 << 8))
	    {
	      cloud_init((hoiblood->xfloat >> 8) + 20, (hoiblood->yfloat >> 8) + 48, 0, 1);
	    }
	  else
	    {
	    }

	  hoiblood->yfloat -= hoiblood->yspd;

	  hoiblood->yspd = (-hoiblood->yspd)-SPD_BOUNCE;
	  if (hoiblood->yspd < 0) hoiblood->yspd = 0;
	  
	  goto endchecky;
	}
      else
	{
	  if (hoiblood->yspd > (8 << 8))
	    {
	      cloud_init((hoiblood->xfloat >> 8) + 20, (hoiblood->yfloat >> 8) + 48, 0, 1);
	    }
	  else
	    {
	    }
	  
	  hoiblood->yfloat -= hoiblood->yspd;

	  hoiblood->yspd = (-hoiblood->yspd)+SPD_BOUNCE;
	  if (hoiblood->yspd > 0) hoiblood->yspd = 0;
	  
	  goto endchecky;
	}
  }

endchecky:

  
  // x-collision


  hoiblood->xfloat += (hoiblood->xspd);


  // pattern collision horizontally

  if (hoiblood->xspd > 0)
    {
      int r;
      int t2 = (((hoiblood->yfloat >> 8)+8) & 15) ? (object->sizey>>4)+1 : (object->sizey>>4);
      for (int t=0; t< t2-1; t++)
	{
	  if (r = collision((hoiblood->xfloat >> 8) + (object->sizex) - 8, (hoiblood->yfloat >> 8) + 8 + (t<<4) ))
	    {
	      if (r != 0)
		{
		  if (hoiblood->xspd > ((8 << 8)))
		    {
		      cloud_init((hoiblood->xfloat >> 8) + 20, (hoiblood->yfloat >> 8) + 48, 0, 1);
		    }
		  else
		    {
		    }
		  
		  hoiblood->xspd = (-hoiblood->xspd)+SPD_BOUNCE;
		  if (hoiblood->xspd > 0) hoiblood->xspd = 0;
		  
		  hoiblood->xfloat += 0x800;
		  (hoiblood->xfloat) &= 0xfffff000;  // 16 pixels
		  hoiblood->xfloat -= 0x800;
		  
		  goto endcheckx;
		}
	    }
	}
    }
  else
    {
      int r;
      int t2 = (((hoiblood->yfloat >> 8)+8) & 15) ? (object->sizey>>4)+1 : (object->sizey>>4);
      for (int t=0; t< t2-1; t++)
	{
	  if (r = collision((hoiblood->xfloat >> 8) + 8, (hoiblood->yfloat >> 8) + 8 + (t<<4) ))
	    {
	      if (r!=0)
		{
		  
		  if (hoiblood->xspd < (0-(8 << 8)))
		    {
		      cloud_init((hoiblood->xfloat >> 8) + 4, (hoiblood->yfloat >> 8) + 48, 0, 1);
		    }
		  else
		    {
		    }
		  hoiblood->xspd = (0-hoiblood->xspd)-SPD_BOUNCE;
		  if (hoiblood->xspd < 0) hoiblood->xspd = 0;
		  
		  hoiblood->xfloat += 0xf00;
		  hoiblood->xfloat += 0x800;
		  (hoiblood->xfloat) &= 0xfffff000;
		  hoiblood->xfloat -= 0x800;
		  
		  goto endcheckx;
		}
	    }
	}
    }

  // check for sokobans

  temp = sokoban_collision(soko1, (hoiblood->xfloat >> 8)+8, (hoiblood->yfloat >> 8)+8, object->sizex-16, object->sizey-24,  (hoiblood->xspd > 0) ? 1 : 3);
  temp |= sokoban_collision(soko2, (hoiblood->xfloat >> 8)+8, (hoiblood->yfloat >> 8)+8, object->sizex-16, object->sizey-24,  (hoiblood->xspd > 0) ? 1 : 3);
  temp |= sokoban_collision(soko3, (hoiblood->xfloat >> 8)+8, (hoiblood->yfloat >> 8)+8, object->sizex-16, object->sizey-24,  (hoiblood->xspd > 0) ? 1 : 3);

// check for puzzle in world 3

  if (world == 2 && level && 1)
  {
    UINT16 tempie;
    if ((hoiblood->xfloat >> 8) > 1440 && (hoiblood->xfloat >> 8) < 2080 && (hoiblood->yfloat >> 8) > 1332 && (hoiblood->yfloat >> 8) < 1974)
	{
  	  tempie = ((((hoiblood->yfloat >> 8)-1332) / 128) * 5) + (((hoiblood->xfloat >> 8)-1440) / 128);

      if (tempie == lastcolumnr) temp |= 1;
    }
  }

  if (temp)
    {
      if (hoiblood->xspd > 0)
	{
	  if (hoiblood->xspd > ((8 << 8)+0))
	    {
	      cloud_init((hoiblood->xfloat >> 8) + 20, (hoiblood->yfloat >> 8) + 48, 0, 1);
	    }
	  else
	    {
	    }
	  
	  hoiblood->xfloat -= (hoiblood->xspd+0);

	  hoiblood->xspd = (0-hoiblood->xspd)+SPD_BOUNCE;
	  if (hoiblood->xspd > 0) hoiblood->xspd = 0;
	  
	  goto endcheckx;
	}
      else
	{
	  if (hoiblood->xspd < (0-(8 << 8)))
	    {
	      cloud_init((hoiblood->xfloat >> 8) + 4, (hoiblood->yfloat >> 8) + 48, 0, 1);
	    }
	  else
	    {
	    }

	  hoiblood->xfloat -= (0+hoiblood->xspd);

	  hoiblood->xspd = (0-hoiblood->xspd)-SPD_BOUNCE;
	  if (hoiblood->xspd < 0) hoiblood->xspd = 0;
	  
	  goto endcheckx;
	}
    }

    if (!mc_autorun)
	{
	  if ((hoiblood->xfloat >> 8) < player1.worldx)    // && (hoiblood->xspd < basespdx))
	  {
        if (hoiblood->xspd < basespdx)
	    {
          hoiblood->xfloat -= (basespdx+hoiblood->xspd);
	      hoiblood->xspd = (basespdx-hoiblood->xspd)-SPD_BOUNCE;
	    }
        if (hoiblood->xspd < basespdx+256) hoiblood->xspd = basespdx+256;

	  }

      if ((hoiblood->xfloat >> 8) > (player1.worldx+580))    // && (hoiblood->xspd < basespdx))
	  {
        if (hoiblood->xspd > basespdx)
	    {
          hoiblood->xfloat -= (basespdx+hoiblood->xspd);
	      hoiblood->xspd = (basespdx-hoiblood->xspd)+SPD_BOUNCE;
	    }
        if (hoiblood->xspd > basespdx-256) hoiblood->xspd = basespdx-256;

	  }
	}

endcheckx:
  

  if ((hoiblood->xfloat >> 8) < (player1.worldx-32))
  {
    // buiten beeld gevallen?

	  
    if (object == hoi)
    {
      player1.energy = 0;
    }
  }

  if (hoiblood->downkey && ((hoiblood->yfloat>>8) == oldy) )
    {
      if (hoiblood->ycamxtra < 160) hoiblood->ycamxtra += 8;
    }

  if (hoiblood->upkey && ((hoiblood->yfloat>>8) == oldy) )
    {
      if (hoiblood->ycamxtra > -160) hoiblood->ycamxtra -= 8;
    }

  if (hoiblood->rightkey && ((hoiblood->xfloat>>8) == oldx) )
    {
      if (hoiblood->xcamxtra < 160) hoiblood->xcamxtra += 8;
    }

  if (hoiblood->leftkey && ((hoiblood->xfloat>>8) == oldx) )
    {
      if (hoiblood->xcamxtra > -160) hoiblood->xcamxtra -= 8;
    }

  if (hoiblood->ycamxtra > 4 ) hoiblood->ycamxtra -= 4;
  if (hoiblood->ycamxtra < -4) hoiblood->ycamxtra += 4;
  if (hoiblood->xcamxtra > 4 ) hoiblood->xcamxtra -= 4;
  if (hoiblood->xcamxtra < -4) hoiblood->xcamxtra += 4;


  //  rememdir = hoiblood->lastdirx;

  //  if (hoiblood->xspd > 512)
  if ((INT32)(((INT32)hoiblood->xfloat >> 8) - (INT32)object->x) > 1)
    {
      hoiblood->lastdirx = 1;
    }
  else
    {
      if ((INT32)(((INT32)hoiblood->xfloat >> 8) - (INT32)object->x) >= -1 && hoiblood->rightkey==1)
	{
	  hoiblood->lastdirx = 1;
	}
    }

  //  if (hoiblood->xspd < -512)
  if ((INT32)(((INT32)hoiblood->xfloat >> 8) - (INT32)object->x) < -1)
    {
      hoiblood->lastdirx = -1;
    }
  else
    {
      if ((INT32)(((INT32)hoiblood->xfloat >> 8) - (INT32)object->x) <= 1 && hoiblood->leftkey==1)
	{
	  hoiblood->lastdirx = -1;
	}
    }

  
  object->x = (hoiblood->xfloat >> 8);
  //object->y = (hoiblood->yfloat + ((sinus512[(hoiblood->hoovercnt) & 1023] * 12) >> 3) >> 8);   //#####
  object->y = ((hoiblood->yfloat + ((sinus512[(hoiblood->hoovercnt) & 1023] * 12) >> 3)) >> 8);   //#####


  // Handle anims

  if (hoiblood->anmdir != hoiblood->lastdirx)
    {
      if (hoiblood->anmturn == 0)   // no anim currently busy?
	{
	  hoiblood->anmturn = 12;
	  hoiblood->anmdir = hoiblood->lastdirx;
	}
    }

  if (hoiblood->anmturn != 0)
    {
      hoiblood->anmturn--;
      anim_setsequence(object->animate, (hoiblood->anmdir == 1) ? SEQ_TURNRIGHT : SEQ_TURNLEFT, SEQ_FORCE);
      object->frame = anim_forceframe(object->animate, hoiblood->anmturn / 4);

      // calculate temp for evt. exhaust

      temp  = (hoiblood->anmdir == 1) ? 12 : 36;
      temp += (hoiblood->anmturn *2);
    }
  else
    {
      if ((INT16) hoiblood->lastdirx == 1)
	{
	  anim_setsequence(object->animate, SEQ_FLYRIGHT, SEQ_FORCE);
	}
      else
	{
	  anim_setsequence(object->animate, SEQ_FLYLEFT, SEQ_FORCE);
	}
  
      temp = (hoiblood->xspd > 0) ? (hoiblood->xspd/1024) : -(hoiblood->xspd/1024);
      if (temp > 2) temp = 2;
      object->frame = anim_forceframe(object->animate, temp);

      // calculate temp for evt. exhaust

      temp *= 2;
      temp += (hoiblood->lastdirx == 1) ? 6 : 0;
    }
  // handle 'flashing'

  if (hoiblood->flashcnt)
    {
      hoiblood->flashcnt--;
      object->blitsizey = flashtab[hoiblood->flashcnt] == 0 ?  1 : object->sizey;
    }

  // exhaust

  hoiblood->exhaustcnt++;
  hoiblood->exhaustcnt&=7;

  temp2 = 0;

  if (hoiblood->downkey)
    {
      if ((hoiblood->exhaustcnt & 7) == 0) temp2 = 1;
    }
  else
    {
      if ((hoiblood->exhaustcnt & 3) == 0) temp2 = 1;
    }

  if (hoiblood->upkey)
    {
      if ((hoiblood->exhaustcnt & 1) == 0) temp2 = 1;
    }

  if (temp2)
    {
      plof_init(object->x+exhaustadd[temp], object->y+exhaustadd[temp+1], 2, 0, 0, 256, 0);
#if 0
      plof_init(object->x+exhaustadd[temp], object->y+exhaustadd[temp+1], 2, 0, -128, 192);
      plof_init(object->x+exhaustadd[temp], object->y+exhaustadd[temp+1], 2, 0, 128, 192);
      plof_init(object->x+exhaustadd[temp], object->y+exhaustadd[temp+1], 2, 0, -64, 400);
      plof_init(object->x+exhaustadd[temp], object->y+exhaustadd[temp+1], 2, 0, 64, 400);
#endif
    }

  // handle shooting

  if (hoiblood->shootkey && temp <12 && shootenabledflg == 1)
    {
      hoiblood->shootkey = 0;
      bullet_init(object->x+shootadd[temp], object->y+shootadd[temp+1], (hoiblood->lastdirx < 0) ? -2848+hoiblood->xspd : 2848+hoiblood->xspd, 0);
      play_shoot(object->x, object->y);
      if (hoiblood->lastdirx < 0)
	{
	  plof_init(object->x+shootadd[temp]+10, object->y+shootadd[temp+1]+12, 4, 0, 0, 0, 0);
	}
      else
	{
	  plof_init(object->x+shootadd[temp]+10, object->y+shootadd[temp+1]+12, 5, 0, 0, 0, 0);
	}
    }
  

  return 0;
}



void hoi_death(OBJECT *object, UINT32 param)
{
}


void hoi_clear(OBJECT *object, UINT32 param)
{
  if (object == hoi)
    {
      hoicollapse_init(object->x, object->y);
    }
}


void hoirise_init(OBJECT *newhoi, UINT16 sx, UINT16 sy, UINT16 dx, UINT16 dy)
{
  OBJECT *hoirise;
  ANIM   *hoiriseanim;

  hoirise = (OBJECT *) malloc (sizeof(OBJECT));
  hoirise->x = sx;
  hoirise->y = sy;
  hoirise->live  = hoirise_live;
  hoirise->death = hoirise_death;
  hoirise->clear = hoirise_clear;
  hoirise->sizex = 64;
  hoirise->sizey = 86;
  hoirise->blitsizex = 0;
  hoirise->blitsizey = 0;
  hoirise->blitstartx = 0;
  hoirise->blitstarty = 0;

	hoi->frame = 0;
      hoi->live = hoi_live_warp;

  hoirise->frame = 0;
  hoirise->animate = 0;

  hoirise->blood = (void *) malloc(sizeof(HOIRISE_BLOOD));
//  vgassert(hoirise->blood);

  HOIRISE_BLOOD *hoiriseblood;
  hoiriseblood = (HOIRISE_BLOOD *)hoirise->blood;

  hoiriseanim         = (ANIM *)copy_anim (orghoirise);
  hoiriseblood->frame = anim_setsequence(hoiriseanim, 0, SEQ_FORCE);

  hoiriseblood->sx = sx;
  hoiriseblood->sy = sy;
  hoiriseblood->dx = dx;
  hoiriseblood->dy = dy;

  if ((sx == dx) && (sy == dy))
  {
	hoiriseblood->distance = 0;
	hoiriseblood->size = 320;
      hoiriseblood->sinxspd = 5;
      hoiriseblood->sinyspd = 4;
	hoiriseblood->deathcnt = 0;
      hoiriseblood->sizeadd = 0;
  }
  else
  {
	if (sx == 0 && sy == 0)
	{
        hoiriseblood->distance = 0;
	  hoiriseblood->size = 2;
        hoiriseblood->sinxspd = 8;
        hoiriseblood->sinyspd = 8;
	  hoiriseblood->deathcnt = 75;
        hoirise->x = hoi->x;
        hoirise->y = hoi->y;
        hoiriseblood->sx = hoi->x;
        hoiriseblood->sy = hoi->y;
        sx = hoi->x;
        sy = hoi->y;
        hoiriseblood->dx = restartx;
        hoiriseblood->dy = restarty;
        hoiriseblood->sizeadd = 60;
	}
	else
	{
	  hoiriseblood->deathcnt = 0;
	  hoiriseblood->distance = 150;
	  hoiriseblood->size = 0;
	  hoiriseblood->sinxspd = 2;
	  hoiriseblood->sinyspd = 1;
        hoiriseblood->sizeadd = 0;
	}
  }
  
  hoiriseblood->sinx = 0;
  hoiriseblood->siny = 2;
  hoiriseblood->fastmode = 1;
  hoiriseblood->newobject = newhoi;

  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)hoi->blood;

  hoi->x = sx;
  hoi->y = sy;
  hoiblood->xfloat = sx<<8;
  hoiblood->yfloat = sy<<8;

  object_add(hoirise);
}


int hoirise_live (OBJECT *object, UINT32 param)
{

  UINT16 sx,sy,sx2,sy2;
  INT16  bx,by;
  UINT16 x,y,i;
  UINT16 blow;
  UBYTE *orgsurface;
  UBYTE *srcsurface;
  UBYTE *moonsurface;

  UINT16 orgpitch;

//  srcdc = player1.curmap->blitbuf->get_dc();
//  orgsurface = (UBYTE *)srcdc->surface;

  orgsurface = video->lock_bbuffer();
  orgpitch = video->get_bpitch();

  HOIRISE_BLOOD *hoiriseblood;
  hoiriseblood = (HOIRISE_BLOOD *)object->blood; /* in case of multiple hois */

  hoiriseblood->sinx += hoiriseblood->sinxspd;
  hoiriseblood->siny += hoiriseblood->sinyspd;

  hoiriseblood->sinx &= 255;
  hoiriseblood->siny &= 255;


  sy2 = hoiriseblood->siny;
  sx2 = hoiriseblood->sinx;

  moonsurface = hoiriseblood->frame->lock_buffer();

  blow = hoiriseblood->size;

  if (!hoiriseblood->deathcnt)
  {
  
  for (y=0; y<object->sizey; y++)
    {

      sx = sx2;
      sy = sy2;

      for (x=0; x<object->sizex; x++)
	{
	  UINT16 px,py;

	  if ( *(moonsurface) )
	    {
	      px = x + ((sinus128[sx]*blow)>>8) + object->x - player1.worldx;
	      py = y + ((sinus128[sy]*blow)>>8) + object->y - player1.worldy;

		//            plotapix(px,py,*(moonsurface));

	      if (px >= 640) goto skip;
	      if (py >= 480) goto skip;

#if 1  // moet nog geprogged worden!

	      srcsurface = orgsurface + px + (py*orgpitch);

//	      *(dotpnt++) = (UINT32)srcsurface;
//	      *(dotcolpnt++) = *(srcsurface);

	      *(srcsurface) = *moonsurface;

#endif

	    skip:
            sx += 5;
            sx &= 255;
            sy += 2;
            sy &= 255;
          }

	  moonsurface++;
	}
      if (blow) blow--;
      sy2 += 1;
      sy2 &= 255;
      sx2 += 2;
      sx2 &= 255;

      moonsurface += hoiriseblood->frame->get_pitch() - hoiriseblood->frame->get_width();
  
  }  
  }
  else
  {
  bx = -34;
  by = -44;
  for (y=0; y<object->sizey; y++)
    {
      bx = -34;
      for (x=0; x<object->sizex; x++)
	{
	  UINT16 px,py;

	  if ( *(moonsurface) )
	    {
	      px = x + ((bx*blow)>>8) + object->x - player1.worldx;
	      py = y + ((by*blow)>>8) + object->y - player1.worldy;

		//            plotapix(px,py,*(moonsurface));

	      if (px >= 640) goto skip2;
	      if (py >= 480) goto skip2;

#if 1  // moet nog geprogged worden!

	      srcsurface = orgsurface + px + (py*orgpitch);

//	      *(dotpnt++) = (UINT32)srcsurface;
//	      *(dotcolpnt++) = *(srcsurface);

	      *(srcsurface) = *moonsurface;

#endif

          }

        skip2:

        bx += 1;
	  moonsurface++;
	}
//      if (blow) blow--;
      by += 1;

      moonsurface += hoiriseblood->frame->get_pitch() - hoiriseblood->frame->get_width();
  
  }  
  }

  video->unlock_bbuffer();
  hoiriseblood->frame->unlock_buffer();

  if (shootkey) hoiriseblood->fastmode = 2;

  for (i=0; i<hoiriseblood->fastmode; i++)
  {
    if (hoiriseblood->deathcnt)
    {
      hoiriseblood->deathcnt--;
      hoiriseblood->size += (hoiriseblood->sizeadd);
      hoiriseblood->sizeadd +=2;
      hoiriseblood->distance = 150;
  
      if (hoiriseblood->deathcnt == 0)
      {
        reinithoi();
        hoiriseblood->sinxspd = 5;
        hoiriseblood->sinyspd = 4;
        hoiriseblood->size = 224;
      }
    }
    else
    {
      if (hoiriseblood->distance > 75)
      {
        if(!hoiriseblood->sizeadd)    // don't do this after a death sequence!
        {
          if (hoiriseblood->size < 512) hoiriseblood->size+= 4;
        }
      }
      else
      {
        hoiriseblood->size-= 2;
        if (hoiriseblood->size < 0) hoiriseblood->size = 0;
      }

      if (hoiriseblood->distance) hoiriseblood->distance--;

      object->x = ((hoiriseblood->sx - hoiriseblood->dx)*hoiriseblood->distance) / 150;
      object->y = ((hoiriseblood->sy - hoiriseblood->dy)*hoiriseblood->distance) / 150;

      object->x += hoiriseblood->dx;
      object->y += hoiriseblood->dy;
    }

    hoi->x = object->x;
    hoi->y = object->y;

    if (hoiriseblood->size == 0)
      {
        if (world != 2)
        {
              hoi_init(hoiriseblood->newobject, object->x, object->y);
        }
        else
        {
              hoi_fly_init(hoiriseblood->newobject, object->x, object->y);
        }
        return 1;
      }
  }   // for i

  return 0;
}


void reinithoi(void)
{
    HOI_BLOOD *hoiblood;

    if (player1.lives == 0) exitflg = 1;

    if (!cheatmode) player1.lives--;
    player1.energy=33;
    player1.newlife = 0;   // start collecting for a new life from the beginning again


    hoiblood = (HOI_BLOOD *) hoi->blood;

    hoiblood->hoi_caps->maxspd = 140;       // shoes uitzetten
    hoiblood->gravy = 0;
    hoiblood->spdx = 0;
    hoiblood->flashcnt = 200;

    reinitlevels();
}


void reinitlevels(void)
{
    paraflg = restartpara;

//    hoiblood->xfloat = restartx<<8;
//    hoiblood->yfloat = restarty<<8;

    fakespeedadd = 0;
    fallspeed = 0;
    drunk_amp = 0;
    autoscrollspd = 0;
	streepflg = 0;
//	totalshootcnt = 0;
//	totalgenercnt = 0;

//level specific re-inits

    if (world == 0 && level == 3) stop_cammove();   // camera sound uit;

    sokoban_reinit(soko1); // re-init evt. sokobans
    sokoban_reinit(soko2);
    sokoban_reinit(soko3);
    sokoban_reinit(soko4);

    if (world == 0 && level == 3)
      {
	// place running shoes
	UINT16 x,y;
	x = 0x1620;
	y = 0x460;
	x &= 0xfff0;
	y &= 0xfff0;
	player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)] = 1000;
	colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 17;

      }
}



void hoirise_death(OBJECT *object, UINT32 param)
{
}

void hoirise_clear(OBJECT *object, UINT32 param)
{
}



void hoicollapse_init(UINT16 x, UINT16 y)
{
  OBJECT *hoicollapse;
  ANIM   *hoicollapseanim;

  hoicollapse = (OBJECT *) malloc (sizeof(OBJECT));
  hoicollapse->x = x;
  hoicollapse->y = y;
  hoi->x = hoicollapse->x;
  hoi->y = hoicollapse->y;
  hoicollapse->live  = hoicollapse_live;
  hoicollapse->death = hoicollapse_death;
  hoicollapse->clear = hoicollapse_clear;
  hoicollapse->sizex = 64;
  hoicollapse->sizey = 86;
  hoicollapse->blitsizex = 0;
  hoicollapse->blitsizey = 0;
  hoicollapse->blitstartx = 0;
  hoicollapse->blitstarty = 0;

  hoicollapse->frame = 0;
  hoicollapse->animate = 0;

  hoicollapse->blood = (void *) malloc(sizeof(HOICOLLAPSE_BLOOD));
//  vgassert(hoicollapse->blood);

  HOICOLLAPSE_BLOOD *hoicollapseblood;
  hoicollapseblood = (HOICOLLAPSE_BLOOD *)hoicollapse->blood;

  hoicollapseanim         = (ANIM *)copy_anim(orghoirise);
  hoicollapseblood->frame = anim_setsequence(hoicollapseanim, 0, SEQ_FORCE);

  hoicollapseblood->sinx = 0;
  hoicollapseblood->siny = 64;
  hoicollapseblood->sinxspd = 5;
  hoicollapseblood->sinyspd = 4;
  hoicollapseblood->size = 0;
  object_add(hoicollapse);
}


int hoicollapse_live (OBJECT *object, UINT32 param)
{
  UINT16 sx,sy,sx2,sy2;
  UINT16 x,y;
  UINT16 blow;
  BYTE  *moonsurface;


  UBYTE *orgsurface;
  UBYTE *srcsurface;

  UINT16 orgpitch;

//  srcdc = player1.curmap->blitbuf->get_dc();
//  orgsurface = (UBYTE *)srcdc->surface;

  orgsurface = vidblitbuf->lock_buffer();
  orgpitch = vidblitbuf->get_pitch();

  HOICOLLAPSE_BLOOD *hoicollapseblood;
  hoicollapseblood = (HOICOLLAPSE_BLOOD *)object->blood; /* in case of multiple hois */

  hoicollapseblood->sinx += hoicollapseblood->sinxspd;
  hoicollapseblood->siny += hoicollapseblood->sinyspd;

  hoicollapseblood->sinx &= 255;
  hoicollapseblood->siny &= 255;


  sy2 = hoicollapseblood->siny;
  sx2 = hoicollapseblood->sinx;

  moonsurface = 0; //hoicollapseblood->frame->get_surface();

  blow = hoicollapseblood->size;


  for (y=0; y<object->sizey; y++)
    {

      sx = sx2;
      sy = sy2;

      for (x=0; x<object->sizex; x++)
	{
	  UINT16 px,py;

	  if ( *(moonsurface) )
	    {
	      px = x + ((sinus128[sx]*blow)>>8) + object->x;
	      py = y + ((sinus128[sy]*blow)>>8) + object->y;

	      //            plotapix(px,py,*(moonsurface));

	      //  if (px > 640) goto skip;
	      if (py > 480) goto skip;

#if 0

	      srcsurface = orgsurface + px + ((py+CLIPMARGINY- player1.curmap->blitbuf->get_top())*(srcdc->mi.xRes+1));

	      *(dotpnt++) = (UINTPTR)srcsurface;
	      *(dotcolpnt++) = *(srcsurface);

	      *(srcsurface) = *moonsurface;

#endif

	    skip:
	      sx += 5;
	      sx &= 255;
	      sy += 0;
	      sy &= 255;
	    }

	  moonsurface++;
	}
      if (blow) blow--;
      sy2 += 1;
      sy2 &= 255;
      sx2 += 2;
      sx2 &= 255;
    }  

  hoicollapseblood->size+= 2;
  if (hoicollapseblood->size == 562)
    {
      return 1;
    }

  return 0;
}


void hoicollapse_death(OBJECT *object, UINT32 param)
{
}

void hoicollapse_clear(OBJECT *object, UINT32 param)
{
}









