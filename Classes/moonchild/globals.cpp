#include <frm_wrk.hpp>
#include <vgdll.hpp>
#include <editor.hpp>
#include <prefs.hpp>
#include <scroll.hpp>
#include <trigger.hpp>
#include <lvlinits.hpp>


/* Various variables */
 
Cvideo *video;
Caudio *audio;
Ctimer *timer;
Cmovie *movie;
BYTE    pal[3*256];

Cblitbuf *vidblitbuf;
Cblitbuf *para;

VG_DLL_LIST *allframes;
VG_DLL_LIST *allsequences;
VG_DLL_LIST *allanims;

TRIGGER_ITEM *triglist;  // pointer to current list with triggers

Cblitbuf **patterntab;      //pointer to table with all blitbufs used for patterns
UINT16    nrofpats;     // nrofpats used in patternpicture

Cspr_frame *bouncey_frame;

UINT16 restartx;
UINT16 restarty;
UINT16 cheatmode;
INT32 mousex = 10;
INT32 mousey = 10;
INT32 globmousex = 10;
INT32 globmousey = 10;
UINT16 mouselbut = 0;
UINT16 mouserbut = 0;
UINT16 mouselchng = 0;
UINT16 mouserchng = 0;

UINT16 realxcnt = 0;       // for score board wobbles
UINT16 realycnt = 0;
UINT16 extralivefloat = 0; // for floating extra life heart!
UINT16 musicstoppedflg;   // is set to 1 if music loops... be sure to set it to 0 yourself!
UINT16 musicrestartflg;   // if 1 cd-audio restarts
UINT16 maxlevel;          // maximal level made
UINT16 blacksperlevel[13]; // number of black diamonds picked up/level
UINT16 scoreblacksperlevel[13]; // number of black diamonds picked up/level in 1 life
UINT16 scoreshift = 128;   // scoreboard which scrolls in
UINT16 scoreposhold[18];   // positions for stuff in scoreboard
UINT16 totalgenercnt;      // teller for how many elements have been generated vs blasted so we can donate a black diamond on succes!
UINT16 totalshootcnt;      // teller for how many elements have been generated vs blasted so we can donate a black diamond on succes!
UINT16 diamcnt;
UINT16 shootenabledflg;

UINT32 blackdiamondlocations[13][7]; //7 per level... 13 levels
UINT32 blackdiamondcollocations[13][7]; //7 per level... 13 levels


/* editor variables */

UINT16 cliptlx = 0;
UINT16 cliptly = 0;
UINT16 clipbrx = 640;
UINT16 clipbry = 480;

UINT16 sokomoved;
UINT16 editflg = 0;            /* we don't start of in the editor */
UINT16 selstartx;
UINT16 selstarty;
UINT16 selendx;
UINT16 selendy;

UINT16 lastkey = 0;
UINT16 exitflg;
UINT16 nextlvlflg;
UINT16 restartcnt;
INT16  mc_autorun;
UINT16 paraflg;
UINT16 restartpara;
UINT16 streepflg;

UINT16 vgaflg=1;
UINT16 animsflg=1;
UINT16 easiervisualsflg=0;
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
UINT16 fullscreenflg=1;
UINT16 gamespeedflg=0;
#endif
UINT16 musicflg=1;
UINT16 sfxflg=1;
UINT16 altmenutuneflg=0;

UINT16 autoscrollspd;  // if something else than 0 the level scrolls with this speed

UINT16  oldcolumnr = 90;
UINT16  lastcolumnr = 91;

OBJECT *switch1;
OBJECT *switch2;
OBJECT *switch3;
OBJECT *switch4;

UINT16 gat1;
UINT16 gat2;
UINT16 gat3;
UINT16 gat4;
UINT16 gat5;
UINT16 gat6;
UINT16 gat7;

// stuff for the chain section in world 3
UINT16 chaincnt1;
UINT16 chaincnt2;
UINT16 chaincnt3;
UINT16 chaincnt4;
OBJECT *paramine1a;
OBJECT *paramine1b;
OBJECT *paramine2a;
OBJECT *paramine2b;
OBJECT *paramine3a;
OBJECT *paramine3b;
OBJECT *paramine4a;
OBJECT *paramine4b;

//stuff for sokobans
OBJECT *soko1;
OBJECT *soko2;
OBJECT *soko3;
OBJECT *soko4;

