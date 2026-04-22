#include <frm_wrk.hpp>
#include <mine.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <plof.hpp>


void mine_init(UINT16 x, UINT16 y, INT16 xspd_, INT16 yspd_, ANIM *anim, UINT16 seqnr, UINT16 lifespan, UINT16 exploflg, UINT16 animspd, UINT16 clipflg)
{
  OBJECT *mine;
  mine = (OBJECT *) malloc (sizeof(OBJECT));

  mine->live  = mine_live;
  mine->death = mine_death;
  mine->clear = mine_clear;
  mine->blitsizex = 0;
  mine->blitsizey = 0;

  if (anim == 0) anim = orgmine;
  mine->animate = copy_anim((ANIM *)anim);
  mine->frame   = anim_setsequence(mine->animate, seqnr, SEQ_FORCE);

  mine->sizex = mine->frame->get_width();
  mine->sizey = mine->frame->get_height();

  x -= (mine->sizex/2);
  y -= (mine->sizey/2);

  mine->x = x;
  mine->y = y;

  mine->coloffsetx = mine->sizex/3;
  mine->coloffsety = mine->sizex/3;
  mine->colwidth   = mine->sizey/3;
  mine->colheight  = mine->sizey/3;

  mine->lethal = 1;  //1 ; // nrg drain

  mine->blood = (void *) malloc(sizeof(MINE_BLOOD));
//  vgassert(mine->blood);
  MINE_BLOOD *mineblood;
  mineblood = (MINE_BLOOD *)mine->blood;
  mineblood->xfloat = x << 8;
  mineblood->yfloat = y << 8;
  mineblood->xspd   = xspd_;
  mineblood->yspd   = yspd_;
  mineblood->lifespan = lifespan;
  mineblood->exploflg = exploflg;
  mineblood->animspd = animspd;
  mineblood->clipflg = clipflg;

  object_add(mine);
}

int mine_live (OBJECT *object, UINT32 param)
{
  UINT16 i;

  MINE_BLOOD *mineblood;
  mineblood = (MINE_BLOOD *)object->blood; /* in case of multiple mines */

  mineblood->xfloat += mineblood->xspd;
  mineblood->yfloat += mineblood->yspd;

  object->x = mineblood->xfloat >> 8;
  object->y = mineblood->yfloat >> 8;

  if (collision(object->x+(object->sizex/2), object->y + (object->sizey/2) ))
  {
    if (mineblood->exploflg) plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 1, 0, 0, 0);
    return 1;
  }

  for (i=0; i<mineblood->animspd; i++)
    {
      object->frame = anim_nextframe(object->animate);
    }

  if (mineblood->lifespan)
  {
    mineblood->lifespan -= 1;

    if (!mineblood->lifespan)
    {
      if (mineblood->exploflg) plof_init(object->x+(object->sizex/2), object->y+(object->sizey/2), 1, 1, 0, 0, 0);
      return 1;
    }
  }

  if (mineblood->clipflg == 0)   // clippen
    {
      if ((UINT16) (object->x - player1.worldx + 700) > 1400)
	{
	  return 1;
	}
    }

  return 0;
}

void mine_death(OBJECT *object, UINT32 param)
{
}

void mine_clear(OBJECT *object, UINT32 param)
{
}


