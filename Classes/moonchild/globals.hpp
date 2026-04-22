#include <frm_wrk.hpp>
#include <vgdll.hpp>
#include <mc.hpp>

/* Various variables */

extern Cvideo *video;
extern Caudio *audio;
extern Ctimer *timer;
extern Cmovie *movie;
extern BYTE    pal[3*256];

extern Cblitbuf *para;
extern Cblitbuf *vidblitbuf;


extern VG_DLL_LIST *allframes;
extern VG_DLL_LIST *allsequences;
extern VG_DLL_LIST *allanims;

extern TRIGGER_ITEM *triglist;  // pointer to current list with triggers

extern Cblitbuf    **patterntab;
extern UINT16    nrofpats;     // nrofpats used in patternpicture


extern Cspr_frame  *bouncey_frame;

extern UINT16    cheatmode;
extern INT32     mousex;
extern INT32     mousey;
extern UINT16    restartx;
extern UINT16    restarty;
extern UINT16    cliptlx;
extern UINT16    cliptly;
extern UINT16    clipbrx;
extern UINT16    clipbry;
extern UINT16    sokomoved;
extern INT32     globmousex;
extern INT32     globmousey;
extern UINT16    mouselbut;
extern UINT16    mouserbut;
extern UINT16    mouselchng;
extern UINT16    mouserchng;
extern UINT16    realxcnt;       // for score board wobbles
extern UINT16    realycnt;
extern UINT16    extralivefloat; // for floating extra life heart!
extern UINT16    musicstoppedflg;   // is set to 1 if music loops... be sure to set it to 0 yourself!
extern UINT16    musicrestartflg;   // if 1 cd-audio restarts
extern UINT16    maxlevel;          // maximal level made
extern UINT16    blacksperlevel[13]; // number of black diamonds picked up/level
extern UINT16    scoreblacksperlevel[13]; // number of black diamonds picked up/level
extern UINT16    scoreshift;         // scoreboard which scrolls in
extern UINT16    scoreposhold[18];   // positions for stuff in scoreboard
extern UINT16    totalgenercnt;      // teller for how many elements have been generated vs blasted so we can donate a black diamond on succes!
extern UINT16    totalshootcnt;      // teller for how many elements have been generated vs blasted so we can donate a black diamond on succes!
extern UINT16    diamcnt;
extern UINT16    shootenabledflg;    // flag voor level 3 of je kan schietn ja of nee

extern UINT32 blackdiamondlocations[13][7]; //7 per level... 13 levels
extern UINT32 blackdiamondcollocations[13][7]; //7 per level... 13 levels


extern UINT16    editflg;            /* we start of in the editor */
extern UINT16    selstartx;
extern UINT16    selstarty;
extern UINT16    selendx;
extern UINT16    selendy;
extern UINT16    lastkey;
extern UINT16    exitflg;
extern UINT16    nextlvlflg;
extern UINT16    restartcnt;
extern INT16     mc_autorun;
extern UINT16    paraflg;
extern UINT16    restartpara;
extern UINT16    streepflg;
extern UINT16    vgaflg;
extern UINT16    animsflg;
extern UINT16    easiervisualsflg;
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
#define MC_GAME_SPEED_VSYNC 0u
#define MC_GAME_SPEED_50HZ  1u
#define MC_GAME_SPEED_60HZ  2u
extern UINT16    fullscreenflg;
extern UINT16    gamespeedflg;
#endif
extern UINT16    musicflg;
extern UINT16    sfxflg;
extern UINT16    altmenutuneflg;
extern UINT16    autoscrollspd;  // if something else than 0 the level scrolls with this speed
extern UINT16    oldcolumnr;
extern UINT16    lastcolumnr;
extern UINT16    chaincnt1;
extern UINT16    chaincnt2;
extern UINT16    chaincnt3;
extern UINT16    chaincnt4;
extern UINT16    gat1;
extern UINT16    gat2;
extern UINT16    gat3;
extern UINT16    gat4;
extern UINT16    gat5;
extern UINT16    gat6;
extern UINT16    gat7;
extern OBJECT   *switch1;
extern OBJECT   *switch2;
extern OBJECT   *switch3;
extern OBJECT   *switch4;
extern OBJECT   *paramine1a;
extern OBJECT   *paramine1b;
extern OBJECT   *paramine2a;
extern OBJECT   *paramine2b;
extern OBJECT   *paramine3a;
extern OBJECT   *paramine3b;
extern OBJECT   *paramine4a;
extern OBJECT   *paramine4b;
extern OBJECT   *soko1;
extern OBJECT   *soko2;
extern OBJECT   *soko3;
extern OBJECT   *soko4;
extern OBJECT   *boss;     // end of level boss
extern char      keytab[256];
extern UINT8     leftkey;
extern UINT8     rightkey;
extern UINT8     upkey;
extern UINT8     downkey;
extern UINT8     jumpkey;
extern UINT8     shootkey;
extern UINT16    slowmode;
extern UINT16    fallspeed;
extern UINT16    fakespeedadd;
extern UINT16    drunk_amp;

// doolhof shit

#define DOOLX (20)
#define DOOLY (20)
#define DOLWIDTH  (30)
#define DOLHEIGHT (30)



extern UINT8 doolhof[DOLHEIGHT+2][DOLWIDTH+2];

extern UINT16 doolhofstack[(DOLHEIGHT+2)*(DOLWIDTH+2)*2];

extern UINT16 doolstackcnt;

extern UINT16 randomcnt;

extern UINT16 doolpats[16];

