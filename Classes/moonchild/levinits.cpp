#include <hoi.hpp>
#include <elevat.hpp>
#include <lift.hpp>
#include <ironring.hpp>
#include <ironauto.hpp>
#include <weight.hpp>
#include <spies.hpp>
#include <bump.hpp>
#include <basic.hpp>
#include <element.hpp>
#include <ptoei.hpp>
#include <bat.hpp>
#include <warp.hpp>
#include <switsj.hpp>
#include <gumbal.hpp>
#include <bee.hpp>
#include <stukhout.hpp>
#include <chain.hpp>
#include <rocket.hpp>
#include <snake.hpp>
#include <plof.hpp>
#include <score.hpp>
#include <trigger.hpp>
#include <objects.hpp>
#include <bullet.hpp>
#include <cannon.hpp>
#include <bolt.hpp>
#include <diamond.hpp>
#include <seamine.hpp>
#include <goodday.hpp>
#include <sokoban.hpp>
#include <mushroom.hpp>
#include <wheel.hpp>
#include <rock.hpp>
#include <gravlift.hpp>
#include <mouth.hpp>
#include <plant.hpp>
#include <anim.hpp>
#include <editor.hpp>
#include <globals.hpp>
#include <scroll.hpp>
#include <asset.hpp>
#include <sound.hpp>
#include <metcan.hpp>
#include <boss.hpp>
#include <paal.hpp>
#include <bbot.hpp>
#include <doldoler.hpp>
#include <sneak.hpp>
#include <asteroid.hpp>
#include <claw.hpp>


//#define POCKETPCDEMO


void close_hole(UINT16 x, UINT16 y, UINT16 pat, UINT16 amount, UINT16 vertflg);
void flame_hit(OBJECT *flame, UINT32 param);
void mine_hit(OBJECT *flame, UINT32 param);
void snake1_ready(OBJECT *snake, UINT32 param);
void snake2_ready(OBJECT *snake, UINT32 param);
void do_doolhof(void);
void gen_doolhof(void);


HOI_CAPS hoi_caps     = {ID_HOI,     1, 1, 1, 140, 4, 54, 54};
HOI_CAPS elf_caps     = {ID_ELF,     1, 1, 1, 140, 5, 36, 36};
HOI_CAPS helmut_caps1 = {ID_HELMUT,  1, 1, 1, 140, 5, 42, 42};
HOI_CAPS helmut_caps2 = {ID_HELMUT,  1, 1, 1, 150, 5, 42, 42};
HOI_CAPS helmut_caps3 = {ID_HELMUT,  1, 1, 1, 150, 5, 60, 60};
HOI_CAPS helmut_caps4 = {ID_HELMUT,  1, 1, 1, 158, 5, 60, 60};

OBJECT *gooddaytab[50];



// #define DEMOVERSION
#define DEMOVERSION2

void init_level_11(void)
{
  OBJECT *elf;

#ifndef POCKETPCDEMO
  warp_init(2892,1179,2846,106,0);  // naar secret area 1
  warp_init(2926,126 ,2867,906,0);  // en weer terug

  basic_init(408,626,basic_pingpong,190,624,2,0,0,0,0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);

  basic_init(205,242,basic_pingpong,64,332,2,0,0,0,0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);

  basic_init(694,434,basic_pingpong,544,844,2,0,0,0,0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);

  elf = hoi_globinit(orgelf, &elf_caps, elf_ai, 0, 0);
  hoi_init(elf, 3204,1194);

  switsj_init(4832, 944, 4000, 1152,1);


// 2 slangetjes op bamboo patterns

  basic_init(5781, 1138, basic_pingpong, 5681, 5881, 2, 0, 0, 0, 0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);
  basic_init(5781, 1042, basic_pingpong, 5681, 5881, -2, 0, 0, 0, 0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);

// rotsblok

  basic_init(3592, 600, 0, 0, 0, 0, basic_gravy, 200, 0, 1,
              0,0,0,0, orgrock,1,0,SEQ_DUMMY,0,0,0,0,0,0);

  basic_init(3592, 900, 0, 0, 0, 0, basic_gravy, 200, 0, 1,
              0,0,0,0, orgrock,1,0,SEQ_DUMMY,0,0,0,0,0,0);

  basic_init(3592, 1600, 0, 0, 0, 0, basic_gravy, 200, 0, 1,
              0,0,0,0, orgrock,1,0,SEQ_DUMMY,0,0,0,0,0,0);

  basic_init(3592, 2000, 0, 0, 0, 0, basic_gravy, 200, 0, 1,
              0,0,0,0, orgrock,1,0,SEQ_DUMMY,0,0,0,0,0,0);


// Inits in pattern field


  // zet bord naar links
  player1.loadedmap->map[(128*player1.loadedmap->mapsizex/32/32)+(2528/32)] = 934;
  player1.loadedmap->map[(128*player1.loadedmap->mapsizex/32/32)+(2560/32)] = 935;

  player1.loadedmap->map[(160*player1.loadedmap->mapsizex/32/32)+(2528/32)] = 954;
  player1.loadedmap->map[(160*player1.loadedmap->mapsizex/32/32)+(2560/32)] = 955;


  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

//  hoirise_init(222,1394);
  hoirise_init(hoi, 100,1098, 100, 1098);
  restartx = 100;
  restarty = 1098;

  player1.worldx = 0;
  player1.worldy = 901;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

  play_waterval();
	real_waterval_volume();
	real_waterval_volume();
#endif
}


void init_level_12(void)
{

  elevat_init(3634,3500,elevat_siny,  0, 4, 128);
  elevat_init(3634,3300,elevat_siny,  512, 8, 128);

  elevat_init(5920,2940-10,elevat_sinx,  00, 4, 192);
  elevat_init(5920,2870-10,elevat_sinx,  64, 4, 208);
  elevat_init(5920,2800-10,elevat_sinx,  128, 4, 224);

  bump_init(4224,3088,2);

  basic_init(3400,3638,basic_sin,0,4,16,basic_sin,0,9,80,
             0,0,0,0, orgbird,1,0,SEQ_FLYLEFT,0,0,0,0,0,prox_vogel_volume);

  basic_init(4219,3980,basic_sin,0,6,32,basic_sin,0,7,108,
              0,0,0,0,  orgbird,1,0,SEQ_FLYLEFT,0,0,0,0,0,prox_vogel_volume);
  basic_init(4580,4180,basic_sin,0,4,256,basic_sin,0,4,128,
              basic_sin,0,8,32,  orgbird,1,0,SEQ_DUMMY,0,0,0,0,0,prox_vogel_volume);

  ptoei_init(856,3056);

  basic_init(4657,4580,basic_sin,1024,2,512,basic_sin,0,3,8,
              0,0,0,0,orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);
  basic_init(4157,4580,basic_sin,0,2,512,basic_sin,2048,3,8,
              0,0,0,0, orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);

  bat_init(5807,4318,basic_sin,1024,2,512,basic_sin,0,3,8);

  bat_init(6645,4304,basic_sin,1024,2,128,basic_sin,0,3,100);

  bump_init(2752,2720,0);

  basic_init(3274,4082,basic_pingpong,3074,3475,2,0,0,0,0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);

  warp_init(4557,4420,5445,4074,0);
  warp_init(1878,4543,4403+128,3594,0);

  warp_init(6944,2532,1881,2026,0);   // to secret spot with shoes........

  //  switsj_init(7456,4432,1248,2976);
  switsj_init(/* 1380 */ 1410,2576,1248,2976,1);
  
//  lift_init(2244,240,800);


  //   spies_init(2272,256,0,192,-2);

//  spies_init(2240,540,0,00+112,-1);


  bump_init(6640,384,2);
  bump_init(3488,512,2);
  bump_init(2448,320,0);


//  bump_init(2944,544,2);


  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);
//  hoirise_init(222,4394);
  hoirise_init(hoi, 222,4394, 222,4394);

  restartx = 222;
  restarty = 4394;

  player1.worldx = 0;
  player1.worldy = 4197;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

	play_vogel();
	play_bat();
	real_bat_volume();
	real_vogel_volume();
	real_bat_volume();
	real_vogel_volume();

}


