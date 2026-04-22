#include <scroll.hpp>
#include <globals.hpp>
#include <editor.hpp>
#include <frm_wrk.hpp>
#include <anim.hpp>
#include <objects.hpp>
#include <prefs.hpp>
#include <hoi.hpp>


#define WORD short
#define DWORD long


void map_build(VIEWPORT *player)
{

  int x = 1;
  int y = 0;
  int xs = (player->worldx/16);
  int xa = (player->worldx & 14);

  video->clear(0);
  
  for (x=1; x<41; x++)
    {
      blitpatcolumn(player, x);
    }
}


void blitpatcolumn(VIEWPORT *player, int x)
{
#if 0
  for (int y=0; y< player->loadedmap->mapsizey/16; y++)
    {
      int sx,sy;
      sx = player->loadedmap->map[(y*player->loadedmap->mapsizex/16)+x+player->xoffsetcoarse].x;
      sy = player->loadedmap->map[(y*player->loadedmap->mapsizex/16)+x+player->xoffsetcoarse].y;
      sx &= 0x0fff; /* property eraf anden */

      MGL_bitBltCoord(player->loadedmap->blitbuf->get_dc(), patsmap->blitbuf->get_dc(),
          sx,sy,sx+16,sy+16,
          x*16 - player->xoffsetfine, y*16,
          MGL_REPLACE_MODE);
      
      if (editflg)    /* if editor is on we also display the attributes */
  {
    sx = player->loadedmap->map[(y*player->loadedmap->mapsizex/16)+x+player->xoffsetcoarse].x;
    sx &= 0xf000;
    sx = sx >> 8;
    
    MGL_transBltCoord(player->loadedmap->blitbuf->get_dc(), patsmap->blitbuf->get_dc(),
          sx,0,sx+16,16,
          x*16 - player->xoffsetfine, y*16,
          0,TRUE);
  }
    }
#endif
}


void hoi_cam(VIEWPORT *player)
{

  INT16 spdx;
  HOI_BLOOD *hoiblood = (HOI_BLOOD *) hoi->blood;
  
  /* handle look ahead movement */
  

    switch (hoiblood->curdirx)
    {
    case -1:
      if (prefs_ahead > (-prefs_aheadmax))
  {
    prefs_ahead -= 8;
  }
      break;
    case 1:
      if (prefs_ahead < (prefs_aheadmax))
  {
    prefs_ahead += 8;
  }
      break;
    case 0:
      break;
    }
  
  /* handle scrolling */
  
  spdx = (player->worldx - (hoi->x+48-(prefs->screenwidth/2)));
  //  spdx += prefs_ahead;
  spdx -= (hoiblood->xahead);
  spdx >>= 2;
  
  if (!hoiblood->onfloor)
    {
      if (hoiblood->yahead < 64) hoiblood->yahead+= 4;
    }
  else
    {
      if (hoiblood->yahead > 0) hoiblood->yahead-=4;
    }
  
  if (player->maxspd == 0) //no override
    {
      player->focusx = player->worldx - spdx;
      player->focusy = player->worldy - (player->worldy - (hoi->y-236+32)-hoiblood->holddown - hoiblood->yahead) /8;
    }
  else
    {
	player->focusx = (hoi->x+48-(prefs->screenwidth/2));
      player->focusy = (hoi->y-236+32)-hoiblood->holddown - hoiblood->yahead;
    }
}


void gein_cam()
{
  static int sincntx= 0;
  static int sincnty= 0;

  sincntx += 3;
  sincnty += 2;
  sincntx &= 255;
  sincnty &= 255;

  return;
  player2.focusx = player1.focusx + ((sinus128[sincntx] * 255)>>8);
  player2.focusy = player1.focusy + ((sinus128[sincnty] * 255)>>8);

}


void camera_override(VIEWPORT *player, UINT16 camx, UINT16 camy, UINT16 maxspd)
{
  player->camx = camx;
  player->camy = camy;
  player->maxspd = maxspd;
}


