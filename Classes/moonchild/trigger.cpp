#include <frm_wrk.hpp>
#include <trigger.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <hoi.hpp>
#include <punt.hpp>
#include <spike.hpp>
#include <wheel.hpp>
#include <stukhout.hpp>
#include <sokoban.hpp>
#include <sound.hpp>


void trig_close_hole(UINT16 x, UINT16 y, UINT16 pat, UINT16 amount);

UINT16 nextdelay = 0;


void trigger()
{
  TRIGGER_ITEM *curtrig;

  if(editflg) return;
  if (!hoi->frame) return;    // MC wappert

  if (!triglist) return;  //no triggerlist available for level

  curtrig = triglist;

  while (curtrig->trigger_fn != 0)
  {
    if (hoi->x > curtrig->x1 && hoi->x < curtrig->x2)
    {
      if (hoi->y > curtrig->y1 && hoi->y < curtrig->y2)
      {
        curtrig->trigger_fn(); // call trigger function
      }
    }    
    curtrig++;
  }
}


void bordwissel_11(void)
{
  player1.loadedmap->map[(128*player1.loadedmap->mapsizex/32/32)+(2528/32)] = 936;
  player1.loadedmap->map[(128*player1.loadedmap->mapsizex/32/32)+(2560/32)] = 937;

  player1.loadedmap->map[(160*player1.loadedmap->mapsizex/32/32)+(2528/32)] = 956;
  player1.loadedmap->map[(160*player1.loadedmap->mapsizex/32/32)+(2560/32)] = 957;

}

void sneaktransfer_11(void)
{
  hoi->y -= 256;
  player1.worldy -= 256;
  player1.focusy -= 256;
}


void madeit_11(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild loopt automatisch naar rechts
}