void init_level_13(void)
{

#ifndef POCKETPCDEMO
  gumbal_init(440,7555,6);
  gumbal_init(320,7555,0);

  elevat_init(288, 7232, elevat_sinx,  00, 2, 256);
  elevat_init(648, 7232, elevat_sinx, 512, 2, 256);

  bee_init(1020, 7160, 920, 1212, 1);

  elevat_init(200, 6785, elevat_siny,   0, 2, 256);
  elevat_init(300, 6785, elevat_siny,  64, 2, 256);
  elevat_init(400, 6785, elevat_siny, 128, 2, 256);
  elevat_init(500, 6785, elevat_siny, 192, 2, 256);
  elevat_init(600, 6785, elevat_siny, 256, 2, 256);

  elevat_init(128, 4716, elevat_siny, 256, 8, 256);

  basic_init(1527, 5330, basic_pingpong,1336,1780,2,0,0,0,0,
              0,0,0,0, orgsnake,2,1,SEQ_DUMMY,0,0,0,0,0,0);

  basic_init(290, 4556, basic_pingpong,290,600,3,0,0,0,0,
              0,0,0,0, orgblubber,2,1,0,0,0,0,0,0,0);
  basic_init(940, 4556, basic_pingpong,630,940,-3,0,0,0,0,
              0,0,0,0, orgblubber,2,1,0,0,0,0,0,0,0);

  basic_init(970, 4556, basic_pingpong,970,1280,3,0,0,0,0,
              0,0,0,0, orgblubber,2,1,0,0,0,0,0,0,0);

  spies_init(1632,4358,0,192,-1,1);

  spies_init(1696,4198,1,192,-1,0);

  spies_init(1632,4038,0,192,-2,0);

  spies_init(1696,3878,1, 32,-2,1);


  basic_init(0, 3506, basic_flyby,-100,1900,-3,basic_sin,0,32,8,
              0,0,0,0, orgjudith,1,1,0,0,0,play_heks,1600,1600,0);

  basic_init(666, 3506, basic_flyby,-100,1900,-3,basic_sin,0,32,8,
              0,0,0,0, orgjudith,1,1,0,0,0,play_heks,1600,1600,0);

  basic_init(1333, 3506, basic_flyby,-100,1900,-3,basic_sin,0,32,8,
              0,0,0,0, orgjudith,1,1,0,0,0,play_heks,1600,1600,0);


  basic_init(0, 3478, basic_flyby,1900,-100,3,basic_sin,0,32,8,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);

  basic_init(666, 3478, basic_flyby,1900,-100,3,basic_sin,0,32,8,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);

  basic_init(1333, 3478, basic_flyby,1900,-100,3,basic_sin,0,32,8,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);


  basic_init(0, 3306, basic_flyby,1900,-100,3,basic_sin,0,56,16,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);

  basic_init(1000, 3306, basic_flyby,1900,-100,3,basic_sin,0,32,16,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);



  basic_init(630, 3190, basic_sin,0,20,16,basic_sin,0,3,240,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);

  basic_init(330, 3190, basic_sin,0,14,16,basic_sin,0,5,240,
              0,0,0,0, orgbat,2,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bat_volume);


  basic_init(0, 3106, basic_flyby,-100,1900,-3,basic_sin,0,32,8,
              0,0,0,0, orgjudith,1,1,0,0,0,play_heks,1600,1600,0);

  basic_init(666, 3106, basic_flyby,-100,1900,-3,basic_sin,0,32,8,
              0,0,0,0, orgjudith,1,1,0,0,0,play_heks,1600,1600,0);

  basic_init(1333, 3106, basic_flyby,-100,1900,-3,basic_sin,0,32,8,
              0,0,0,0, orgjudith,1,1,0,0,0,play_heks,1600,1600,0);



  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

//  hoirise_init(222,1394);
  hoirise_init(hoi, 1441,9386, 1441,9386);

  restartx = 1441;
  restarty = 9386;

  player1.worldx = 1172;
  player1.worldy = 9120;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

  play_rumble();
  play_waterval();
	play_vogel();
	play_bat();
	play_bee();
	real_bat_volume();
	real_bee_volume();
	real_vogel_volume();
	real_waterval_volume();
	real_bee_volume();
	real_bat_volume();
	real_vogel_volume();
	real_waterval_volume();
#endif
}


void init_level_14(void)
{
  OBJECT *rock;

#ifndef POCKETPCDEMO
  basic_init(2586, 1038, basic_sin,0,20,16,basic_sin,0,10,100,
       0,0,0,0, orgbat,2,0,SEQ_FLYLEFT,0,0,0,0,0,prox_bat_volume);

  mushroom_init(4870, 590, 13, 220, 10, 32);

  mushroom_init(5490, 790, 6, 350, 10, 32);

  mushroom_init(5776, 590, 6, 420, 10, 32);

  mushroom_init(6160, 526, 3, 512, 10, 40);

  gravlift_init(384-10,1024+4);
  gravlift_init(320-10,938+4);
  gravlift_init(256-10,832+4);
  gravlift_init(448-10,768+4);

  gravlift_init(608-10,736+4);
  gravlift_init(768-10,736+4);
  gravlift_init(960-10,768+4);

  gravlift_init(1120-10,1088+4);
  gravlift_init(1216-10,1056+4);
  gravlift_init(1472-10,1152+4);

  gravlift_init(1952-10,1024+4);
  gravlift_init(1824-10, 928+4);
  gravlift_init(1696-10, 832+4);
  gravlift_init(1888-10, 768+4);

  gravlift_init(3168-10, 1024+4);
  gravlift_init(3104-10,  960+4);

  switsj_init( 4134,  304, 4496, 224,1);


  mouth_init( 9728, 868, -9);
  mouth_init( 9986, 868, -9);
  mouth_init(10232, 836, -9);
  mouth_init(10346, 836, -9);

  basic_init(11030, 490, basic_sin,0,3,500,basic_sin,0,3,16,
              0,0,0,0, orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);

  basic_init(11330, 440, basic_sin,0,3,500,basic_sin,0,3,16,
              0,0,0,0, orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);

  basic_init(11630, 490, basic_sin,0,3,500,basic_sin,0,3,16,
              0,0,0,0, orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);

  basic_init(11930, 440, basic_sin,0,3,500,basic_sin,0,3,16,
              0,0,0,0, orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);

  basic_init(12230, 490, basic_sin,0,3,500,basic_sin,0,3,16,
              0,0,0,0, orgbat,2,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);


  rock = rock_init(14760, 882, 14760, 15400);

  plant_init(15080, 928, 5, 5, rock);



  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

//  hoirise_init(222,1394);
  hoirise_init(hoi, 100,1066, 100,1066);
  restartx = 100;
  restarty = 1066;

  player1.worldx = 0;
  player1.worldy = 736;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

	play_vogel();
	play_bat();
	play_roltnlp();
	real_bat_volume();
	real_vogel_volume();
	real_roltnlp_volume();
	real_bat_volume();
	real_vogel_volume();
	real_roltnlp_volume();
#endif
}


void init_level_21(void)
{

#ifndef POCKETPCDEMO

  basic_init(1440, 6160, basic_pingpong, 1140, 1740, 2, 0, 0, 0, 0,
              0,0,0,0, orgpneu,3,1,0,0,0,0,0,0,prox_drilboor_volume);

  basic_init(3488, 5648, basic_pingpong, 3488, 3720, 2, 0, 0, 0, 0,
              0,0,0,0, orgpneu,3,1,0,0,0,0,0,0,prox_drilboor_volume);
  basic_init(3720, 5488, basic_pingpong, 3488, 3720, -2, 0, 0, 0, 0,
              0,0,0,0, orgpneu,3,1,0,0,0,0,0,0,prox_drilboor_volume);

  ironring_init( 345, 5555, ironring_sinxy, 00, 2, 256, 0);
  ironring_init(3430, 5103, ironring_sinx,  00, 2, 256, 0);


  weight_init(2608,4128, 0,1);
  weight_init(2768,4128,48,1);
  weight_init(2928,4128,96,1);


//  mixer_init(1056,4448,900,1200,2);

  basic_init(1056, 4476-26, basic_pingpong, 700, 1700, 2, 0, 0, 0, 0,
              0,0,0,0, orgmixer,1,0,1,0,0,0,0,0,0);

  basic_init(1056, 4476, basic_pingpong, 700, 1700, 2, 0, 0, 0, 0,
              0,0,0,0, orgmixer,1,0,0,0,0,0,0,0,0);

  basic_init(1056, 4476-26, basic_pingpong, 700, 1700, 3, 0, 0, 0, 0,
              0,0,0,0, orgmixer,1,0,1,0,0,0,0,0,0);

  basic_init(1056, 4476, basic_pingpong, 700, 1700, 3, 0, 0, 0, 0,
              0,0,0,0, orgmixer,1,0,0,0,0,0,0,0,0);

  
  basic_init(2084, 6096, basic_flipflap, 2084, 4000, 150, 0, 0, 0, 0,
              0,0,0,0, orgfire,1,0,0,0,0,0,0,0,prox_vuur_volume);

  ironring_init(276, 3872, ironring_siny,  00, 2, 256, 0);
  ironring_init(500, 3692, ironring_sinx,  00, 4, 256, 0);
  ironring_init(832, 3548, ironring_sinxy, 00, 2, 256, 0);

  chain_init(2102, 3128, 0, 3, 320);
  chain_init(2680, 2068, 1, 4, 400);

  helmut_init(2663,2620, 0, &helmut_caps1);
  helmut_init( 921,1421, 0, &helmut_caps2);

  ironring_init(3540, 3944, ironring_siny,  00, 4, 512, 0);
  ironring_init(3540, 3160, ironring_siny,  900, 2, 1024, 0);




  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

//  hoirise_init(222,1394);
  hoirise_init(hoi, 64, 6122, 64, 6122);
  restartx = 64;
  restarty = 6122;

  player1.worldx = 0;
  player1.worldy = 5920;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

	play_vuur();
	play_drilboor();
	play_loopband();
	real_loopband_volume();
	real_vuur_volume();
	real_drilboor_volume();
	real_loopband_volume();
	real_vuur_volume();
	real_drilboor_volume();

#endif

}