void scrolling(VIEWPORT *player)
{
  INT16 spdx;
  INT16 spdy;

  if (world != 2)
    {
      if (!player->maxspd)  //override active?
  {

    spdx = player->focusx - player->worldx;
//    if (spdx >  40) spdx =  40;
//    if (spdx < -40) spdx = -40;
    
    spdy = player->focusy - player->worldy;
//    if (spdy >  40) spdy =  40;
//    if (spdy < -40) spdy = -40;
  }
      else
  {
    spdx = ((player->camx-320) - player->worldx)>>4;
    if (spdx == 0)
      {
        if ((player->camx-320) != player->worldx)
    {
      spdx = 1;
    }
      }
    if (spdx >  player->maxspd) spdx =  player->maxspd;
    if (spdx < -player->maxspd) spdx = -player->maxspd;
    
    spdy = ((player->camy-240) - player->worldy)>>4;
    if (spdy == 0)
      {
        if ((player->camy-240) != player->worldy)
    {
      spdy = 1;
    }
      }
    if (spdy >  player->maxspd) spdy =  player->maxspd;
    if (spdy < -player->maxspd) spdy = -player->maxspd;
    
    player->camx   = 0;
    player->camy   = 0;
    player->maxspd = 0;
  }
      if (mc_autorun == 0  || mc_autorun == 2)
      {
        if (autoscrollspd)
        {
          player->worldx += (autoscrollspd > 0) ? (autoscrollspd-1) : (autoscrollspd+1);
        }
        else
        {
          player->worldx = player->worldx + spdx;
        }

        player->worldy = player->worldy + spdy;
      }
    }      
  else
    {
      INT32 x,y;
      INT16 spdx;
      INT16 spdy;
      HOI_BLOOD *hoiblood = (HOI_BLOOD *) hoi->blood;
      
      x = ( 16*hoiblood->xspd);
      y = ( 16*hoiblood->yspd);
      
      if (hoiblood->xahead < (x>>8) )
  {
    hoiblood->xahead += 16;
    if (hoiblood->xahead > (x>>8) ) hoiblood->xahead = (x>>8);
  }
      else
  {
    hoiblood->xahead -= 16;
    if (hoiblood->xahead < (x>>8) ) hoiblood->xahead = (x>>8);
  }
      
      
      if (hoiblood->yahead < (y>>8) )
  {
    hoiblood->yahead += 16;
    if (hoiblood->yahead > (y>>8) ) hoiblood->yahead = (y>>8);
  }
      else
  {
    hoiblood->yahead -= 16;
    if (hoiblood->yahead < (y>>8) ) hoiblood->yahead = (y>>8);
  }
      
      
      x = (hoiblood->xfloat>>8) + hoiblood->xahead;
      y = (hoiblood->yfloat>>8) + hoiblood->yahead;
      
      x += hoiblood->xcamxtra;  // verder kijken dan neus lang is door door te duwen tegen wand
      y += hoiblood->ycamxtra;  // verder kijken dan neus lang is door door te duwen tegen wand
      
      
      if (mc_autorun == 0 || mc_autorun == 2)
	  {
        if (autoscrollspd)
        {
          player->worldx += (autoscrollspd > 0) ? (autoscrollspd-1) : (autoscrollspd+1);
        }
        else
        {
          spdx           = (player->worldx - (x+48-(prefs->screenwidth/2)));
          spdx           = spdx >> 3;
          player->worldx = player->worldx - spdx;
        }
      
        spdy           = (player->worldy - (y+48-(prefs->screenheight/2)));
        spdy           = spdy >> 3;
        player->worldy = player->worldy - spdy;
	  }
      
    }
  
  /* xcoord */
  if (player->worldx > (player->curmap->mapsizex - prefs->screenwidth-64))
    {
      player->worldx = (player->curmap->mapsizex - prefs->screenwidth-64);
    }
  
  if (player->worldx < 32) 
    {
      player->worldx = 32;
    }
  
  /* ycoord */
  if (player->worldy > (player->curmap->mapsizey - player->height-64 + ((editflg && mouserbut) ? -256 : 0)))
    {
      player->worldy = (player->curmap->mapsizey - player->height-64 + ((editflg && mouserbut) ? -256 : 0));
    }
  
  if (player->worldy < 32) 
    {
      player->worldy = 32;
    }

}