void reallymadeit_11(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void madeit_00(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild loopt automatisch naar rechts
}


void reallymadeit_00(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void madeit_12(void)
{
  play_madeit();
  mc_autorun = -1;  //moonchild loopt automatisch naar links
}


void reallymadeit_12(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void madeit_13(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild loopt automatisch naar rechts
}


void reallymadeit_13(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void sneaktransfer_13(void)
{
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)hoi->blood;

  if (hoiblood->gravy >= 0)
  {
    hoi->x -= 768;
    player1.worldx -= 768;
    player1.focusx -= 768;
  }
}

void sneaktransfer2_13(void)
{
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)hoi->blood;

  hoi->y -= 768;
  player1.worldy -= 768;
  player1.focusy -= 768;
}


void sneaktransfer3_13(void)
{
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)hoi->blood;

  hoi->y += 128;
  player1.worldy += 128;
  player1.focusy += 128;
}

void sneaktransfer4_13(void)
{
  HOI_BLOOD *hoiblood;
  hoiblood = (HOI_BLOOD *)hoi->blood;

  hoi->x -= 768;
  player1.worldx -= 768;
  player1.focusx -= 768;
}

UINT16 quakeamount[100] =
  { 0,1,2,3,4,5,6,5,4,3,2,2,1,1,0,0,
    0,0,0,0,1,2,3,4,3,
    2,1,0,0,0,0,0,0,0,0,1,1,2,2,3,3,
    3,3,4,4,5,5,6,6,6,
    5,5,4,3,2,1,0,0,0,1,2,3,4,4,3,3,
    2,2,1,1,0,0,0,0,0,
    0,0,0,0,0,0,0,1,1,2,2,3,3,2,2,1,
    1,0,0,0,0,0,0,0,0,
  };

void earthquake_13(void)
{
  static int quakecnt = 10;
  static int quakeamountcnt = 0;

  rumblesoundharder();

  quakeamountcnt++;
  if (quakeamountcnt == 400) quakeamountcnt = 0;

  player1.quakex = MC_RANDOM(quakeamount[quakeamountcnt>>2]);
  player1.quakey = MC_RANDOM(quakeamount[quakeamountcnt>>2]);

  quakecnt--;
  if (!quakecnt)
  {
    quakecnt = MC_RANDOM(18) + 1;
    punt_init(MC_RANDOM(45)*32+256, 5920);
  }
}


UINT16 logxtab[5] = {146,368,632,928,1152};


void loginitter_13(void)
{
  static int logxindex = 0;
  static int logcnt = 10;

  static INT32 maxvol;

  maxvol = 300;

  if (hoi->x > 1022)
    {
      maxvol -= ((hoi->x - 1022)*7);
    }

  logcnt--;
  if (!logcnt)
  {
    logcnt = 80;
    logxindex++;
    if (logxindex == 5) logxindex = 0;
    stukhout_init(logxtab[logxindex], 4960, 5791);
  }
}


void warp_13(void)
{
  player1.nrofblack = 0;
  mc_autorun = -1;  //moonchild loopt automatisch naar rechts

  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
  play_madeit();
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}

void warp_14(void)
{
  player1.nrofblack = 0;

  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
  play_madeit();
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}

void stopautoscroll_14(void)
{
      if (!hoi->frame) return;   // mc is not visible
	if (autoscrollspd != 1)
	{
		play_camstop();
		stop_cammove();
	}
	autoscrollspd = 1;
}


void stopautoscroll2_14(void)
{
      if (!hoi->frame) return;   // mc is not visible
	if (autoscrollspd != 0)
	{ 
		play_camstop();
		stop_cammove();
	}
  autoscrollspd = 0;
}


void startautoscroll_14(void)
{
	static int autocntoff = 0;
      if (!hoi->frame) return;   // mc is not visible
	if (autoscrollspd != 2)
	{
		play_camstart();
		autocntoff = 50;
		autoscrollspd = 2;
	}

	if (autocntoff && autoscrollspd==2)
	{
		autocntoff--;
		play_cammove();
	}
}


void stopfastrun_14(void)
{
  HOI_BLOOD *hoiblood;

  hoiblood = (HOI_BLOOD *) hoi->blood;
  hoiblood->hoi_caps->maxspd = 140;
}


void madeit_14(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild loopt automatisch naar rechts
}


void reallymadeit_14(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void notkneeldood_21(void)
{
  HOI_BLOOD *hoiblood;

  hoiblood = (HOI_BLOOD *) hoi->blood;

  if (!hoiblood->kneelcnt)
  {
     player1.energy = 0;
  }
}


void madeit_21(void)
{
  play_madeit();
  mc_autorun = -1;  //moonchild loopt automatisch naar links
}


void reallymadeit_21(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void sneaktransfer_22(void)
{
  fallspeed += 1;
  fallspeed &= 127;

  if (fallspeed != 0)
  {
    hoi->y -= 128;
    player1.worldy -= 128;
    player1.focusy -= 128;
    fakespeedadd = 128;
  }
}


void floatingobj_22(void)
{
  static UINT16 floatcnt = 10;

  fakespeedadd = 0;

  if (fallspeed != 0 && fallspeed < 100)
  {
    floatcnt--;
    if (!floatcnt)
    {
      floatcnt = MC_RANDOM(30) + 20;

      spike_init(MC_RANDOM(600)+2450, 1160, 1920);   //1860
    }
  }
}

void fanl1_22(void)
{
  if (hoi->x > 820)
  {
    hoi->x -= 6;
  }
  else
  {
    UINT16 spd;
    spd = 820 - hoi->x;
    spd = spd / 32;
    if (spd > 6) spd = 6;
    spd = 6 - spd;
    hoi->x -= spd;
  }
}


void fanr1_22(void)
{
  if (hoi->x < 900)
  {
    hoi->x += 6;
  }
  else
  {
    UINT16 spd;
    spd = hoi->x - 900;
    spd = spd / 32;
    if (spd > 6) spd = 6;
    spd = 6 - spd;
    hoi->x += spd;
  }
}

void fanr2_22(void)
{
  if (hoi->x < 900)
  {
    hoi->x += 8;
  }
  else
  {
    UINT16 spd;
    spd = hoi->x - 900;
    spd = spd / 32;
    if (spd > 8) spd = 8;
    spd = 8 - spd;
    hoi->x += spd;
  }
}


void fanb1_22(void)
{
  UINT16 spd;
  HOI_BLOOD *hoiblood;

  hoiblood = (HOI_BLOOD *)hoi->blood;

  spd = hoi->y - 3220;
  spd = spd /32;

  hoiblood->gravy -= spd;
}


void fanb2_22(void)
{
  UINT16 spd;
  HOI_BLOOD *hoiblood;

  hoiblood = (HOI_BLOOD *)hoi->blood;

  spd = hoi->y - 3970;
  spd = spd /32;

  hoiblood->gravy -= spd;
}


void drunk_22(void)
{
  static UINT16 toggle = 0;
  static UINT16 c1 = 0;
  static UINT16 c2 = 0;

  toggle ^= 1;

  player1.quakex = ((sinus512[c1]*(drunk_amp>>3))>>10);
  player1.quakey = ((sinus512[c2]*(drunk_amp>>3))>>10);


  if ((hoi->x > 2716 || hoi->x < 497) && drunk_amp > 0)
  {
    if (drunk_amp) drunk_amp--;
    if (drunk_amp) drunk_amp--;
    if (drunk_amp) drunk_amp--;
    if (drunk_amp) drunk_amp--;
  }
  else
  {
    if (drunk_amp < 512) drunk_amp++;
  }


  if (toggle)
  {
    player1.quakex = -player1.quakex;
    player1.quakey = -player1.quakey;
  }

  c1 += 3;
  c1 &= 1023;
  c2 += 5;
  c2 &= 1023;
}


void warp_22(void)
{
  player1.nrofblack = 0;
  mc_autorun = -1;  //moonchild loopt automatisch naar rechts

  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
  play_madeit();
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void madeit_22(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild loopt automatisch naar rechts
}


void reallymadeit_22(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void tyres_23(void)
{
  static int tyredelay = 2;

  tyredelay--;
  if (!tyredelay)
    {
      wheel_init(394+64, 17570, 19142);
      tyredelay = 350;
    }
}


void sneaktransfer_23(void)
{
  hoi->y -= 640;
  player1.worldy -= 640;
  player1.focusy -= 640;
}


void sneaktransfer2_23(void)
{
  hoi->y -= 160;
  player1.worldy -= 160;
  player1.focusy -= 160;
}

void sneaktransfer3_23(void)
{
  hoi->y -= 160;
  player1.worldy -= 160;
  player1.focusy -= 160;
}


void madeit_23(void)
{
  play_madeit();
  mc_autorun = -1;  //moonchild loopt automatisch naar rechts
}


void reallymadeit_23(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void changepara_31(void)
{
  paraflg = 1;
}

void changepara2_31(void)
{
  paraflg = 2;
}

void opendoor_31(void)
{
  trig_close_hole(2688, 2144, 0, 4);
}


void opendoor2_31(void)
{
  UINT16 x,y;

  x = 1968;
  y = 160;

  player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = 204;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;

  player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = 224;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;

  player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = 204;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;

  player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = 224;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;
  colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
  y += 16;

}


void restart_31(void)
{
  if (restartcnt != 1)
  {
    restartcnt = 1;
    HOI_BLOOD *hoiblood;
    hoiblood = (HOI_BLOOD *)hoi->blood;
      
    restartx = (hoi->x);
    restarty = (hoi->y);
    hoiblood->flashcnt = 200;

    play_restart();
  }
}


void trig_close_hole(UINT16 x, UINT16 y, UINT16 pat, UINT16 amount)
{
  UINT16 j;
  
  amount *= 2;

  for (j=0; j<amount; j++)
    {
      player1.loadedmap->map[(y*player1.loadedmap->mapsizex/32/32)+(x/32)]  = pat;
      colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = (pat==0) ? 0 : 1;
      x += 16;
    }
}


void madeit_31(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild vliegt automatisch naar rechts
}


void reallymadeit_31(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void shootenable_31(void)
{
  shootenabledflg = 1;
}


void resetsoko_32(void)
{
  sokoban_reinit(soko2);
         
  play_warp();

  hoirise_init(hoi, hoi->x, hoi->y, 145, 1000);
}


void verkleur_32(void)
{
  UINT16  newcolumnr;

  UINT16  startx;
  UINT16  starty;

  UINT16  colpat;
  UINT16 *patpoint;

  HOI_BLOOD *hoiblood;

  hoiblood = (HOI_BLOOD *) hoi->blood;
  
  
  newcolumnr = ((((hoiblood->yfloat >> 8)-1332) / 128) * 5) + (((hoiblood->xfloat >> 8)-1440) / 128);

  if (newcolumnr != oldcolumnr)
  {
    lastcolumnr = oldcolumnr;
    oldcolumnr = newcolumnr;

    startx = ((newcolumnr%5)*128)+1472;
    starty = ((newcolumnr/5)*128)+1376;

	patpoint = &player1.loadedmap->map[(starty*player1.loadedmap->mapsizex/32/32)+(startx/32)];

    if (*patpoint == 204)
	{
      colpat = 200;
      play_qbert1(hoi->x,hoi->y);
	}
	else
	{
      colpat = 204;
      play_qbert2(hoi->x,hoi->y);
	}


    if (*(patpoint) > 198 && *(patpoint) < 240)
	{
      *(patpoint) = colpat;
	}
    if (*(patpoint+1) > 198 && *(patpoint+1) < 240)
	{
      *(patpoint+1) = colpat+1;
	}
    if (*(patpoint+2) > 198 && *(patpoint+2) < 240)
	{
      *(patpoint+2) = colpat;
	}
    if (*(patpoint+3) > 198 && *(patpoint+3) < 240)
	{
      *(patpoint+3) = colpat+1;
	}

	patpoint+=(player1.loadedmap->mapsizex/32);
  
    if (*(patpoint) > 198 && *(patpoint) < 240)
	{
      *(patpoint) = colpat+20;
	}
    if (*(patpoint+1) > 198 && *(patpoint+1) < 240)
	{
      *(patpoint+1) = colpat+21;
	}
    if (*(patpoint+2) > 198 && *(patpoint+2) < 240)
	{
      *(patpoint+2) = colpat+20;
	}
    if (*(patpoint+3) > 198 && *(patpoint+3) < 240)
	{
      *(patpoint+3) = colpat+21;
	}
  
	patpoint+=(player1.loadedmap->mapsizex/32);
 
    if (*(patpoint) > 198 && *(patpoint) < 240)
	{
      *(patpoint) = colpat;
	}
    if (*(patpoint+1) > 198 && *(patpoint+1) < 240)
	{
      *(patpoint+1) = colpat+1;
	}
    if (*(patpoint+2) > 198 && *(patpoint+2) < 240)
	{
      *(patpoint+2) = colpat;
	}
    if (*(patpoint+3) > 198 && *(patpoint+3) < 240)
	{
      *(patpoint+3) = colpat+1;
	}

	patpoint+=(player1.loadedmap->mapsizex/32);
  
    if (*(patpoint) > 198 && *(patpoint) < 240)
	{
      *(patpoint) = colpat+20;
	}
    if (*(patpoint+1) > 198 && *(patpoint+1) < 240)
	{
      *(patpoint+1) = colpat+21;
	}
    if (*(patpoint+2) > 198 && *(patpoint+2) < 240)
	{
      *(patpoint+2) = colpat+20;
	}
    if (*(patpoint+3) > 198 && *(patpoint+3) < 240)
	{
      *(patpoint+3) = colpat+21;
	}
  
  }

}


void madeit_32(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild vliegt automatisch naar rechts
}


void reallymadeit_32(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void restartcnt_33(void)
{
    totalshootcnt = 0;
    totalgenercnt = 0;
}

void restart_33(void)
{
  if (restartcnt != 1)
  {
    restartcnt = 1;
    HOI_BLOOD *hoiblood;
    hoiblood = (HOI_BLOOD *)hoi->blood;
      
    restartx = 4848;
    restarty = 1236;
    hoiblood->flashcnt = 200;

    play_restart();
  }
}

void restart2_33(void)
{
  if (restartcnt != 2)
  {
    restartcnt = 2;
    HOI_BLOOD *hoiblood;
    hoiblood = (HOI_BLOOD *)hoi->blood;
      
    restartx = 12188;
    restarty = 757;
    hoiblood->flashcnt = 200;

    play_restart();
  }
}


void madeit_33(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild vliegt automatisch naar rechts
}


void reallymadeit_33(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void shootenable_34(void)
{
  shootenabledflg = 1;
}

void restart_34(void)
{
  if (restartcnt != 1)
  {
    restartcnt = 1;
    HOI_BLOOD *hoiblood;
    hoiblood = (HOI_BLOOD *)hoi->blood;
      
    restartx = 4410;
    restarty = 4529;
    hoiblood->flashcnt = 200;

    play_restart();
  }
}

void madeit_34(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild vliegt automatisch naar rechts
}


void reallymadeit_34(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 200;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}


void changepara_41(void)
{
  paraflg = 2;
}


void changepara2_41(void)
{
  paraflg = 3;
}


void changepara3_41(void)
{
  paraflg = 5;
  hoi->y += 256;
  player1.worldy += 256; 
  player1.focusy += 256;

  play_tik();

  para->clear(0);    // this clears the paralax background.... so we can blit unlimited bobs on there!
}


void changepara4_41(void)
{
  paraflg = 6;
}


void changepara5_41(void)
{
  paraflg = 7;
}

void changepara8_41(void)
{
  paraflg = 8;
}


void camcircle_41(void)
{
  static UINT16 c1 = 0;
  static UINT16 c2 = 256;

  player1.quakex = ((sinus512[c1]*(drunk_amp>>3))>>10);
  player1.quakey = ((sinus512[c2]*(drunk_amp>>3))>>10);


  if ((hoi->y < 520 || hoi->y > 2400) && drunk_amp > 0 && hoi->x < 5320)
  {
    if (drunk_amp) drunk_amp--;
    if (drunk_amp) drunk_amp--;
    if (drunk_amp) drunk_amp--;
    if (drunk_amp) drunk_amp--;
  }
  else
  {
    if (drunk_amp < 512) drunk_amp++;
  }

  if (hoi->x > 5320 && hoi->y < 1000)
    {
      int r;
      r = 512 - (1000 - hoi->y);
      if (r < 0) r = 0;
      drunk_amp = r;
    }



  if (hoi->y < 1037 && hoi->x < 5320)
    {
      c1 += 21;
      c2 += 21;
      c1 &= 1023;
      c2 &= 1023;
    }
  else
    {
      c1 += 42;
      c2 += 42;
      c1 &= 1023;
      c2 &= 1023;
    }
}


void flipcam_41(void)
{
  static UINT16 flipper = 5;

  paraflg = 4;

  if (!hoi->frame) return;   // mc is not visible
  
  if (flipper) flipper--;
  if (!flipper)
    {
      flipper = 50;
      if (hoi->y > 492)
  {
    hoi->y -= 256;
    player1.worldy -= 256;
    player1.focusy -= 256;
    play_tik();
  }
      else
  {
    hoi->y += 256;
    player1.worldy += 256;
    player1.focusy += 256;
    play_tak();
  }
    }
}


void madeit_41(void)
{
  play_madeit();
  mc_autorun = 1;  //moonchild vliegt automatisch naar rechts
}


void reallymadeit_41(void)
{
  if(nextlvlflg == 0 && nextdelay == 0)
  {
    nextdelay = 2;
    //    player1.score += 5000;
  }

  if (nextdelay)
  {
    nextdelay--;
    if (nextdelay == 0) nextlvlflg = 1;
  }
}