void init_level_22(void)
{

#ifndef POCKETPCDEMO
  helmut_init( 2120,1874, 1, &helmut_caps3);
  helmut_init( 3438,2212, 1, &helmut_caps4);
  helmut_init( 2488,2347, 1, &helmut_caps4);
  helmut_init(  947,1582, 1, &helmut_caps3);
  
  switsj_init(3609, 2096, 1184, 1536,1);


  basic_init(2400,3803,basic_sin,1024,2,1024,basic_sin,0,7,128,
              0,0,0,0,orgtechnobee,1,0,SEQ_DUMMY,0,0,0,0,0,prox_bee2_volume);

  basic_init(1000,4233,basic_sin, 512,2,1024,basic_sin,0,7,128,
              0,0,0,0,orgtechnobee,1,0,SEQ_DUMMY,0,0,0,0,0,prox_bee2_volume);

  warp_init(3674,1600,2291,1258,0);


  basic_init(1964, 4941, basic_pingpong,1964,2164,3,0,0,0,0,
              0,0,0,0, orgblubber,2,1,0,0,0,0,0,0,0);

  basic_init(2394, 4941, basic_pingpong,2194,2394,-3,0,0,0,0,
              0,0,0,0, orgblubber,2,1,0,0,0,0,0,0,0);


  basic_init(731,526,basic_sin,  0,6,32,basic_sin,0,3,700,
              0,0,0,0,orgtechnobee,1,0,SEQ_FLYLEFT,0,0,0,0,0,prox_bee2_volume);
  basic_init(731,526,basic_sin,512,6,32,basic_sin,0,4,700,
              0,0,0,0,orgtechnobee,1,0,SEQ_FLYLEFT,0,0,0,0,0,prox_bee2_volume);

  basic_init(1329,456,basic_sin,  0,6,32,basic_sin,0,4,700,
              0,0,0,0,orgtechnobee,1,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bee2_volume);
  basic_init(1329,456,basic_sin,512,6,32,basic_sin,0,5,700,
              0,0,0,0,orgtechnobee,1,0,SEQ_FLYRIGHT,0,0,0,0,0,prox_bee2_volume);


  ironring_init(2390, 4781, ironring_siny,  00, 5, 10, 0);
  ironring_init(2190, 4781, ironring_siny,  00, 6, 10, 0);

 
  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

  ////  hoirise_init(222,1394);



#ifdef DEMOVERSION
  hoirise_init(hoi, 578, 1674, 578, 1674);
  restartx = 578;
  restarty = 1674;
  player1.worldx = 308;
  player1.worldy = 1473;
#else
  hoirise_init(hoi, 3612, 266, 3612, 266);
  restartx = 3612;
  restarty = 266;
  player1.worldx = 3136;
  player1.worldy =   61;
#endif

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

	play_ventltor();
	real_ventltor_volume();
	real_ventltor_volume();

	play_chemo();
	real_chemo_volume();
	real_chemo_volume();

      play_chemo2();
      chemo2volume(-10000);

//	play_bee2();
//	real_bee2_volume();
//	real_bee2_volume();
#endif
}

void init_level_23(void)
{

#ifndef POCKETPCDEMO
  metcan_init( 64,17436,  12, 110,1);
  metcan_init(562,17120, -12, 90, 1);
  metcan_init(480,16896,  12, 80, 1);

  metcan_init( 32,16572,     12, 130, 1);
  metcan_init( 32,16572-48,  12, 130, 66);

  metcan_init( 32,16250,     12, 120, 1);
  metcan_init( 32,16250-48,  12, 120, 66);

  ironring_init(248, 15904, ironring_siny,  00, 6, 192, 0);
  ironring_init(64,  15784, ironring_siny,  00, 6, 192, 0);
  ironring_init(248, 15644, ironring_siny,  00, 6, 192, 0);
  ironring_init(64,  15474, ironring_siny,  00, 6, 192, 0);
  ironring_init(248, 15334, ironring_siny, 128, 6, 192, 0);
  ironring_init(64,  15224, ironring_siny,  00, 6, 192, 0);
  ironring_init(248, 15054, ironring_siny,  00, 6, 256, 0);

  warp_init( 265, 16053, 660, 16362, 0);
  warp_init( 400, 16363, 120, 16042, 0);
  warp_init( 774, 15940, 774, 14730, 0);


  ironring_init(286, 14216, ironring_flipflapx,  286,  2000, 1,  200);
  ironring_init(466, 14216, ironring_flipflapx,  466,  2000, 33, 200);
  ironring_init(646, 14216, ironring_flipflapx,  646,  2000, 65, 200);
  ironring_init(806, 14216, ironring_flipflapx,  806,  2000, 97, 200);

  ironring_init(806, 14006, ironring_flipflapx,  806,  2000, 1,  100);
  ironring_init(706, 13906, ironring_flipflapx,  706,  2000, 33, 100);
  ironring_init(606, 13806, ironring_flipflapx,  606,  2000, 65, 100);
  ironring_init(506, 13706, ironring_flipflapx,  506,  2000, 97, 100);
  ironring_init(406, 13606, ironring_flipflapx,  406,  2000, 129, 100);

  ironring_init(160, 13312, ironring_flipflapx,  160,  2000,  1,  64);
  ironring_init(240, 13312, ironring_flipflapx,  240,  2000, 10,  64);
  ironring_init(320, 13312, ironring_flipflapx,  320,  2000, 20,  64);
  ironring_init(400, 13312, ironring_flipflapx,  400,  2000, 30,  64);
  ironring_init(480, 13312, ironring_flipflapx,  480,  2000, 40,  64);
  ironring_init(560, 13312, ironring_flipflapx,  560,  2000, 50,  64);
  ironring_init(640, 13312, ironring_flipflapx,  640,  2000, 60,  64);
  ironring_init(720, 13312, ironring_flipflapx,  720,  2000, 70,  64);

  ironring_init(160, 13120, ironring_flipflapx,  160,  2000,  1,  64);
  ironring_init(240, 13120, ironring_flipflapx,  240,  2000, 10,  64);
  ironring_init(320, 13120, ironring_flipflapx,  320,  2000, 20,  64);
  ironring_init(400, 13120, ironring_flipflapx,  400,  2000, 30,  64);
  ironring_init(480, 13120, ironring_flipflapx,  480,  2000, 40,  64);
  ironring_init(560, 13120, ironring_flipflapx,  560,  2000, 50,  64);
  ironring_init(640, 13120, ironring_flipflapx,  640,  2000, 60,  64);
  ironring_init(720, 13120, ironring_flipflapx,  720,  2000, 70,  64);

  ironring_init(80, 13120, ironring_flipflapx,   80,  2000, 119,  64);
  ironring_init(0 , 13060, ironring_flipflapx,    0,  2000,  50,  64);

  basic_init(484,12834,basic_sin,0,5,612,basic_sin,0,4,16,
              0,0,0,0,orgtechnobee,1,0,SEQ_DUMMY,0,0,0,0,0,prox_bee2_volume);

  basic_init(504,12568,basic_sin,0,8,270,basic_sin,512,4,16,
              0,0,0,0,orgtechnobee,1,0,SEQ_DUMMY,0,0,0,0,0,prox_bee2_volume);

  basic_init(504,12612,basic_sin,0,6,270,basic_sin,0,4,16,
              0,0,0,0,orgtechnobee,1,0,SEQ_DUMMY,0,0,0,0,0,prox_bee2_volume);



  metcan_init(  848,10368, -12, 180, 1);
  ironring_init(280, 10400, ironring_sinx,  00, 2, 192, 0);
  ironring_init(600, 10400, ironring_sinx, 512, 2, 192, 0);

  ironring_init(770, 10180, ironring_siny,  00, 4, 292, 0);

  ironring_init(390,  9973, ironring_sinx,  00, 1, 584, 0);
  ironring_init(390,  9973, ironring_sinx,  44, 1, 584, 0);
  metcan_init(   32+160,9888, 12, 180, 1);

  metcan_init(  848,9568,   -10, 150, 1);
  metcan_init(   32,9568-48, 10, 150, 1);
  metcan_init(  432,9248,   -12,  50, 1);

  metcan_init(  688,9248,   -12,  60, 1);

  metcan_init(  688,8960,   -12,  90, 1);
  metcan_init(  688,8992,   -12,  80, 1);


  weight_init(  814,10720-160, 0,2);

  
  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

//  hoirise_init(222,1394);
  hoirise_init(hoi, 820, 19018, 820, 19018);
  restartx = 820;
  restarty = 19018;

  player1.worldx = 256;
  player1.worldy =  18656;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

  play_bee2();
  real_bee2_volume();
  real_bee2_volume();
#endif
}


UINT16 ironautotab1[] = {
      0xff00,150,
      0x00ff,208,
      0x0100,150,
      0x0001,208,
      0,0
};