void visual_updates(void)
{

  if ((world == 0) && (level == 0 || level == 2))   // LEVEL 1/ENV 1
  {
    static int framenr = 0;
    Cspr_frame *frame;

    framenr++;
    framenr &= 15;

    frame = anim_setsequence(orgwater, 0, SEQ_FORCE); // left part of conveyor belt!
    frame = anim_forceframe (orgwater, framenr);
    frame->draw(*patterntab[846], 0,0,0,0,32,32);

    frame->draw(*patterntab[847], 0,0,32,0,64,32);



  }

  if (world == 1 && level == 0)   // LEVEL 2 construction site!
  {
    static int framenr = 0;
    Cspr_frame *frame;

    frame = anim_setsequence(orgbelt, 0, SEQ_FORCE); // left part of conveyor belt!
    frame = anim_forceframe (orgbelt, framenr);
    frame->draw(*patterntab[800], 0, 0);

    frame = anim_setsequence(orgbelt, 1, SEQ_FORCE); // middle part of conveyor belt!
    frame = anim_forceframe (orgbelt, framenr);
    frame->draw(*patterntab[801], 0, 0);

    frame = anim_setsequence(orgbelt, 2, SEQ_FORCE); // right part of conveyor belt!
    frame = anim_forceframe (orgbelt, framenr);
    frame->draw(*patterntab[802], 0, 0);

    frame = anim_setsequence(orgbelt, 0, SEQ_FORCE); // left part of conveyor belt!
    frame = anim_forceframe (orgbelt, 7-framenr);
    frame->draw(*patterntab[803], 0, 0);

    frame = anim_setsequence(orgbelt, 1, SEQ_FORCE); // middle part of conveyor belt!
    frame = anim_forceframe (orgbelt, 7-framenr);
    frame->draw(*patterntab[804], 0, 0);

    frame = anim_setsequence(orgbelt, 2, SEQ_FORCE); // right part of conveyor belt!
    frame = anim_forceframe (orgbelt, 7-framenr);
    frame->draw(*patterntab[805], 0, 0);

    framenr++;
    framenr &= 7;

    UINT16 x,y;

    x = (31-player1.worldx/2)&31;
    y = (31-player1.worldy/2)&31;
    frame = anim_setsequence(orghek, 0, SEQ_FORCE); // left part of conveyor belt!
    frame = anim_forceframe (orghek, 0);
    patterntab[806]->clear(0);
    frame->draw(*patterntab[806], 0,0,x,y,x+32,y+32);

    x = (31-player1.worldx/2)&31;
    y = (31-player1.worldy/2)&31;
    frame = anim_setsequence(orghek, 0, SEQ_FORCE); // left part of conveyor belt!
    frame = anim_forceframe (orghek, 1);
    patterntab[807]->clear(0);
    frame->draw(*patterntab[807], 0,0,x,y,x+32,y+32);

    static UINT16 rx,ry;
    static UINT16 rx2,ry2;
    static UINT16 sx;
    
    frame = anim_setsequence(orgrain, 0, SEQ_FORCE); // rain frame!
    frame = anim_forceframe (orgrain, 1);

    patterntab[796]->clear(175);
    patterntab[797]->clear(175);
    patterntab[816]->clear(175);
    patterntab[817]->clear(175);
    rx+=2;
    ry-=1;
    rx&=63;
    ry&=63;

    frame->draw(*patterntab[796], 0, 0,rx   ,ry   ,rx+32   ,ry+32   );
    frame->draw(*patterntab[797], 0, 0,rx+32,ry   ,rx+32+32,ry+32   );
    frame->draw(*patterntab[816], 0, 0,rx   ,ry+32,rx+32   ,ry+32+32);
    frame->draw(*patterntab[817], 0, 0,rx+32,ry+32,rx+32+32,ry+32+32);

    sx += 12;
    sx &= 1023;
    rx2 += ((sinus512[sx]*3)>>10)+4;
    rx2 &= 63;
    ry2-=2;
    ry2&=63;

    frame = anim_setsequence(orgrain, 0, SEQ_FORCE); // rain frame!
    frame = anim_forceframe (orgrain, 0);

    frame->draw(*patterntab[796], 0, 0,rx2   ,ry2   ,rx2+32   ,ry2+32   );
    frame->draw(*patterntab[797], 0, 0,rx2+32,ry2   ,rx2+32+32,ry2+32   );
    frame->draw(*patterntab[816], 0, 0,rx2   ,ry2+32,rx2+32   ,ry2+32+32);
    frame->draw(*patterntab[817], 0, 0,rx2+32,ry2+32,rx2+32+32,ry2+32+32);
  }


  if (world == 1 && level == 1)   // LEVEL 2 construction site!
  {
    static UINT16 y= 0;
    static UINT16 slijkcnt = 0;
    static UINT16 fancnt = 0;
    Cspr_frame *frame;

    y += fallspeed;
    y &= 127;
    frame = anim_setsequence(orgparpat, 0, SEQ_FORCE); // parpat!
    frame = anim_forceframe (orgparpat, 0);
    frame->draw(*patterntab[820],  0,  0,  0,  y    ,  32,  y+32 );
    frame->draw(*patterntab[821],  0,  0, 32,  y    ,  64,  y+32 );
    frame->draw(*patterntab[822],  0,  0, 64,  y    ,  96,  y+32 );
    frame->draw(*patterntab[823],  0,  0, 96,  y    , 128,  y+32 );

    frame->draw(*patterntab[840],  0,  0,  0,  y+32 ,  32,  y+64 );
    frame->draw(*patterntab[841],  0,  0, 32,  y+32 ,  64,  y+64 );
    frame->draw(*patterntab[842],  0,  0, 64,  y+32 ,  96,  y+64 );
    frame->draw(*patterntab[843],  0,  0, 96,  y+32 , 128,  y+64 );

    frame->draw(*patterntab[860],  0,  0,  0,  y+64 ,  32,  y+96 );
    frame->draw(*patterntab[861],  0,  0, 32,  y+64 ,  64,  y+96 );
    frame->draw(*patterntab[862],  0,  0, 64,  y+64 ,  96,  y+96 );
    frame->draw(*patterntab[863],  0,  0, 96,  y+64 , 128,  y+96 );

    frame->draw(*patterntab[880],  0,  0,  0,  y+96 ,  32,  y+128);
    frame->draw(*patterntab[881],  0,  0, 32,  y+96 ,  64,  y+128);
    frame->draw(*patterntab[882],  0,  0, 64,  y+96 ,  96,  y+128);
    frame->draw(*patterntab[883],  0,  0, 96,  y+96 , 128,  y+128);

// fan

    fancnt++;
    if (fancnt == 7) fancnt = 0;

    frame = anim_setsequence(orgfan, 1, SEQ_FORCE); // right part of fan!
    frame = anim_forceframe (orgfan, fancnt);
//    patterntab[811]->clear(0);
//    patterntab[831]->clear(0);
//    patterntab[812]->clear(0);
//    patterntab[832]->clear(0);
//    patterntab[814]->clear(0);
//    patterntab[815]->clear(0);

    frame->draw_nokey(*patterntab[811], 0, 0, 0,  0, 32, 32);
    frame->draw_nokey(*patterntab[831], 0, 0, 0, 32, 32, 64);

    frame = anim_setsequence(orgfan, 0, SEQ_FORCE); // left part of fan!
    frame = anim_forceframe (orgfan, fancnt);

    frame->draw_nokey(*patterntab[812], 0, 0, 0,  0, 32, 32);
    frame->draw_nokey(*patterntab[832], 0, 0, 0, 32, 32, 64);

    frame = anim_setsequence(orgfan, 2, SEQ_FORCE); // bottom part of fan!
    frame = anim_forceframe (orgfan, fancnt);

    frame->draw_nokey(*patterntab[814], 0, 0, 0,  0, 32, 32);
    frame->draw_nokey(*patterntab[815], 0, 0, 32, 0, 64, 32);

// draw some slijk

    slijkcnt--;
    slijkcnt&=31;
    frame = anim_setsequence(orgslijk, 0, SEQ_FORCE); // parpat!
    frame = anim_forceframe (orgslijk, 0);
    frame->draw_nokey(*patterntab[779], 0, 0, 0 ,slijkcnt , 32, slijkcnt+32);

  }

  if (world == 2 && level == 0)   // LEVEL 3 jetpack fun!
  {
    static UINT16 x= 0;
    static UINT16 y= 0;
    Cspr_frame *frame;

    if (paraflg == 1)
      {
  x = player1.worldx;
  y = player1.worldy;
  
  x &= 63;
  y &= 63;
  frame = anim_setsequence(orgparapats, 0, SEQ_FORCE); // parpat!
  frame = anim_forceframe (orgparapats, 0);
  frame->draw_nokey(*patterntab[488], 0, 0, x   , y   , x+32, y+32);
  frame->draw_nokey(*patterntab[489], 0, 0, x+32, y   , x+64, y+32);
  frame->draw_nokey(*patterntab[508], 0, 0, x   , y+32, x+32, y+64);
  frame->draw_nokey(*patterntab[509], 0, 0, x+32, y+32, x+64, y+64);
      }
    else
      {
  x = player1.worldy>>1;
  y = player1.worldx>>1;
  
  x &= 63;
  y &= 63;
  frame = anim_setsequence(orgparapats, 2, SEQ_FORCE); // parpat!
  frame = anim_forceframe (orgparapats, 0);
  frame->draw_nokey(*patterntab[488], 0, 0, x   , y   , x+32, y+32);
  frame->draw_nokey(*patterntab[489], 0, 0, x+32, y   , x+64, y+32);
  frame->draw_nokey(*patterntab[508], 0, 0, x   , y+32, x+32, y+64);
  frame->draw_nokey(*patterntab[509], 0, 0, x+32, y+32, x+64, y+64);
      }

    frame = anim_setsequence(orgparapats, 3, SEQ_FORCE); // chain parpat!
    frame = anim_forceframe (orgparapats, 0);
    
    chaincnt1 += 3;
    chaincnt2 += 4;
    chaincnt3 += 5;
    
    x =  2608 + ((sinus512[chaincnt1 & 1023] * 64) >> 8);
    player1.loadedmap->map[(3200*player1.loadedmap->mapsizex/32/32)+((x    )/32)]  = 168;
    player1.loadedmap->map[(3200*player1.loadedmap->mapsizex/32/32)+((x+32 )/32)]  =   0;
    player1.loadedmap->map[(3200*player1.loadedmap->mapsizex/32/32)+((x+256)/32)]  =   0;
    player1.loadedmap->map[(3200*player1.loadedmap->mapsizex/32/32)+((x+288)/32)]  = 168;
    if(paramine1a)
    {
      paramine1a->x = x-8;
      paramine1a->y = 3200-8;
      paramine1b->x = x-8+256;
      paramine1b->y = 3200-8;
    }

    x &= 31;
    x ^= 31;
    frame->draw_nokey(*patterntab[168], 0, 0, x, 0, x+32, 32);
    
    x =  2608 + ((sinus512[chaincnt2 & 1023] * 64) >> 8);
    player1.loadedmap->map[(3328*player1.loadedmap->mapsizex/32/32)+((x    )/32)]  = 188;
    player1.loadedmap->map[(3328*player1.loadedmap->mapsizex/32/32)+((x+32 )/32)]  =   0;
    player1.loadedmap->map[(3328*player1.loadedmap->mapsizex/32/32)+((x+256)/32)]  =   0;
    player1.loadedmap->map[(3328*player1.loadedmap->mapsizex/32/32)+((x+288)/32)]  = 188;
    if(paramine2a)
    {
      paramine2a->x = x-8;
      paramine2a->y = 3328-8;
      paramine2b->x = x-8+256;
      paramine2b->y = 3328-8;
    }
    x &= 31;
    x ^= 31;
    frame->draw_nokey(*patterntab[188], 0, 0, x, 0, x+32, 32);
    
    x =  2608 + ((sinus512[chaincnt3 & 1023] * 64) >> 8);
    player1.loadedmap->map[(3456*player1.loadedmap->mapsizex/32/32)+((x    )/32)]  = 208;
    player1.loadedmap->map[(3456*player1.loadedmap->mapsizex/32/32)+((x+32 )/32)]  =   0;
    player1.loadedmap->map[(3456*player1.loadedmap->mapsizex/32/32)+((x+256)/32)]  =   0;
    player1.loadedmap->map[(3456*player1.loadedmap->mapsizex/32/32)+((x+288)/32)]  = 208;
    if(paramine3a)
    {
      paramine3a->x = x-8;
      paramine3a->y = 3456-8;
      paramine3b->x = x-8+256;
      paramine3b->y = 3456-8;
    }
    x &= 31;
    x ^= 31;
    frame->draw_nokey(*patterntab[208], 0, 0, x, 0, x+32, 32);
    
#if 0
    x =  2640 + ((sinus512[chaincnt4 & 1023] * 64) >> 8);
    player1.loadedmap->map[(3680*player1.loadedmap->mapsizex/32/32)+((x    )/32)]  = 228;
    player1.loadedmap->map[(3680*player1.loadedmap->mapsizex/32/32)+((x+32 )/32)]  =   0;
    player1.loadedmap->map[(3680*player1.loadedmap->mapsizex/32/32)+((x+256)/32)]  =   0;
    player1.loadedmap->map[(3680*player1.loadedmap->mapsizex/32/32)+((x+288)/32)]  = 228;
    paramine4a->x = x-16;
    paramine4a->y = 3680-16;
    paramine4b->x = x-16+256;
    paramine4b->y = 3680-16;
    x &= 31;
    x ^= 31;
    frame->draw_nokey(*patterntab[228], 0, 0, x, 0, x+32, 32);
#endif

  }

  if (world == 2 && level == 3)   // LEVEL 3
  {
    UINT16 x= 0;
    UINT16 x2= 0;
    UINT16 y= 0;
    Cspr_frame *frame;

      x = player1.worldx;
      y = player1.worldy;
  
      x &= 63;
      y &= 63;
      x2 = 63-x;
      frame = anim_setsequence(orgparapats, 0, SEQ_FORCE); // parpat!
      frame = anim_forceframe (orgparapats, 0);
      frame->draw_nokey(*patterntab[488], 0, 0, x   , y   , x+32, y+32);
      frame->draw_nokey(*patterntab[489], 0, 0, x+32, y   , x+64, y+32);
      frame->draw_nokey(*patterntab[508], 0, 0, x2   , y+32, x2+32, y+64);
      frame->draw_nokey(*patterntab[509], 0, 0, x2+32, y+32, x2+64, y+64);
  }

  if (world == 3 && level == 0)   // LEVEL 4   "Helemoal naar de kloteuh!"
  {
    static UINT16 x= 0;
    static UINT16 y= 0;
    Cspr_frame *frame;
    
    if (paraflg == 1 || paraflg == 2)
      {
	static int sincnt = 0;

	sincnt+=5;
	sincnt&=1023;
	
	x =  ((sinus512[(sincnt)      & 1023] * 48) >> 8);
	y =  ((sinus512[(sincnt +256) & 1023] * 48) >> 8);

	x &= 63;
	y &= 63;
	frame = anim_setsequence(orgparapats, 0, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[260], 0, 0, x   , y   , x+32, y+32);
	frame->draw_nokey(*patterntab[261], 0, 0, x+32, y   , x+64, y+32);
	frame->draw_nokey(*patterntab[280], 0, 0, x   , y+32, x+32, y+64);
	frame->draw_nokey(*patterntab[281], 0, 0, x+32, y+32, x+64, y+64);

	x = 63-x;
	x &= 63;
	y &= 63;
	frame = anim_setsequence(orgparapats, 0, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[262], 0, 0, x   , y   , x+32, y+32);
	frame->draw_nokey(*patterntab[263], 0, 0, x+32, y   , x+64, y+32);
	frame->draw_nokey(*patterntab[282], 0, 0, x   , y+32, x+32, y+64);
	frame->draw_nokey(*patterntab[283], 0, 0, x+32, y+32, x+64, y+64);

	y = 63-y;
	x &= 63;
	y &= 63;
	frame = anim_setsequence(orgparapats, 0, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[264], 0, 0, x   , y   , x+32, y+32);
	frame->draw_nokey(*patterntab[265], 0, 0, x+32, y   , x+64, y+32);
	frame->draw_nokey(*patterntab[284], 0, 0, x   , y+32, x+32, y+64);
	frame->draw_nokey(*patterntab[285], 0, 0, x+32, y+32, x+64, y+64);

	x = 63-x;
	x &= 63;
	y &= 63;
	frame = anim_setsequence(orgparapats, 0, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[266], 0, 0, x   , y   , x+32, y+32);
	frame->draw_nokey(*patterntab[267], 0, 0, x+32, y   , x+64, y+32);
	frame->draw_nokey(*patterntab[286], 0, 0, x   , y+32, x+32, y+64);
	frame->draw_nokey(*patterntab[287], 0, 0, x+32, y+32, x+64, y+64);
      }

    if (paraflg == 2 || paraflg == 3)
      {
	static int sincnt = 0;
	static int sincnt2 = 0;

	sincnt+=5;
	sincnt&=1023;

	sincnt2+=4;
	sincnt2&=1023;
	
	x =  ((sinus512[(sincnt)      & 1023] * 16) >> 8);
	y =  ((sinus512[(sincnt +256 + sincnt2) & 1023] * 16) >> 8);

	x &= 63;
	y &= 63;

	frame = anim_setsequence(orgparapats, 2, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[268], 0, 0, x   , y   , x+32, y+32);
	frame->draw_nokey(*patterntab[269], 0, 0, x+32, y   , x+64, y+32);
	frame->draw_nokey(*patterntab[288], 0, 0, x   , y+32, x+32, y+64);
	frame->draw_nokey(*patterntab[289], 0, 0, x+32, y+32, x+64, y+64);

	x =  ((sinus512[(sincnt2)      & 1023] * 16) >> 8);
	y =  ((sinus512[(sincnt2 +256) & 1023] * 16) >> 8);

	x &= 63;
	y &= 63;

	frame = anim_forceframe (orgparapats, 1);

	frame->draw(*patterntab[268], 0, 0, x   , y   , x+32, y+32);
	frame->draw(*patterntab[269], 0, 0, x+32, y   , x+64, y+32);
	frame->draw(*patterntab[288], 0, 0, x   , y+32, x+32, y+64);
	frame->draw(*patterntab[289], 0, 0, x+32, y+32, x+64, y+64);

      }
    if (paraflg == 3 || paraflg == 4 || paraflg == 5 || paraflg == 6 || paraflg == 8)
      {

	x = -player1.worldx>>3;
	x &= 63;
	y = -player1.worldy>>3;
	y &= 63;

	frame = anim_setsequence(orgparapats, 1, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[270], 0, 0, x   , y   , x+32, y+32);
	frame->draw_nokey(*patterntab[271], 0, 0, x+32, y   , x+64, y+32);
	frame->draw_nokey(*patterntab[290], 0, 0, x   , y+32, x+32, y+64);
	frame->draw_nokey(*patterntab[291], 0, 0, x+32, y+32, x+64, y+64);

	x = -player1.worldx>>2;
	x &= 63;
	y = -player1.worldy>>2;
	y &= 63;

	frame->draw(*patterntab[270], 0, 0, x   , y   , x+32, y+32);
	frame->draw(*patterntab[271], 0, 0, x+32, y   , x+64, y+32);
	frame->draw(*patterntab[290], 0, 0, x   , y+32, x+32, y+64);
	frame->draw(*patterntab[291], 0, 0, x+32, y+32, x+64, y+64);

	x = -player1.worldx>>1;
	x &= 63;
	y = -player1.worldy>>1;
	y &= 63;

	frame->draw(*patterntab[270], 0, 0, x   , y   , x+32, y+32);
	frame->draw(*patterntab[271], 0, 0, x+32, y   , x+64, y+32);
	frame->draw(*patterntab[290], 0, 0, x   , y+32, x+32, y+64);
	frame->draw(*patterntab[291], 0, 0, x+32, y+32, x+64, y+64);

      }
    if (paraflg == 7)
      {
	static int sincnt = 0;
	static int sincnt2 = 0;

	sincnt+=2;
	sincnt&=1023;
	sincnt2+=3;
	sincnt2&=1023;

	x =  ((sinus512[(sincnt)       & 1023] * 256) >> 8);
	y =  ((sinus512[(sincnt2 +256) & 1023] * 256) >> 8);

	x &= 127;
	y &= 127;
	frame = anim_setsequence(orgparapats, 3, SEQ_FORCE); // parpat!
	frame = anim_forceframe (orgparapats, 0);
	frame->draw_nokey(*patterntab[180], 0, 0, x   , y   , x+32 , y+32);
	frame->draw_nokey(*patterntab[181], 0, 0, x+32, y   , x+64 , y+32);
	frame->draw_nokey(*patterntab[182], 0, 0, x+64, y   , x+96 , y+32);
	frame->draw_nokey(*patterntab[183], 0, 0, x+96, y   , x+128, y+32);

	frame->draw_nokey(*patterntab[200], 0, 0, x   , y+32, x+32 , y+64);
	frame->draw_nokey(*patterntab[201], 0, 0, x+32, y+32, x+64 , y+64);
	frame->draw_nokey(*patterntab[202], 0, 0, x+64, y+32, x+96 , y+64);
	frame->draw_nokey(*patterntab[203], 0, 0, x+96, y+32, x+128, y+64);

	frame->draw_nokey(*patterntab[220], 0, 0, x   , y+64, x+32 , y+96);
	frame->draw_nokey(*patterntab[221], 0, 0, x+32, y+64, x+64 , y+96);
	frame->draw_nokey(*patterntab[222], 0, 0, x+64, y+64, x+96 , y+96);
	frame->draw_nokey(*patterntab[223], 0, 0, x+96, y+64, x+128, y+96);

	frame->draw_nokey(*patterntab[240], 0, 0, x   , y+96, x+32 , y+128);
	frame->draw_nokey(*patterntab[241], 0, 0, x+32, y+96, x+64 , y+128);
	frame->draw_nokey(*patterntab[242], 0, 0, x+64, y+96, x+96 , y+128);
	frame->draw_nokey(*patterntab[243], 0, 0, x+96, y+96, x+128, y+128);



      }

  }
  int x=0;
  int y=0;

  x = (63-player1.worldx)&63;
  y = (63-player1.worldy)&63;
//  x++;
//  x&=63;
//  y++;
//  y&=63;

//  patsmap->blitbuf->draw(*patsmap->blitbuf, 448, 496, 320+x, 496+y, 320+64+x, 496+64+y);
}


