#include <frm_wrk.hpp>
#include <sokoban.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>


OBJECT *sokoban_init(SOKOBAN_POSSES *group1,
         SOKOBAN_POSSES *group2,
         SOKOBAN_POSSES *group3,
         SOKOBAN_POSSES *group4)
{
  OBJECT *sokoban;
  SOKOBAN_POSSES *curpos;
  UINT16 j;

  sokoban = (OBJECT *) malloc (sizeof(OBJECT));
  sokoban->x = 0;
  sokoban->y = 0;
  sokoban->live  = sokoban_live;
  sokoban->death = sokoban_death;
  sokoban->clear = sokoban_clear;

  sokoban->blitsizex = 0;
  sokoban->blitsizey = 0;
  sokoban->blitstartx = 0;
  sokoban->blitstarty = 0;

  sokoban->lethal = 0;             //harmless

  sokoban->animate = 0;
  sokoban->frame = 0;

  sokoban->sizex = 10;
  sokoban->sizey = 10;

  sokoban->blitsizex = sokoban->sizex;
  sokoban->blitsizey = sokoban->sizey;
  sokoban->blitstartx = 0;
  sokoban->blitstarty = 0;
  sokoban->lethal = 0;
  sokoban->coloffsetx = 6;
  sokoban->coloffsety = 6;
  sokoban->colwidth = sokoban->sizex - 12;
  sokoban->colheight= sokoban->sizey - 12;

  sokoban->blood = (void *) malloc(sizeof(SOKOBAN_BLOOD));
//  vgassert(sokoban->blood);
  SOKOBAN_BLOOD *sokobanblood;

  sokobanblood = (SOKOBAN_BLOOD *)sokoban->blood;
  sokobanblood->movingcnt = 0;
  sokobanblood->directionflg = 0;


  curpos = group1;

  for (j=0; j<4; j++)
    {
      UINT16 i;
      
      SOKOBAN_GROUP *curgroup;
      sokobanblood->groups[j] = (SOKOBAN_GROUP *) malloc(sizeof(SOKOBAN_GROUP));
//      vgassert(sokoban->blood);
      
      curgroup = sokobanblood->groups[j];
      
      curgroup->checkflg = 0;
      curgroup->movingflg = 0;
      curgroup->orgpos = curpos;

      if (curpos == 0)
	{
	  curgroup->seqnr = 0;
	}
      else
	{
	  curgroup->seqnr = curpos->seqnr;
	  curgroup->moveability = curpos->moveability;
	}

      for (i=0; i<16; i++)
	{
	  OBJECT *block;
	  
	  if (curpos == 0)
	    {
	      curgroup->block[i] = 0;
	    }
	  else
	    {
	      if ( curpos->xpos[i] == 0 )      // block not defined?
		{
		  curgroup->block[i] = 0;
		}
	      else
		{

		  block = (OBJECT *) malloc (sizeof(OBJECT));
		  block->x = curpos->xpos[i];
		  block->y = curpos->ypos[i];
		  block->live  = 0;
		  block->death = 0;
		  block->clear = 0;
		  block->blitsizex = 0;
		  block->blitsizey = 0;
		  
		  block->lethal = 0; // non lethal
		  
		  block->animate = copy_anim(orgsokoban);
		  block->frame   = anim_setsequence(block->animate, curgroup->seqnr, SEQ_FORCE);
		  
		  block->sizex = block->frame->get_width();
		  block->sizey = block->frame->get_height();
		  
		  block->coloffsetx = 8;
		  block->coloffsety = 8;
		  block->colwidth  = 16;
		  block->colheight = 16;
		  
		  block->blood = 0;
		  
		  object_add(block);
		  curgroup->block[i] = block;
		}
	    }
	}

// cycle posses    (a bit lame method of cycling)

      if (curpos == group3) curpos = group4;

      if (curpos == group2) curpos = group3;

      if (curpos == group1)
	{
	  curpos = group2;
	}
    }

  object_add(sokoban);
  return sokoban;
}