UINT16 ironautotab2[] = {
      0x00ff,100,
      0x0100,256,
      0x00ff,200,
      0xfe00,128,
      0x0003,100,
      0,0
};

UINT16 ironautotab3[] = {
      0xff00,192,
      0x0004,50,
      0xff00,100,
      0xfe00,100,
      0xff00,50,
      0x0000,100,
      0x00ff,200,
      0x0200,271,
      0,0
};

UINT16 ironautotab4[] = {
      0x0200,50,
      0x00ff,75,
      0x0300,50,
      0x00ff,75,
      0xfc00,50,
      0x0000,100,
      0x0400,50,
      0x00ff,75,
      0xfd00,50,
      0x0000,50,
      0xfe00,50,
      0x0003,75,
      0,0
};

UINT16 ironautotab5[] = {
      0x00ff,1600,
      0x0002,800,
      0,0
};

UINT16 ironautotab6[] = {
      0x0400,200,
      0x0100,200,
      0x0000,200,
      0xfc00,10,
      0x0000,100,
      0x0200,600,
      0,0
};

UINT16 ironautotab7[] = {
      0x0002,100,
      0x0202,40,
      0x0300,10,
      0x0200,10,
      0x0100,420,
      0x02fe,136,
      0x0200,108,
      0x0002,220,
      0,0
};

UINT16 ironautotab8[] = {
      0x00fe,168,
      0xfe00,400,
      0,0
};

UINT16 ironautotab9[] = {
      0x0008,32,
      0x0000,100,
      0x00f8,32,
      0x0000,100,
      0,0
};

UINT16 ironautotab10[] = {
      0x00f8,96,
      0x0000,100,
      0x0010,48,
      0x0000,100,
      0,0
};

UINT16 ironautotab11[] = {
      0x0008,20,
      0x0000,100,
      0x00f0,20,
      0x0000,100,
      0x0008,20,
      0x0000,100,
      0,0
};

UINT16 ironautotab12[] = {
      0x00f0,16,
      0x0010,16,
      0x0000,100,
      0,0
};

UINT16 ironautotab13[] = {
      0x00f0,44,
      0x0000,50,
      0x0010,28,
      0x0000,30000,
      0x0010,16,
      0x0000,100,
      0,0
};

UINT16 ironautotab14[] = {
      0x00f0,10,
      0x0000,100,
      0x00f0,24,
      0x0000,30000,
      0x0010,34,
      0,0
};

UINT16 ironautotab15[] = {
      0x0008,20,
      0x0000,100,
      0x0100,192,
      0x0000,100,
      0x00f8,20,
      0x0000,100,
      0x0008,20,
      0x0000,100,
      0xff00,192,
      0x0000,100,
      0x00f8,20,
      0x0000,100,
      0,0
};

UINT16 ironautotab16[] = {
      0x00f8,76,
      0x0000,100,
      0x0008,76,
      0x0000,100,
      0,0
};

UINT16 ironautotab17[] = {
      0x00f8,24,
      0x0000,100,
      0x0008,24,
      0x0000,100,
      0,0
};

UINT16 ironautotab18[] = {
      0x00f8,36,
      0x0000,30000,
      0x0008,36,
      0x0000,100,
      0,0
};

void init_level_24(void)
{
  UINT16 i;
  OBJECT **gooddaypnt;
  OBJECT *lift;

#ifndef POCKETPCDEMO
  ironring_init(3080, 6820, ironring_sinx,  00, 1, 1080, 0);
  ironring_init(3080, 6820, ironring_sinx,  8, 1, 1080, 0);
  ironauto_init(2356, 6676, ironautotab1, 0, 0,0);
  ironauto_init(2496, 6369, ironautotab2, 1, 0,0);
  ironauto_init(2204, 5951, ironautotab3, 1, 0,0);
  ironauto_init(740 , 5947, ironautotab4, 0, 0,0);

  gooddaypnt = gooddaytab;
  
  for (i=0; i<25; i++)
  {
    *(gooddaypnt++) = goodday_init(162,5469 - (i*60));
    *(gooddaypnt++) = goodday_init(362,5469 - (i*60));
  }
  
  lift = ironauto_init(280+80, 5600, ironautotab5, 2, 0, 0);
  ironauto_init(120+80, 5600, ironautotab5, 1, lift,0);
//  ironauto_init(440, 5600, ironautotab5, 0);

  ironauto_init(64 , 3906, ironautotab6, 1, 0,0);

  paal_init(1472, 3908+32, 0, 0, 128, 6);
  paal_init(2240, 4160, 1, 0, 128, 6);
//  paal_init(2272, 4160, 1, 128, 128, 6);
  
  ironauto_init(1760 , 4120, ironautotab7, 1, 0,0);

  paal_init(3232+(9*32), 3936, 1, 0, 128, 12);
  paal_init(3232+(8*32), 3936, 1, 64, 128, 12);
  paal_init(3232+(7*32), 3936, 1, 128, 128, 12);
  paal_init(3232+(6*32), 3936, 1, 192, 128, 12);
  paal_init(3232+(5*32), 3936, 1, 256, 128, 12);
  paal_init(3232+(4*32), 3936, 1, 320, 128, 12);
  paal_init(3232+(3*32), 3936, 1, 384, 128, 12);
  paal_init(3232+(2*32), 3936, 1, 448, 128, 12);
  paal_init(3232+(1*32), 3936, 1, 512, 128, 12);
  paal_init(3232, 3936, 1, 576, 128, 6);

  paal_init(3712, 4160, 0, 0, 128, 10);
  paal_init(3776, 4160, 0, 0, 128, 8);
  paal_init(3840, 4160, 0, 0, 128, 6);
  paal_init(3904, 4160, 0, 0, 128, 4);

//  elevat_init(4300, 4214, elevat_sinx,  00, 2, 640);

  ironring_init(4300, 4214, ironring_sinx,  00, 2, 640, 0);
  ironring_init(4300, 4214, ironring_sinx,  30, 2, 640, 0);


  elevat_init(4847, 4140, elevat_sinxy,  100, 4, 240);
  elevat_init(5168, 3950, elevat_sinxy,  270, -4, 240);
  elevat_init(5168+448, 3950, elevat_sinxy,  270, 2, 360);
  elevat_init(5168+448, 3950-416, elevat_sinxy,  400, 4, 360);

  ironauto_init(5168+448-32 , 3264, ironautotab8, 1, 0,0);
  ironauto_init(5168+448-32 , 3264, ironautotab8, 1, 0,0);

  elevat_init(5168+448, 3950-416-352-16-300, elevat_sinxy,  400,8, 480);

  paal_init(6880, 2880, 0, 0, 128, 8);
  paal_init(6880, 2880-384, 1, 0, 128, 8);

  paal_init(7328, 2816, 0, 0, 128, 12);
  paal_init(7328, 2848-352, 1, 0, 128, 12);

//  paal_init(7776, 2816-64, 0, 512, 96, 16);
//  paal_init(7776, 2880-320-64, 1, 0, 96, 16);


  ironauto_init(3232 , 5920, ironautotab9,  1, 0,1);
  ironauto_init(3424 , 5920, ironautotab10, 1, 0,1);
  ironauto_init(3424 , 6176, ironautotab11, 1, 0,1);

  ironauto_init(3424 , 6592, ironautotab12, 1, 0,1);
  ironauto_init(3616 , 6016, ironautotab13, 1, 0,1);
  ironauto_init(4192 , 6336, ironautotab14, 1, 0,1);

  ironauto_init(4384 , 6592, ironautotab15, 1, 0,1);
  ironauto_init(4000 , 6944, ironautotab16, 1, 0,1);
  
  ironauto_init(4960 , 6592,     ironautotab17, 0, 0,0);
  ironauto_init(4960 , 6592-192, ironautotab17, 0, 0,0);
  ironauto_init(4960 , 6592-384, ironautotab17, 0, 0,0);
  ironauto_init(4960 , 6592-576, ironautotab17, 0, 0,0);

  ironauto_init(4960 , 6880, ironautotab18, 1, 0,1);
  ironauto_init(5344 , 6400, ironautotab17, 1, 0,1);


  switsj_init(/* 1380 */ 4610, 5728-48, 4736, 6016-128, 1);
  switsj_init(/* 1380 */ 3657, 5152-48, 3776, 5184, 1);
  switsj_init(/* 1380 */ 3867, 5312-48, 1152, 5888, 1);


  gat1 = 0;  // gatletjes voor mr bombs
  gat2 = 0;
  gat3 = 0;
  gat4 = 0;
  gat5 = 0;
  gat6 = 0;
  gat7 = 0;

  switch1 = switsj_init(8583, 2880-48, 0, 0, 1);
  switch2 = switsj_init(9077, 2880-48-128, 0, 0, 1);
  switch3 = switsj_init(8283, 2880-48-256, 0, 0, 1);
  switch4 = switsj_init(9043, 2880-48-384, 0, 0, 1);
  
  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

//  hoirise_init(222,1394);
  hoirise_init(hoi, 3089, 6634, 3089, 6634);
  restartx = 3089;
  restarty = 6634;

  player1.worldx = 2817;
  player1.worldy = 6437;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

	play_slowlift();
	real_slowlift_volume();
	real_slowlift_volume();

#endif

}