void update(VIEWPORT *player)
{
  static UINT16 shakeyinv = 0;
  short x,y;

  int of_x,of_y;     // offset (32 pixels per keer)
  int sm_x,sm_y;     // smooth value

  of_x = (player->worldx+player->quakex)>>5;
  of_y = (player->worldy+player->shakey+player->quakey)>>5;
  sm_x = (player->worldx+player->quakex) &31;
  sm_y = (player->worldy+player->shakey+player->quakey) &31;

  if ( mouserbut && editflg )  // display editpage
  {
    for (y= 0; y<17; y++)
    {
      for (x=0; x<22; x++)
      {
        UINT16 curpat;
        Cblitbuf *srcblitbuf;

        curpat = player->curmap->map[((y-1+of_y)*player->curmap->mapsizex/32)+(x-1+of_x)];
	curpat &= 0x7fff;
	player->curmap->map[((y-1+of_y)*player->curmap->mapsizex/32)+(x-1+of_x)] = curpat;

        if (transmap[curpat]!=2)
        {
          INT16 sx,sy,fsx,fsy,fex,fey;

          sx  = x*32-sm_x;
          sy  = y*32-sm_y;
          fsx = x*32-sm_x;
          fsy = y*32-sm_y;
          fex = x*32+32-sm_x;
          fey = y*32+32-sm_y;

          if (fex >= 640) fex = 640;
          if (fey >= 480) fey = 480;

          if (fsx >= -32 && fsx < 0)
          {
            sx += (-fsx);
            fsx = 0;
          }

          if (fsy >= -32 && fsy < 0)
          {
            sy += (-fsy);
            fsy = 0;
          }

          if (fsx < 640 && fsy < 480 && fex >= 0 && fey >= 0)
          {
            para->draw_nokey(*player1.loadedmap->blitbuf, sx, sy, fsx, fsy, fex, fey);
          }
        }
#if 1
        if (transmap[curpat]!=0)
        {
          srcblitbuf = patterntab[curpat];
          if (srcblitbuf)
          {
            if (transmap[curpat]==1)  //semi transparent
            {
              srcblitbuf->draw(*player1.loadedmap->blitbuf, x*32-sm_x, y*32-sm_y, 0, 0, 32, 32);
            }
            else
            {
              srcblitbuf->draw_nokey(*player1.loadedmap->blitbuf, x*32-sm_x, y*32-sm_y, 0, 0, 32, 32);
            }
          }
        }
      }
#endif
    }
  }
  else   // in game!
  {
    for (y= -1; y<17; y++)
    {
      for (x=-1; x<22; x++)
      {
        UINT16 curpat;
        Cblitbuf *srcblitbuf;

        curpat = player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x)];
	curpat &= 0x7fff;
        player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x)] = curpat;
          if(curpat>=nrofpats) curpat=0;

        if (transmap[curpat]!=2)
        {
          INT16 sx,sy,fsx,fsy,fex,fey;

          sx  = x*32-sm_x;
          sy  = y*32-sm_y;
          fsx = x*32-sm_x;
          fsy = y*32-sm_y;
          fex = x*32+32-sm_x;
          fey = y*32+32-sm_y;

          if (fex >= 640) fex = 640;
          if (fey >= 480) fey = 480;

          if (fsx >= -32 && fsx < 0)
          {
            sx += (-fsx);
            fsx = 0;
          }

          if (fsy >= -32 && fsy < 0)
          {
            sy += (-fsy);
            fsy = 0;
          }

          if (fsx < 640 && fsy < 480 && fex >= 0 && fey >= 0)
          {
            para->draw_nokey(*player1.loadedmap->blitbuf, sx, sy, fsx, fsy, fex, fey);
          }
        }

