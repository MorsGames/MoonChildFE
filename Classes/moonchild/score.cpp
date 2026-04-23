#include <frm_wrk.hpp>
#include <score.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sokoban.hpp>
#include <boss.hpp>
#include <sound.hpp>

// 7x9
static const char* smallfont[] =
{
  "  ...  "
  " .   . "
  " .   . "
  " .   . "
  " .   . "
  " .   . "
  " .   . "
  " .   . "
  "  ...  "
  ,
  "   .   "
  "  ..   "
  " . .   "
  "   .   "
  "   .   "
  "   .   "
  "   .   "
  "   .   "
  " ..... "
  ,
  "  ...  "
  " .   . "
  " .   . "
  "     . "
  "    .  "
  "   .   "
  "  .    "
  " .     "
  " ..... "
  ,
  "  ...  "
  " .   . "
  " .   . "
  "     . "
  "  ...  "
  "     . "
  " .   . "
  " .   . "
  "  ...  "
  ,
  "    . "
  "    .. "
  "   . . "
  "  .  . "
  " .   . "
  " ..... "
  "     . "
  "     . "
  "     . "
  ,
  " ..... "
  " .     "
  " .     "
  " .     "
  " ....  "
  "     . "
  "     . "
  " .   . "
  "  ...  "
  ,
  "  ...  "
  " .     "
  " .     "
  " .     "
  " ....  "
  " .   . "
  " .   . "
  " .   . "
  "  ...  "
  ,
  " ..... "
  "     . "
  "     . "
  "    .  "
  "    .  "
  "    .  "
  "   .   "
  "   .   "
  "   .   "
  ,
  "  ...  "
  " .   . "
  " .   . "
  " .   . "
  "  ...  "
  " .   . "
  " .   . "
  " .   . "
  "  ...  "
  ,
  "  ...  "
  " .   . "
  " .   . "
  " .   . "
  "  .... "
  "     . "
  "     . "
  "     . "
  "  ...  "
  ,
  "       "
  "       "
  "       "
  "  .    "
  "       "
  "       "
  "  .    "
  "       "
  "       "
  ,
  "       "
  "       "
  "       "
  "       "
  "       "
  "       "
  "       "
  "       "
  "  .    "
};
static const char* smallfontmap = "0123456789:.";

static void drawsmallglyph(Cblitbuf& blitbuf, char glyph, BYTE color, UINT16 x, UINT16 y)
{
  UINT16 i;
  const char* glyphptr = nullptr;
  UINT16 xcnt;
  UINT16 ycnt;

  for (i=0;i<strlen(smallfontmap);i++)
  {
    if (smallfontmap[i] == glyph)
    {
      glyphptr = smallfont[i];
      break;
    }
  }
  if (glyphptr == nullptr) return;

  xcnt = 0;
  ycnt = 0;

  BYTE* buf = blitbuf.lock_buffer();
  UINT16 pitch = blitbuf.get_pitch();
  while (*glyphptr)
  {
    if (*glyphptr == ' ')
    {
      xcnt++;
    }
    else
    {
      buf[((y+ycnt)*pitch) + ((x+xcnt)*1) + 0] = color;

      xcnt++;
    }

    if (xcnt == 7)
    {
      xcnt = 0;
      ycnt++;
    }

    glyphptr++;
  }
  blitbuf.unlock_buffer();
}

static void drawsmallfontstring(Cblitbuf& blitbuf, const char* str, BYTE color, UINT16 x, UINT16 y)
{
  UINT16 xcnt = x;

  for (int i = 0; i < strlen(str); i++)
  {
    drawsmallglyph(blitbuf, str[i], color, xcnt, y);
    xcnt += 8;
  }
}

static void drawshadowsmallfontstring(Cblitbuf& blitbuf, const char* str, BYTE color, UINT16 x, UINT16 y)
{
  drawsmallfontstring(blitbuf, str, 0, x+1, y);
  drawsmallfontstring(blitbuf, str, 0, x, y+1);
  drawsmallfontstring(blitbuf, str, 0, x+1, y+1);
  drawsmallfontstring(blitbuf, str, color, x, y);
}