OBJECT *boss;     // end of level boss

// stuff for level2 fast fall section
UINT16 fallspeed = 0;       // for level 2.2 fall part
UINT16 fakespeedadd = 0;

// stuff for level2 chemical fumes section
UINT16 drunk_amp;

char   keytab[256];

UINT8  leftkey;   // use these global key flags for movement decisions
UINT8  rightkey;  // they can also be hooked to joystick
UINT8  upkey;     // mouse or any other input device
UINT8  downkey;
UINT8  jumpkey;
UINT8  shootkey;

UINT16 slowmode;

// doolhof shit

#define DOOLX (20)
#define DOOLY (20)
#define DOLWIDTH  (30)
#define DOLHEIGHT (30)



UINT8 doolhof[DOLHEIGHT+2][DOLWIDTH+2];

UINT16 doolhofstack[(DOLHEIGHT+2)*(DOLWIDTH+2)*2];

UINT16 doolstackcnt;

UINT16 randomcnt;

UINT16 doolpats[16] = { 148, 240, 240, 66,
						240, 550, 549, 527,
						240, 530, 529, 548,
						 88, 547, 528, 240};

#define ZETBLOKJE(y,x,p,c) player1.loadedmap->map[(((y)+DOOLY)*player1.loadedmap->mapsizex/32)+((x)+DOOLX)]   = (p); \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)] = c; \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)+1] = c; \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)+(player1.loadedmap->mapsizex/16)] = c; \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)+(player1.loadedmap->mapsizex/16)+1] = c 

#define KIJKBLOKJE(y,x)  (player1.loadedmap->map[(((y)+DOOLY)*player1.loadedmap->mapsizex/32)+((x)+DOOLX)])


// All used animations in game

ANIM *orgbird;
ANIM *orgcloud;
ANIM *orghoi;
ANIM *orghoirise;
ANIM *orgbouncey;
ANIM *orgbump;
ANIM *orgelevat;
ANIM *orglift;
ANIM *orgspies;
ANIM *orgplof;
ANIM *orgptoei;
ANIM *orgrobyn;
ANIM *orgbat;
ANIM *orgsnake;
ANIM *orgwarp;
ANIM *orgswitsj;
ANIM *orgwall;
ANIM *orgbonus;
ANIM *orgscore;
ANIM *orgelf;
ANIM *orgbomb;
ANIM *orggumbal;
ANIM *orgbee;
ANIM *orgpunt;
ANIM *orgpneu;
ANIM *orgbelt;
ANIM *orghek;
ANIM *orgwater;
ANIM *orgstukhout;
ANIM *orgironring;
ANIM *orgironauto;
ANIM *orgrain;
ANIM *orgweight;
ANIM *orgmixer;
ANIM *orgheart;
ANIM *orgdiamond;
ANIM *orgcolormond;
ANIM *orgfire;
ANIM *orgrock;
ANIM *orgblubber;
ANIM *orgchainbal;
ANIM *orgbiels;
ANIM *orghook;
ANIM *orgshoes;
ANIM *orghelmut;
ANIM *orgjudith;
ANIM *orgparpat;
ANIM *orgspike;
ANIM *orgfan;
ANIM *orgslijk;
ANIM *orgparapats;
ANIM *orgtechnobee;
ANIM *orgbullet1;
ANIM *orgbullet2;
ANIM *orgbullet3;
ANIM *orgrocketup;
ANIM *orgrocketdown;
ANIM *orgsegment;
ANIM *orgmine;
ANIM *orgcannon;
ANIM *orgbolt;
ANIM *orgseamine;
ANIM *orggoodday;
ANIM *orgsokoban;
ANIM *orgmushroom;
ANIM *orgmouth;
ANIM *orgplant;
ANIM *orgbarrel;
ANIM *orgwheel;
ANIM *orgmetbal;
ANIM *orgmetcan;
ANIM *orgsmallheart;
ANIM *orglev4objects;
ANIM *orglev4bal;
ANIM *orgendoflevel;
ANIM *orgpaal;
ANIM *orgbbot;
ANIM *orgdooldoler;
ANIM *orgheatsneaker;
ANIM *orgasteroids;
ANIM *orgclaw;
ANIM *orgglim;



UINT16 scrtrigiteminlist = 0;