#if 1
        if (transmap[curpat]!=0)
        {
          srcblitbuf = patterntab[curpat];
          if (srcblitbuf)
          {
            if (transmap[curpat]==1) // semi transparent
            {
              srcblitbuf->draw(*player1.loadedmap->blitbuf, x*32-sm_x, y*32-sm_y, 0, 0, 32, 32);
            }
            else
            {
              srcblitbuf->draw_nokey(*player1.loadedmap->blitbuf, x*32-sm_x, y*32-sm_y, 0, 0, 32, 32);
            }
          }
        }
      }
#endif
    }
  }
  




#if 1

  if (editflg)    /* if editor is on we also display the attributes */
  {
    if (player->curmap == player->loadedmap)  // only if the edited level is displayed
    {
      of_x = (player->worldx)>>4;
      of_y = (player->worldy)>>4;
      sm_x = (player->worldx) &15;
      sm_y = (player->worldy) &15;

      for (y=0; y<32; y++)
      {
        for (x=0; x<42; x++)
        {
          Cblitbuf *srcblitbuf;
          srcblitbuf = patterntab[colmap[((y+of_y)*player->curmap->mapsizex/16)+(x+of_x)]];

          if (colmap[((y+of_y)*player->curmap->mapsizex/16)+(x+of_x)])
          {
            if (srcblitbuf)
            {
              srcblitbuf->draw(*player1.loadedmap->blitbuf, x*16-sm_x, y*16-sm_y, 0, 0, 16, 16);
            }
          }
        }
      }
    }
  }

