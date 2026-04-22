#include <frm_wrk.hpp>
#include <doldoler.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sound.hpp>
#include <mine.hpp>


// x-y offset voor met klok mee draaiien
// x-y offset voor tegen klok in draaiien
// x-y offset voor beweging
// animatie sequence

INT16 dooldirtab[] = { 0,-32, -32,32,  -32,0, 0,-2, 0,
                         64,0, 0,-32, 32,-32,  2,0, 1,
                        32,64,  64,0,  64,32,  0,2, 2,
					   -32,32, 32,64,   0,64, -2,0, 3};


void dooldoler_init (UINT16 x, UINT16 y, UINT16 startdir)
{
  OBJECT *dooldoler;
  dooldoler = (OBJECT *) malloc (sizeof(OBJECT));
  dooldoler->x = x;
  dooldoler->y = y;
  dooldoler->live  = dooldoler_live;
  dooldoler->death = dooldoler_death;
  dooldoler->clear = dooldoler_clear;
  dooldoler->blitsizex = 0;
  dooldoler->blitsizey = 0;
  dooldoler->blitstartx = 0;
  dooldoler->blitstarty = 0;


  dooldoler->animate = copy_anim((ANIM *)orgdooldoler);
  dooldoler->frame = anim_setsequence(dooldoler->animate, 0, SEQ_FORCE);


  dooldoler->sizex = dooldoler->frame->get_width();
  dooldoler->sizey = dooldoler->frame->get_height();

  dooldoler->blitsizex = dooldoler->sizex;
  dooldoler->blitsizey = dooldoler->sizey;
  dooldoler->blitstartx = 0;
  dooldoler->blitstarty = 0;
  dooldoler->lethal = 1;
  dooldoler->coloffsetx = 8;
  dooldoler->coloffsety = 8;
  dooldoler->colwidth = dooldoler->sizex - 16;
  dooldoler->colheight= dooldoler->sizey - 16;


  dooldoler->blood = (void *) malloc(sizeof(DOOLDOLER_BLOOD));
//  vgassert(dooldoler->blood);
  DOOLDOLER_BLOOD *dooldolerblood;

  dooldolerblood = (DOOLDOLER_BLOOD *)dooldoler->blood;
  dooldolerblood->curdir     = startdir;
  dooldolerblood->shootdelay = 75;
  object_add(dooldoler);
}


int dooldoler_live (OBJECT *object, UINT32 param)
{
  DOOLDOLER_BLOOD *dooldolerblood;
  UINT16           offset;

  dooldolerblood = (DOOLDOLER_BLOOD *)object->blood; /* in case of multiple dooldolers */

  offset = dooldolerblood->curdir * 9;

  object->x += dooldirtab[offset+6];
  object->y += dooldirtab[offset+7];

  object->frame = anim_nextframe(object->animate);

  if (((object->x & 31) == 0) && ((object->y & 31) == 0))
  {
    if (player1.loadedmap->map[((object->y+dooldirtab[offset+1])*player1.loadedmap->mapsizex/32/32)+((object->x+dooldirtab[offset])/32)] != 0)
	{
      dooldolerblood->curdir++;
      dooldolerblood->curdir&=3;
      offset = dooldolerblood->curdir * 9;
      object->frame = anim_setsequence(object->animate, dooldirtab[offset+8], SEQ_FORCE);
      return 0;
	}
    if ((player1.loadedmap->map[((object->y+dooldirtab[offset+3])*player1.loadedmap->mapsizex/32/32)+((object->x+dooldirtab[offset+2])/32)] == 0) && (player1.loadedmap->map[((object->y+dooldirtab[offset+5])*player1.loadedmap->mapsizex/32/32)+((object->x+dooldirtab[offset+4])/32)] == 0))
	{
      dooldolerblood->curdir--;
      dooldolerblood->curdir&=3;
      offset = dooldolerblood->curdir * 9;
      object->frame = anim_setsequence(object->animate, dooldirtab[offset+8], SEQ_FORCE);
      return 0;
	}
  }

  if (object->visible)
  {
	dooldolerblood->shootdelay--;
    if (!dooldolerblood->shootdelay)
    {
      dooldolerblood->shootdelay = 100;
//      mine_init(object->x+24, object->y+16, ((hoi->x+32 - object->x)*256)/75, ((hoi->y+40-object->y)*256)/75, 0, 0, 100, 1, 1, 0);
    }
  }
  else
  {
    dooldolerblood->shootdelay = 100;
  }

  return 0;
}

void dooldoler_death(OBJECT *object, UINT32 param)
{
}

void dooldoler_clear(OBJECT *object, UINT32 param)
{
}