SCR_TRIGGER_ITEM scr_triggertab[] = {
	{   0,   0,   0},
	{  44,   1, flying_objects},
	{ 660,   0, 0},
	{2470,   1, flying_objects2},
	{2700,   0, 0},
	{3070,   1, falling_objects1},
	{4440,   0, 0},
	{4450,   1, falling_objects2},
	{5400,   0, 0},
	{7580,   1, floating_objects},
	{8900,   0, 0},
	{12118,  1, fast_objects1},
	{14118,  0, 0},
	{16463,  0, circle_objects1},
	{17163,  0, 0},
	{65535,  0, 0}                     // END OF LIST MARKER
};


MAP_DESCR _loadedmap1 =
{
  NULL,
  NULL,
  7680,
  4800,
};

MAP_DESCR _loadedmap2 =
{
  NULL,
  NULL,
  7680,
  4800,
};

MAP_DESCR _patsmap = 
{
  NULL,
  NULL,
  640,
  1632
};

MAP_DESCR *patsmap = &_patsmap;

BYTE *orgpatssurface = NULL;

/* USE THIS VARIABLE ONLY FOR ACCESS TO THE MAP...
   NOT TO ACCES THE UNDERLYING BLITBUF */

MAP_DESCR *levelmap = &_loadedmap1;

unsigned char *colmap;                        // map which has the properties of the patterns in
                                     // the levelmap stored
char *transmap;                      // crossreference map with info about wether the
                                     // pattern is empty(0)/half-full(1)/full(2)

VIEWPORT player1 =
{
  0,0,
  0,0,
  0,0,
//  0,8,640,224,
  0,60,640,480,

  0,
  0,
  0,0,0,0,0,

  //  0,0,3,7,33,               // score,delayscore,lives,nrofblack,nrg
  3,7,33,0,

  NULL,
  &_loadedmap1,
  NULL
};

VIEWPORT player2 =
{
  0,0,
  0,0,
  0,0,
  0,248,640,224,

  0,
  1,
  0,0,0,0,0,

  //  0,0,3,7,33,               // score,delayscore,lives,nrofblack,nrg
  3,7,33,0,

  NULL,
  &_loadedmap2,
  NULL,
};


PREFS defprefs = {
//  0,
//  70,
//  300,
//  400,

  0,0,640,480,

  PREFS_HIRES,
//  'J',
//  'L',
//  'I',
//  'K',

//    0x64,       // left   keypad
//    0x66,       // right
//    0x68,       // up
//    0x62,       // down
//  ' '

    VK_LEFT,
    VK_RIGHT,
    VK_UP,
    VK_DOWN,
    VK_UP,
    ' ',

//  0,
//  0,
//  640,
//  480,
//  PREFS_HIRES,
//  'a',
//  'd',
//  's'
};

PREFS *prefs = &defprefs;

INT16 prefs_ahead;     /* how much pixels must screen look ahead? */
INT16 prefs_aheadmax;  /* what's the maximum ?!? */

VG_DLL_LIST *objects;  /* list of all objects currently active */
OBJECT *hoi; 

INT16 sinus128[256];   /* sinustab(256 vals)  with amplitude 128 */
INT16 sinus512[1024];  /* sinustab(1024 vals) with amplitude 512 */

BYTE   *recbuf;        //pointer to start of recorded data
BYTE   *recpnt;        //points to current location in recbuf
UINT16  playflg = 1;       // if 1 than autoplay  (0 record)
UINT16  recflg;        // if 1 than rec/play is active
UINT16  intensity;     // use to fade level in

// STORAGE FOR THE PIXEL DATABASE

UINTPTR *dotbase;      // used to store the restore positions of upto 10000 pixels in
UINTPTR *dotpnt;       // points to next available position in dotbase.
UINT8   *dotcolbase;
UINT8   *dotcolpnt;


// LEVEL DESCRIPTIONS

UINT16 world;
UINT16 level;

/* trigger lijsten voor level */

TRIGGER_ITEM triggerlist_00[] =
{
  {2258,  1638, 2278, 4200, madeit_00},
  {2600,  1638, 4000, 4200, reallymadeit_00},
  {0,0,0,0,0}
};