#endif
  
  shakeyinv += 1;
  shakeyinv &= 1;

  if (shakeyinv)
  {
    if (player->shakey)
    {
      player->shakey = -player->shakey;
    
      if (player->shakey < 0)
      {
        player->shakey>>=1;
      }
      else
      {
        player->shakey>>=1;
      }
    }
  }


#if 0
  MGLDC *destdc, *srcdc, *paradc;
  char *destsurface;
  char *srcsurface;
  char *parasurface;

  destdc = video->get_dc();
  destsurface = (char *)destdc->surface + ((player->topy) * (destdc->mi.xRes+1));


  srcdc = player1.curmap->blitbuf->get_dc();
  srcsurface  = (char *)srcdc->surface + 16 + ((player->worldy+player->shakey) * (srcdc->mi.xRes+1));

  paradc = para->get_dc();
  parasurface = (char *)paradc->surface+ 16 + (((player->worldx)/2)&0xfffffffe) + (((player->worldy+player->shakey)/2) * (paradc->mi.xRes+1));

#if 1
  asm_paralax(destsurface, srcsurface, parasurface, 640, player->height,
        destdc->mi.xRes+1, srcdc->mi.xRes+1, paradc->mi.xRes+1);


#endif

#if 0

  MGL_bitBltCoord(temppara->get_dc(), player->curmap->blitbuf->get_dc(),
      player->topx, player->worldy,
      player->topx+player->width, player->worldy+player->height,
      0, 0, MGL_REPLACE_MODE);