void wavegoodday(void)
{
  static UINT16 sin1cnt;
  static UINT16 sin2cnt;
  UINT16 sin1bcnt;
  UINT16 sin2bcnt;
  UINT16 i;
  UINT16 x;
  OBJECT **gooddaypnt;

  sin1cnt += 6;
  sin1cnt &= 1023;
  sin2cnt += 4;
  sin2cnt &= 1023;

  sin1bcnt = sin1cnt;
  sin2bcnt = sin2cnt;

  gooddaypnt = gooddaytab;
  
  for (i=0; i<25; i++)
  {
    sin1bcnt += 21;
    sin1bcnt &= 1023;
    sin2bcnt += 25;
    sin2bcnt &= 1023;

    x = 200 + ((sinus512[sin1bcnt]* 160)>>10) + ((sinus512[sin2bcnt]* 160)>>10);
    gooddaypnt[i*2]->x = x;
    gooddaypnt[i*2+1]->x = x+300;
  }
}


SNAKETAB snake1 = 
{
    32,
    0,   1,    32,
    256, 3,    32,
    0,   5,    64,
    256, 7,    64,
};


SNAKETAB snake2 = 
{
    32,
    0,   3,    16,
    512, 3,    32,
    0,   10,   32,
    256, 9,    16,
};

SNAKETAB snake3 = 
{
    29,
    0,   -1,    32,
    256, -1,    32,
    0,   -1,    32,
    256, -1,    32,
};


SNAKETAB snakelev4 = 
{
    64,
    0,   30,    7,
    256, 34,    8,
    0,   34,    7,
    256, 30,    8,
};

SNAKETAB snakelev4b = 
{
    16,
    0,   16,    64,
    256, 6,   512,
    0,   0,     0,
    256, 0,     0,
};

SNAKETAB snakelev4c = 
{
    29,
    0+100,   2,    32,
    256+100, 2,    32,
    0,   1,    32,
    100, 1,    32,
};

SNAKETAB snakelev4d = 
{
    32,
    0  , -16,    32,
    256, -16,    32,
    0  , -14,    32,
    256, -14,    32,
};

SNAKETAB snakelev4e = 
{
    16,
    0  , -3,    32,
    256, -3,    32,
    0  , -3,    32,
    256, -3,    32,
};

SNAKETAB snakelev4f = 
{
    32,
    0, 4, 4,
    0, 4, 4,
    0, 31, 31,
    0, 30, 31,
};

void close_hole(UINT16 x, UINT16 y, UINT16 pat, UINT16 amount, UINT16 vertflg)
{
  UINT16 j;
  
  amount *= 2;
  
  for (j=0; j<amount; j++)
    {
      player1.loadedmap->map[((y/32)*player1.loadedmap->mapsizex/32)+(x/32)]  = pat;
      colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = (pat==0) ? 0 : 1;
      
      if (vertflg)
  {
    y += 16;
  }
      else
  {
    x += 16;
  }
    }
}


