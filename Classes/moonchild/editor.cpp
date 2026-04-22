#include <globals.hpp>
#include <editor.hpp>
#include <objects.hpp>
#include <scroll.hpp>
#include <anim.hpp>
//#include <conio.h>

static void do_glow(void);

static INT16 rememhx;
static INT16 rememhy;
static INT16 rememwx;
static INT16 rememwy;
static INT16 rememfx;
static INT16 rememfy;
static INT16 rememflg = 0;

void editor(VIEWPORT *player)
{
  if (!editflg) return;

  autoscrollspd = 0;

  hoi->frame = anim_setsequence(hoi->animate, SEQ_CURSOR /* SEQ_FLYLEFT */ , SEQ_FORCE);
  hoi->frame = anim_forceframe (hoi->animate, 0);
  do_glow();

  if (mouserbut)
    {
      player1.loadedmap->blitbuf->set_clipping(0,0, 640,480);

      if (!rememflg) /* not yet remembered so lets first remember the */
  {            /* current x-y position */
    rememhx   = hoi->x;
    rememhy   = hoi->y;
    rememwx   = player->worldx;
    rememwy   = player->worldy;
    rememfx   = player->focusx;
    rememfy   = player->focusy;
    player->worldx = 0;
    player->worldy = 0;
    player->focusx = 0;
    player->focusy = 0;
    hoi->x = 0;
    hoi->y = 0;
    rememflg = 1;
  }

      player->curmap = patsmap;

      if (mouselchng)   /* listen for left button interaction */
  {
    switch(mouselbut)
      {
      case 0:  /* los gelaten */
        selendx = hoi->x-32;
        selendy = hoi->y-32;
        if (selstartx > selendx)
    {
      selstartx = selendx;
    }
        if (selstarty > selendy)
    {
      selstarty = selendy;
    }
        break;
      case 1:  /* ingedrukt */
        selstartx = hoi->x-32;
        selstarty = hoi->y-32;
        if (selstartx > selendx)
    {
      selendx = selstartx;
    }
        if (selstarty > selendy)
    {
      selendy = selstarty;
    }
        break;
      }
  }

    }
  else
    {
//      player1.loadedmap->blitbuf->set_clipping(32,32, 640-32-32,480-32-32);
      if (rememflg) /* was there a x-y coordinate pair we remembered? */
  {            /* if so let's set 'm back */

    player->curmap = player->loadedmap;

    hoi->x = rememhx;
    hoi->y = rememhy;
    player->worldx = rememwx;
    player->worldy = rememwy;
    player->focusx = rememfx;
    player->focusy = rememfy;
    rememflg = 0;
    scrolling(player);      // update some internal variables first
//    map_build(player);
  }


      selstartx &= 0xffe0;
      selstarty &= 0xffe0;
      selendx &= 0xffe0;
      selendy &= 0xffe0;
            
      if (mouselbut)
  {
    int sx,sy,dx,dy;
    dx = hoi->x & 0xffe0;
    dy = hoi->y & 0xffe0;
    sx = selstartx;
    sy = selstarty;
    

    if (sy == 0)    /* property gekozen */
      {
        sx  = sx >> 5;
        sx &= 31;

        dx = hoi->x & 0xfff0;
        dy = hoi->y & 0xfff0;
        
        colmap[(dy*player->loadedmap->mapsizex/16/16)+(dx/16)] = sx;

      }
    else
      {
        for (int y=0; y< ((selendy - selstarty)/32)+1; y++)
    {
      for (int x=0; x< ((selendx - selstartx)/32)+1; x++)
        {
          int patnr;
          dx = (hoi->x & 0xffe0) + (x*32);
          dy = (hoi->y & 0xffe0) + (y*32);
          
          sx = selstartx + (x*32);
          sy = selstarty + (y*32);

          patnr = (sy*patsmap->mapsizex/32/32) + (sx/32);

          player->loadedmap->map[(dy*player->loadedmap->mapsizex/32/32)+(dx/32)] = patnr;
 //         log_out("patnr = %d",patnr);
        }
    }
      }
//    map_build(player);
  }
    }
}


static int glowcnt = 0;

static char glowtab[128] =
{
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,
47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,
31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,
15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
};

static void do_glow(void)
{

  glowcnt += 1;
  if (glowcnt >= 128) glowcnt = 0;

//  _outp(0x3c8,1);
//  _outp(0x3c9,(glowtab[glowcnt]));
//  _outp(0x3c9,(glowtab[glowcnt]));
//  _outp(0x3c9,(glowtab[glowcnt]));
}