//  paralax(temppara->get_dc(), player->curmap->blitbuf->get_dc(),
//    para->get_dc(),
//    player->topx, player->worldy,
//    player->width, player->height);


  MGL_transBltCoord(temppara->get_dc(), para->get_dc(),
        0,0,player->width,player->height,
        0,0,
        0,FALSE);


  MGL_bitBltCoord(video->get_dc(), temppara->get_dc(),
      0, 0,
      player->width, player->height,
      16+CLIPMARGINX, 0, MGL_REPLACE_MODE);

#endif



#if 0
  video->copy_blitbuf(*player->curmap->blitbuf, player->topx, player->topy, 
          player->width, player->height,
          16+CLIPMARGINX, player->worldy + player->shakey);
#endif


#if 0



//  video->para_blitbuf(*para, prefs->screentopx, prefs->screentopy, 
//          prefs->screenwidth, prefs->screenheight,
//          prefs->screentopx, prefs->screentopy);
#endif

#endif
}


void level4flash(VIEWPORT *player)
{
  static UINT16 shakeyinv = 0;
  short x,y;

  int of_x,of_y;     // offset (32 pixels per keer)

 
  if (world != 3 || paraflg < 5) return;

  of_x = (player->worldx+player->quakex)>>5;
  of_y = (player->worldy+player->shakey+player->quakey)>>5;

  if (of_x < 5) of_x = 5;
  if (of_y < 5) of_y = 5;

  if ( !(editflg && mouserbut))  // display editpage
    {
      for (y= -4; y<20; y++)   // 3 teveel aan beide kanten
	{
	  for (x=-4; x<25; x++)
	    {
	      short curpat;
	      
	      curpat = player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x)];
	      
	      if (curpat > 299 && curpat <331)
		{
		  curpat++;
		  player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x)] = curpat;
		  if (curpat == 301)
		    {
		      if (player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x-1)] == 331)
			{
			  if (x!= -1)
			    {
			      player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x-1)] = 300 | 0x8000;
			    }
			}
		      if (player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x+1)] == 331)
			{
			  if (x!= 21)
			    {
			      player->curmap->map[((y+of_y)*player->curmap->mapsizex/32)+(x+of_x+1)] = 300 | 0x8000;
			    }
			}
		      if (player->curmap->map[((y+of_y+1)*player->curmap->mapsizex/32)+(x+of_x)] == 331)
			{
			  if (y!= 16)
			    {
			      player->curmap->map[((y+of_y+1)*player->curmap->mapsizex/32)+(x+of_x)] = 300 | 0x8000;
			    }
			}
		      if (player->curmap->map[((y+of_y-1)*player->curmap->mapsizex/32)+(x+of_x)] == 331)
			{
			  if (y!= -1)
			    {
			      player->curmap->map[((y+of_y-1)*player->curmap->mapsizex/32)+(x+of_x)] = 300 | 0x8000;
			    }
			}
		    }
		}
	    }
	}
    }
}


