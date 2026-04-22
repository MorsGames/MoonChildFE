#include <frm_wrk.hpp>
#include <score.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <sokoban.hpp>
#include <boss.hpp>
#include <sound.hpp>


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
}