static void drawgametime(UINT32 ticks, Cblitbuf& blitbuf, BYTE color, UINT16 x, UINT16 y)
{
  UINT32 centiseconds = ticks * 100 / 60;
  UINT32 seconds = ticks / 60;
  UINT32 minutes = seconds / 60;
  UINT32 hours = seconds / 3600;
  seconds = seconds % 60;
  minutes = minutes % 60;
  centiseconds = centiseconds % 100;
  char timebuf[32];
  snprintf(timebuf, sizeof(timebuf), "%02u:%02u:%02u.%02u", hours, minutes, seconds, centiseconds);

  drawshadowsmallfontstring(blitbuf, timebuf, color, x, y);
}

void score_display(VIEWPORT *player)
{
  Cspr_frame *frame;
  UINT16 i;
  INT16 cyfer;
  UINT16 xcnt;
  UINT16 ycnt;
  UINT16 holdcnt;
  static UINT16 diamondknip;

  holdcnt = 0;

  if (scoreshift) scoreshift--;

  if (player->energy == 0)
  {
    player1.energy=33;
    hoirise_init(hoi,0,0,restartx,restarty);
    play_mcdood(hoi->x, hoi->y);
  }


  xcnt = realxcnt;
  realxcnt -= 14;
  realxcnt &= 1023;

  ycnt = realycnt;
  realycnt += 18;
  realycnt &= 1023;

#if 0
  deler = 100000;
  score = player->delayscore;
//  score = restartx;

  if (player->delayscore < player->score)
  {
    player->delayscore += 21;
    if (player->delayscore > player->score) player->delayscore = player->score;
  }
#endif

  cyfer = (INT16) player->lives;

  if (cyfer > 9) cyfer = 9;
  if (cyfer < 0) cyfer = 0;

  frame = anim_setsequence(orgscore, 0, SEQ_FORCE);
  frame = anim_forceframe (orgscore, cyfer);

//  if (world !=3)
    {
      frame->draw(*player->loadedmap->blitbuf,70 + (0*40) + ((sinus512[xcnt]*10)>>10) - scoreshift, 414 + ((sinus512[ycnt]*10)>>10) );
    }


  xcnt += 85;
  xcnt &= 1023;
  ycnt -= 60;
  ycnt &= 1023;

  frame = anim_setsequence(orgheart, 0, SEQ_FORCE);
  frame = anim_forceframe (orgheart, 16 - (player->energy >> 1));
  frame->draw(*player->loadedmap->blitbuf,70 + (1*40) + ((sinus512[xcnt]*10)>>10) - scoreshift, 414 + ((sinus512[ycnt]*10)>>10) );

  scoreposhold[holdcnt   ] = 70 + (1*40) + ((sinus512[xcnt]*10)>>10) - scoreshift + 8;
  scoreposhold[holdcnt+1 ] = 414 + ((sinus512[ycnt]*10)>>10) + 8;
  holdcnt+=2;

  xcnt += 85;
  xcnt &= 1023;
  ycnt -= 60;
  ycnt &= 1023;


  frame = anim_setsequence(orgscore, 0, SEQ_FORCE);

#if 0

  for (i=3;i<9;i++)
  {  
    cyfer = score / deler;
    frame = anim_forceframe (orgscore, cyfer);
    frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );

    xcnt += 85;
    xcnt &= 1023;
    ycnt -= 60;
    ycnt &= 1023;

    score -= cyfer * deler;

    deler /= 10;
  }