void rebuild(VIEWPORT *player)
{
  static int toggle = 0;

#if 0
  MGLDC *mydc;
  mydc = loadedmap->blitbuf->get_dc();

  MGL_makeCurrentDC(mydc);

  MGL_fillRectCoord(624,0,640,480);

  MGL_makeCurrentDC(video->get_dc());
#endif

//  toggle ^= 1;
//  if (toggle)
//    {
      blitpatcolumn(player, 41);
//    }
//  else
//    {
      blitpatcolumn(player, 1);
//    }

#if 0
  int x = 41;
  int y = 0;
  int xs = (worldx/16);
  int xa = (worldx & 14);

  for (y=0; y< loadedmap->mapsizey/16; y++)
    {
      int sx,sy;
      sx = loadedmap->map[(y*loadedmap->mapsizex/16)+x+xs].x;
      sy = loadedmap->map[(y*loadedmap->mapsizex/16)+x+xs].y;
      MGL_bitBltCoord(loadedmap->blitbuf->get_dc(), patsmap->blitbuf->get_dc(),
          sx,sy,sx+16,sy+16,
          x*16 - xa, y*16,
          MGL_REPLACE_MODE);
    }

  x = 1;
  for (y=0; y< loadedmap->mapsizey/16; y++)
    {
      int sx,sy;
      sx = loadedmap->map[(y*loadedmap->mapsizex/16)+x+xs].x;
      sy = loadedmap->map[(y*loadedmap->mapsizex/16)+x+xs].y;
      MGL_bitBltCoord(loadedmap->blitbuf->get_dc(), patsmap->blitbuf->get_dc(),
          sx,sy,sx+16,sy+16,
          x*16 - xa, y*16,
          MGL_REPLACE_MODE);
    }
#endif

}


// restore previously plotted dots from the screen
// The maximum amount of dots = 10000
// Use the global dotpnt (pointer) to access the dot-database.

void restoredots(void)
{
  UINTPTR *dotty;
  UINT8   *dotty2;
  UINT16   dotcnt;
  UINT8    pixcol;
  UINT8   *pixpnt;

  if (dotpnt == dotbase) return;

  dotty = dotpnt;
  dotty2 = dotcolpnt;
  dotcnt = dotpnt - dotbase;

  do
    {
      pixcol = *(--dotty2);
      pixpnt = (UINT8 *) ( *(--dotty));
      *(pixpnt) = pixcol;
    } while (--dotcnt);

  dotpnt    = dotbase;
  dotcolpnt = dotcolbase;
}


void plotapix(UINT16 x, UINT16 y, UBYTE col)
{
#if 0
  MGLDC *srcdc;
  UBYTE *srcsurface;

  if (x > 640) return;
  if (y > 480) return;

  srcdc = player1.curmap->blitbuf->get_dc();
  srcsurface = (UBYTE *)srcdc->surface;

  srcsurface = srcsurface + x + ((y+CLIPMARGINY- player1.curmap->blitbuf->get_top())*(srcdc->mi.xRes+1));

  *(dotpnt++) = (UINTPTR)srcsurface;
  *(dotcolpnt++) = *(srcsurface);

  *(srcsurface) = col;
#endif
}

