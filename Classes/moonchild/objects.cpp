#include <objects.hpp>
#include <hoi.hpp>
#include <sound.hpp>


static void object_process(int (*process)(VG_DLL_ITEM *));

static VIEWPORT *curplayer;  /* used as temporarily medium for transferring
                                current player */

void object_initialise(void)
{
  objects = vg_dll_create(0);
}


/* if specified process function returns TRUE, the object will
   be removed from the list!!! */

static void object_process(int (*process)(VG_DLL_ITEM *))
{
  OBJECT *oldobj;
  OBJECT *obj;
  int dummy;

  obj = 0;
  dummy = 0;

  if (vg_dll_first(objects))
  {
    do
    {

      oldobj = obj;

      obj = (OBJECT *) vg_dll_collect(objects);

      if (!obj) goto endofprocess;

      if (process(obj))
      {
        if (obj->clear) obj->clear(obj, NULL);

        if (obj->blood) free(obj->blood);
        if (obj->animate) free(obj->animate);

        vg_dll_remcur(objects);

        free (obj);

        if ((OBJECT *) vg_dll_collect(objects) == oldobj) // deleted item was last in list
        {
          goto endofprocess;
        }

      }
      else
      {
        if(vg_dll_next(objects) == 0) goto endofprocess;
      }
    } while (1);

endofprocess:

    dummy++;
  }
}


void object_shutdown(void)
{
  object_process(cb_object_shutdown);
  vg_dll_free(objects);
}

int cb_object_shutdown(VG_DLL_ITEM *item)
{
  log_out("object shutted down");
  return 1;     /* remove the object from the list */
}


void object_add(OBJECT *obj)
{
  vg_dll_add(objects, (VG_DLL_ITEM *)obj, VG_DLL_LAST);
}

void object_add_first(OBJECT *obj)
{
  vg_dll_add(objects, (VG_DLL_ITEM *)obj, VG_DLL_FIRST);
}

void object_del(OBJECT *obj)
{
  vg_dll_search(objects, (VG_DLL_ITEM *)obj, VG_DLL_FORWARDS);
  vg_dll_remcur(objects);
}


void object_movement(void)
{
  object_process(cb_object_movement);
}

int cb_object_movement(VG_DLL_ITEM *item)
{
  OBJECT *obj;

  obj = (OBJECT *)item;
  if (obj->live)
    {
      return (obj->live(obj, NULL));
    }
  else
    {
      return 0;
    }
}


void object_collision(void)
{
  object_process(cb_object_collision);
}

int cb_object_collision(VG_DLL_ITEM *item)
{
  OBJECT *obj;

  obj = (OBJECT *)item;

  if ((obj != hoi) && (mc_autorun == 0))
  {
    if (obj->lethal != 0)
    {
      INT16 x;
      x = (INT16)(obj->x+obj->coloffsetx) - (INT16)(hoi->x+hoi->coloffsetx);
      x += obj->colwidth;
      if ((UINT16)x < hoi->colwidth+obj->colwidth)
      {
        INT16 y;
        y = (INT16)(obj->y+obj->coloffsety) - (INT16)(hoi->y+hoi->coloffsety);
        y += obj->colheight;
        if ((UINT16)y < hoi->colheight+obj->colheight)
        {
          if (editflg)
          {
            RASTCOL2(207, 255, 255, 255);
            play_mcdrain();
          }
          else
          {
            if (obj->frame && hoi->frame)
            {
              if (obj->lethal == 1) // energy drain?
              {
                HOI_BLOOD *hoiblood;
                hoiblood = (HOI_BLOOD *)hoi->blood;
                if (!hoiblood->flashcnt)  // niet onverwoestbaar!
                {
                  if (player1.energy)
                  {
                    player1.energy--;
                    play_mcdrain();
                  }
                }
              }
              if (obj->lethal == 2)   // instanteneous death?
              {
                player1.energy = 0;
              }
              if (obj->lethal == 3)   // fastenergy drain?
              {
                HOI_BLOOD *hoiblood;
                hoiblood = (HOI_BLOOD *)hoi->blood;

                if (!hoiblood->flashcnt)  // niet onverwoestbaar!
                {
                  if (player1.energy)
                  {
                    if (player1.energy) player1.energy--;
                    if (player1.energy) player1.energy--;
                    if (player1.energy) player1.energy--;
                    play_mcdrain();
                  }
                }
              }
              if (obj->lethal == 4)   // notify if collided with?
              {
                obj->collided = 1;    // set collision flg
              }
            }
          }
        }
      }
    }
  }

  return 0;
}


void object_visualise(VIEWPORT *player)
{
  curplayer = player;
  object_process(cb_object_visualise);
}

int cb_object_visualise(VG_DLL_ITEM *item)
{
  INT16 x,y;
  OBJECT *obj;

  obj = (OBJECT *)item;
  
  if (obj->frame)   // does it have normal methods of visualisation?
    {
      x = obj->x - curplayer->worldx - curplayer->quakex;
      y = obj->y - curplayer->worldy - curplayer->quakey;
      
#if 0
      if (x > /*curplayer->worldx*/ + 630) return 0;
      if (x < 0/*curplayer->worldx*/ ) return 0;
      if (y > curplayer->curmap->mapsizey + 32) return 0;
#endif
      
      if (obj->blitsizex && obj->blitsizey)
	{
	  if (obj->frame->draw(*curplayer->loadedmap->blitbuf,x , y,
			       obj->blitstartx, obj->blitstarty,
			       obj->blitstartx+obj->blitsizex, obj->blitstarty+obj->blitsizey))
	    {
	      if (obj->visible<1024) obj->visible++;
	    }
	  else
	    {
	      obj->visible = 0;
	    }
	}
      else  /* no size override */
	{
	  if ( obj->frame->draw(*curplayer->loadedmap->blitbuf,x , y ))
	    {
	      if (obj->visible<1024) obj->visible++;
	    }
	  else
	    {
	      obj->visible = 0;
	    }

	}
    }
  else
    {
      obj->visible = 0; // object is not visible unless stated otherwise
    }
  
  return 0; /* niet uit lijst deleten */
}