SOKOBAN_POSSES pos1_1 = {
  0,
  3,
  {  0x920,  0x940,  0x860, 0x860, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  { 0x1180, 0x1180, 0x1040, 0x1020, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES pos1_2 = {
  1,
  3,
  {  0x800, 0x800,  0x800,  0x980,  0x980,  0x980,  0x920,  0x940, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  { 0x1080,0x10a0, 0x10c0, 0x1080, 0x10a0, 0x10c0, 0x1140, 0x1140, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES pos2_1 = {
  2,
  3,
  { 0x800, 0x800, 0x800, 0x8c0, 0x8c0, 0x8c0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  { 0xea0, 0xec0, 0xee0, 0xea0, 0xec0, 0xee0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};


void init_level_31(void)
{
  UINT16 i;

// initialise 6 sea-mines drifting on the chains

  paramine1a = goodday_init(300, 300);
  paramine1b = goodday_init(300, 300);
  paramine2a = goodday_init(300, 300);
  paramine2b = goodday_init(300, 300);
  paramine3a = goodday_init(300, 300);
  paramine3b = goodday_init(300, 300);

  soko1 = sokoban_init(&pos1_1, &pos1_2, 0, 0);
  soko2 = sokoban_init(&pos2_1, 0, 0, 0);

  diamond_init(2098, 4128);
  diamond_init(2448, 4368);


  cannon_init( 1908, 3419,  83, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0);
  cannon_init( 2360, 3419,  83, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0);

  warp_init(2167, 3650, 1960, 4132,1);


// close hole which opens after snake 1 is blown to bits */


  close_hole(  64,  800, 66, 7, 0);
  close_hole(2688, 2144, 66, 4, 0);
  close_hole(2688, 2688, 66, 4, 0);
  close_hole(1968,  160, 88, 4, 1);

// ***

  rocket_init(1606,     416, 8, -60);
  rocket_init(1606+128, 416, 8, -40);
  rocket_init(1606+256, 416, 8, -20);

  snake_init (900,   400, orgsegment, 1, 192, 1, 1, snake1_ready, &snake1);

  snake_init (3304, 1908, orgsegment, 0, 192, 1, 1, snake2_ready, &snake2);

  snake_init (1904, 1554, orgsegment, 2, 300, 0, 0, 0, &snake3);



  warp_init(2504, 1608, 1800, 1447, 0);
  diamond_init(2000, 1647);
  warp_init(1897, 1544, 2480, 1668, 0);



  cannon_init( 1440, 1090,  83, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,0, 0);


  cannon_init( 52, 1200,  83, 70, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,0, 0);
  cannon_init(378,  955, 100, 70, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,0, 0);

  //flamethrowers

  cannon_init( 52, 1861,   3, 40, 0, orgplof, 0,  41, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, prox_vlamwerp_volume, 0);
  cannon_init(474, 1861,   3, 40, 0, orgplof, 0,  41, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, prox_vlamwerp_volume, 0);


  cannon_init(820, 2408,   3, 40, 0, orgplof, 0,  41, 0, 1, 0,
        basic_sin,256,8,32,basic_sin,0,8,32,
        0,0,0,0,  100, 12, 24, flame_hit, prox_vlamwerp_volume, 0);


  // moving cannon

  cannon_init(1896,  2228, 25, 50, 0, orgmine, 0, 250, 1, 0, 1,
        basic_sin,256,8,80,0,0,0,0,
        0,0,0,0, 0, 0, 6, 0, 0, 0);

  cannon_init(1896,  2586, 25, 50, 0, orgmine, 0, 250, 1, 0, 1,
        basic_sin,256,8,80,0,0,0,0,
        0,0,0,0, 0, 0, 6, 0, 0, 0);

  // 4 cannons in splitsing

  cannon_init(2548,  2228, 50, 90, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0);

  cannon_init(2548+359,  2228, 46, 90, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0);

  cannon_init(2548,  2228+359, 42, 90, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0);

  cannon_init(2548+359,  2228+359, 38, 90, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0);


  bolt_init(3247, 2336, 0, 0, 8, 0, 1,  130, 50);
  bolt_init(3375, 2336, 0, 0, 8, 0, 44, 130, 50);
  bolt_init(3503, 2336, 0, 0, 8, 0, 87, 130, 50);


  bolt_init(2656, 1633, 1, 0, 8, 40, 96,  128, 64);
  bolt_init(2656, 1353, 1, 0, -8, 40, 32,  128, 64);

  diamond_init(3651, 2424);


  basic_init(3168, 4040, basic_sin, 512, 12, 64, basic_sin, 0, 3, 500,
              0, 0, 0, 0, orgsegment, 1, 0, 2, 1, 0,0,0,0,0);

  diamond_init(2952, 4174);

//  hoi_fly_init(hoi, 222,1394);
  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);
  hoirise_init(hoi, 3612, 340, 3612, 340);

  restartx = 3612;
  restarty = 340;
  restartcnt = 0;

  player1.worldx = 3136;
  player1.worldy =   148;

  for (i=0; i<50; i++)
  {
    hoi_cam(&player1);
    scrolling(&player1);
  }

  shootenabledflg = 0;

    play_vlamwerp();
	play_stroom();
	play_backpak();
	real_vlamwerp_volume();
	real_vlamwerp_volume();
	real_stroom_volume();
	real_stroom_volume();

}


SOKOBAN_POSSES sok1_1 = {
  0,
  3,
  {  0x120,  0x120,  0x220, 0x220, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  {  0x180,  0x280,  0x280, 0x180, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES sok1_2 = {
  1,
  3,
  {  0x320, 0x340,  0x360,  0x380, 0x3c0,  0x00,  0x00,  0x00, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  {  0x320, 0x320,  0x320,  0x320, 0x2a0,  0x00,  0x00,  0x00, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES sok2_1 = {
  2,
  3,
  { 0x160, 0x160, 0x2a0, 0x2a0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  { 0x3a0, 0x3c0, 0x3a0, 0x3c0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES sok2_2 = {
  3,
  3,
  { 0x240, 0x240, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 },
  { 0x3a0, 0x3c0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }
};


SOKOBAN_POSSES sok3_1 = {
  0,
  2,
  { 0x800, 0x820, 0x840, 0x800, 0x820, 0x840, 0x800, 0x820, 0x840, 0x800, 0x820, 0x840, 0x000, 0x000, 0x000, 0x000 },
  { 0x200, 0x200, 0x200, 0x280, 0x280, 0x280, 0x300, 0x300, 0x300, 0x380, 0x380, 0x380, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES sok3_2 = {
  1,
  2,
  { 0x880, 0x8a0, 0x8c0, 0x880, 0x8a0, 0x8c0, 0x880, 0x8a0, 0x8c0, 0x880, 0x8a0, 0x8c0, 0x000, 0x000, 0x000, 0x000 },
  { 0x200, 0x200, 0x200, 0x280, 0x280, 0x280, 0x300, 0x300, 0x300, 0x380, 0x380, 0x380, 0x000, 0x000, 0x000, 0x000 }
};

SOKOBAN_POSSES sok3_3 = {
  2,
  1,
  { 0x760, 0x760, 0x760, 0x7e0, 0x7e0, 0x7e0, 0x860, 0x860, 0x860, 0x8e0, 0x8e0, 0x8e0, 0x000, 0x000, 0x000, 0x000 },
  { 0x220, 0x240, 0x260, 0x220, 0x240, 0x260, 0x220, 0x240, 0x260, 0x220, 0x240, 0x260, 0x000, 0x000, 0x000, 0x000 }
};


SOKOBAN_POSSES sok3_4 = {
  3,
  1,
  { 0x7e0, 0x7e0, 0x7e0, 0x860, 0x860, 0x860, 0x8e0, 0x8e0, 0x8e0, 0x960, 0x960, 0x960, 0x000, 0x000, 0x000, 0x000 },
  { 0x2a0, 0x2c0, 0x2e0, 0x2a0, 0x2c0, 0x2e0, 0x2a0, 0x2c0, 0x2e0, 0x2a0, 0x2c0, 0x2e0, 0x000, 0x000, 0x000, 0x000 }
};

void init_level_32(void)
{
  UINT16 i;

// puzzle 1
#ifndef POCKETPCDEMO
  
  soko1 = sokoban_init(&sok1_1, &sok1_2, 0, 0);
  warp_init(96, 224, 96, 384, 0);
  diamond_init(848, 856);
  warp_init(840, 920, 128, 128, 0);

// puzzle 2

  soko2 = sokoban_init(&sok2_1, &sok2_2, 0, 0);
  diamond_init(120, 1227);
  warp_init(969,  874, 145, 1000, 0);
  warp_init(237, 1255, 1938, 443, 0);

// puzzle 3

  soko3 = sokoban_init(&sok3_1, &sok3_2, &sok3_3, &sok3_4);
  diamond_init(1888, 704);
  diamond_init(1888, 832);
  diamond_init(2080, 390);
  diamond_init(2528, 704);

// puzzle 4

  diamond_init(1353+16, 1471+16);
  warp_init(1257, 1387, 1257, 1200, 0);

  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);
  hoirise_init(hoi, 136, 133, 136, 133);

  shootenabledflg = 0;

  restartx = 136;
  restarty = 133;
  restartcnt = 0;

  player1.worldx = 32;
  player1.worldy =   32;

  for (i=0; i<50; i++)
  {
    hoi_cam(&player1);
    scrolling(&player1);
  }

	play_backpak();

#endif
}


void init_level_33(void)
{
  UINT16 i;

  autoscrollspd = 1;

#ifndef POCKETPCDEMO
  rocket_init(1192, 864, 8, -20);
  rocket_init(1192+128, 864, 8, -20);
  rocket_init(1192+256, 864, 8, -20);
  rocket_init(1192+384, 864, 8, -20);

  cannon_init(1652, 571, 50, 90, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0);

  cannon_init(2041, 827, 50, 90, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0);

  cannon_init(2491-32-199, 1012+32, 50, 180, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0);
  cannon_init(2491-32-199, 1012-96, 50, 180, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0);
  cannon_init(2491-32-199, 1012-192, 50, 180, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0);
  cannon_init(2491-32-199, 1012-352, 50, 180, 0, orgmine, 0, 250, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0);


  cannon_init(3582, 1307, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  cannon_init(3582+64, 1307, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  cannon_init(3582+128, 1307, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  cannon_init(3582+192, 1307, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);


  cannon_init(3582, 1319-114, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  cannon_init(3582+64, 1319-114, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  cannon_init(3582+128, 1319-114, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  cannon_init(3582+192, 1319-114, 50, 300, 0, orgmine, 0, 50, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

  

  
  cannon_init(5985, 1248,   2, 30, 0, orgmine, 0,  22, 1, 1, 0,
        basic_sin,256,8,32,basic_sin,0,8,32,
        0,0,0,0,  100, 12, 34, mine_hit, 0, 0);
  
  cannon_init(7837-16, 1283-16,   2, 30, 0, orgmine, 0,  22, 1, 1, 0,
        basic_sin,512,6,160,basic_sin,256,6,160,
        0,0,0,0,  100, -6, 14, 0, 0, 0);

  
  bolt_init(6672, 1184, 0,   0, 8, 72, 32,  128, 64);
  bolt_init(6672, 1184, 0, 512, 8, 72, 96,  128, 64);
  

  bolt_init(7181, 1184, 0,   0, 8, 0, 32,  96, 40);
  bolt_init(7381, 1184, 0, 512, 8, 0, 64,  96, 40);
  

  bbot_init(15008, 932, 0, 9, 64, 0, 14900, 15300, 1);

  bbot_init(15730, 932, 512, 12, 64, 0, 15530, 15930, 1);
//  bbot_init(15730, 932, 0, 12, 64, 0, 15530, 15930, -1);   // was VEEEEL te moeilijk...(stelletje sissies allemaal)
  
  bbot_init(16410, 932, 0, 32, 128, 0, 16160, 16660, -2);


  diamond_init( 6434, 1120);
  diamond_init( 6434, 1420);
  diamond_init( 7822,  941);
  diamond_init(10454,  576);

  totalshootcnt = 0;
  totalgenercnt = 0;
  diamcnt = 0;          // total generated diamonds untill now

  shootenabledflg = 1;

  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);
  hoirise_init(hoi, 145, 636, 145, 636);

  restartx = 145;
  restarty = 636;
  restartcnt = 0;

  player1.worldx = 32;
  player1.worldy = 448;

  for (i=0; i<50; i++)
  {
    hoi_cam(&player1);
    scrolling(&player1);
  }

	play_backpak();

	play_stroom();
	real_stroom_volume();
	real_stroom_volume();

#endif
}


void flying_objects(void)
{
	static UINT16 deltel;

	deltel++;
	if (deltel > 15) deltel = 0;

	if (deltel == 0 && (hoi->frame))
	{
	    element_init(player1.worldx+680, 520+MC_RANDOM(280), -1, 0, element_sin, 512, 12, 64, element_sin, 0, 3, 20,
                     0, 0, 0, 0, orgsegment, 1, 0, 0, 1, 1,0,0,0,0);
	}
}


void flying_objects2(void)
{
	static UINT16 deltel;
    static UINT16 ytel = 256;

	deltel++;
	deltel &= 7;

	ytel+=5;
	ytel &= 1023;

	if (deltel == 0 && (hoi->frame))
	{
	    element_init(player1.worldx+680,1270, -1, 0, element_sin, 512, 8, 200, element_sin, ytel, 9, 340,
                     0, 0, 0, 0, orgsegment, 1, 0, 0, 1, 1,0,0,0,0);
	}
}


void falling_objects1(void)
{
	static UINT16 deltel;

	deltel++;
	if (deltel>28) deltel = 0;

	if (deltel == 0 && (hoi->frame))
	{
	    element_init(4767, 697, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, orgseamine, 1, 0, 0, 1, 1,0,0,0,0);
	}
}


void falling_objects2(void)
{
	static UINT16 deltel;

	deltel++;
	if (deltel>70) deltel = 0;

	if (deltel == 0 && (hoi->frame))
	{
	    element_init(5440, 697, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, orgseamine, 1, 0, 0, 1, 1,0,0,0,0);
	}
}

void floating_objects(void)
{
	static UINT16 deltel;

	deltel++;
	if (deltel > 47) deltel = 0;
	if (deltel == 0 && (hoi->frame))
	{
	    element_init(player1.worldx+680, 1040+MC_RANDOM(400), 0, 0, 0, 0, 0, 0, element_sin, 0, 9, 128,
                     0, 0, 0, 0, orgsegment, 1, 0, 2, 1, 0,0,0,0,0);
	}
}


void fast_objects1(void)
{
	static UINT16 deltel;
    INT16 tijd,speed;
	deltel++;
	if (deltel>50) deltel = 0;

    tijd = MC_RANDOM(2);
	tijd *= 80;

	speed = ((player1.worldx - 12118)/300)+4;

	if (deltel == 0 && (hoi->frame))
	{
	    element_init(player1.worldx+680, 719+tijd, -speed, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, orgsegment, 1, 0, 2, 1, 0,0,0,0,0);
        play_morphsht(hoi->x+100,719+tijd);
	}
}


void circle_objects1(void)
{
	static UINT16 deltel;
    static UINT16 i;

	deltel = 0;

	for (i=0; i<64; i++)
	{
	  element_init(player1.worldx+880, 771, 0, 0, element_sin, deltel, 3, 480, element_sin, (deltel+256)&1023, 3, 480,
                   0, 0, 0, 0, orgsegment, 1, 0, 1, 1, 1,0,0,0,0);
      deltel += 16;
	}
}


void init_level_34(void)
{
  UINT16 i;

#ifndef POCKETPCDEMO
  gen_doolhof();

  diamond_init(  710, 4410);
  diamond_init( 2238, 2238);
  diamond_init( 4410,  710);

  for (i=0; i<16; i++)
  {
    UINT16 x,y;

    do
    {
	  x = MC_RANDOM(DOLWIDTH);
	  y = MC_RANDOM(DOLHEIGHT);


    } while ((doolhof[y+1][x+1] & 1) == 1);

    dooldoler_init((x*128)+32+(DOOLX*32), (y*128)+64+(DOOLY*32), 1);
  }
  

  
  cannon_init( 3816, 4680,  63, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);

  cannon_init( 2837   , 4502,  63, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);

  cannon_init( 2837-64, 4502,  63, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);


  cannon_init(    96, 3971,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(    96, 3971-40,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(    96, 3971-80,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(    96, 3971-120,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(   608, 3971,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(   608, 3971-40,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(   608, 3971-80,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);
  cannon_init(   608, 3971-120,   8, 70, 0, orgmine, 0, 250, 1, 0, 1,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 1);

  totalshootcnt = 0;
  totalgenercnt = 0;
  
  asteroid_init(2200, 300,  800, 96,  2431, 639, 512,-256, 16, 0);

  diamond_init( 1606, 5132);

  diamond_init(  128, 1859);
  diamond_init(  576, 1859);

  warp_init(344, 690, 2200, 300, 1);

  claw_init( 288, 5408, -11, 288, 1664, 1, 3);
  claw_init( 288, 5824, -11, 288, 1664, 0, 3);

  claw_init(  288, 5408, -11, 288, 1664, 1,  4);
  claw_init( 1664, 5824, -11, 288, 1664, 0, -4);

  diamond_init( 1813, 5571);

  shootenabledflg = 0;

  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);
  hoirise_init(hoi, 117, 113, 117, 113);

  restartx = 117;
  restarty = 113;
  restartcnt = 0;

  player1.worldx = 32;
  player1.worldy = 32;

  for (i=0; i<50; i++)
  {
    hoi_cam(&player1);
    scrolling(&player1);
  }

  play_backpak();
  play_stroom();
  play_heatsnkr();
  real_stroom_volume();
  real_stroom_volume();
  real_heatsnkr_volume();
  real_heatsnkr_volume();
#endif
}


// bit 0 = left // bit 1 = right // bit 2 = up // bit 3 = down

void gen_doolhof(void)
{
  UINT16 i, x, y;

// clear doolhof

  memset(doolhof, 0, (DOLHEIGHT+2)*(DOLWIDTH+2));
  
// prepare borders around doolhof

  for (i=0; i<DOLWIDTH+2; i++)
  {
    doolhof[0]          [i] = 0x0f;
    doolhof[DOLHEIGHT+1][i] = 0x0f;
  }

  for (i=0; i<DOLHEIGHT+2; i++)
  {
    doolhof[i][0]          = 0x0f;
    doolhof[i][DOLWIDTH+1] = 0x0f;
  }

  doolstackcnt = 0; //initialize

  doolhofstack[doolstackcnt]  = DOLWIDTH;
  doolhofstack[doolstackcnt+1]= 1;
  doolstackcnt += 2;

  randomcnt = 0;
  
  while (doolstackcnt)
  {
    do_doolhof();  // <--- semi-recursive shit
  }

// Erase old labyrinth

  for (y=0; y<(DOLHEIGHT)*4; y++)
  {
    for (x=0; x<(DOLHEIGHT)*4; x++)
    {
      ZETBLOKJE((y), (x), 0, 0);
	}
  }
  
// now visualise the labyrinth

  for (y=0; y<DOLHEIGHT+1; y++)
  {
    for (x=0; x<DOLWIDTH+1; x++)
    {
  	  UINT16 hoek;

      if(!(doolhof[y+1][x+1] & 1))   // linker poortje
	  {
        ZETBLOKJE((y*4), (x*4), doolpats[12], 1);
        ZETBLOKJE((y*4)+1, (x*4), doolpats[12], 1);
        ZETBLOKJE((y*4)+2, (x*4), doolpats[12], 1);
        ZETBLOKJE((y*4)+3, (x*4), doolpats[12], 1);
	  }
      if(!(doolhof[y+1][x+1] & 2))   // rechter poortje
	  {
        ZETBLOKJE((y*4), (x*4)+4, doolpats[12], 1);
        ZETBLOKJE((y*4)+1, (x*4)+4, doolpats[12], 1);
        ZETBLOKJE((y*4)+2, (x*4)+4, doolpats[12], 1);
        ZETBLOKJE((y*4)+3, (x*4)+4, doolpats[12], 1);
	  }
      if(!(doolhof[y+1][x+1] & 4))   // boven poortje
	  {
        ZETBLOKJE((y*4), (x*4), doolpats[3], 1);
        ZETBLOKJE((y*4), (x*4)+1, doolpats[3], 1);
        ZETBLOKJE((y*4), (x*4)+2, doolpats[3], 1);
        ZETBLOKJE((y*4), (x*4)+3, doolpats[3], 1);
	  }
      if(!(doolhof[y+1][x+1] & 8))   // onder poortje
	  {
        ZETBLOKJE((y*4)+4, (x*4),   doolpats[3], 1);
        ZETBLOKJE((y*4)+4, (x*4)+1, doolpats[3], 1);
        ZETBLOKJE((y*4)+4, (x*4)+2, doolpats[3], 1);
        ZETBLOKJE((y*4)+4, (x*4)+3, doolpats[3], 1);
	  }

      hoek = 0;

      if((KIJKBLOKJE((y*4),(x*4)-1) == 0) || x == 0)   // linker poortje
	  {
        hoek |= 1;
	  }
      if((KIJKBLOKJE((y*4),(x*4)+1) == 0) || x == (DOLWIDTH))  // linker poortje
	  {
        hoek |= 2;
	  }
      if((KIJKBLOKJE((y*4)-1,(x*4)) == 0) || y == 0)   // linker poortje
	  {
        hoek |= 4;
	  }
      if((KIJKBLOKJE((y*4)+1,(x*4)) == 0) || y == (DOLHEIGHT))  // linker poortje
	  {
        hoek |= 8;
	  }
      hoek ^= 0x0f;
	  hoek &= 15;
      if (hoek != 0)
	  {
		  ZETBLOKJE((y*4), (x*4), doolpats[hoek], 1);
	  }
	}
  }

// openingen maken

  ZETBLOKJE(0, 0, doolpats[15], 1);
  ZETBLOKJE(0, 1, doolpats[0], 0);
  ZETBLOKJE(0, 2, doolpats[0], 0);
  ZETBLOKJE(0, 3, doolpats[0], 0);
  ZETBLOKJE(0, 4, doolpats[15], 1);

  ZETBLOKJE((DOLWIDTH*4),   (DOLHEIGHT*4)-4, doolpats[15], 1);
  ZETBLOKJE((DOLWIDTH*4), 1+(DOLHEIGHT*4)-4, doolpats[0], 0);
  ZETBLOKJE((DOLWIDTH*4), 2+(DOLHEIGHT*4)-4, doolpats[0], 0);
  ZETBLOKJE((DOLWIDTH*4), 3+(DOLHEIGHT*4)-4, doolpats[0], 0);
  ZETBLOKJE((DOLWIDTH*4), 4+(DOLHEIGHT*4)-4, doolpats[15], 1);

}




void do_doolhof(void)
{
    UINT16 x,y,ox,oy;
	UINT16 dir;
    UINT16 muur,ran;
    UINT16 temp;

	doolstackcnt -= 2;

// randomize stack a little bit

	randomcnt += 1;
	randomcnt &= 7;


	if (doolstackcnt != 0 && randomcnt == 0)
	{
      ran = MC_RANDOM(doolstackcnt);
	  ran &= 0xfffe;

      temp = doolhofstack[doolstackcnt];
      doolhofstack[doolstackcnt] = doolhofstack[ran];
      doolhofstack[ran] = temp;

      temp = doolhofstack[doolstackcnt+1];
      doolhofstack[doolstackcnt+1] = doolhofstack[ran+1];
      doolhofstack[ran+1] = temp;
    }
	

    x = doolhofstack[doolstackcnt  ];
    y = doolhofstack[doolstackcnt+1];

	ox = x;
	oy = y;

    if (doolhof[y][x-1] && doolhof[y][x+1] && doolhof[y-1][x] && doolhof[y+1][x]) // dead end road
	{
	  return;
	}

    muur = 1;

    while(muur)        // muurtje weggehaald
	{
      dir = MC_RANDOM(4);

	  switch(dir)
	  {
	  case 0:
		if (doolhof[y][x-1] == 0)
		{
			doolhof[y][x]   |= 1;
			doolhof[y][x-1] |= 2;
			muur = 0;
            x--;
		}
		break;
	  case 1:
		if (doolhof[y][x+1] == 0)
		{
			doolhof[y][x]   |= 2;
			doolhof[y][x+1] |= 1;
			muur = 0;
            x++;
		}
		break;
	  case 2:
		if (doolhof[y-1][x] == 0)
		{
			doolhof[y][x]   |= 4;
			doolhof[y-1][x] |= 8;
			muur = 0;
            y--;
		}
		break;
	  case 3:
		if (doolhof[y+1][x] == 0)
		{
			doolhof[y][x]   |= 8;
			doolhof[y+1][x] |= 4;
			muur = 0;
            y++;
		}
		break;
      default:
//		log_out("we should have NO default!");
		break;
	  }
	}

// en de andere richtingen evt in de stack

    if (doolhof[y][x-1] || doolhof[y][x+1] || doolhof[y-1][x] || doolhof[y+1][x]) // dead end road
	{
  	  doolhofstack[doolstackcnt]  = ox;
      doolhofstack[doolstackcnt+1]= oy;
      doolstackcnt += 2;
	}

	doolhofstack[doolstackcnt]  = x;
    doolhofstack[doolstackcnt+1]= y;
    doolstackcnt += 2;

	return;
}

void init_level_41(void)
{

#ifndef POCKETPCDEMO
  snake_init (760,   384, orglev4objects, 0, 128, 0, 1, 0, &snakelev4);

  snake_init (2060,  166, orglev4objects, 1, 256, 0, 1, 0, &snakelev4b);

  snake_init (2860,  360, orglev4objects, 0, 256, 0, 1, 0, &snakelev4c);

  snake_init (4756,  498, orglev4objects, 0, 220, 0, 1, 0, &snakelev4d);

  basic_init(3780,476,basic_sin,0,9,384,basic_sin,0,30,40,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);

  basic_init(4162,394,0,0,0,0,basic_sin,0,17,200,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);


  basic_init(8600,1420,basic_sin,0,509,108,basic_sin,0,30,48,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);

  basic_init(8356,1420,basic_sin,0,12,32,basic_sin,0,333,128,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);


  basic_init(8256,1656,basic_sin,0,12,32,basic_sin,300,9,128,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);
  basic_init(8256+64,1656,basic_sin,60,12,32,basic_sin,240,9,128,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);
  basic_init(8256+128,1656,basic_sin,120,12,32,basic_sin,180,9,128,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);
  basic_init(8256+192,1656,basic_sin,180,12,32,basic_sin,120,9,128,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);
  basic_init(8256+256,1656,basic_sin,240,12,32,basic_sin,60,9,128,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);
  basic_init(8256+320,1656,basic_sin,300,12,32,basic_sin,0,9,128,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);

  basic_init(8824,1850,basic_sin,0,508,128,basic_sin,0,507,48,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);

  basic_init(8998,2126,basic_sin,0,12,260,basic_sin,0,10,18,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);

  snake_init (9186, 1704, orglev4objects, 0, 100, 0, 1, 0, &snakelev4e);

  snake_init (8638, 2281, orglev4objects, 3, 96, 0, 1, 0, &snakelev4f);


  basic_init(8170,2244,basic_sin,0,509,320,basic_sin,0,4,18,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);

  basic_init(8058,2352,basic_sin,0,257,50,basic_sin,256,257,50,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);

  basic_init(8058+224,2352,basic_sin,0,257,50,basic_sin,256,257,50,
             0,0,0,0, orglev4objects,1,0, 1 /* seq */ ,0,0,0,0,0,0);


  elevat_init(5764,2296,elevat_siny,  128, 4, 224);

  elevat_init(5524,2340,elevat_sinx,  128, 4, 224);

  elevat_init(5270,2222,elevat_siny,  128, 6, 224);
  basic_init (5120,2222,basic_sin,0,4,20,basic_sin,0,4,224,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);

  elevat_init(4970,2222,elevat_siny,  128, 6, 224);

  elevat_init(4790,2340,elevat_sinx,  128, 510, 224);

  elevat_init(4500,2340,elevat_sinx,  128, 510, 224);

  elevat_init(4320,2340,elevat_siny,  128, 510, 224);

  elevat_init(4200,2340,elevat_siny,  128, 510, 224);


  basic_init (7530,2170,basic_sin,0,4,20,basic_sin,1440,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (7450,2170,basic_sin,0,4,20,basic_sin,1360,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (7370,2170,basic_sin,0,4,20,basic_sin,1280,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (7290,2170,basic_sin,0,4,20,basic_sin,1200,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (7210,2170,basic_sin,0,4,20,basic_sin,1120,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (7130,2170,basic_sin,0,4,20,basic_sin,1040,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (7050,2170,basic_sin,0,4,20,basic_sin,960,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6970,2170,basic_sin,0,4,20,basic_sin,880,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6890,2170,basic_sin,0,4,20,basic_sin,800,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6810,2170,basic_sin,0,4,20,basic_sin,720,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6730,2170,basic_sin,0,4,20,basic_sin,640,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6650,2170,basic_sin,0,4,20,basic_sin,560,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6570,2170,basic_sin,0,4,20,basic_sin,480,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6490,2170,basic_sin,0,4,20,basic_sin,400,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6410,2170,basic_sin,0,4,20,basic_sin,320,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6330,2170,basic_sin,0,4,20,basic_sin,240,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6250,2170,basic_sin,0,4,20,basic_sin,160,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6170,2170,basic_sin,0,4,20,basic_sin,80,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);
  basic_init (6090,2170,basic_sin,0,4,20,basic_sin,0,11,160,
             0,0,0,0, orglev4objects,1,0, 2 /* seq */ ,0,0,0,0,0,0);

  elevat_init(4039,2440,elevat_siny,  128, 12, 24);


  boss = boss_init(2308,2050);

  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);

#ifndef DEMOVERSION2
  hoirise_init(hoi, 224,138, 224, 138);
#else
  hoi_init(hoi, 224,138);
#endif

  restartx = 224;
  restarty = 138;

  player1.worldx = 32;
  player1.worldy = 32;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

	play_woeiwoei();
	woeiwoeivolume(-10000);
#endif
}


void flame_hit(OBJECT *flame, UINT32 param)
{
  CANNON_BLOOD *flameblood;
  flameblood = (CANNON_BLOOD *)flame->blood;
  flameblood->x -= 6;
}


void mine_hit(OBJECT *flame, UINT32 param)
{
  CANNON_BLOOD *flameblood;
  flameblood = (CANNON_BLOOD *)flame->blood;
  flameblood->x += 8;
}


void snake1_ready(OBJECT *snake, UINT32 param)
{
  UINT16 x,y,i;

  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(64/32)] = 489;
  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(96/32)] = 488;
  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(128/32)] = 489;
  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(160/32)] = 488;
  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(192/32)] = 489;
  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(224/32)] = 488;
  player1.loadedmap->map[(800*player1.loadedmap->mapsizex/32/32)+(256/32)] = 489;

  x = 64;
  y = 800;

  for (i=0;i<14;i++)
    {
      colmap[(y*player1.loadedmap->mapsizex/16/16)+(x/16)] = 0;
      x+= 16;
    }

  diamond_init(((UINT32) param)&0x0000ffff, (((UINT32) param)&0xffff0000)>>16 );
//  log_out("snake killed with %d, %d = %x ", ((UINT32) param)&0x0000ffff, (((UINT32) param)&0xffff0000)>>16 , param);

 // log_out("snake1 killed");
}


void snake2_ready(OBJECT *snake, UINT32 param)
{

  diamond_init(((UINT32) param)&0x0000ffff, (((UINT32) param)&0xffff0000)>>16 );

  warp_init(3304, 1896, 2712, 2850, 1);

  close_hole(2688, 2688, 0, 4, 0);


//  log_out("snake2 killed");
}


void init_level_00(void)
{

  switsj_init(2461, 608-48, 2496, 992-64,1);
  basic_init(2269,1369,basic_sin,0,-4,320,basic_sin,0,4,160,
              basic_sin,0,8,32,  orgbat,1,0,SEQ_DUMMY,0,0,0,0,0,prox_bat_volume);

  hoi = hoi_globinit(orghoi, &hoi_caps, hoi_ai, 0, 0);
  hoirise_init(hoi, 196,458, 196, 458);
  restartx = 196;
  restarty = 458;

  player1.worldx = 32;
  player1.worldy = 261;

//  for (i=0; i<300; i++)
//  {
    hoi_cam(&player1);
    scrolling(&player1);
//  }

  play_bat();
  real_bat_volume();
  real_bat_volume();
}