void sokoban_reinit(OBJECT *soko)
{
  SOKOBAN_BLOOD* sokoblood;
  UINT16 i,j;

  if (world != 2) return;     // No Sokoban-blocks outside world 2
  if (soko == 0) return;

  sokoblood = (SOKOBAN_BLOOD *) soko->blood;

  for (j=0; j<4; j++)
    {
      SOKOBAN_GROUP *sokogroup;

      sokogroup = sokoblood->groups[j];

      if (sokogroup != 0)
	{
	  if (sokogroup->orgpos != 0)
	    {
	      for (i=0; i<16; i++)
		{
		  if (sokogroup->block[i] != 0)
		    {
		      sokogroup->block[i]->x = sokogroup->orgpos->xpos[i];
		      sokogroup->block[i]->y = sokogroup->orgpos->ypos[i];
		    }
		}
	    }
	}
    }
}


// returns TRUE  if collisioned
// returns FALSE if not

int sokoban_collision(OBJECT *soko, UINT16 mx, UINT16 my, UINT16 mw, UINT16 mh, UINT16 direction)
{
  SOKOBAN_BLOOD* sokoblood;
  UINT16 i,j;
  UINT16 moveflg;
  UINT16 scanflg;
  INT16  dx,dy;       /* wanted movement of puzzleblocks (deltax and delta y) */
  HOI_BLOOD *hoiblood;

  if (world != 2) return FALSE;     // No Sokoban-blocks outside world 2
  if (soko == 0) return FALSE;     // No Sokoban-blocks

  sokoblood = (SOKOBAN_BLOOD *) soko->blood;

  moveflg = 0; // no move necessarry
  scanflg = 0;

  switch (direction)
    {
    case 0:
      dx = 0;
      dy = -32;
      break;
    case 1:
      dx = 32;
      dy = 0;
      break;
    case 2:
      dx = 0;
      dy = 32;
      break;
    case 3:
      dx = -32;
      dy = 0;
      break;
    }

  for (j=0; j<4; j++)
    {
      SOKOBAN_GROUP *sokogroup;

      sokogroup = sokoblood->groups[j];

      sokogroup->checkflg = 0;   // reset to zero
      for (i=0; i<16; i++)
	{
	  OBJECT *block;
	  INT16 x;
	  INT16 y;

	  block = sokogroup->block[i];
	  if (block == 0) goto breakfori;

	  x = (INT16)(block->x) - (INT16)(mx);
	  x += 31;                            // size of sokoban block
	  if ((UINT16)x < (mw+31))
	    {
	      y = (INT16)(block->y) - (INT16)(my);
	      y += 31;                        // size of sokoban block
	      if ((UINT16)y < (mh+31))
		{

//		  RASTCOL2(207,255,0,255);
		  if (sokoblood->movingcnt) return TRUE; // currently moving so return collision no matter what

		  sokogroup->checkflg = 1;
		  scanflg = 1;     // At least one valid collision so start scanning away!
		  goto breakfori;
		}
	    }
	}
breakfori:
      sokogroup = 0;   // dummy instruction for label
    }

// check groups if they should and can move

  if (!scanflg) return FALSE;


  hoiblood = (HOI_BLOOD *)hoi->blood;

  if (hoiblood->xcamxtra > -24 && hoiblood->xcamxtra < 24 && dx != 0) return TRUE; // eerst duwen!!!
  if (hoiblood->ycamxtra > -24 && hoiblood->ycamxtra < 24 && dy != 0) return TRUE; // eerst duwen!!!


  while(scanflg)
    {    
      UINT16 i;
      SOKOBAN_GROUP *sokogroup;

      scanflg = 0;
      for (j=0; j<4; j++)
	{
	  sokogroup = sokoblood->groups[j];

  
	  if (sokogroup->checkflg == 1)
	    {
	      scanflg = 1;

// new!

		  if ((sokogroup->moveability&1) == 0 && dx != 0) return TRUE;   // movement in X while block is NOT allowed to!
	      if ((sokogroup->moveability&2) == 0 && dy != 0) return TRUE;   // movement in Y while block is NOT allowed to!

		  // check if all (upto) 16 blocks are able to move
	      
	      for(i=0; i<16; i++)
		{
		  if (sokogroup->block[i] == 0) goto breakilus;
		  if (collision(sokogroup->block[i]->x+dx, sokogroup->block[i]->y+dy))
		    {
		      return TRUE;   // one block collisioned so nothing moves
		    }

		  if(sokocollision(soko,sokogroup->block[i]->x+dx, sokogroup->block[i]->y+dy, j))
		    {
		      scanflg = 1;
		    }
		}
	    breakilus:
	      sokogroup->checkflg = 2;  // all blocks could move
	    }
	}
    }

// check which groups should move and label them as such

  for (j=0; j<4; j++)
    {
      SOKOBAN_GROUP *sokogroup;

      sokogroup = sokoblood->groups[j];

      sokogroup->movingflg = 0;

      if (sokogroup->checkflg == 2)
	{
		play_schuif();
		sokogroup->movingflg = 1;
	  sokoblood->movingcnt = 32;
	  sokoblood->directionflg = direction;
	}
      sokogroup->checkflg = 0;
    }

  return FALSE;
}