TRIGGER_ITEM triggerlist_11[] =
{
  {52,526,160,568,bordwissel_11},
  {5636,440,5700,696,sneaktransfer_11},
  {7304,400,7324,3000,madeit_11},
  {7616,400,17836,3000,reallymadeit_11},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_12[] =
{
  {515,2600,535,3200,madeit_12},
  {  2,2600, 34,9200,reallymadeit_12},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_13[] =
{
  {  24,4942,   48,5142, warp_13}, 
  {1248,9288, 1374,9304, sneaktransfer_13}, 
  {376, 8980, 903, 8998, sneaktransfer2_13},
  {376, 8129, 903, 8148, sneaktransfer3_13},
  {1425-64,8523, 2000-64,8900, sneaktransfer4_13},
  {0   ,5900, 1640,6149, earthquake_13},
  {26  ,4820-200, 1860,5840, loginitter_13},
  {1259,2700, 1290,3080, madeit_13},
  {1420,2700, 8920,3080, reallymadeit_13},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_14[] =
{
  {558,   993,  873, 3000, warp_14}, 
  {1842,    2, 1882, 1242, stopautoscroll_14}, 
  {2072,    2, 2092,  642, startautoscroll_14}, 
  {5038,  270, 5238,  642, stopautoscroll2_14}, 
  {9256,  900, 9296, 3000, stopfastrun_14}, 
  { 435, 2600,  455, 3200, madeit_14},
  {   2, 2600,   34, 9200, reallymadeit_14},
  { 0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_21[] =
{
  {2180,5320, 2210,5324, notkneeldood_21},
  {1231,1886, 1251,2886, madeit_21},
  {  2, 1000,   66,2886, reallymadeit_21},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_22[] =
{
  {3613,2930, 4000, 3080, warp_22},
  {2334, 590, 3092, 1357, floatingobj_22},
  {2370, 900, 3092,  920, sneaktransfer_22},
  {   2,1034, 1347, 1342, fanl1_22},
  { 561,1715, 1292, 1846, fanr1_22},
  { 561,1847, 1292, 1970, fanr2_22},
  { 620,3220, 3460, 3600, fanb1_22},
  { 192,3970, 4510, 4450, fanb2_22},
  {   2,4410, 5000, 5000, drunk_22},
  {2716,4410, 2746, 5000, madeit_22},
  {3610,4410, 7900, 5000, reallymadeit_22},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_23[] =
{
  {   2,11800, 313,11948, sneaktransfer_23 },
  { 440,11460,  460,11468, sneaktransfer2_23},
  { 576,11904,  796,11904+80, sneaktransfer3_23},
  {   2,17650, 2000,20000, tyres_23},
  { 503, 8600,  543, 8811, madeit_23},
  {  2,  7700,   80, 8700, reallymadeit_23},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_24[] =
{
//  { 503, 8600,  543, 8811, madeit_24},
//  {  2,  7700,   80, 8700, reallymadeit_24},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_31[] =
{
  {1547,  247, 1567, 700, shootenable_31},
  {35  , 1470,  800, 1490, changepara_31},
  {666 , 2277,  696, 2580, changepara2_31},
  {3560, 2320, 3580, 2580, opendoor_31},
  {1890, 1770, 1910, 1970, opendoor2_31},
  { 211, 2130,  291, 2150, restart_31},
  {2955, 4488, 2975, 4900, madeit_31},
  {3455, 4488, 7000, 4900, reallymadeit_31},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_32[] =
{
  {1440, 1332+8, 1440+640, 1332+640, verkleur_32},
  { 380, 1010,  428, 1018, resetsoko_32},
  {2742,   20, 2762, 1200, madeit_32},
  {3455,   20, 7000, 1200, reallymadeit_32},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_33[] =
{
  { 2229,  701,  3200,  721, restartcnt_33},
  { 4848, 1140,  4901, 1434, restart_33},
  {12188,  720, 12208, 4200, restart2_33},
  {16050,  708, 16150, 1108, restartcnt_33},
  {18194,  720, 18212, 4200, madeit_33},
  {18939,  720, 19959, 4200, reallymadeit_33},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_34[] =
{
  { 2180,  280,  2220,  320, shootenable_34},
  { 4363, 4529,  4901, 4549, restart_34},
  {18194,  720, 18212, 4200, madeit_34},
  {18939,  720, 19959, 4200, reallymadeit_34},
  {0,0,0,0,0}
};

TRIGGER_ITEM triggerlist_41[] =
{
  {1950,   60, 1970,  800, changepara_41},
  {3494,  330, 3513,  900, changepara_41},
  {3514,  330, 3534,  900, changepara2_41},
  {4495,  400, 5808, 1800, camcircle_41},
  {6560,   10, 8500, 1000, flipcam_41},
  {8902,   10, 8922,  592, changepara3_41},
  {6862, 1800, 6882, 3592, changepara4_41},
  {3148, 1776, 3168, 4000, changepara5_41},   //3340 3360
  {3168, 1776, 3188, 4000, changepara8_41},
  {5085, 1592, 5115, 4000, changepara8_41},
//  {2955, 4488, 2975, 4900, madeit_41},
//  {1856, 1955, 2768, 2320, reallymadeit_41},
  {0,0,0,0,0}
};

// level descriptors

LEVEL_DESCR levels[5][4] = 
{
  {
    {
      "lvl11.des",
      "assets\\para1.pcx",
      "assets\\pats1.pcx",
      "lvl11.map",
      "lvl11col.map",
      7680,1440,
      init_level_11,
      triggerlist_11
    },
    {
      "lvl12.des",
      "assets\\para1.pcx",
      "assets\\pats1.pcx",
      "lvl12.map",
      "lvl12col.map",
      7680,4800,
      init_level_12,
      triggerlist_12
    },
    {
      "lvl13.des",
      "assets\\para1.pcx",
      "assets\\pats1.pcx",
      "lvl13.map",
      "lvl13col.map",
      1920, 9600,
      init_level_13,
      triggerlist_13
    },
    {
      "lvl14.des",
      "assets\\para1.pcx",
      "assets\\pats1.pcx",
      "lvl14.map",
      "lvl14col.map",
      19200, 1280,
      init_level_14,
      triggerlist_14
    }
  },
  {
    {
      "lvl21.des",
      "assets\\para2.pcx",
      "assets\\pats2.pcx",
      "lvl21.map",
      "lvl21col.map",
      3840, 6400,
      init_level_21,
      triggerlist_21
    },
    {
      "lvl22.des",
      "assets\\para2.pcx",
      "assets\\pats2.pcx",
      "lvl22.map",
      "lvl22col.map",
      3840, 8000,
      init_level_22,
      triggerlist_22
    },
    {
      "lvl23.des",
      "assets\\para2.pcx",
      "assets\\pats2.pcx",
      "lvl23.map",
      "lvl23col.map",
      960, 19200,
      init_level_23,
      triggerlist_23
    },
    {
      "lvl24.des",
      "assets\\para2.pcx",
      "assets\\pats2.pcx",
      "lvl24.map",
      "lvl24col.map",
      9600, 9600,
      init_level_24,
      triggerlist_24
    }
  },
  {
    {
      "lvl31.des",
      "assets\\para3.pcx",
      "assets\\pats3.pcx",
      "lvl31.map",
      "lvl31col.map",
      3840, 6400,
      init_level_31,
      triggerlist_31
    },
    {
      "lvl32.des",
      "assets\\para3.pcx",
      "assets\\pats3.pcx",
      "lvl32.map",
      "lvl32col.map",
      3840, 6400,
      init_level_32,
      triggerlist_32
    },
    {
      "lvl33.des",
      "assets\\para3.pcx",
      "assets\\pats3.pcx",
      "lvl33.map",
      "lvl33col.map",
      19200, 2400,
      init_level_33,
      triggerlist_33
    },
    {
      "lvl34.des",
      "assets\\para3.pcx",
      "assets\\pats3.pcx",
      "lvl34.map",
      "lvl34col.map",
      9200, 9200,
      init_level_34,
      triggerlist_34
    }
  },
  {
    {
      "lvl41.des",
      "assets\\para4.pcx",
      "assets\\pats4.pcx",
      "lvl41.map",
      "lvl41col.map",
      9600, 9600,
      init_level_41,
      triggerlist_41
    },
    {
      0
    },
    {
      0
    },
    {
      0
    }
  },
  {
    {
      "lvl00.des",
      "assets\\para1.pcx",
      "assets\\pats1.pcx",
      "lvl00.map",
      "lvl00col.map",
      3000, 2000,
      init_level_00,
      triggerlist_00
    },
    {
      0
    },
    {
      0
    },
    {
      0
    }
  }
};