#endif

  frame = anim_setsequence(orgdiamond, 0, SEQ_FORCE);

  diamondknip++;
  diamondknip&=31;

  for (i=3;i<10;i++)
  {  

    if ((7-player->nrofblack) > (i-3))
      {
	frame = anim_forceframe (orgdiamond, 0);
      }
    else
      {
	frame = anim_forceframe (orgdiamond, 1);
      }

    scoreposhold[holdcnt   ] = 70 + (i*40) + ((sinus512[xcnt]*10)>>10);
    scoreposhold[holdcnt+1 ] = 414 + ((sinus512[ycnt]*10)>>10) + scoreshift;
    holdcnt+=2;

    if (player->nrofblack )
      {
	if (world !=3)
	  {
	    frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );
	  }
      }
    else
      {
	if ((i-3) == (diamondknip/4))
	  {

	  }
	else if ((i-3) == (((diamondknip+4)&31)/4))
	  {
	    frame = anim_forceframe (orgdiamond, 1);
	    if (world !=3)
	      {
		frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );
	      }
	  }
	else if ((i-3) == (((diamondknip-4)&31)/4))
	  {
	    frame = anim_forceframe (orgdiamond, 1);
	    if (world !=3)
	      {
		frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );
	      }
	  }
	else
	  {
	    frame = anim_forceframe (orgdiamond, 0);
	    if (world !=3)
	      {
		frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );
	      }
	  }
      }

    xcnt += 85;
    xcnt &= 1023;
    ycnt -= 60;
    ycnt &= 1023;

  }


  if (world == 3)
    {
      static UINT16 flits;

      if (paraflg == 7)
	  {
	    flits+=1;
	    flits&=31;
	  }
	  else
	  {
		flits = 0;
	  }

	  frame = anim_setsequence(orgdiamond, 0, SEQ_FORCE);
	frame = anim_forceframe (orgdiamond, 0);

	frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );


	if (boss!=0 && flits<16)
	  {
	    BOSS_BLOOD *bossblood;
	    bossblood = (BOSS_BLOOD *)boss->blood;

	    cyfer = bossblood->hit;


	    frame = anim_setsequence(orgscore, 0, SEQ_FORCE);
	    frame = anim_forceframe (orgscore, cyfer/10);

	    xcnt += 85;
	    xcnt &= 1023;
	    ycnt -= 60;
	    ycnt &= 1023;
	    i++;

	    frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );



	    frame = anim_forceframe (orgscore, cyfer%10);

	    xcnt += 85;
	    xcnt &= 1023;
	    ycnt -= 60;
	    ycnt &= 1023;
	    i++;

	    frame->draw(*player->loadedmap->blitbuf,70 + (i*40) + ((sinus512[xcnt]*10)>>10), 414 + ((sinus512[ycnt]*10)>>10) + scoreshift );

	  }


    }



  if (player->newlife >= 32)
    {
      player->newlife = 0;
      extralivefloat = 400;
    }

  frame = anim_setsequence(orgcolormond, 0, SEQ_FORCE);
  frame = anim_forceframe (orgcolormond, player->newlife/2);
  if (world !=3)
    {
      frame->draw(*player->loadedmap->blitbuf,70 + (11*40) + ((sinus512[xcnt]*10)>>10) + scoreshift, 414 + ((sinus512[ycnt]*10)>>10) );
    }
  scoreposhold[holdcnt   ] = 70 + (11*40) + ((sinus512[xcnt]*10)>>10) + scoreshift+8;
  scoreposhold[holdcnt+1 ] = 414 + ((sinus512[ycnt]*10)>>10)+8;
  holdcnt+=2;

  if (extralivefloat)
    {
      extralivefloat -= 2;

      if (extralivefloat & 8)
	{
	  frame = anim_setsequence(orgcolormond, 0, SEQ_FORCE);
	  frame = anim_forceframe (orgcolormond, 16);
	  if (world !=3)
	    {
	      frame->draw(*player->loadedmap->blitbuf,110 + extralivefloat + ((sinus512[xcnt]*10)>>10) + scoreshift, 414 + ((sinus512[ycnt]*10)>>10) );
	    }
	}

      if (extralivefloat == 0) player->lives++;
    }

#if 0
  xcnt += 85;
  xcnt &= 1023;
  ycnt -= 60;
  ycnt &= 1023;


  cyfer = player->nrofblack;
  if (cyfer > 9) cyfer = 9;

  frame = anim_setsequence(orgscore, 0, SEQ_FORCE);
  frame = anim_forceframe (orgscore, cyfer);
  frame->draw(*player->loadedmap->blitbuf,70 + (12*40) + ((sinus512[xcnt]*10)>>10) + scoreshift, 414 + ((sinus512[ycnt]*10)>>10) );
#endif

  if (speedrun_state.running)
  {
    drawgametime(speedrun_state.gametime, *player->loadedmap->blitbuf, 0x10, 8, 8);

    drawgametime(speedrun_state.leveltime[world*4+level], *player->loadedmap->blitbuf, 0x10, 8, 20);
  }
}