int sokocollision(OBJECT *soko, UINT16 bx, UINT16 by, UINT16 group)
{
  SOKOBAN_BLOOD* sokoblood;
  UINT16 i;
  UINT16 j;


  sokoblood = (SOKOBAN_BLOOD *) soko->blood;

  for (j=0; j<4; j++)
    {
      SOKOBAN_GROUP *sokogroup;

      if (j != group)  // is it the same group? don't check
	{
	  sokogroup = sokoblood->groups[j];

	  if (sokogroup->checkflg==0) // is the group allready marked for movement or checking?
	    {
//
	      for (i=0; i<16; i++)
		{
		  OBJECT *block;
		  
		  block = sokogroup->block[i];
		  if (block == 0) goto breakforit;
		  
		  if (block->x == bx && block->y == by)
		    {
		      
		      sokogroup->checkflg = 1;
		      //			  scanflg = 1;     // At least one valid collision so start scanning away!
		      return 1;
		    }
		}
	    breakforit:
	      i=0;
//
	   }
	}
    }
  return 0;
}


UINT16 sokomove[32] = { 
  1,0,1,0,1,1,2,2,3,3,4,3,
  3,2,2,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0
};


int sokoban_live (OBJECT *object, UINT32 param)
{
  SOKOBAN_BLOOD *sokoblood;
  UINT16 i;
  UINT16 j;
  INT16  dx,dy;       /* wanted movement of puzzleblocks (deltax and delta y) */
  SOKOBAN_GROUP *sokogroup;

  sokoblood = (SOKOBAN_BLOOD *)object->blood; /* in case of multiple sokobans */

  if (sokoblood->movingcnt)
    {
      sokoblood->movingcnt--;
      {
	switch (sokoblood->directionflg)
	  {
	  case 0:
	    dx = 0;
	    dy = -1;
	    break;
	  case 1:
	    dx = 1;
	    dy = 0;
	    break;
	  case 2:
	    dx = 0;
	    dy = 1;
	    break;
	  case 3:
	    dx = -1;
	    dy = 0;
	    break;
	  }
	
	dx *= sokomove[31-sokoblood->movingcnt];
	dy *= sokomove[31-sokoblood->movingcnt];
	

    sokomoved = ((dy<<8)&0xff00)+(dx & 0xff);    // global flag that a sokomoved this frame and that moonchild thusly doesn't move!

//    hoi->x += dx;
//    hoi->y += dy;
	
//    hoiblood = (HOI_BLOOD *)hoi->blood;
//    hoiblood->xfloat += dx<<8;
//    hoiblood->yfloat += dy<<8;

    for (j=0; j<4; j++)
	  {
	    sokogroup = sokoblood->groups[j];
	    if (sokogroup->movingflg == 1)
	      {
		for(i=0; i<16; i++)
		  {
		    if (sokogroup->block[i] == 0) goto breakilus2;
		    
		    sokogroup->block[i]->x+=dx;
		    sokogroup->block[i]->y+=dy;
		    
		  }
	      breakilus2:
		sokogroup->checkflg = 2;  // all blocks could move
	      }
	  }
      }
    }
  return 0;
}

void sokoban_death(OBJECT *object, UINT32 param)
{
}

void sokoban_clear(OBJECT *object, UINT32 param)
{
  SOKOBAN_BLOOD *sokobanblood;

  sokobanblood = (SOKOBAN_BLOOD *) object->blood;

  free(sokobanblood->groups[0]);
  free(sokobanblood->groups[1]);
  free(sokobanblood->groups[2]);
  free(sokobanblood->groups[3]);
}