#define ZETBLOKJE(y,x,p,c) player1.loadedmap->map[(((y)+DOOLY)*player1.loadedmap->mapsizex/32)+((x)+DOOLX)]   = (p); \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)] = c; \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)+1] = c; \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)+(player1.loadedmap->mapsizex/16)] = c; \
                           colmap[(((y)+DOOLY)*player1.loadedmap->mapsizex/8)+(((x)+DOOLX)*2)+(player1.loadedmap->mapsizex/16)+1] = c 

#define KIJKBLOKJE(y,x)  (player1.loadedmap->map[(((y)+DOOLY)*player1.loadedmap->mapsizex/32)+((x)+DOOLX)])



extern ANIM     *orgbird;
extern ANIM     *orgcloud;
extern ANIM     *orghoi;
extern ANIM     *orghoirise;
extern ANIM     *orgbouncey;
extern ANIM     *orgbump;
extern ANIM     *orgelevat;
extern ANIM     *orglift;
extern ANIM     *orgspies;
extern ANIM     *orgplof;
extern ANIM     *orgptoei;
extern ANIM     *orgrobyn;
extern ANIM     *orgbat;
extern ANIM     *orgsnake;
extern ANIM     *orgwarp;
extern ANIM     *orgswitsj;
extern ANIM     *orgwall;
extern ANIM     *orgbonus;
extern ANIM     *orgscore;
extern ANIM     *orgelf;
extern ANIM     *orgbomb;
extern ANIM     *orggumbal;
extern ANIM     *orgbee;
extern ANIM     *orgpunt;
extern ANIM     *orgpneu;
extern ANIM     *orgbelt;
extern ANIM     *orghek;
extern ANIM     *orgwater;
extern ANIM     *orgstukhout;
extern ANIM     *orgironring;
extern ANIM     *orgironauto;
extern ANIM     *orgrain;
extern ANIM     *orgweight;
extern ANIM     *orgmixer;
extern ANIM     *orgheart;
extern ANIM     *orgdiamond;
extern ANIM     *orgcolormond;
extern ANIM     *orgfire;
extern ANIM     *orgrock;
extern ANIM     *orgblubber;
extern ANIM     *orgchainbal;
extern ANIM     *orgbiels;
extern ANIM     *orghook;
extern ANIM     *orgshoes;
extern ANIM     *orghelmut;
extern ANIM     *orgjudith;
extern ANIM     *orgparpat;
extern ANIM     *orgspike;
extern ANIM     *orgfan;
extern ANIM     *orgslijk;
extern ANIM     *orgparapats;
extern ANIM     *orgtechnobee;
extern ANIM     *orgbullet1;
extern ANIM     *orgbullet2;
extern ANIM     *orgbullet3;
extern ANIM     *orgrocketup;
extern ANIM     *orgrocketdown;
extern ANIM     *orgsegment;
extern ANIM     *orgmine;
extern ANIM     *orgcannon;
extern ANIM     *orgbolt;
extern ANIM     *orgseamine;
extern ANIM     *orggoodday;
extern ANIM     *orgsokoban;
extern ANIM     *orgmushroom;
extern ANIM     *orgmouth;
extern ANIM     *orgplant;
extern ANIM     *orgbarrel;
extern ANIM     *orgwheel;
extern ANIM     *orgmetbal;
extern ANIM     *orgmetcan;
extern ANIM     *orgsmallheart;
extern ANIM     *orglev4objects;
extern ANIM     *orglev4bal;
extern ANIM     *orgendoflevel;
extern ANIM     *orgpaal;
extern ANIM     *orgbbot;
extern ANIM     *orgdooldoler;
extern ANIM     *orgheatsneaker;
extern ANIM     *orgasteroids;
extern ANIM     *orgclaw;
extern ANIM     *orgglim;

extern UINT16    scrtrigiteminlist;

extern SCR_TRIGGER_ITEM scr_triggertab[];

extern VIEWPORT  player1;
extern VIEWPORT  player2;
extern BYTE     *orgpatssurface;
extern MAP_DESCR *patsmap;
extern MAP_DESCR *levelmap;   /* BEWARE... READ GLOBALS.CPP!! */
extern unsigned char      *colmap;
extern char *transmap;
extern PREFS *prefs;
extern INT16 prefs_ahead;     /* how much pixels must screen look ahead? */
extern INT16 prefs_aheadmax;  /* what's the maximum ?!? */
extern VG_DLL_LIST   *objects;  /* list of all objects currently active */
extern OBJECT *hoi; 
extern INT16 sinus128[256];     /* sinustab(256 vals)  with amplitude 128 */
extern INT16 sinus512[1024];    /* sinustab(1024 vals) with amplitude 512 */

extern BYTE   *recbuf;        //pointer to start of recorded data
extern BYTE   *recpnt;        //points to current location in recbuf
extern UINT16  playflg;       //if 1 than autoplay  (0 record)
extern UINT16  recflg;        // if 1 than rec/play is active

extern UINT16  intensity;     //use to fade level in

// storage for the pixel database

extern UINTPTR *dotbase;      // used to store the restore positions of upto 10000 pixels in
extern UINTPTR *dotpnt;       // points to next available position in dotbase.
extern UINT8   *dotcolbase;
extern UINT8   *dotcolpnt;

extern UINT16 world;
extern UINT16 level;

extern LEVEL_DESCR levels[5][4];

#define VK_LEFT 37
#define VK_UP 38
#define VK_RIGHT 39
#define VK_DOWN 40
#define VK_ESCAPE 0x1B
#define VK_RETURN 0x0D
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_SHIFT 0x10
#define VK_CTRL 0x11
#define VK_ALT 0x12