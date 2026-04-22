// REMEMBER WHEN MAKING FINAL VERSION

// - DECIDE ON EACH OF THE DEFINE'S!!!
// - ALSO REMOVE THE DEFINE IN THE 'LEVINITS.CPP' FILE!!!
// - REMOVE THE LOGGING DEFINE IN THE LIBRARY
// - COMPILE WITHOUT DEBUGGING LABELS
// - INSERT THE REGISTRY LINE IN THE BEGINNING


// DEFINE WELKE ERVOOR ZORGT DAT DE GAME ALLEEN LEVEL 1.2 MAG STARTEN
// OOK VERANDEREN IN LVLINITS.CPP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define POCKETPCDEMO


// if defined Moon Child either records or plays depending on the existence
// of the playback file

// in seconds!
#define RECTIME 600

extern int g_MouseFlg;
extern int g_MouseActualFlg;
extern int g_MouseXDown;
extern int g_MouseYDown;


int	  FirstTimeShowCredzFlg;


#define LOG(x)


// if defined start level immediately

//#define FASTSTART


// If defined don't use the editor since not all patterns are loaded.
// Performance is not optimal when not defined!

#define DISCARD_UNUSED_PATS


// if defined the editor is ready to go

//#define EDITOR


// if defined you're able to grab screenshots from movies and game!

//#define GRABBER


// define if CD audio should be used

#define USE_CD
 

// define if you want a text in the corner with 'demoversion' blinking

// #define FULLVERSION


// Define if this is the Milia Demo Version version!

//#define DEMOVERSION


 
// Define if this is the Demo version!

//#define DEMOVERSION2



// define if movies should be shown

#define MOVIES


/*
//#include <ztimer.h>
*/

#if defined (DEMOVERSION) || defined(DEMOVERSION2) 
#define STARTTRACK (2)
#else
#define STARTTRACK (3)
#endif

//#include <iostream.h>
//#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include <direct.h>
//#include <iostream.h>
#include <stdlib.h>
//#include <fstream.h>
#include <frm_wrk.hpp>
//#include <conio.h>
#include <math.h>
#include <mc.hpp>
#include <vgdll.hpp>
#include <prefs.hpp>
#include <hoi.hpp>
#include <bullet.hpp>
#include <score.hpp>
#include <trigger.hpp>
#include <objects.hpp>
#include <anim.hpp>
#include <editor.hpp>
#include <globals.hpp>
#include <scroll.hpp>
#include <asset.hpp>
#include <sound.hpp>
#include <lvlinits.hpp>
#include <plof.hpp>
#include <sokoban.hpp>
#include <glim.hpp>
#include <time.h>

#include "DisplayBridge.h"
#include "InputBridge.h"

#define PARA

/* Various prototypes */

void configure_level(int worldnr, int levelnr);
void prep_engine(void);
void rec_savemap(void);
void rec_prepmap(void);
void anim_init(void);
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
static void sync_fullscreen(int enabled);
#endif
void makeanimglobal(void);
void level_init(void);
void savelevelmap(void);
void glob_init(void);
void glob_game_init(void);
void check_keys(void);
void recorder(void);
void mouse_handling(void);
void eventhandle(void);
void mc_timer(UINT16 timerid);
void fader(void);
void load_patterns(char *patternpic);
void grabber();
void grabber2();
void globals_clear();
void start_cd(void);
void stop_cd(void);
void changetodatadir(void);
void colorcycles(void);
void cycleit(UINT16 startcnt, UINT16 nrofcols, UINT16 cyclepos,  unsigned char *cycletab);
int  loadinifile( char *filename, char *destbuf);
void peekabooh(void);
void gentab3(void);
void gentab3b(void);
void paraballetjes(void);
void levelspecific(void);
void startbombs(void);
void checkhol(void);
void oogknippen();
void fullversion_display(void);

void close_hole(UINT16 x, UINT16 y, UINT16 pat, UINT16 amount, UINT16 vertflg);
void flame_hit(OBJECT *flame, UINT32 param);
void snake1_ready(OBJECT *snake, UINT32 param);
void snake2_ready(OBJECT *snake, UINT32 param);

void updatedarkdiam(void);
void sparkle(void);


void query_joystickloop(int joynumber);
void query_joystick(int joynumber);
void init_joystick(int joynumber);
int normalize (int val, int minval, int maxval);

void killlevel(void);

void load_highscores(void);
void save_highscores(void);
void load_options(void);
void save_options(void);
void requestdir(void);
void orgdir(void);



HEARTBEAT_FN MC_loadlevel(void);
HEARTBEAT_FN MC_nextlevel(void);
HEARTBEAT_FN MC_abortlevel(void);

HEARTBEAT_FN MC_endtitle(void);
HEARTBEAT_FN MC_endtitle2(void);

HEARTBEAT_FN fadereturn;          // pointer to return function
HEARTBEAT_FN MC_fadeoutslow(void);
HEARTBEAT_FN MC_fadeout(void);
HEARTBEAT_FN MC_fadein(void);
HEARTBEAT_FN MC_fadehalfin(void);

HEARTBEAT_FN MC_heartbeatstart(void);
HEARTBEAT_FN MC_startsampler(void);
HEARTBEAT_FN MC_soundsampler(void);

HEARTBEAT_FN MC_prepentername(void);
HEARTBEAT_FN MC_prepentername2(void);
HEARTBEAT_FN MC_preptitlesequence(void);
HEARTBEAT_FN MC_endsequence(void);
HEARTBEAT_FN MC_startdemo(void);

HEARTBEAT_FN MC_notenoughvram(void);
HEARTBEAT_FN MC_showfirstdemopic(void);
HEARTBEAT_FN MC_waitfirstdemopic(void);
HEARTBEAT_FN MC_levelstartmovie(void);
HEARTBEAT_FN MC_startmovie(void);
HEARTBEAT_FN MC_playingmovie(void);
HEARTBEAT_FN MC_showdemopic(void);
HEARTBEAT_FN MC_endofdemo1(void);
HEARTBEAT_FN MC_endofdemo2(void);
HEARTBEAT_FN MC_prependofdemo2(void);
HEARTBEAT_FN MC_showtitlesequence1(void);
HEARTBEAT_FN MC_titlesequence1(void);
HEARTBEAT_FN MC_showtitlesequence2(void);
HEARTBEAT_FN MC_titlesequence2(void);
HEARTBEAT_FN MC_showtitlesequence3(void);
HEARTBEAT_FN MC_titlesequence3(void);
HEARTBEAT_FN MC_showhighscore(void);
HEARTBEAT_FN MC_highscore(void);
HEARTBEAT_FN MC_showcredz1(void);
HEARTBEAT_FN MC_credz1(void);
HEARTBEAT_FN MC_showcredz2(void);
HEARTBEAT_FN MC_credz2(void);
HEARTBEAT_FN MC_showcredz3(void);
HEARTBEAT_FN MC_credz3(void);
HEARTBEAT_FN MC_showcredz4(void);
HEARTBEAT_FN MC_credz4(void);
HEARTBEAT_FN MC_luxaflex(void);
HEARTBEAT_FN MC_showoptions(void);
HEARTBEAT_FN MC_options(void);
HEARTBEAT_FN MC_darkentitlesequence(void);
HEARTBEAT_FN MC_menu(void);
HEARTBEAT_FN MC_leavemenu(void);
HEARTBEAT_FN MC_buildmenu(void);
HEARTBEAT_FN MC_towindows(void);
HEARTBEAT_FN MC_showentername(void);
HEARTBEAT_FN MC_entername(void);
HEARTBEAT_FN MC_preppuzzleselect(void);
HEARTBEAT_FN MC_puzzleselect(void);
HEARTBEAT_FN MC_endpuzzle(void);
HEARTBEAT_FN MC_abortpuzzle(void);
HEARTBEAT_FN MC_puzzleshow(void);

HEARTBEAT_FN MC_alldiams(void);


void killpuzzle(void);
UINT16 compscore(char *score1, char *score2);
int  startmovie( char *movname, char *movkadername);
void handleinput1shot(void);
void handleinputloop(void);
UINT16 convscore(void);
void scrolltextline(void);
void insertname(void);
int enablefastfile(void);
void disablefastfile(void);
void calcdist(WAV2_FN wav2_fn, UINT16 x, UINT16 y);

void killsequence();

void menupuntblit(void);

void menuf11(void);
void menuf12(void);
void menuf121(void);
void menuf1210(void);
void menuf1211(void);
void menuf12111(void);
void menuf121111(void);
void menuf1211111(void);
void menuf12111111(void);
void menuf122(void);
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
void menuf1221(void);
void menuf1222(void);
#endif
void menuf1223(void);
void menuf1224(void);
void menuf123(void);
void menuf1231(void);
void menuf1232(void);
void menuf1233(void);
void menuf1234(void);
void menuf124(void);
void menuf13(void);
void menuf131(void);
void menuf132(void);
void menuf14(void);

void letterswirlscan();
void letterswirlin();
void swirladd(char lett, UINT16 swirlx, UINT16 swirly);
void letterswirlreset();


int MC_FLG;

int registrypath(void);

char inibuf[512];        // max space for ini file to be

Smack    *mcsmk;          // smacker currently open for playback
Cblitbuf *movbuf;         // temp blitbuf for movie playback
Cblitbuf *movkaderbuf;         // temp blitbuf for movie playback
Cblitbuf *alldiamspic;     // All diamnds made picture
Cblitbuf *demopic = 0;        // temp blitbuf for demosequence
Cblitbuf *refreshpic;     // blitbuf for fast blitting
Cblitbuf *puzzlepic;      // blitbuf for fast blitting when in puzzle selection screen
Cblitbuf *titlepic;       // temp blitbuf for titlepic
BYTE      titlepal[768];
UINT16    titlebrightness;
Cblitbuf *highpic;        // temp blitbuf for highscore
BYTE      highpal[768];
UINT16    highbrightness;
Cblitbuf *chars32;        // blitbuf for characterset!
Cblitbuf *fullversion;    // text in corner that this is NOT the full version

UINT16    abortblackdiamonds; // nr of blackdiamonds to restore when escaped!
UINT16    scoreabortblackdiamonds; // nr of blackdiamonds to restore when escaped!
char      luxatab[16];    // only 14 needed
UINT32    freevram;       // nr of bytes free vram
UINT16    luxacnt;
UINT16    darken;
UINT16    option;         // current option selected
HEARTBEAT_FN menuleavefunc;
HEARTBEAT_FN movieleavefunc;
HEARTBEAT_FN luxaleavefunc;

UINT16    keyquery;

UINT16    seqcnt;         // used for timing the length that screen appear

int curdrive;

UINT16 grabflg = 0;
UINT16    timerid;   // current ID of timer waiting for CD to finish

UINT16    popupmenu;    // force popupmenu after spacebar in title menu
UINT16    ingameflg;    // ingame flg   is 1 when we are IN the game and not in the title sequence
UINT16    levelloadedflg = 0;   // if 0 no level is loaded!
UINT16    puzzleactiveflg = 0;  //if 1 puzzle is on screen
#ifndef DEMOVERSION
UINT16    recplay = 1;         // can change during the program!
#else
UINT16    recplay = 1;         // can change during the program!
#endif 

UINT16    mc_inactive = 0;     // only used in demoversion to make sure no
                               // MC can be seen while 'screenshots' are displayed

UINT16    fadeout = 0;  // if defined we fade out first!
UINT16    fadein  = 0;  // if defined we fade in first!
UINT16    titleseqflg = 1;   // if defined start off with title seq
UINT16    pauseflg;
UINT16    alldiamflg = 0; // cheat mode plaatje al eens gezien?

// Add 4 seconds for CD startup

#ifdef DEMOVERSION
UINT32   cdlength[6] = {((1*60)+6)*1000,
                        ((1*60)+7)*1000,
                        ((1*60)+7)*1000,
						((5*60)+8)*1000,
						((5*60)+14)*1000,
                        ((2*60)+50)*1000};
#else
UINT32   cdlength[6] = {((5*60)+14)*1000,
                        ((5*60)+20)*1000,
                        ((5*60)+5)*1000,
						((5*60)+8)*1000,
						((5*60)+14)*1000,
                        ((2*60)+50)*1000};

#endif


unsigned char dummypal[768];
unsigned char glowriginal[144];    // original glow colors for exhaust
unsigned char gamepal[768];
unsigned char demopal1[768];
unsigned char demopal2[768];
unsigned char demopal3[768];
unsigned char demopal4[768];
unsigned char alldiamspal[768];
unsigned char ingamepal[768];   // always the correct palette for ingame!
unsigned char ingamepal4[768];   // always the correct palette for ingame world 4!
unsigned char puzzlepal[768];

UINT16 oogblinkcnt;

char oogblinkanim[256] = 
{
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};



unsigned char oogdicht[18] = 
{
  1,232,0,
  2,255,0,
  1,232,0,
  1,212,0,
  1,187,0,
  1,192,0
};

unsigned char ooghalf[18] = 
{
  1,232,0,
  2,255,0,
  0,0,0,
  188,188,188,
  221,221,221,
  0,0,0
};

unsigned char oogopen[39] = 
{
  0,0,0,
  255,255,255,
  0,0,0,
  188,188,188,
  221,221,221,
  0,0,0
};



char *swirlinpoint;
UINT16 swirlextrax;
UINT16 swirlx;
UINT16 swirly;
UINT16 swirlrot;
INT16 swirlrotspd;
INT16 swirlrotspd2;
INT16 swirlrotlock;

char   letchain[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ -#. 0123456789 $ % ABCDEFGHIJKLMNOPQRSTUVWXYZ";
INT16  letchainpos;
INT16  lastchaindir;
INT16  chainvulcnt;
UINT16 invulexit;
INT16  letscrollspd;

UINT16 puzzleflash;
UINT16 puzzleinteractive;
UINT16 curpuzzle;


char tempscore[] = "0000000000";

char optionstext[] = // "--------------------"
#ifdef MOONCHILD_EMSCRIPTEN
                      "                    "
                      "                    "
                      "                    "
                      "                    "
                      "                    "
                      "                    "
                      "=       PLAY        "
                      "                    "
                      "=     SETTINGS      "
                      "                    "
                      "=       DEMO        "
                      "                    "
                      "                    "
                      "                    "
                      "                   @";
#else
                      "                    "
                      "                    "
                      "                    "
                      "                    "
                      "                    "
                      "=       PLAY        "
                      "                    "
                      "=     SETTINGS      "
                      "                    "
                      "=       DEMO        "
                      "                    "
                      "=       EXIT        "
                      "                    "
                      "                    "
                      "                   @";
#endif


struct MENU_ITEM
{ 
  UINT16      linenr;
  MENU_FN     menu_fn;
  char       *menutext;
};

MENU_ITEM *menupoint;
UINT16     menuitem;
UINT16     start_afterbuilditem;


char enter_menu_buf1[64];	//"nr of black diamonds"
char enter_menu_buf2[64];    //"      .......       "


MENU_ITEM enter_menu[] =
{
  {  4, 0, enter_menu_buf1},
  {  7, 0, "= ENTER YOUR NAME:  "},
  {  9, 0, enter_menu_buf2},
  { 0,0,0}
};

MENU_ITEM menu1[] =
{
#ifdef MOONCHILD_EMSCRIPTEN
  {  6, menuf11, "=       PLAY        "},
  {  8, menuf12, "=     SETTINGS      "},
  { 10, menuf14, "=       DEMO        "},
#else
  {  5, menuf11, "=       PLAY        "},
  {  7, menuf12, "=     SETTINGS      "},
  {  9, menuf14, "=       DEMO        "},
  { 11, menuf13, "=       EXIT        "},
#endif
  { 0,0,0}
};

MENU_ITEM menu12[] =
{
  {  5, menuf121, "        KEYS        "},
  {  7, menuf122, "+      VIDEO        "},
  {  9, menuf123, "+      AUDIO        "},
  { 11, menuf124, "=      RETURN       "},
  { 0,0,0}
};

MENU_ITEM menu121[] =
{
  {  7, menuf1210, "   PRESS JUMP KEY   "},
  { 0,0,0}
};
MENU_ITEM menu1211[] =
{
  {  7, menuf1211, "  PRESS ACTION KEY  "},
  { 0,0,0}
};
MENU_ITEM menu12111[] =
{
  {  7, menuf12111, "    PRESS UP KEY    "},
  { 0,0,0}
};
MENU_ITEM menu121111[] =
{
  {  7, menuf121111, "   PRESS DOWN KEY   "},
  { 0,0,0}
};
MENU_ITEM menu1211111[] =
{
  {  7, menuf1211111, "   PRESS LEFT KEY   "},
  { 0,0,0}
};
MENU_ITEM menu12111111[] =
{
  {  7, menuf12111111, "+ PRESS RIGHT KEY   "},
  { 0,0,0}
};

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
char *fullscreenontxt  = "+ FULLSCREEN : ON   ";
char *fullscreenofftxt = "  FULLSCREEN : OFF  ";
char *gamespeedmenu_vs  = " GAME SPEED : VSYNC ";
char *gamespeedmenu_50  = "+GAME SPEED : 50HZ  ";
char *gamespeedmenu_60  = "+GAME SPEED : 60HZ  ";
#endif

char *animsontxt    = "+   MOVIES : ON     ";
char *animsofftxt   = "    MOVIES : OFF    ";
char *easyvisontxt  = "+  SAFE GFX : ON    ";
char *easyvisofftxt = "   SAFE GFX : OFF   ";

MENU_ITEM menu122[] =
{
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
  {   5, menuf1221, fullscreenontxt },
  {   7, menuf1222, gamespeedmenu_vs },
  {   9, menuf1223, easyvisontxt },
  {  11, menuf1224, "=      RETURN       "},
#else
  {   6, menuf1223, easyvisontxt },
  {   8, menuf1224, "=      RETURN       "},
#endif
  { 0,0,0}
};

char *musicontxt    = "     MUSIC : ON     ";
char *musicofftxt   = "+   MUSIC : OFF     ";
char *sfxontxt      = "      SFX : ON      ";
char *sfxofftxt     = "+    SFX : OFF      ";
char *altmenuontxt  = "+EX MENU TUNE : ON  ";
char *altmenuofftxt = " EX MENU TUNE : OFF ";


MENU_ITEM menu123[] =
{
  {   5, menuf1231, musicontxt },
  {   7, menuf1232, sfxontxt },
  {   9, menuf1233, altmenuofftxt },
  {  11, menuf1234, "=      RETURN       "},
  { 0,0,0}
};

MENU_ITEM menu13[] =
{
  {  6, menuf131, "+LEAVE YOUR FRIEND  "},
  {  8, menuf132, "=      CANCEL       "},
  { 0,0,0}
};


char hightext[] = // "--------------------"
                      "                    "
                      "+MOON CHILD HEROES  "
                      "                    "
                      "=  80# - ........   "
                      "   70# - ........   "
                      "   60# - ........   "
                      "   50# - ........   "
                      "   40# - ........   "
                      "   30# - ........   "
                      "   20# - ........   "
                      "   15# - ........   "
                      "   10# - ........   "
                      "    5# - ........   "
                      "                   @";
char savebuf[2000];
UINT16 optsavebuf[32];



char credstext[] = // "--------------------"
"                    "
"+- MOON CHILD FE -  "
"                    "
"                    "
"=   BASED ON THE    "
"    1997 GAME BY    "
"                    "
"+REINIER VAN VLIET  "
"+   METIN SEVEN     "
"= RAMON BRAUMULLER  "
"                    "
"+                   "
"SOURCE PORT BY MORS "
"                   @";


char creds2text[] = // "--------------------"
                      "                    "
                      "=    MOON CHILD     "
                      "                    "
                      "                    "
                      "      THE LOOK      "
                      "+   METIN SEVEN     "
                      "                    "
                      "=     THE FEEL      "
                      "+REINIER VAN VLIET  "
                      "                    "
                      "+    THE SOUND      "
                      "= RAMON BRAUMULLER  "
                      "                    "
                      "                   @";


char creds3text[] = // "--------------------"
"                    "
"                    "
"=   3D ANIMATION    "
"   RICCARDO RUSSO   "
"+ VIKTOR RIETVELD   "
"                    "
"   VIDEO EDITING    "
"=  LUC BUITENHUIS   "
"   OSCAR DE WAARD   "
"                    "
"+ AUDIO MASTERING   "
"  MARTIJN BEEKERS   "
"   JACCO LENSTRA    "
"                    "
"                   @";

char creds4text[] = // "--------------------"
                      "                    "
                      "                    "
                      "+SPECIAL THANKS TO  "
                      "+  RODERIC LEIGH    "
                      "=  FOR MAKING THE   "
                      "+  ORIGINAL GAME    "
                      "=     POSSIBLE      "
                      "                    "
                      "=    MOON CHILD     "
                      "=  COPYRIGHT 2026   "
                      "+REINIER VAN VLIET  "
                      "+   METIN SEVEN     "
                      "= RAMON BRAUMULLER  "
                      "                    "
                      "                   @";


UINT16 swirltab[64*4];  //upto 64 swirling letters simultaneously  (x,y,amp,char)

//-------------------------------------------------------------------------
/*!	Converts the given filename to its fullpath using unicode
*///-----------------------------------------------------------------------

extern char *FullPath( char *a_File );
extern char *FullWritablePath( char *a_File );
#ifdef __EMSCRIPTEN__
extern void SyncPersistentStorage();
#endif


HEARTBEAT_FN MC_testje2(void);

HEARTBEAT_FN MC_testje1(void)
{
	int rc;
  video->clear(0);
  video->palette(gamepal);
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();


  //  refreshpic = new Cblitbuf("assets\\title.pcx", 0, 0);   // only keep this one in vidmem
//  chars32    = new Cblitbuf("assets\\char32.pcx", 0, 0);  // and this one too!
  titlepic   = new Cblitbuf("assets\\title.pcx", 0, 0);   // doesn't matter
//  highpic    = new Cblitbuf("assets\\hiscore.pcx", 0, 0); // doesn't matter

//  vgassert(refreshpic);
//  vgassert(titlepic);
//  vgassert(highpic);
//  vgassert(chars32);

  rc = PCX_ff_load("assets\\title.pcx", NULL, titlepal);
//  vgassert(rc);


  video->palette(titlepal);
	titlepic->draw_nokey(*vidblitbuf,0,0,0,0,640,480);
  video->swap();
	titlepic->draw_nokey(*vidblitbuf,0,0,0,0,640,480);


	return (HEARTBEAT_FN) MC_testje2;
}

HEARTBEAT_FN MC_testje2(void)
{
  video->swap();
  video->scansync();
	return (HEARTBEAT_FN) MC_testje2;
}



HEARTBEAT_FN framework_InitGame(Cvideo *newvideo, Caudio *newaudio, Ctimer *newtimer, Cmovie *newmovie)
{
	int TestBuf[4];
	int *TestPtr;
  UINT16 rc;

	  FirstTimeShowCredzFlg = 0;

	TestPtr = TestBuf;
	TestPtr = (int *)  (((char *)TestPtr)+4);;
	*TestPtr = 1;

  video  = newvideo;  //make video structure global
  audio  = newaudio;  //make audio structure global
  timer  = newtimer;
  movie  = newmovie;

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
  DisplayBridge::SetFullscreenChangeCallback(sync_fullscreen);
#endif

  levelloadedflg = 0;
 

//  timer->setcallback(mc_timer);

  timerid = 0;

  freevram = video->queryfreevram();

  if (freevram <1100000)
  {
//    framework_util_displayerror("Your video card has insufficient VRAM to run Moon Child properly.\nThis results in jerky movement and loss of video performance.\nWe strongly advise you to upgrade your video card to 2MB VRAM.");
  }
  
  freevram = 2000000;      // <------- we force the videoram

    memset(gamepal, 0, 768);
    video->palette(gamepal);
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();

  vidblitbuf = new Cblitbuf(video);
  vidblitbuf->set_clipping(0, 0, 640, 480);



//  if (registrypath() == 1)    // change to path which is specified in registry
//  {
//	framework_util_displayerror("Directory where Moon Child was installed doesn't exist anymore\nTry re-installing Moon Child");
//	return 0;
//  }

  
  rc = enablefastfile();   // we start fastfile before changing to other directory!

  requestdir();
  load_highscores();
  load_options();

  changetodatadir();    // CD-ROM?

  if (rc == 0)    // couldn't find fastfile in local harddisk directory
    {
      rc = enablefastfile();   // we start fastfile again. This time from CDRom
      if (rc == 0) 
	  {
//		framework_util_displayerror("Can't locate data file... Is the Moon Child CD inserted?");
		return 0;
	  }
    }

  // LOAD RECORDMAP IF AVAILABLE

  rec_prepmap();


// GLOBAL INITIALISATIONS

  glob_init();

// INITIALISE JOYSTICK

  init_joystick(0);

// CONFIGURE LEVEL

#ifdef EDITOR
  configure_level(4,1);   // numbers starting from 1 (not 0)
#else
  configure_level(1,1);   // numbers starting from 1 (not 0)
#endif

//4,1



#ifdef FASTSTART
  configure_level(3,3);   // numbers starting from 1 (not 0)
  return (HEARTBEAT_FN) MC_loadlevel;
#endif

  
//#ifdef FULLVERSION
//  return (HEARTBEAT_FN) MC_showfirstdemopic;
//  return (HEARTBEAT_FN) MC_showdemopic;
//  return (HEARTBEAT_FN) MC_loadlevel;
//#else
//  return (HEARTBEAT_FN) MC_testje1;


// start jaytrax


  return (HEARTBEAT_FN) MC_startmovie;
//#endif
}


void save_highscores(void)
{
  UINT16 i;
  char *scrambler;

  memcpy (savebuf+4, hightext, sizeof(hightext));

  scrambler = savebuf;
  *(scrambler++) = 'H';
  *(scrambler++) = 'I';
  *(scrambler++) = 'G';
  *(scrambler++) = 'H';

  for (i=0; i<sizeof(hightext); i++)
  {
    *(scrambler++) ^= 0xaa;
  }

  savedocfile("mc_high.dat", savebuf, sizeof(hightext)+4);

}


void load_highscores(void)
{
  UINT16 i;
  char *scrambler;

  loaddocfile("mc_high.dat", savebuf, sizeof(hightext)+4);

  scrambler = savebuf;
  if (*(scrambler++) != 'H') return;
  if (*(scrambler++) != 'I') return;
  if (*(scrambler++) != 'G') return;
  if (*(scrambler++) != 'H') return;

  for (i=0; i<sizeof(hightext); i++)
  {
    *(scrambler++) ^= 0xaa;
  }

  memcpy (hightext, savebuf+4, sizeof(hightext));

}


#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
  #define MC_OPTS_SLOT_COUNT 13
#else
  #define MC_OPTS_SLOT_COUNT 11
#endif
#define MC_OPTS_BYTE_COUNT (MC_OPTS_SLOT_COUNT * 2)

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
static void sync_fullscreen(int enabled)
{
  fullscreenflg = enabled ? 1 : 0;
  menu122[0].menutext = fullscreenflg ? fullscreenontxt : fullscreenofftxt;
}
#endif

void save_options(void)
{
  int i = 0;
  optsavebuf[i++] = animsflg;
#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
  optsavebuf[i++] = fullscreenflg;
  optsavebuf[i++] = (UINT16)(gamespeedflg + 10u);
#endif
  optsavebuf[i++] = easiervisualsflg;
  optsavebuf[i++] = musicflg;
  optsavebuf[i++] = sfxflg;
  optsavebuf[i++] = altmenutuneflg;
  optsavebuf[i++] = prefs->shootkey;
  optsavebuf[i++] = prefs->upkey;
  optsavebuf[i++] = prefs->downkey;
  optsavebuf[i++] = prefs->leftkey;
  optsavebuf[i++] = prefs->rightkey;
  optsavebuf[i++] = prefs->jumpkey;

  savedocfile("mc_opts.dat", (char *) optsavebuf, MC_OPTS_BYTE_COUNT);
}


void load_options(void)
{
  UINT16 rc;
  memset(optsavebuf, 0, sizeof(optsavebuf));
  rc = loaddocfile("mc_opts.dat", (char *) optsavebuf, MC_OPTS_BYTE_COUNT);

  if (rc)
  {
    int i = 0;
    animsflg = optsavebuf[i++];
    #ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
        fullscreenflg = optsavebuf[i++];
        {
          UINT16 rawGs = optsavebuf[i++];
          if (rawGs < 10u)
          {
            gamespeedflg = (rawGs == 1u) ? MC_GAME_SPEED_VSYNC : MC_GAME_SPEED_60HZ;
          }
          else
          {
            UINT16 v = (UINT16)(rawGs - 10u);
            gamespeedflg = (v > MC_GAME_SPEED_60HZ) ? MC_GAME_SPEED_VSYNC : v;
          }
        }
    #endif
    easiervisualsflg = optsavebuf[i++];
    musicflg = optsavebuf[i++];
    sfxflg = optsavebuf[i++];
    altmenutuneflg = optsavebuf[i++];
    prefs->shootkey = optsavebuf[i++];
    prefs->upkey = optsavebuf[i++];
    prefs->downkey = optsavebuf[i++];
    prefs->leftkey = optsavebuf[i++];
    prefs->rightkey = optsavebuf[i++];
    prefs->jumpkey = optsavebuf[i++];

    if (sfxflg)
    {
      menu123[1].menutext = sfxontxt;
    }
    else
    {
      menu123[1].menutext = sfxofftxt;
    }
    if (musicflg)
    {
      menu123[0].menutext = musicontxt;
    }
    else
    {
      menu123[0].menutext = musicofftxt;
    }
  }

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
  menu122[0].menutext = fullscreenflg ? fullscreenontxt : fullscreenofftxt;
  menu122[1].menutext = (gamespeedflg == MC_GAME_SPEED_50HZ) ? gamespeedmenu_50
                      : (gamespeedflg == MC_GAME_SPEED_60HZ) ? gamespeedmenu_60
                                                            : gamespeedmenu_vs;
  menu122[2].menutext = easiervisualsflg ? easyvisontxt : easyvisofftxt;
  DisplayBridge::SetFullscreen(fullscreenflg ? 1 : 0);
  DisplayBridge::SetVSync(gamespeedflg == MC_GAME_SPEED_VSYNC ? 1 : 0);
#else
  menu122[0].menutext = easiervisualsflg ? easyvisontxt : easyvisofftxt;
#endif
  menu123[2].menutext = altmenutuneflg ? altmenuontxt : altmenuofftxt;
}


HEARTBEAT_FN MC_startmovie(void)
{
#ifdef DEMOVERSION
  movieleavefunc = (HEARTBEAT_FN) MC_showdemopic;
#else
  movieleavefunc = (HEARTBEAT_FN) MC_preptitlesequence;
#endif

#ifdef DEMOVERSION2
  recplay = 0;
  configure_level(2,2);   // numbers starting from 1 (not 0)
  movieleavefunc = (HEARTBEAT_FN) MC_loadlevel;
#endif


#ifdef MOVIES
  if (startmovie("intro.smk", "assets\\frmintro.pcx") == 0)
  {
	  return 0;
  }
  return (HEARTBEAT_FN) MC_playingmovie;
#endif

  return (HEARTBEAT_FN) movieleavefunc;
}


int registrypath(void)
{
	 UINT16 rc;
#if 0
	 if (RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE", &tempkey1) == ERROR_SUCCESS)
	 {
	   if (RegOpenKey(tempkey1, "Valkieser", &tempkey2) == ERROR_SUCCESS)
	   {
	     if (RegOpenKey(tempkey2, "Moon Child", &tempkey3) == ERROR_SUCCESS)
	     {
// moon child is installed... let's now obtain the executable path so we can find Moon Child's path

			 if (RegOpenKey(tempkey1, "Microsoft", &tempkey4) == ERROR_SUCCESS)
			 {
			   if (RegOpenKey(tempkey4, "Windows", &tempkey5) == ERROR_SUCCESS)
			   {
			     if (RegOpenKey(tempkey5, "CurrentVersion", &tempkey6) == ERROR_SUCCESS)
			     {
			       if (RegOpenKey(tempkey6, "App Paths", &tempkey7) == ERROR_SUCCESS)
			       {
			         if (RegOpenKey(tempkey7, "mc.exe", &tempkey8) == ERROR_SUCCESS)
			         {
		               dblength = 800;
				       if(RegQueryValueEx(tempkey8, "Path", &dbreserved, &dbtype, dbdata, &dblength) == ERROR_SUCCESS)
					   {
						 rc = chdir( (char *)dbdata );
                         if (rc == 0) log_out("drive changed succesfully to the path from the registry");
					   }
 		               RegCloseKey(tempkey8);
					 }
		             RegCloseKey(tempkey7);
				   }
			       RegCloseKey(tempkey6);
			     }
                 RegCloseKey(tempkey5);
			   }
			   RegCloseKey(tempkey4);
			 }
			 RegCloseKey(tempkey3);
		 }   
		 RegCloseKey(tempkey2);
	   }
  	   RegCloseKey(tempkey1);
	 }
#endif
  rc = 1;
  return rc;
}


void requestdir(void)
{
}


void orgdir(void)
{
}


void changetodatadir(void)
{
#if 0
  char cdpath[] = "E:\\data";

  if (loadinifile("MC.INI", inibuf))
    {
      if (inibuf[0] < 'D' || inibuf[0] > 'Z')
  {
    chdir("data");         // geen ini file dus niet de cdrom   dan maar relatief proberen
    log_out("MC ini file is corrupt... changed path to data");
    return;
	  }
      else
  {
    cdpath[0]    = inibuf[0];

    chdir(cdpath);
    log_out("changed path to: %s",cdpath);
  }
    }
  else
    {
      chdir("data");         // geen ini file dus niet de cdrom   dan maar relatief proberen
//      log_out("changed path to data");
    }
#endif
}

int loadinifile( char *filename, char *destbuf)
{
   
#if 0
    ifstream fin(filename);
    if (!fin.good()) {
      return 0;
    }

    while (fin.good()) {
        *destbuf++ = (char) fin.get();
    }
    return 1;
#endif
    return 0;
}


int enablefastfile(void)
{
  int rc;
  rc = FastFileInit(FullPath("mc.art"),16);
  if (!rc)
    {
//      log_out("couldn't find mc.art");
      return 0;
    }

  // set framework to use fastfile from now
  framework_usefastfile(TRUE);
  return 1;
}


void disablefastfile(void)
{
  FastFileFini();
  // set framework not to use fastfile from now
  framework_usefastfile(FALSE);
}


int startmovie( char *movname, char *movkadername)
{

// are the movies turned on???
      
  if (!animsflg) return 1;
      
  if (vgaflg == 0)
  {
    movbuf = new Cblitbuf(320,240,0,0,0);   //640*480 if zooming is on!
  }
  else
  {
    movbuf = new Cblitbuf(640,480,0,0,0);   //640*480 if zooming is on!
  }
  
//  vgassert(movbuf);

//  log_out("movbufcreate = %x",movbuf);

//** new bit for kader enzo...

  movkaderbuf = new Cblitbuf (movkadername, 0, 0);
 
  
//  vgassert(!mcsmk);      // still one open?
  mcsmk = movie->open(movname);
  if (mcsmk == 0) 
  {
//    framework_util_displayerror("Can't locate movie files... Is the Moon Child CD inserted?");
	return 0;
  }

  video->clear(0);
  if (vgaflg == 0)movkaderbuf->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->clear(0);
  if (vgaflg == 0)movkaderbuf->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->clear(0);
  if (vgaflg == 0)movkaderbuf->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();

//  log_out("opened moviefile: %s",movname);

  movie->playtovideo(mcsmk, video, movbuf, (vgaflg == 1) ? 2 : 1);    // laatste getal is zoomfactor

//  log_out("to the movie proc loop");
  return 1;
}


HEARTBEAT_FN MC_playingmovie(void)
{
  // are the movies turned on???
      
  if (!animsflg) return (HEARTBEAT_FN) movieleavefunc;;

      
  handleinput1shot();
  grabber2();

  if (movie->stillplaying() && !shootkey)
  {
    return (HEARTBEAT_FN) MC_playingmovie;
  }
  else
  {
    shootkey = 0;
    movie->close(mcsmk);
    mcsmk = 0;
    delete movbuf;
    delete movkaderbuf;
    movbuf = 0;
    movkaderbuf = 0;
    video->clear(0);
    video->swap();
    video->clear(0);
    video->swap();
    video->clear(0);
    video->swap();
    return (HEARTBEAT_FN) movieleavefunc;
  }
}


// ALL DIAMONDS PICKED UP PICTURE //





HEARTBEAT_FN MC_alldiams(void)
{

  handleinput1shot();

  if (!shootkey)
  {
    return (HEARTBEAT_FN) MC_alldiams;
  }
  else
  {
    shootkey = 0;
    delete alldiamspic;
    alldiamspic = 0;
    video->clear(0);
    video->swap();
    video->clear(0);
    video->swap();
    video->clear(0);
    video->swap();
    return (HEARTBEAT_FN) MC_nextlevel;
  }
}


// only for demo

HEARTBEAT_FN MC_showdemopic(void)
{
  UINT16 rc;
  configure_level(2,2);   // numbers starting from 1 (not 0)        
  video->clear(0);

  demopic   = new Cblitbuf("assets\\mcdtxt01.pcx", 0, 0);    //mcdtxt01


  memcpy (gamepal, ingamepal, 768);

  for (rc =0; rc < 20; rc++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      demopic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
      video->palette(gamepal);
    }

  delete demopic;
  demopic = 0;

  fadeout = 127;

  return (HEARTBEAT_FN) MC_loadlevel;
}


HEARTBEAT_FN MC_showfirstdemopic(void)
{
  UINT16 rc;

  video->clear(0);

//  if (freevram > 1050000)
//  {
    demopic   = new Cblitbuf("assets\\eval.pcx", 0, 0);
//  }
//  else
//  {
//  demopic   = new Cblitbuf("assets\\vram.pcx", 0, 0);
//  }
  memcpy (gamepal, ingamepal, 768);
  

  for (rc =0; rc < 20; rc++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      demopic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
      video->palette(gamepal);
    }

  delete demopic;
  demopic = 0;

  fadeout = 63;

//  if (freevram > 1050000)
//  {
    return (HEARTBEAT_FN) MC_waitfirstdemopic;
//  }
//  else
//  {
//    return (HEARTBEAT_FN) MC_notenoughvram;
//  }
}


HEARTBEAT_FN MC_notenoughvram(void)
{
  video->swap();

  if(keytab[' '] == 0)   return (HEARTBEAT_FN) MC_notenoughvram;

  fadereturn = (HEARTBEAT_FN)MC_towindows;
  return (HEARTBEAT_FN) MC_fadeout;
}


HEARTBEAT_FN MC_waitfirstdemopic(void)
{
  static UINT16 dummywait = 250;

  video->swap();

  if(dummywait--)   return (HEARTBEAT_FN) MC_waitfirstdemopic;


  fadereturn = (HEARTBEAT_FN) MC_startmovie;
  return (HEARTBEAT_FN) MC_fadeout;
}


void killlevel(void)
{
  int i;

  if (!levelloadedflg) return;

  video->DrawLoading();
//  log_out("freeing all memory");

/////// FREE MEMORY

// SOUNDS

  sound_destroy();


#ifndef DEMOVERSION2
  if (musicflg) stop_cd();
#endif

// OBJECT_INITIALISE

  object_shutdown();

// ANIM_INIT

//	log_out("removing assets");

  asset_removeall();

// PREPENGINE PART

  free(player1.loadedmap->map);

  //  delete player1.loadedmap->blitbuf;
  //  delete player2.loadedmap->blitbuf;
  delete para;

  free(patsmap->map);

  free(colmap);

  free(transmap);


	log_out("freeing pattern tab");
  
	for (i=0; i<nrofpats; i++)
  {
    if (patterntab[i]) delete patterntab[i];
  }

  free(patterntab);

  levelloadedflg = 0;
}


HEARTBEAT_FN MC_abortlevel(void)
{
  UINT16 curlevel;
  if (!fadeout)
    {
      video->clear(0);
      video->swap();
      video->clear(0);
      video->swap();
      video->clear(0);
      video->swap();
    }

  reinitlevels();
  killlevel();

  if (world == 4)
  {
    world = 0;
	level = 0;
    return (HEARTBEAT_FN) MC_preptitlesequence;
  }

  curlevel = world*4+level;
//  blacksperlevel[curlevel] = abortblackdiamonds;
//  scoreblacksperlevel[curlevel] = scoreabortblackdiamonds;

  blacksperlevel[curlevel] = 0;
  scoreblacksperlevel[curlevel] = 0;
  
  
  player1.newlife = 0;    // extra life saving is lost

  // LOAD LEVEL

  puzzleinteractive = 1;                      // after playing 1 level you can select which level to play
  return (HEARTBEAT_FN) MC_preppuzzleselect;
}


HEARTBEAT_FN MC_nextlevel(void)
{
  UINT16 nr_b,i;

  if (!fadeout)
    {
      video->clear(0);
      video->swap();
      video->clear(0);
      video->swap();
      video->clear(0);
      video->swap();
    }

  killlevel();


  if (world == 4)
  {
    world = 0;
	level = 0;
    return (HEARTBEAT_FN) MC_preptitlesequence;
  }


  nr_b = 0;
  for (i=0; i<13; i++)
    {
      nr_b += blacksperlevel[i];      // add up all blacks picked up per level
    }


  if (nr_b == 84 && alldiamflg ==0)  // al een keertje gezien?
  {
    alldiamspic = new Cblitbuf ("alldiams.pcx", 0, 0);
	alldiamflg = 1;
    video->clear(0);
    alldiamspic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    video->palette(alldiamspal);
    video->swap();
    video->clear(0);
    alldiamspic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    video->palette(alldiamspal);
    video->swap();
    video->clear(0);
    alldiamspic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    video->palette(alldiamspal);
    video->swap();
    return (HEARTBEAT_FN) MC_alldiams;
  }
  
  //////// CONFIGURE LEVEL

  level++;        //temporarily level advancement
  if (level==4 && world==0)   // van 1.4 naar 2.1
  {
    world++;
    level = 0;
  }

  if (level==4 && world==1)   // van 2.4 naar 3.1
  {
    world++;
    level = 0;
  }


  if (level==4 && world==2)  // van 3.4 naar 4.1
  {
    if (nr_b >= 77)
	{
	  world++;
      level = 0;
	}
	else
	{
      world = 2;
	  level = 3;
	}
  }

  if (nr_b >= 77 && maxlevel != 12)   // genoeg diamonds
  {
    world = 3;
	level = 0;
  }
	  
  if (level==1 && world==3)   // van 4 to endsequence
  {
    level--;
    movieleavefunc = (HEARTBEAT_FN) MC_prepentername2;  //MC_preppuzzleselect;
    if (startmovie("extro.smk", "assets\\frmextro.pcx") == 0)
    {
	    return 0;
    }
    return (HEARTBEAT_FN) MC_playingmovie;
  }

  
  
  configure_level(world+1, level+1);   // numbers starting from 1 (not 0)

// LOAD LEVEL

#ifndef DEMOVERSION
  puzzleinteractive = 1;                      // after playing 1 level you can select which level to play
  return (HEARTBEAT_FN) MC_levelstartmovie;
  return (HEARTBEAT_FN) MC_preppuzzleselect;
#endif


  return (HEARTBEAT_FN) MC_loadlevel;
}


HEARTBEAT_FN MC_levelstartmovie(void)
{
  movieleavefunc = (HEARTBEAT_FN) MC_preppuzzleselect;

#ifdef MOVIES

  if (level == 0)
  {
    if (world == 1)
    {
      if(startmovie("bumper12.smk", "assets\\frmenv12.pcx") == 0)
	  {
		  return 0;
	  }
      return (HEARTBEAT_FN) MC_playingmovie;
    }
    if (world == 2)
    {
      if(startmovie("bumper23.smk", "assets\\frmenv23.pcx") == 0)
	  {
		  return 0;
	  }
      return (HEARTBEAT_FN) MC_playingmovie;
    }
    if (world == 3)
    {
      if (startmovie("bumper34.smk", "assets\\frmenv34.pcx") == 0)
	  {
		  return 0;
	  }
      return (HEARTBEAT_FN) MC_playingmovie;
    }
  }


#endif

  return (HEARTBEAT_FN) movieleavefunc;
}

HEARTBEAT_FN MC_loadlevel(void)
{
//  video->DrawLoading();

//  if (levelloadedflg) vgassert(0);

// PREPARE GAME-ENGINE

  prep_engine();

// LOAD ALL ANIMATION DATA FOR LEVEL IN

  anim_init();
  log_out("ready with anim init");
  makeanimglobal();
  log_out("ready with making anims global");

// PREPARE GLOBAL VARIABLES

  glob_game_init();

// INITIALISE OBJECTS

  object_initialise();
  log_out("ready initialising objects");

// LOAD SOUNDS!

  sound_load();

// LEVEL SPECIFIC INITIALISATIONS (PLACEMENT OF OBJECTS / SPECIAL FX)

  level_init();
  log_out("ready initialising the level");

//  rc = PCX_load("assets\\title.pcx", NULL, titlepal);
//  vgassert(rc);
//  video->palette(titlepal);

#ifndef DEMOVERSION
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
#endif

  memcpy (gamepal, ingamepal, 768);
  video->palette(gamepal);

//  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
//  SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

  log_out("Ready initialising");

  //  preptitle();
  //  if (titleseqflg) return (HEARTBEAT_FN) MC_showtitle;

  levelloadedflg = 1;

  fadereturn = (HEARTBEAT_FN) MC_heartbeatstart;
  if (fadeout)
    {
      fadeout = 127;
      return (HEARTBEAT_FN) MC_fadeoutslow;
    }

  return (HEARTBEAT_FN) MC_heartbeatstart;
}


HEARTBEAT_FN MC_heartbeatstart(void)
{
  UINT16 curlevel;

#ifdef DEMOVERSION
  musicrestartflg = 0;     // no restarting musics
#else
  musicrestartflg = 1;
#endif

  musicstoppedflg = 0;     // set to 0 for demoversion
  pauseflg = 0;

#ifdef DEMOVERSION2
  if (world != 3)
  {
#endif

    if (musicflg) start_cd();     // music turned on??? then let's rock!
#ifdef DEMOVERSION2
  }
#endif


  if (world == 3)
    {
      memcpy (gamepal, ingamepal4, 768);
    }
  else
    {
      memcpy (gamepal, ingamepal, 768);
    }
  video->palette(gamepal);

  curlevel = world*4+level;

//  abortblackdiamonds = blacksperlevel[curlevel];
//  scoreabortblackdiamonds = scoreblacksperlevel[curlevel];
  

#ifdef DEMOVERSION2
  if (world != 3)
  {
    play_demosound();
  }
#endif
  
  return (HEARTBEAT_FN) MC_heartbeat;
}


// routine which fades screen to black and goes on with next part

HEARTBEAT_FN MC_fadeout(void)
{
  static UINT16 cleanem = 10;

  video->set_fade_swap_from_snapshot(true);

  handleinput1shot();
  if (shootkey) popupmenu = 1;
  
  if (fadeout != 0 && fadeout < 64)
  {
    fadeout--;
    video->palette(gamepal, fadeout*4);
    cleanem  =10;
    video->scansync();
    video->swap();
 
    return (HEARTBEAT_FN) MC_fadeout;
  }

  video->swap();

  if (cleanem)
    {
      cleanem--;
      video->clear(0);

      return (HEARTBEAT_FN) MC_fadeout;
    }

  video->set_fade_swap_from_snapshot(false);
  return (HEARTBEAT_FN) fadereturn;

}

// routine which fades screen to black and goes on with next part

HEARTBEAT_FN MC_fadeoutslow(void)
{
  static UINT16 cleanem = 10;

  video->set_fade_swap_from_snapshot(true);

  if (fadeout != 0 && fadeout < 128)
  {
    fadeout--;
    video->palette(gamepal, fadeout*2);
    cleanem  =10;
    video->scansync();
    video->swap();
 
    return (HEARTBEAT_FN) MC_fadeoutslow;
  }

  video->swap();

  if (cleanem)
    {
      cleanem--;
      video->clear(0);

      return (HEARTBEAT_FN) MC_fadeoutslow;
    }

  video->set_fade_swap_from_snapshot(false);
  
  return (HEARTBEAT_FN) fadereturn;

}


// routine which fades screen in and goes on with next part

HEARTBEAT_FN MC_fadein(void)
{

  handleinput1shot();
  if (shootkey) popupmenu = 1;

      
  if (fadein < 64)
  {
    fadein++;
    video->palette(gamepal, fadein*4);
    video->scansync();
    video->swap();
 
    return (HEARTBEAT_FN) MC_fadein;
  }

  return (HEARTBEAT_FN) fadereturn;

}

// routine which fades screen half in and goes on with next part

HEARTBEAT_FN MC_fadehalfin(void)
{
  if (fadein < 32)
  {
    fadein++;
    video->palette(gamepal, fadein*4, 240);
//    video->palette(gamepal, fadein*4);
    video->scansync();
    video->swap();
 
    return (HEARTBEAT_FN) MC_fadehalfin;
  }

  return (HEARTBEAT_FN) fadereturn;

}


void start_cd(void)
{

  musicstoppedflg = 0;

    if (!musicflg)
      return;

#ifdef USE_CD


#ifdef DEMOVERSION2
    if (timerid) timer->removetimer(timerid);
    timerid = timer->installtimer(((2*60)+50)*1000);
    if (audio->play_cd(2))
    {
      if (audio->play_cd(2))
      {
        if (audio->play_cd(2))
        {
          log_out("couldn't start cd");
        }
      }
    }
	return;
#endif


//  if (timerid) timer->removetimer(timerid);
//  timerid = timer->installtimer(cdlength[world]);
//  log_out("Playing song %d of %d min,%d sec",world+1, cdlength[world]/ 60000, (cdlength[world] % 60000) / 1000);

#ifdef DEMOVERSION
  if (world == 3)
    {
      log_out("demoversion hasn't got music under world 4");
      return;
    }

#endif


#ifdef DEMOVERSION2
  if (world == 3)
    {
      log_out("demoversion hasn't got music under world 4");
      return;
    }

#endif
if (world < 4)
  {
    if (audio->play_cd(STARTTRACK+world))
    {
      if (audio->play_cd(STARTTRACK+world))
      {
        if (audio->play_cd(STARTTRACK+world))
        {
          log_out("couldn't start cd");
        }
      }
    }
  }
if (world == 4)
  {
    if (audio->play_cd(STARTTRACK+0))
    {
      if (audio->play_cd(STARTTRACK+0))
      {
        if (audio->play_cd(STARTTRACK+0))
        {
          log_out("couldn't start cd");
        }
      }
    }
  }
if (world == 5)
  {
    UINT16 menutrack = (UINT16)(STARTTRACK - 1);
    if (altmenutuneflg)
      {
        menutrack = 7;
      }
    if(audio->play_cd(menutrack))
    {
      if(audio->play_cd(menutrack))
      {
        if(audio->play_cd(menutrack))
        {
          log_out("couldn't start cd");
        } 
      }
    }
  }

  log_out("CD audio started");
#endif

}


void stop_cd(void)
{

#ifdef USE_CD
//  if (timerid) timer->removetimer(timerid);
  timerid = 0;
  audio->stop_cd();
  log_out("CD audio stopped");
#endif

}


HEARTBEAT_FN MC_endtitle(void)
{
  static UINT16 fade = 255;
  static UINT16 entercnt = 0;
  static UINT16 x;
  static UINT16 y;
  static INT16  i;
  static INT16 scrolltab[64];
  static INT16 eventcnt = 550;

  if (fade)
  {
    video->palette(gamepal, fade);
    fade--;

    if (!fade)
      {
  if (entercnt < 2) stop_cd();
      }
    return (HEARTBEAT_FN) MC_endtitle;
  }

  if (!demopic)
  {
    entercnt++;

    if (entercnt == 1) demopic = new Cblitbuf("assets\\mcdtxt02.pcx", 0, 0);
    if (entercnt == 2) demopic = new Cblitbuf("assets\\mcdtxt03.pcx", 0, 0);
    if (entercnt == 3) demopic = new Cblitbuf("assets\\env1.pcx", 0, 0);
    if (entercnt == 4) demopic = new Cblitbuf("assets\\env2.pcx", 0, 0);
    if (entercnt == 5) demopic = new Cblitbuf("assets\\env3.pcx", 0, 0);
    if (entercnt == 6) demopic = new Cblitbuf("assets\\env4.pcx", 0, 0);
    if (entercnt == 7) demopic = new Cblitbuf("assets\\env5.pcx", 0, 0);
    if (entercnt == 8) demopic = new Cblitbuf("assets\\env6.pcx", 0, 0);
    if (entercnt == 9) demopic = new Cblitbuf("assets\\mcdtxt04.pcx", 0, 0);
    if (entercnt == 10) demopic = new Cblitbuf("assets\\mcchars.pcx", 0, 0);
    if (entercnt == 11) demopic = new Cblitbuf("assets\\mcdtxt08.pcx", 0, 0);
    if (entercnt == 12)
      {


#ifdef MOVIES
  movieleavefunc = (HEARTBEAT_FN) MC_startsampler;
  if(startmovie("extro.smk", "assets\\frmextro.pcx") == 0)
  {
	  return 0;
  }
  return (HEARTBEAT_FN) MC_playingmovie;
#endif

  return (HEARTBEAT_FN) MC_startsampler;
      }


#if 0
    if (entercnt > 2 && entercnt < 8)
      {
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
      }
#endif

    return (HEARTBEAT_FN) MC_endtitle;
  }

  video->scansync();
  video->swap();
    
  if (eventcnt > 540)
    {
      //      video->clear(0);
      demopic->draw(*player1.loadedmap->blitbuf,0,0,0,0,640,480);
    }

  if (eventcnt == 538)       video->palette(gamepal);


  eventcnt--;

  fadeout = 63;
  world = 0;
  level = 0;
  recplay = 0;
  titleseqflg = 0;
  exitflg = 0;
  fadereturn = (HEARTBEAT_FN) MC_endtitle;

  if (eventcnt < 500  &&  entercnt < 2)  
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;


      return (HEARTBEAT_FN) MC_nextlevel;
    }

  if (eventcnt < 200 && entercnt == 2)
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (eventcnt < 470  &&  entercnt >= 3 && entercnt < 9) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      if (entercnt>2 && entercnt<8)
  {
    fade = 0;
    return (HEARTBEAT_FN) MC_endtitle;
  }
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (eventcnt < 200  &&  entercnt >= 8 && entercnt < 10 ) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      if (entercnt >2 && entercnt <8)
  {
    fade = 0;
    return (HEARTBEAT_FN) MC_endtitle;
  }
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (eventcnt < 10  &&  entercnt == 10 ) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (eventcnt < 500  &&  entercnt == 11)
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      killlevel();
      configure_level(4,1);   // numbers starting from 1 (not 0)
      return (HEARTBEAT_FN) MC_loadlevel;
    }







  if (eventcnt < 470  &&  entercnt == 12 ) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;

    }

  return (HEARTBEAT_FN) MC_endtitle;
}


HEARTBEAT_FN MC_endtitle2(void)
{
  static UINT16 fade = 255;
  static UINT16 entercnt = 0;
  static UINT16 x;
  static UINT16 y;
  static INT16  i;
  static INT16 scrolltab[64];
  static INT16 eventcnt = 550;

  if (fade)
  {
    video->palette(gamepal, fade);
    fade--;

    if (!fade)
      {
        sound_destroy();
//		if (entercnt < 1) stop_cd();
      }
    return (HEARTBEAT_FN) MC_endtitle2;
  }

  if (!demopic)
  {
    entercnt++;

    if (entercnt == 1) demopic = new Cblitbuf("assets\\mcd2txt1.pcx", 0, 0);
    if (entercnt == 2) demopic = new Cblitbuf("assets\\mcd2txt2.pcx", 0, 0);
    if (entercnt == 3) demopic = new Cblitbuf("assets\\env1.pcx", 0, 0);
    if (entercnt == 4) demopic = new Cblitbuf("assets\\env2.pcx", 0, 0);
    if (entercnt == 5) demopic = new Cblitbuf("assets\\env3.pcx", 0, 0);
    if (entercnt == 6) demopic = new Cblitbuf("assets\\env4.pcx", 0, 0);
    if (entercnt == 7) demopic = new Cblitbuf("assets\\env5.pcx", 0, 0);
    if (entercnt == 8) demopic = new Cblitbuf("assets\\env6.pcx", 0, 0);
    if (entercnt == 9) demopic = new Cblitbuf("assets\\mcd2txt3.pcx", 0, 0);
    if (entercnt == 10) demopic = new Cblitbuf("assets\\mcd2txt4.pcx", 0, 0);

    if (entercnt == 1) memcpy (gamepal, demopal1, 768);
    if (entercnt == 2) memcpy (gamepal, demopal2, 768);
    if (entercnt == 3) memcpy (gamepal, ingamepal, 768);
    if (entercnt == 4) memcpy (gamepal, ingamepal, 768);
    if (entercnt == 5) memcpy (gamepal, ingamepal, 768);
    if (entercnt == 6) memcpy (gamepal, ingamepal, 768);
    if (entercnt == 7) memcpy (gamepal, ingamepal, 768);
    if (entercnt == 8) memcpy (gamepal, ingamepal, 768);
    if (entercnt == 9) memcpy (gamepal, demopal3, 768);
    if (entercnt == 10)memcpy (gamepal, demopal4, 768);
  


#if 0
    if (entercnt > 2 && entercnt < 8)
      {
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
      }
#endif

    return (HEARTBEAT_FN) MC_endtitle2;
  }

  video->scansync();
  video->swap();
    
  if (eventcnt > 540)
    {
      //      video->clear(0);
      demopic->draw(*player1.loadedmap->blitbuf,0,0,0,0,640,480);
    }

  if (eventcnt == 538)       video->palette(gamepal);


  eventcnt--;

  fadeout = 63;
  world = 0;
  level = 0;
  recplay = 0;
  titleseqflg = 0;
  exitflg = 0;
  fadereturn = (HEARTBEAT_FN) MC_endtitle2;


  if (eventcnt < 40  &&  entercnt >= 1 && entercnt < 3) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (eventcnt < 470  &&  entercnt >= 3 && entercnt < 9) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      if (entercnt>2 && entercnt<8)
		{
			fade = 0;
			return (HEARTBEAT_FN) MC_endtitle2;
		}
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (eventcnt < 500  &&  entercnt == 9)
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      killlevel();
      configure_level(4,1);   // numbers starting from 1 (not 0)
      return (HEARTBEAT_FN) MC_loadlevel;
    }

  if (eventcnt < -400  &&  entercnt == 10 ) 
    {
      delete demopic;
      demopic = 0;
      eventcnt = 550;
      fade = 255;
      killlevel();
      return (HEARTBEAT_FN) 0;
    }



  return (HEARTBEAT_FN) MC_endtitle2;
}


HEARTBEAT_FN MC_startsampler(void)
{
  UINT16 rc;

  video->clear(0);
  demopic   = new Cblitbuf("assets\\mcdtxt05.pcx", 0, 0);

  memcpy (gamepal, ingamepal, 768);

  for (rc =0; rc < 20; rc++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      demopic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
      video->palette(gamepal);
    }

  delete demopic;
  demopic = 0;


  world = 0;    // song track 1
  start_cd();
  return (HEARTBEAT_FN) MC_soundsampler;
}


HEARTBEAT_FN MC_soundsampler(void)
{
  handleinput1shot();

  if (keytab['1'] == 1)
    {
      world = 0;
      keytab['1'] = 0;
      start_cd();
    }
  if (keytab['2'] == 1)
    {
      world = 1;
      keytab['2'] = 0;
      start_cd();
    }
  if (keytab['3'] == 1)
    {
      world = 2;
      keytab['3'] = 0;
      start_cd();
    }
  if (shootkey)
    {
      UINT16 rc;
      shootkey = 0;
      stop_cd();
      video->palette(gamepal, 0);
      video->clear(0);
      video->swap();
      video->scansync();
      video->clear(0);
      video->swap();
      video->scansync();
      video->clear(0);
      video->swap();
      video->scansync();

      demopic   = new Cblitbuf("assets\\mcdtxt06.pcx", 0, 0);
         
      for (rc =0; rc < 20; rc++)
  {
    video->swap();
    video->scansync();
    video->clear(0);
    demopic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  }
      
      delete demopic;
      demopic = 0;

      video->palette(gamepal);
      
      return (HEARTBEAT_FN) MC_endofdemo1;
    }

  return (HEARTBEAT_FN) MC_soundsampler;
}


HEARTBEAT_FN MC_endofdemo1(void)
{
  static int endcount = 650;

  video->swap();
  video->scansync();

  endcount--;
  if (!endcount)
    {
      fadereturn = (HEARTBEAT_FN) MC_prependofdemo2;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  return (HEARTBEAT_FN) MC_endofdemo1;
}


HEARTBEAT_FN MC_prependofdemo2(void)
{
  UINT16 rc;
  shootkey = 0;
  stop_cd();
  video->palette(gamepal, 0);
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  
  demopic   = new Cblitbuf("assets\\mcdtxt07.pcx", 0, 0);
 
  
  for (rc =0; rc < 20; rc++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      demopic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }
  
  delete demopic;
  demopic = 0;
  
  video->palette(gamepal);
  
  return (HEARTBEAT_FN) MC_endofdemo2;
}


HEARTBEAT_FN MC_endofdemo2(void)
{
  video->swap();
  video->scansync();

  if (keytab[' '])
    {
      keytab[' '] = 0;
      fadereturn = (HEARTBEAT_FN) MC_towindows;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
  }

  return (HEARTBEAT_FN) MC_endofdemo2;
}
 

extern int frmwrk_CenterX;
extern int frmwrk_CenterY;

HEARTBEAT_FN MC_heartbeat(void)
{
  int i;

  MC_RANDOM(4);
  eventhandle();
  check_keys();

	// values doorgeven aan het framework (over hacken gesproken)
if(hoi!=NULL)
{
    HOI_BLOOD *hoiblood = (HOI_BLOOD *)hoi->blood; /* in case of multiple hois */

  frmwrk_CenterX = (hoi->x+32) + (hoiblood->spdx >> 4) - player1.worldx;// - hoi->x;
  frmwrk_CenterY = (hoi->y+48) + (hoiblood->gravy >> 3) - player1.worldy;// - hoi->y;
}

    // ***DEMOVERSION

#ifdef DEMOVERSION2
  if (exitflg)         return (HEARTBEAT_FN) MC_endtitle2;
  if (musicstoppedflg) return (HEARTBEAT_FN) MC_endtitle2;
  if (nextlvlflg)      return (HEARTBEAT_FN) MC_endtitle2;
#endif



#ifdef DEMOVERSION
  if (exitflg)         return (HEARTBEAT_FN) MC_endtitle;
  if (musicstoppedflg) return (HEARTBEAT_FN) MC_endtitle;
  if (nextlvlflg)      return (HEARTBEAT_FN) MC_endtitle;

#else
  if (exitflg == 1)
  {
    video->returnpal(gamepal);
    fadeout = 63;
    fadereturn = (HEARTBEAT_FN)MC_prepentername;  // exit  application
    return (HEARTBEAT_FN) MC_fadeout;
  }
  if (exitflg == 2)
  {
    video->returnpal(gamepal);
    fadeout = 63;
    fadereturn = (HEARTBEAT_FN)MC_abortlevel;     // abort application
    return (HEARTBEAT_FN) MC_fadeout;
  }
  if (nextlvlflg)
  {
    video->returnpal(gamepal);
    fadeout = 63;
    fadereturn = (HEARTBEAT_FN)MC_nextlevel;
    return (HEARTBEAT_FN) MC_fadeout;
  }

#endif


// if pause is active we stop here

  if (pauseflg) return (HEARTBEAT_FN) MC_heartbeat;

  


  recorder();

#ifdef DEMOVERSION
  peekabooh();
#endif
#ifdef DEMOVERSION2
  peekabooh();
#endif

  editor(&player1);
  mouse_handling();
////  object_remember(&player1);
//      object_remember(&player2);

    video->ShowMouseCursor();
  object_visualise(&player1);

  fullversion_display();
  
  globals_clear();
  trigger();

  object_movement();
  bullet_swap();
  sparkle();

  levelspecific();

  updatedarkdiam();


#if defined(DEMOVERSION) || defined (DEMOVERSION2)
  if (world != 3)
    {
      score_display(&player1);
    }
#else
  score_display(&player1);
#endif



//    RASTCOL(0,0,0);
//      object_visualise(&player2);

  object_collision();

#ifdef DEMOVERSION
  if (world != 3)
    {
#endif
#ifdef DEMOVERSION2
  if (world != 3)
    {
#endif
  hoi_cam(&player1);
  scrolling(&player1);

#ifdef DEMOVERSION
    }
#endif
#ifdef DEMOVERSION2
    }
#endif

//      scrolling(&player2);

  if (slowmode) for(i=0;i<750000;i++)updatedarkdiam();
//  video->scansync();

      grabber();
  oogknippen();
  colorcycles();


//  RASTCOL2(207,0,0,0);
//  video->scansync();
//  RASTCOL2(207,0,0,255);
  video->swap();
//  RASTCOL(235,0, 0);
//  RASTCOL2(207,255,0,0);


//  video->clear(0);

  paraballetjes();

//  RASTCOL2(207,0,0,235);
  visual_updates();

  level4flash(&player1);
  update(&player1);
//      if (!editflg) update(&player2);
////  object_remove(&player1);
//      object_remove(&player2);
  restoredots();

  rebuild(&player1);
//      rebuild(&player2);
  
  return (HEARTBEAT_FN) MC_heartbeat;
}


void fullversion_display(void)
{

#ifdef FULLVERSION
  fullversion->draw(*vidblitbuf, 400, 20);
#endif

}


void updatedarkdiam(void)
{
  UINT16 curlevel;
// fill picked up amount of black diamonds in structure

  if (world == 4) return;    // demo level
  
  curlevel = world*4+level;
  blacksperlevel[curlevel] = 7-player1.nrofblack;
  scoreblacksperlevel[curlevel] = 7-player1.nrofblack;

}


void sparkle(void)
{
  UINT16 x;
  UINT16 y;
  UINT16 r;

  x = MC_RANDOM(21);
  y = MC_RANDOM(17);

  x <<= 5;
  y <<= 5;

  r =collision((player1.worldx & 0xffe0) + x, (player1.worldy & 0xffe0) + y);

  if (r == 11 || r == 10)
  {
	glim_init((player1.worldx & 0xffe0) + x - 4, (player1.worldy & 0xffe0) + y + 12);
  }

}


void levelspecific(void)
{
  if (world==0 && level == 2)
  {
    rumblesoundsofter();
		real_bee_volume();
	}

  if (world==0 && level == 3)
  {
    real_roltnlp_volume();
  }

  if (world == 0 && level==1)
	{
		real_bat_volume();
		real_vogel_volume();
	}

	if ((world == 0 && level ==0) || (world == 0  && level == 2))
	{
		if (level == 0)
		{
			calcdist(prox_waterval_volume, 3613, 1188);
		}
		else
		{
			calcdist(prox_waterval_volume, 1154, 5702);
			calcdist(prox_waterval_volume,  854, 5702);
			calcdist(prox_waterval_volume,  554, 5702);
			calcdist(prox_waterval_volume,  154, 5702);

  		calcdist(prox_waterval_volume, 1154, 5352);
			calcdist(prox_waterval_volume,  854, 5352);
			calcdist(prox_waterval_volume,  554, 5352);
			calcdist(prox_waterval_volume,  154, 5352);

			calcdist(prox_waterval_volume, 1154, 5044);
			calcdist(prox_waterval_volume,  854, 5044);
			calcdist(prox_waterval_volume,  554, 5044);
			calcdist(prox_waterval_volume,  154, 5044);
		}
		real_waterval_volume();
	}
	
	if (world == 1 && level == 0)
	{
// de 2 lopende banden
		
		calcdist(prox_loopband_volume, 2240, 5409);
		calcdist(prox_loopband_volume, 2530, 5541);
		
		real_vuur_volume();
		real_loopband_volume();
		real_drilboor_volume();
	}

	if (world == 1 && level == 1)
	{
		calcdist(prox_ventltor_volume, 1358, 1306);
		calcdist(prox_ventltor_volume,  590, 1890);

		calcdist(prox_ventltor_volume,  828, 3600);
		calcdist(prox_ventltor_volume, 1128, 3600);
		calcdist(prox_ventltor_volume, 1428, 3600);
		calcdist(prox_ventltor_volume, 1728, 3600);
		calcdist(prox_ventltor_volume, 2038, 3600);
		calcdist(prox_ventltor_volume, 2348, 3600);
		calcdist(prox_ventltor_volume, 2658, 3600);
		calcdist(prox_ventltor_volume, 2968, 3600);
		calcdist(prox_ventltor_volume, 3377, 3600);

		calcdist(prox_ventltor_volume,  361, 4444);
		calcdist(prox_ventltor_volume,  661, 4444);
		calcdist(prox_ventltor_volume,  961, 4444);
		calcdist(prox_ventltor_volume, 1261, 4444);
		calcdist(prox_ventltor_volume, 1561, 4444);
		calcdist(prox_ventltor_volume, 1861, 4444);
		calcdist(prox_ventltor_volume, 2161, 4444);
		calcdist(prox_ventltor_volume, 2461, 4444);
		calcdist(prox_ventltor_volume, 2761, 4444);
		calcdist(prox_ventltor_volume, 3061, 4444);
		calcdist(prox_ventltor_volume, 3361, 4444);
		calcdist(prox_ventltor_volume, 3661, 4444);

		calcdist(prox_chemo_volume,  596, 4766);
		calcdist(prox_chemo_volume, 1076, 4628);
		calcdist(prox_chemo_volume, 1493, 4574);
		calcdist(prox_chemo_volume, 1780, 4804);

            if (drunk_amp>6)
            {
              chemo2volume(-1536 + (drunk_amp*3));
            }
            else
            {
              chemo2volume(-10000);
            }

            real_bee2_volume();
		real_ventltor_volume();
		real_chemo_volume();
	}

	if (world == 1 && level == 2)
      {
		real_bee2_volume();
      }

	if (world == 1 && level == 3)
      {
		    real_slowlift_volume();
            wavegoodday();
            startbombs();
            checkhol();
      }

    if (world == 2 && level == 0)
	{
		real_vlamwerp_volume();
		real_stroom_volume();
	}

	if (world == 2 && level == 1)
	{
		static UINT16 sokogoed = 0;
		static UINT16 mazegoed = 0;
        UINT16        testpat;
		UINT16        startx,starty;
        SOKOBAN_BLOOD *sokobanblood;

		sokobanblood = (SOKOBAN_BLOOD *) soko2->blood;
		if (sokogoed == 0)
		{        
			if (sokobanblood->groups[0]->block[0]->x == 0xc0 && sokobanblood->groups[0]->block[0]->y == 0x340 && sokobanblood->groups[1]->block[0]->x == 0x160 && sokobanblood->groups[1]->block[0]->y == 0x340)
			{
				sokogoed = 1;

         
				play_warp();

				hoirise_init(hoi, hoi->x, hoi->y, 108, 1125);
				
			}
		}
	    else
		{
			if (sokobanblood->groups[0]->block[0]->x != 0xc0 || sokobanblood->groups[0]->block[0]->y != 0x340 || sokobanblood->groups[1]->block[0]->x != 0x160 || sokobanblood->groups[1]->block[0]->y != 0x340)
			{
				sokogoed = 0;
			}
		}

		startx = 1472;
		starty = 1376;
	    
		testpat = player1.loadedmap->map[(starty*player1.loadedmap->mapsizex/32/32)+(startx/32)];

	    for(starty = 1376; starty<(1376+640); starty+=128)
		{
		  for(startx = 1472; startx<(1472+640); startx+=128)
		  {
			  if(player1.loadedmap->map[(starty*player1.loadedmap->mapsizex/32/32)+(startx/32)] != testpat) goto nietzelfde;
		  
		  }
		}	
//welzelfde
        if(mazegoed == 0)
		{
			mazegoed = 1;
     		play_warp();

			hoirise_init(hoi, hoi->x, hoi->y, 1353, 1471);

		}
        goto skipzelfde;

nietzelfde:
        mazegoed = 0;
skipzelfde:
        startx = 0; //dummy instruction

	}

	
	if (world == 2 && level == 2)
	{
		real_stroom_volume();
        if (hoi->live == hoi_fly_live)
		{

			if (player1.worldx < 20000)
			{
				autoscrollspd = 2;
			}

			if (player1.worldx < 14269)
			{
				autoscrollspd = 3;
			}

			if (player1.worldx < 10720)
			{
				autoscrollspd = 2;
			}

			if (player1.worldx < 9812)
			{
				autoscrollspd = 3;
			}

			if (player1.worldx < 5940)
			{
				autoscrollspd = 2;
			}

			if (player1.worldx < 4740)
			{
				autoscrollspd = 3;
			}

			if (player1.worldx < 3429)
			{
				autoscrollspd = 2;
			}
		}
		else
		{
			autoscrollspd = 0;
		}

		if ( editflg != 0 )
		{
			autoscrollspd = 0;
		}
		
		if (player1.worldx < scr_triggertab[scrtrigiteminlist].x)
	    {
		    scrtrigiteminlist--;
			return;              // belangrijk dat 'ie dus NIET de routine uitvoert
			                     // als 'ie terugloopt
		}

	    if (player1.worldx > scr_triggertab[scrtrigiteminlist+1].x)
	    {
		    scrtrigiteminlist++;
	    }

		if (scr_triggertab[scrtrigiteminlist].scr_trigger_fn != 0)
		{
		
			if (scr_triggertab[scrtrigiteminlist].wholeregionflg == 0)
			{
				if (player1.worldx == (scr_triggertab[scrtrigiteminlist].x+1))
				{
					scr_triggertab[scrtrigiteminlist].scr_trigger_fn();
				}
			}
			else
			{
				scr_triggertab[scrtrigiteminlist].scr_trigger_fn();
			}
		}
	}

	if (world == 3 && level == 0)
	{
      if (drunk_amp>6)
      {
        woeiwoeivolume(-1536 + (drunk_amp*3));
      }
      else
      {
        woeiwoeivolume(-10000);
      }
	}
	
	if (world == 2 && level == 3)
	{
  	  static UINT16 nextdelay;

      real_heatsnkr_volume();
      if (totalgenercnt == totalshootcnt)
	  {
        if(nextlvlflg == 0 && nextdelay == 0)
        {
          nextdelay = 200;
		  play_madeit();
        }

        if (nextdelay)
        {
          nextdelay--;
          if (nextdelay == 0) nextlvlflg = 1;
        }
	  }  
	}
}


HOI_CAPS bomb_caps    = {ID_BOMB,    1, 1, 1, 140 , 7, 36, 36};

void startbombs(void)
{
  OBJECT *bomb;
  static UINT16 interval = 0;
  
  interval++;
  interval&=255;

  if (interval== 1)
  {
    bomb = hoi_globinit(orgbomb, &bomb_caps, bomb_ai, 0, 0);
    hoi_init(bomb, 9366,1817);
  }
}


void checkhol(void)
{
  static UINT16 leveldelay = 350;
      
      
  if (gat1 && gat2 && gat3 && gat4 && gat5 && gat6 && gat7)
  {
    UINT16 nosound;
    if (leveldelay==350) play_madeit();

    leveldelay--;

    nosound == 1;  // geen sound

    if ((leveldelay & 31) == 0) nosound = 0;

//    if ((leveldelay & 1) == 0) plof_init(MC_RANDOM(640)+player1.worldx, MC_RANDOM(480)+player1.worldy, 0, 0, 0, 0, nosound);
    if (leveldelay < 250)
	{
		plof_init(MC_RANDOM(640)+player1.worldx, MC_RANDOM(480)+player1.worldy, 0, 0, 0, 0, nosound);
        player1.quakex = MC_RANDOM(16);
        player1.quakey = MC_RANDOM(16);
	}
    if (leveldelay == 1) nextlvlflg = 1;
  }
  else
  {
    leveldelay = 350;
  }

  gat1 = 0;
  gat2 = 0;
  gat3 = 0;
  gat4 = 0;
  gat5 = 0;
  gat6 = 0;
  gat7 = 0;
}


void calcdist(WAV2_FN wav_fn, UINT16 x, UINT16 y)
{
	INT32 distxl,distyl;
	INT32 distxr,distyr;

	distxl = abs(hoi->x - (x-64));
	distyl = abs(hoi->y - y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (x+64));
	distyr = abs(hoi->y - y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	wav_fn(-(distxl*4), -(distxr*4));   // proximity.... is equal to volume
}


unsigned char cycle1tab[] =
{
   0,  0,  0,
   0,  0, 32,
   0,  0, 64,
   0,  0, 96,
   0,  0,128,
   0,  0,160,
   0,  0,192,
   0,  0,224,
  32, 32,255,
  64, 64,255,
  96, 96,255,
 128,128,255,
 160,160,255,
 192,192,255,
 224,224,255,
 255,255,255,
   0,  0,  0,
   0,  0, 32,
   0,  0, 64,
   0,  0, 96,
   0,  0,128,
   0,  0,160,
   0,  0,192,
   0,  0,224,
  32, 32,255,
  64, 64,255,
  96, 96,255,
 128,128,255,
 160,160,255,
 192,192,255,
 224,224,255,
 255,255,255
};


unsigned char cycle2tab[] =
{
   0,255,255,
  16,255,255,
  32,255,255,
  48,255,255,
  64,255,255,
  80,255,255,
  96,255,255,
 112,255,255,
 128,255,255,
 144,255,255,
 160,255,255,
 176,255,255,
 192,255,255,
 208,255,255,
 224,255,255,
 240,255,255,
 255,255,255,
 248,255,255,
 232,255,255,
 216,255,255,
 200,255,255,
 184,255,255,
 168,255,255,
 152,255,255,
 136,255,255,
 120,255,255,
 104,255,255,
  88,255,255,
  72,255,255,
  56,255,255,
  40,255,255,
  24,255,255,
   8,255,255,
   0,255,255,
  16,255,255,
  32,255,255,
  48,255,255,
  64,255,255,
  80,255,255,
  96,255,255,
 112,255,255,
 128,255,255,
 144,255,255,
 160,255,255,
 176,255,255,
 192,255,255,
 208,255,255,
 224,255,255,
 240,255,255,
 255,255,255,
 248,255,255,
 232,255,255,
 216,255,255,
 200,255,255,
 184,255,255,
 168,255,255,
 152,255,255,
 136,255,255,
 120,255,255,
 104,255,255,
  88,255,255,
  72,255,255,
  56,255,255,
  40,255,255,
  24,255,255,
   8,255,255,
};


unsigned char cycle2btab[] =
{
   0,255,255,
   8,255,255,
  16,255,255,
  24,255,255,
  32,255,255,
  40,255,255,
  48,255,255,
  56,255,255,
  64,255,255,
  72,255,255,
  80,255,255,
  88,255,255,
  96,255,255,
 104,255,255,
 112,255,255,
 120,255,255,
 128,255,255,
 136,255,255,
 144,255,255,
 152,255,255,
 160,255,255,
 168,255,255,
 176,255,255,
 184,255,255,
 192,255,255,
 200,255,255,
 208,255,255,
 216,255,255,
 224,255,255,
 232,255,255,
 240,255,255,
 248,255,255,
 255,255,255,
 255,255,255,
 248,255,255,
 240,255,255,
 232,255,255,
 224,255,255,
 216,255,255,
 208,255,255,
 200,255,255,
 192,255,255,
 184,255,255,
 176,255,255,
 168,255,255,
 160,255,255,
 152,255,255,
 144,255,255,
 136,255,255,
 128,255,255,
 120,255,255,
 112,255,255,
 104,255,255,
  96,255,255,
  88,255,255,
  80,255,255,
  72,255,255,
  64,255,255,
  56,255,255,
  48,255,255,
  40,255,255,
  32,255,255,
  24,255,255,
  16,255,255,
   8,255,255,
   0,255,255,
   0,255,255,
   8,255,255,
  16,255,255,
  24,255,255,
  32,255,255,
  40,255,255,
  48,255,255,
  56,255,255,
  64,255,255,
  72,255,255,
  80,255,255,
  88,255,255,
  96,255,255,
 104,255,255,
 112,255,255,
 120,255,255,
 128,255,255,
 136,255,255,
 144,255,255,
 152,255,255,
 160,255,255,
 168,255,255,
 176,255,255,
 184,255,255,
 192,255,255,
 200,255,255,
 208,255,255,
 216,255,255,
 224,255,255,
 232,255,255,
 240,255,255,
 248,255,255,
 255,255,255,
 255,255,255,
 248,255,255,
 240,255,255,
 232,255,255,
 224,255,255,
 216,255,255,
 208,255,255,
 200,255,255,
 192,255,255,
 184,255,255,
 176,255,255,
 168,255,255,
 160,255,255,
 152,255,255,
 144,255,255,
 136,255,255,
 128,255,255,
 120,255,255,
 112,255,255,
 104,255,255,
  96,255,255,
  88,255,255,
  80,255,255,
  72,255,255,
  64,255,255,
  56,255,255,
  48,255,255,
  40,255,255,
  32,255,255,
  24,255,255,
  16,255,255,
   8,255,255,
   0,255,255
};


unsigned char cycle2ctab[] =
{
   0,  0,  0,
   0,  0, 16,
   0,  0, 32,
   0,  0, 48,
   0,  0, 64,
   0,  0, 80,
   0,  0, 96,
   0,  0,112,
   0,  0,128,
   0,  0,144,
   0,  0,160,
   0,  0,176,
   0,  0,192,
   0,  0,208,
   0,  0,224,
   0,  0,240,
   0,  0,255,
   0, 16,255,
   0, 32,255,
   0, 48,255,
   0, 64,255,
   0, 80,255,
   0, 96,255,
   0,112,255,
   0,128,255,
   0,144,255,
   0,160,255,
   0,176,255,
   0,192,255,
   0,208,255,
   0,224,255,
   0,240,255,
   0,255,255,
  16,255,255,
  32,255,255,
  48,255,255,
  64,255,255,
  80,255,255,
  96,255,255,
 112,255,255,
 128,255,255,
 144,255,255,
 160,255,255,
 176,255,255,
 192,255,255,
 208,255,255,
 224,255,255,
 240,255,255,
 255,255,255,
 248,255,255,
 232,255,255,
 216,255,255,
 200,255,255,
 184,255,255,
 168,255,255,
 152,255,255,
 136,255,255,
 120,255,255,
 104,255,255,
  88,255,255,
  72,255,255,
  56,255,255,
  40,255,255,
  24,255,255,
   8,255,255,
   0,255,255,
   0,240,255,
   0,224,255,
   0,208,255,
   0,192,255,
   0,176,255,
   0,160,255,
   0,144,255,
   0,128,255,
   0,112,255,
   0, 96,255,
   0, 80,255,
   0, 64,255,
   0, 48,255,
   0, 32,255,
   0, 16,255,
   0,  0,255,
   0,  0,240,
   0,  0,224,
   0,  0,208,
   0,  0,192,
   0,  0,176,
   0,  0,160,
   0,  0,144,
   0,  0,128,
   0,  0,112,
   0,  0, 96,
   0,  0, 80,
   0,  0, 64,
   0,  0, 48,
   0,  0, 32,
   0,  0, 16,
   0,  0,  0,
   0,  0,  0,
   0,  0, 16,
   0,  0, 32,
   0,  0, 48,
   0,  0, 64,
   0,  0, 80,
   0,  0, 96,
   0,  0,112,
   0,  0,128,
   0,  0,144,
   0,  0,160,
   0,  0,176,
   0,  0,192,
   0,  0,208,
   0,  0,224,
   0,  0,240,
   0,  0,255,
   0, 16,255,
   0, 32,255,
   0, 48,255,
   0, 64,255,
   0, 80,255,
   0, 96,255,
   0,112,255,
   0,128,255,
   0,144,255,
   0,160,255,
   0,176,255,
   0,192,255,
   0,208,255,
   0,224,255,
   0,240,255,
   0,255,255,
  16,255,255,
  32,255,255,
  48,255,255,
  64,255,255,
  80,255,255,
  96,255,255,
 112,255,255,
 128,255,255,
 144,255,255,
 160,255,255,
 176,255,255,
 192,255,255,
 208,255,255,
 224,255,255,
 240,255,255,
 255,255,255,
 248,255,255,
 232,255,255,
 216,255,255,
 200,255,255,
 184,255,255,
 168,255,255,
 152,255,255,
 136,255,255,
 120,255,255,
 104,255,255,
  88,255,255,
  72,255,255,
  56,255,255,
  40,255,255,
  24,255,255,
   8,255,255,
   0,255,255,
   0,240,255,
   0,224,255,
   0,208,255,
   0,192,255,
   0,176,255,
   0,160,255,
   0,144,255,
   0,128,255,
   0,112,255,
   0, 96,255,
   0, 80,255,
   0, 64,255,
   0, 48,255,
   0, 32,255,
   0, 16,255,
   0,  0,255,
   0,  0,240,
   0,  0,224,
   0,  0,208,
   0,  0,192,
   0,  0,176,
   0,  0,160,
   0,  0,144,
   0,  0,128,
   0,  0,112,
   0,  0, 96,
   0,  0, 80,
   0,  0, 64,
   0,  0, 48,
   0,  0, 32,
   0,  0, 16,
   0,  0,  0
};


unsigned char cycle2dtab[] =
{
   0,  0,  0,
   0,  0, 32,
   0,  0, 64,
   0,  0, 96,
   0,  0,128,
   0,  0,160,
   0,  0,192,
   0,  0,224,
   0,  0,255,
  32, 32,255,
  64, 64,255,
  96, 96,255,
 128,128,255,
 160,160,255,
 192,192,255,
 224,224,255,
 255,255,255,
 224,224,255,
 192,192,255,
 160,160,255,
 128,128,255,
  96, 96,255,
  64, 64,255,
  32, 32,255,
   0,  0,255,
   0,  0,224,
   0,  0,192,
   0,  0,160,
   0,  0,128,
   0,  0, 96,
   0,  0, 64,
   0,  0, 32,
   0,  0,  0,
   0,  0, 32,
   0,  0, 64,
   0,  0, 96,
   0,  0,128,
   0,  0,160,
   0,  0,192,
   0,  0,224,
   0,  0,255,
  32, 32,255,
  64, 64,255,
  96, 96,255,
 128,128,255,
 160,160,255,
 192,192,255,
 224,224,255,
 255,255,255,
 224,224,255,
 192,192,255,
 160,160,255,
 128,128,255,
  96, 96,255,
  64, 64,255,
  32, 32,255,
   0,  0,255,
   0,  0,224,
   0,  0,192,
   0,  0,160,
   0,  0,128,
   0,  0, 96,
   0,  0, 64,
   0,  0, 32
};


unsigned char cycle3tab[] =
{
   0,  0,  0,
   0,  0, 16,
   0,  0, 32,
   0,  0, 48,
   0,  0, 64,
   0,  0, 80,
   0,  0, 96,
   0,  0,112,
   0,  0,128,
   0,  0,144,
   0,  0,160,
   0,  0,176,
   0,  0,192,
   0,  0,208,
   0,  0,224,
   0,  0,240,
   0,  0,255,
   0,  0,240,
   0,  0,224,
   0,  0,208,
   0,  0,192,
   0,  0,176,
   0,  0,160,
   0,  0,144,
   0,  0,128,
   0,  0,112,
   0,  0, 96,
   0,  0, 80,
   0,  0, 64,
   0,  0, 48,
   0,  0, 32,
   0,  0, 16,
   0,  0,  0,
   0,  0, 16,
   0,  0, 32,
   0,  0, 48,
   0,  0, 64,
   0,  0, 80,
   0,  0, 96,
   0,  0,112,
   0,  0,128,
   0,  0,144,
   0,  0,160,
   0,  0,176,
   0,  0,192,
   0,  0,208,
   0,  0,224,
   0,  0,240,
   0,  0,255,
   0,  0,240,
   0,  0,224,
   0,  0,208,
   0,  0,192,
   0,  0,176,
   0,  0,160,
   0,  0,144,
   0,  0,128,
   0,  0,112,
   0,  0, 96,
   0,  0, 80,
   0,  0, 64,
   0,  0, 48,
   0,  0, 32,
   0,  0, 16
};


unsigned char cycle3btab[] =
{
  96,  0,  0,
 106,  0,  0,
 116,  0,  0,
 126,  0,  0,
 136,  0,  0,
 146,  0,  0,
 156,  0,  0,
 166,  0,  0,
 176,  0,  0,
 186,  0,  0,
 196,  0,  0,
 206,  0,  0,
 216,  0,  0,
 226,  0,  0,
 236,  0,  0,
 246,  0,  0,
 255,  0,  0,
 246,  0,  0,
 236,  0,  0,
 226,  0,  0,
 216,  0,  0,
 206,  0,  0,
 196,  0,  0,
 186,  0,  0,
 176,  0,  0,
 166,  0,  0,
 156,  0,  0,
 146,  0,  0,
 136,  0,  0,
 126,  0,  0,
 116,  0,  0,
 106,  0,  0,
  96,  0,  0,
 106,  0,  0,
 116,  0,  0,
 126,  0,  0,
 136,  0,  0,
 146,  0,  0,
 156,  0,  0,
 166,  0,  0,
 176,  0,  0,
 186,  0,  0,
 196,  0,  0,
 206,  0,  0,
 216,  0,  0,
 226,  0,  0,
 236,  0,  0,
 246,  0,  0,
 255,  0,  0,
 246,  0,  0,
 236,  0,  0,
 226,  0,  0,
 216,  0,  0,
 206,  0,  0,
 196,  0,  0,
 186,  0,  0,
 176,  0,  0,
 166,  0,  0,
 156,  0,  0,
 146,  0,  0,
 136,  0,  0,
 126,  0,  0,
 116,  0,  0,
 106,  0,  0
};


unsigned char cycle3ctab[] =
{
  0,0,32,
  0,0,64,
  0,0,96,
  0,0,128,
  0,0,160,
  0,0,192,
  0,0,224,
  0,0,255,
  0,32,255,
  0,64,255,
  0,96,255,
  0,128,255,
  0,160,255,
  0,192,255,
  0,224,255,
  0,255,255,
  32,255,255,
  64,255,255,
  96,255,255,
  128,255,255,
  160,255,255,
  192,255,255,
  224,255,255,
  255,255,255,
  255,255,224,
  255,255,192,
  255,255,160,
  255,255,128,
  255,255,96,
  255,255,64,
  255,255,32,
  255,255,0,
  255,224,0,
  255,192,0,
  255,160,0,
  255,128,0,
  255,96,0,
  255,64,0,
  255,32,0,
  255,0,0,
  224,0,0,
  192,0,0,
  160,0,0,
  128,0,0,
  96,0,0,
  64,0,0,
  32,0,0,
  0,0,0,
  0,0,32,
  0,0,64,
  0,0,96,
  0,0,128,
  0,0,160,
  0,0,192,
  0,0,224,
  0,0,255,
  0,32,255,
  0,64,255,
  0,96,255,
  0,128,255,
  0,160,255,
  0,192,255,
  0,224,255,
  0,255,255,
  32,255,255,
  64,255,255,
  96,255,255,
  128,255,255,
  160,255,255,
  192,255,255,
  224,255,255,
  255,255,255,
  255,255,224,
  255,255,192,
  255,255,160,
  255,255,128,
  255,255,96,
  255,255,64,
  255,255,32,
  255,255,0,
  255,224,0,
  255,192,0,
  255,160,0,
  255,128,0,
  255,96,0,
  255,64,0,
  255,32,0,
  255,0,0,
  224,0,0,
  192,0,0,
  160,0,0,
  128,0,0,
  96,0,0,
  64,0,0,
  32,0,0,
  0,0,0
};


unsigned char cycle3dtab[] =
{
  32,0,0,
  64,0,0,
  96,0,0,
  128,0,0,
  160,0,0,
  192,0,0,
  224,0,0,
  255,0,0,
  255,32,32,
  255,64,64,
  255,96,96,
  255,128,128,
  255,160,160,
  255,192,192,
  255,224,224,
  255,255,255,
  255,224,224,
  255,192,192,
  255,160,160,
  255,128,128,
  255,96,96,
  255,64,64,
  255,32,32,
  255,0,0,
  224,0,0,
  192,0,0,
  160,0,0,
  128,0,0,
  96,0,0,
  64,0,0,
  32,0,0,
  0,0,0,
  32,0,0,
  64,0,0,
  96,0,0,
  128,0,0,
  160,0,0,
  192,0,0,
  224,0,0,
  255,0,0,
  255,32,32,
  255,64,64,
  255,96,96,
  255,128,128,
  255,160,160,
  255,192,192,
  255,224,224,
  255,255,255,
  255,224,224,
  255,192,192,
  255,160,160,
  255,128,128,
  255,96,96,
  255,64,64,
  255,32,32,
  255,0,0,
  224,0,0,
  192,0,0,
  160,0,0,
  128,0,0,
  96,0,0,
  64,0,0,
  32,0,0,
  0,0,0
};


unsigned char cycle3etab[] =
{
   0,  0, 32,
   0,  0, 64,
   0,  0, 96,
   0,  0,128,
   0,  0,160,
   0,  0,192,
   0,  0,224,
   0,  0,255,
  32, 32,255,
  64, 64,255,
  96, 96,255,
 128,128,255,
 160,160,255,
 192,192,255,
 224,224,255,
 255,255,255,
 224,224,255,
 192,192,255,
 160,160,255,
 128,128,255,
  96, 96,255,
  64, 64,255,
  32, 32,255,
   0,  0,255,
   0,  0,224,
   0,  0,192,
   0,  0,160,
   0,  0,128,
   0,  0, 96,
   0,  0, 64,
   0,  0, 32,
   0,  0,  0,
   0,  0, 32,
   0,  0, 64,
   0,  0, 96,
   0,  0,128,
   0,  0,160,
   0,  0,192,
   0,  0,224,
   0,  0,255,
  32, 32,255,
  64, 64,255,
  96, 96,255,
 128,128,255,
 160,160,255,
 192,192,255,
 224,224,255,
 255,255,255,
 224,224,255,
 192,192,255,
 160,160,255,
 128,128,255,
  96, 96,255,
  64, 64,255,
  32, 32,255,
   0,  0,255,
   0,  0,224,
   0,  0,192,
   0,  0,160,
   0,  0,128,
   0,  0, 96,
   0,  0, 64,
   0,  0, 32,
   0,  0,  0
};


unsigned char cycle4tab[] =
{
   0,  0,  0,
   0,  0, 16,
   0,  0, 32,
   0,  0, 48,
   0,  0, 64,
   0,  0, 80,
   0,  0, 96,
   0,  0,112,
   0,  0,128,
   0,  0,144,
   0,  0,160,
   0,  0,176,
   0,  0,192,
   0,  0,208,
   0,  0,224,
   0,  0,240,
   0,  0,255,
   0, 16,255,
   0, 32,255,
   0, 48,255,
   0, 64,255,
   0, 80,255,
   0, 96,255,
   0,112,255,
   0,128,255,
   0,144,255,
   0,160,255,
   0,176,255,
   0,192,255,
   0,208,255,
   0,224,255,
   0,240,255,
   0,255,255,
   0,255,255,
   0,240,255,
   0,224,255,
   0,208,255,
   0,192,255,
   0,176,255,
   0,160,255,
   0,144,255,
   0,128,255,
   0,112,255,
   0, 96,255,
   0, 80,255,
   0, 64,255,
   0, 48,255,
   0, 32,255,
   0, 16,255,
   0,  0,255,
   0,  0,240,
   0,  0,224,
   0,  0,208,
   0,  0,192,
   0,  0,176,
   0,  0,160,
   0,  0,144,
   0,  0,128,
   0,  0,112,
   0,  0, 96,
   0,  0, 80,
   0,  0, 64,
   0,  0, 48,
   0,  0, 32,
   0,  0, 16
};


void colorcycles(void)
{
  static UINT16 cycle1cnt = 0;
  static UINT16 cycle2cnt = 0;
  static UINT16 cycle3cnt = 0;
  static UINT16 cycle4cnt = 0;
  static UINT16 cycle5cnt = 0;

  static UINT16 glowcnt1 = 0;
  static UINT16 glowcnt2 = 80;
  static UINT16 glowcnt3 = 0;
  static UINT16 glowcnt4 = 200;

  if (world == 3)   // not world 4? then go....
  {
    if (easiervisualsflg)
    {
      static UINT16 awesome_pal_tick;
      if ((awesome_pal_tick++ % 5) != 0)
        return;
    }

    if (streepflg)
	{
		UINT16 i;

      for (i=0; i<768; i++)
	  {
		  if (gamepal[i] != 255) gamepal[i]++;;
	  }
	  video->palette(gamepal);
 	  return;
	}

    glowcnt1++;
    glowcnt1&=63;

    cycle1cnt++;
    cycle1cnt&=15;

    cycle2cnt--;
    cycle2cnt&=31;

    cycle4cnt++;
    cycle4cnt&=63;

    cycle5cnt++;
    if (cycle5cnt ==48) cycle5cnt = 0;


    gamepal[(14*3)] = cycle3btab[glowcnt1*3];
    gamepal[(14*3)+1] = cycle3btab[glowcnt1*3+1];
    gamepal[(14*3)+2] = cycle3btab[glowcnt1*3+2];

    gamepal[(15*3)] = cycle3btab[((glowcnt1+16)&63)*3+2];
    gamepal[(15*3)+1] = cycle3btab[((glowcnt1+16)&63)*3+1];
    gamepal[(15*3)+2] = cycle3btab[((glowcnt1+16)&63)*3];

    if (paraflg == 2 || paraflg == 3)
      {
        cycleit(3*16, 16, cycle1cnt, cycle1tab);
      }

    if (paraflg == 3)
      {
        cycleit(7*16, 32, cycle2cnt, cycle2dtab);
        cycleit(9*16, 32, 0, cycle3tab);    //paralax backgrnd
      }

    if (paraflg == 4 || paraflg == 5 || paraflg == 6 || paraflg == 7 || paraflg == 8)
      {
        cycleit(3*16, 16, cycle1cnt, cycle1tab);
        cycleit(7*16, 32, cycle2cnt, cycle2dtab);
        cycleit(9*16, 32, 0, cycle3tab);    //paralax backgrnd
//        cycleit(14*16, 32, cycle4cnt>>1, cycle3btab);

//        cycleit(12*16, 32, cycle5cnt, cycle3ctab);
      }

    if (paraflg == 8 || paraflg == 7)
	{
        cycleit(12*16, 32, cycle4cnt/2, cycle3dtab);
        cycleit(14*16, 32, cycle4cnt>>1, cycle3etab);
	}
	else
	{
      if (paraflg == 4 || paraflg == 5 || paraflg == 6)
      {
        cycleit(14*16, 32, cycle4cnt>>1, cycle3btab);
        cycleit(12*16, 32, cycle5cnt, cycle3ctab);
      }
	}
		
    if (paraflg == 4 || paraflg == 3)
      {
        gentab3();
      }

    if (paraflg == 5 || paraflg == 6 || paraflg == 8)
      {
        gentab3b();
      }

    video->palette_index(gamepal+(14*3), 14, 242);
//    video->palette(gamepal);
  }

  if ((world == 0) || (world == 1) || (world == 4))   // not world 1? then go....
  {
    BYTE glows[12];

    glows[0] = 0;
    glows[1] = 0;
    glows[3] = 0;
    glows[4] = 0;
    glows[6] = 0;
    glows[7] = 0;
    glows[9] = 0;
    glows[10] = 0;

    glowcnt1++;
    glowcnt1&=511;
    glowcnt2+=2;
    glowcnt2&=511;
    glowcnt3+=3;
    glowcnt3&=511;
    glowcnt4+=4;
    glowcnt4&=511;

    glows[2]  = cycle4tab[((glowcnt1/8)*3)+2];
    glows[5]  = cycle4tab[((glowcnt2/8)*3)+2];
    glows[8]  = cycle4tab[((glowcnt3/8)*3)+2];
    glows[11] = cycle4tab[((glowcnt4/8)*3)+2];

    glows[1]  = cycle4tab[((glowcnt1/8)*3)+1];
    glows[4]  = cycle4tab[((glowcnt2/8)*3)+1];
    glows[7]  = cycle4tab[((glowcnt3/8)*3)+1];
    glows[10] = cycle4tab[((glowcnt4/8)*3)+1];

    glows[0]  = cycle4tab[((glowcnt1/8)*3)+1];
    glows[3]  = cycle4tab[((glowcnt2/8)*3)+1];
    glows[6]  = cycle4tab[((glowcnt3/8)*3)+1];
    glows[9]  = cycle4tab[((glowcnt4/8)*3)+1];

    video->palette_index(glows, 240, 4);
  }


  if ((world == 2))   // not world 2? then go....
  {
    BYTE glows[48];

    glowcnt1--;
    glowcnt1&=31;

    memcpy(glows, glowriginal+(glowcnt1*3), 48);

    video->palette_index(glows, 240, 16);
  }
}

void oogknippen()
{
  oogblinkcnt++;
  oogblinkcnt&=511;

  if (streepflg) return;

  if (oogblinkanim[oogblinkcnt/2] == 0)
  {
    video->palette_index(oogopen, 1, 6);
    memcpy(&gamepal[3],oogopen,18);
  }
  if (oogblinkanim[oogblinkcnt/2] == 1)
  {
    video->palette_index(ooghalf, 1, 6);
    memcpy(&gamepal[3],ooghalf,18);
  }
  if (oogblinkanim[oogblinkcnt/2] == 2)
  {
    video->palette_index(oogdicht, 1, 6);
    memcpy(&gamepal[3],oogdicht,18);
  }
}


void gentab3(void)
{
  static UINT16 sincnt1;
  static UINT16 sincnt2;
  static UINT16 sincnt3;
  static UINT16 sincnt4;

  UINT16 sinbcnt1;
  UINT16 sinbcnt2;
  UINT16 sinbcnt3;
  UINT16 sinbcnt4;

  UINT16 temp;
  UINT16 i;
  INT16 ampie;

  unsigned char *tabbie;

  tabbie = cycle3tab;

  sincnt1-=6;
  sincnt2+=2;
  sincnt3-=1;
  sincnt4-=8;

  sincnt1&=1023;
  sincnt2&=1023;
  sincnt3&=1023;
  sincnt4&=1023;
  
  sinbcnt1 = sincnt1;
  sinbcnt2 = sincnt2;
  sinbcnt3 = sincnt3;
  sinbcnt4 = sincnt4;

  ampie = ((sinus512[(sinbcnt3)       & 1023] * 64) >> 10)+64;

  for (i=0; i<32; i++)
    {
      *(tabbie+2) = ((sinus512[(sinbcnt1)       & 1023] * 127) >> 10)+128;
      *(tabbie+1) = 0;
      *(tabbie) = ((sinus512[(sinbcnt2)       & 1023] * ampie) >> 10)+(ampie);


      tabbie+=3;

      sinbcnt1+=32;
      sinbcnt2+=64;
      sinbcnt3+=64;
      sinbcnt4-=8;
      
      sinbcnt1&=1023;
      sinbcnt2&=1023;
      sinbcnt3&=1023;
      sinbcnt4&=1023;
    }

  temp = ((sinus512[(sincnt1)       & 1023] * 255) >> 10);

}

void gentab3b(void)
{
  static UINT16 sincnt1;
  static UINT16 sincnt2;
  static UINT16 sincnt3;
  static UINT16 sincnt4;

  UINT16 sinbcnt1;
  UINT16 sinbcnt2;
  UINT16 sinbcnt3;
  UINT16 sinbcnt4;

  UINT16 temp;
  UINT16 i;

  unsigned char *tabbie;

  tabbie = cycle3tab;

  sincnt1+=63;
  sincnt2+=65;
  sincnt3-=8;
  sincnt4-=34;

  sincnt1&=1023;
  sincnt2&=1023;
  sincnt3&=1023;
  sincnt4&=1023;
  
  sinbcnt1 = sincnt1;
  sinbcnt2 = sincnt2;
  sinbcnt3 = sincnt3;
  sinbcnt4 = sincnt4;

  for (i=0; i<32; i++)
    {
            *(tabbie  ) = ((sinus512[(sinbcnt1)       & 1023] * 127) >> 10)+128;
       *(tabbie+1) = ((sinus512[(sinbcnt2)       & 1023] * 255) >> 10)+128;

      temp      = ((sinus512[(sinbcnt3)       & 1023] * 127) >> 10)+128;
      temp += ((sinus512[(sinbcnt4)       & 1023] * 127) >> 10)+128;
      temp >>=1;
            *(tabbie+2) = (unsigned char) temp;

      tabbie+=3;

      sinbcnt1+=122;
      sinbcnt2+=64;
      sinbcnt3+=2;
      sinbcnt4-=8;
      
      sinbcnt1&=1023;
      sinbcnt2&=1023;
      sinbcnt3&=1023;
      sinbcnt4&=1023;
    }

  temp = ((sinus512[(sincnt1)       & 1023] * 255) >> 10);

}


void cycleit(UINT16 startcnt, UINT16 nrofcols, UINT16 cyclepos,  unsigned char *cycletab)
{
  unsigned char *src;
  unsigned char *dest;

  src =  &cycletab[cyclepos*3];
  dest = &gamepal[startcnt*3];
  
  while(nrofcols--)
    {
      *dest++ = *src++;
      *dest++ = *src++;
      *dest++ = *src++;
    }
}


void paraballetjes(void)
{
  Cspr_frame *frame;
  UINT16 i;

  static UINT16 col;

  static UINT16 sa1x,sa2x,sa3x,sa1y,sa2y,sa3y;
  static UINT16 sb1x,sb2x,sb3x,sb1y,sb2y,sb3y;

  if (world != 3 ) return;
  if (paraflg < 6 ) return;


  col++;
  col&=31;

  sa1x+=3;
  sa2x+=8;
  sa3x+=12;
  sa1y+=4;
  sa2y+=9;
  sa3y+=13;

  sa1x &=1023;
  sa2x &=1023;
  sa3x &=1023;
  sa1y &=1023;
  sa2y &=1023;
  sa3y &=1023;

  sb1x = sa1x;
  sb2x = sa2x;
  sb3x = sa3x;
  sb1y = sa1y;
  sb2y = sa2y;
  sb3y = sa3y;

  frame = anim_setsequence(orglev4bal, 0, SEQ_FORCE);

  for (i=0; i<32; i++)
    {
      UINT16 x,y;
      
      sb1x+=32;
      sb2x+=4;
      sb3x+=6;
      sb1y+=32;
      sb2y+=10;
      sb3y+=11;
      
      sb1x &=1023;
      sb2x &=1023;
      sb3x &=1023;
      sb1y &=1023;
      sb2y &=1023;
      sb3y &=1023;

      x =  ((sinus512[(sb1x)       & 1023] * 256) >> 10);
      x += ((sinus512[(sb2x)       & 1023] * 256) >> 10);
      x += ((sinus512[(sb3x)       & 1023] * 256) >> 10);
      
      y =  ((sinus512[(sb1y)       & 1023] * 256) >> 10);
      y += ((sinus512[(sb2y)       & 1023] * 256) >> 10);
      y += ((sinus512[(sb3y)       & 1023] * 256) >> 10);

      x+=320;
      y+=240;

      frame = anim_forceframe (orglev4bal, col);
      frame->draw(*para, x, y );
    }


}


HEARTBEAT_FN MC_prepentername(void)
{
  UINT16 curlevel;
      
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();

  killlevel();

  if (world == 4)
  {
    world = 0;
	level = 0;
    return (HEARTBEAT_FN) MC_preptitlesequence;
  }

  curlevel = world*4+level;

  blacksperlevel[curlevel] = 0;           // if you die... the last black diamonds aren't remembered!
//  scoreblacksperlevel[curlevel] = 0;    // if you die... the last black diamonds are added to your score!

  movieleavefunc = (HEARTBEAT_FN) MC_preptitlesequence;

#ifdef MOVIES

  if (startmovie("gameover.smk", "assets\\frmgamov.pcx") == 0)
  {
	  return 0;
  }
  return (HEARTBEAT_FN) MC_playingmovie;
#endif

  return (HEARTBEAT_FN) movieleavefunc;
}

HEARTBEAT_FN MC_prepentername2(void)
{
  UINT16 curlevel;
      
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();
  video->clear(0);
  video->swap();
  video->scansync();

  killlevel();

  curlevel = world*4+level;

  blacksperlevel[curlevel] = 0;           // if you die... the last black diamonds aren't remembered!
//  scoreblacksperlevel[curlevel] = 0;    // if you die... the last black diamonds are added to your score!


  return (HEARTBEAT_FN) MC_preptitlesequence;
}


void grabber()
{
  static char *grabname = "c:\\mc\\grab1368.pcx";

#ifdef GRABBER
  if (keytab['G'] == 1)
    {
	    keytab['G'] = 0;
		grabflg ^= 1;
	}
	  
	if (grabflg)
	{
	  
//            exitflg = 1;
//            musicstoppedflg = 1;
            player1.loadedmap->blitbuf->save_pcx(grabname, gamepal);
            log_out("grabbed and saved the file: '%s'", grabname);
            *(grabname+13) += 1;
			if (*(grabname+13) == 0x3a)
			{
				*(grabname+13) = 0x30;
	            *(grabname+12) += 1;
				if (*(grabname+12) == 0x3a)
				{
					*(grabname+12) = 0x30;
		            *(grabname+11) += 1;
					if (*(grabname+11) == 0x3a)
					{
						*(grabname+11) = 0x30;
						*(grabname+10) += 1;
					}
				}
			}
    }
#endif
}

void grabber2()
{
  static char *grabname = "c:\\mc\\grab1368.pcx";

#ifdef GRABBER
  if (keytab['G'] == 1)
    {
	    keytab['G'] = 0;
		grabflg ^= 1;
	}
	  
	if (grabflg)

	{
//            exitflg = 1;
//            musicstoppedflg = 1;
            log_out("returning palette");
            movie->returnpal(dummypal);
            log_out("palette returned");
            vidblitbuf->save_pcx(grabname, dummypal);
            log_out("grabbed and saved the file: '%s'", grabname);
            *(grabname+13) += 1;
			if (*(grabname+13) == 0x3a)
			{
				*(grabname+13) = 0x30;
	            *(grabname+12) += 1;
				if (*(grabname+12) == 0x3a)
				{
					*(grabname+12) = 0x30;
		            *(grabname+11) += 1;
					if (*(grabname+11) == 0x3a)
					{
						*(grabname+11) = 0x30;
						*(grabname+10) += 1;
					}
				}
			}
    }
#endif
}


void mc_timer(UINT16 timerids)
{
  // ***DEMO 

  musicstoppedflg = 1;        // set on 0 yourself if you need it

//  log_out("music timer activated");

  if (timerids != timerid)
  {
//    log_out("Timer-id's don't match... should be impossible");
  }

  if (musicrestartflg) start_cd();


}


void framework_EventHandle(int event, int param)
{
  mouselchng=0;
  mouserchng=0;
  switch(event)
  {
      case FW_CDREADY:
        if (musicflg) start_cd();
        break;
      case FW_KEYDOWN:
        keytab[param]=1;
        lastkey = param;
        break;
      case FW_KEYUP:
        keytab[param]=0;
        lastkey = 0;
        break;
      case FW_LBUTTONUP:
        mouselbut = 0;
        mouselchng = 1;
        break;
      case FW_RBUTTONUP:
        mouserbut = 0;
        mouserchng = 1;
        break;
      case FW_LBUTTONDOWN:
        mouselbut = 1;
        mouselchng = 1;
        break;
      case FW_RBUTTONDOWN:
        mouserbut = 1;
        mouserchng = 1;
        break;
      case FW_MOUSEMOVE:
//        mousex = param & 0xffff;
//        mousey = (param>>16) & 0xffff;
//        if (mousex != 320 || mousey != 240)
//        {
//          framework_util_SetMouse(320,240);
//        }
        break;
  }
}


void framework_ExitGame(void)
{

  log_out("game exitting");


  stop_cd();

  if (levelloadedflg)
    {
//      log_out("last position:   Manx: %d(%x),   Many: %d(%x)\n", hoi->x, hoi->x, hoi->y, hoi->y);
//      log_out("last position: Worldx: %d, Worldy: %d\n", player1.worldx, player1.worldy);
//      log_out("record pos tot %d", recpnt-recbuf);
    }


// KILL resources of title-sequence

  killsequence();

// KILL LEVEL IF IT'S STILL BUSY

  log_out("killing level");

  killlevel();

  log_out("level killed");

// Clean up movies which where still playing

  if (mcsmk)
    {
      movie->close(mcsmk);
      mcsmk = 0;
      delete movbuf;
      delete movkaderbuf;
      movbuf = 0;
      movkaderbuf = 0;
    }

// kill puzzle pieces
    
  killpuzzle();

// SAVE LEVELMAP

//  savelevelmap();


// save highscore and options

  orgdir();
  save_highscores();
  save_options();

  
  // SAVE RECORDED PLAY

  rec_savemap();

// GLOB_INIT
  free(dotbase);
  free(dotcolbase);

#ifdef FULLVERSION
  free(fullversion);
#endif

// VIDEO SCREEN

  delete vidblitbuf;

// DISABLE FASTFILE

  disablefastfile();
}


void configure_level(int worldnr, int levelnr)
{
	
  world = worldnr-1;
  level = levelnr-1;

  if (world != 4)   // demo level?
  {  
    if (maxlevel < world*4+level)
    {
      maxlevel = world*4+level;
//      log_out("Maxlevel has become %d",maxlevel);
    }
  }

// CREATE SPACE FOR VIRTUAL BITMAPS & MAP & LOAD BACKGROUND GFX

  player1.loadedmap->mapsizex = levels[world][level].mapsizex;
  player1.loadedmap->mapsizey = levels[world][level].mapsizey;
  player2.loadedmap->mapsizex = levels[world][level].mapsizex;
  player2.loadedmap->mapsizey = levels[world][level].mapsizey;

// init triggerlist

  triglist = levels[world][level].triglist;
}


void rec_savemap(void)
{
 return;
  if (recplay)
  {
    UINT16 rc;
    if (playflg == 0)
    {
      *(recpnt++) = 0xff; // end of play marker
    
      rc = savedocfile("mcrecord.buf", (char *) recbuf, RECTIME*50 );
      if (rc)
      {
        log_out("Autoplay saved correctly");
      }
      else
      {
        log_out("Autoplay not saved...!");
      }
    }
  }  
}


void rec_prepmap(void)
{
  if (recplay)
    {
      UINT16 rc;

      recflg = 1;  // use replayer or recorder!
      recbuf = (BYTE *) malloc((RECTIME*50) + 100);
      rc = loaddocfile("mcrecord.buf", (char *) recbuf, RECTIME*50);
      recpnt = recbuf;
      playflg = 0;          // lets record
      if (rc) playflg = 1;
    }
}

void recorder(void)
{
  BYTE recbyte;

  if (recplay)
  {
    if (recflg == 0) return;
    if (playflg)
    {
    // extract key info uit recorded byte
    
      recbyte = *(recpnt++);
    
      if (shootkey == 1)        // someone pressed shoot during playback? let's end!
      {
        exitflg = 1;
      }
      leftkey  = 0;
      rightkey = 0;
      upkey    = 0;
      downkey  = 0;
      jumpkey  = 0;
      shootkey = 0;
    
      if(recbyte == 0xff)  // end of play marker encountered
      {
        exitflg = 1;
        return;
      }
    
      if(recbyte & 1)  leftkey  = 1;
      if(recbyte & 2)  rightkey = 1;
      if(recbyte & 4)  { jumpkey = 1; upkey = 1; }
      if(recbyte & 8)  downkey  = 1;
      if(recbyte & 16) shootkey = 1;      
    }
    else
    {
    // make recbyte and record 'm
    
      recbyte = 0;
      if(leftkey)  recbyte |= 1;
      if(rightkey) recbyte |= 2;
      if(jumpkey || upkey) recbyte |= 4;
      if(downkey)  recbyte |= 8;
      if(shootkey) recbyte |= 16;
    
      *(recpnt++) = recbyte;
    }
  }
}


void peekabooh(void)
{
  if (world == 3)
    {
      HOI_BLOOD *hoiblood;
      static int peekcnt = 450;
      leftkey  = 1;
      rightkey = 0;
      upkey    = 0;
      downkey  = 0;
      jumpkey  = 0;
      shootkey = 0;

      hoiblood = (HOI_BLOOD *) hoi->blood;

      hoiblood->curdirx = -1;
      hoiblood->lastdirx = -1;
      
      peekcnt--;

      if (peekcnt > 0)
  {
    player1.worldx = 4703;
    player1.worldy = 222;
    hoi->x = 5024;
    hoi->y = 426;
    paraflg = 3;
  }
      if (peekcnt > 150)
  {
    player1.worldx = 2568;
    player1.worldy = 134;
    hoi->x = 2976;
    hoi->y = 522;
    paraflg = 2;
  }
      if (peekcnt > 300)
  {
    player1.worldx = 3362;
    player1.worldy = 674;
    hoi->x = 3648;
    hoi->y = 906;
    paraflg = 2;
  }
      if (peekcnt == 0) exitflg = 1;
    }
}


void anim_init(void)
{

#if 0
  sprcomp_clear_transparant();
  sprcomp_set_transparant(0);
#endif

  allframes    = vg_dll_create(1);
  allsequences = vg_dll_create(1);
  allanims     = vg_dll_create(1);
  
	if (!asset_parse(FullPath(levels[world][level].descr)))
    {
      printf("Error parsing level definition file\n");
      exit(0);
    }
}


void makeanimglobal(void)
{

// put all anims of all levels in here! they will be 0 when not found (and used)
    
  orgbird = anim_retrieveanim("bird"); //search for animpoint to bird in assets
  orgcloud = anim_retrieveanim("skidcloud");
  orghoi = anim_retrieveanim("moonchild");
  orghoirise = anim_retrieveanim("moonrise");
  orgbouncey = anim_retrieveanim("bullet1");
  orgbump = anim_retrieveanim("bump");
  orgelevat = anim_retrieveanim("platform");
  orglift = anim_retrieveanim("lift");
  orgspies = anim_retrieveanim("spies");
  orgplof = anim_retrieveanim("explosion");
  orgptoei = anim_retrieveanim("ptoei");
  orgrobyn = anim_retrieveanim("robyn");
  orgbat = anim_retrieveanim("bat");
  orgsnake = anim_retrieveanim("snake");
  orgwarp = anim_retrieveanim("warp");
  orgswitsj = anim_retrieveanim("switch");
  orgwall = anim_retrieveanim("wall");
  orgbonus = anim_retrieveanim("bonus");
  orgscore = anim_retrieveanim("cyfers");
  orgelf = anim_retrieveanim("elf");
  orgbomb = anim_retrieveanim("bomb");
  orggumbal = anim_retrieveanim("gumbal");
  orgbee = anim_retrieveanim("bee");
  orgpunt = anim_retrieveanim("punt");
  orgpneu = anim_retrieveanim("pneumatic");
  orgbelt = anim_retrieveanim("convbelt");
  orghek = anim_retrieveanim("hek");
  orgwater = anim_retrieveanim("water");
  orgstukhout = anim_retrieveanim("stukhout");
  orgironring = anim_retrieveanim("ironring");
  orgironauto = anim_retrieveanim("autoring");
  orgrain = anim_retrieveanim("rain");
  orgweight = anim_retrieveanim("weight");
  orgmixer = anim_retrieveanim("mixer");
  orgheart = anim_retrieveanim("heart");
  orgdiamond = anim_retrieveanim("diamond");
  orgcolormond = anim_retrieveanim("colormond");
  orgfire = anim_retrieveanim("fire");
  orgrock = anim_retrieveanim("rock");
  orgblubber = anim_retrieveanim("blubber");
  orgchainbal = anim_retrieveanim("chainbal");
  orgbiels = anim_retrieveanim("biels");
  orghook = anim_retrieveanim("hook");
  orgshoes = anim_retrieveanim("shoes");
  orghelmut = anim_retrieveanim("helmut");
  orgjudith = anim_retrieveanim("judith");
  orgparpat = anim_retrieveanim("parpat");
  orgspike = anim_retrieveanim("spike");
  orgfan = anim_retrieveanim("fan");
  orgslijk = anim_retrieveanim("slijk");
  orgparapats = anim_retrieveanim("parapats");
  orgtechnobee = anim_retrieveanim("technobee");
  orgbullet1 = anim_retrieveanim("bullet1");
//  orgbullet2 = anim_retrieveanim("bullet2");
//  orgbullet3 = anim_retrieveanim("bullet3");
  orgrocketup = anim_retrieveanim("rocketup");
  orgsegment = anim_retrieveanim("segment");
  orgmine = anim_retrieveanim("mine");
  orgcannon = anim_retrieveanim("cannon");
  orgbolt = anim_retrieveanim("bolt");
  orgseamine = anim_retrieveanim("seamine");
  orggoodday = anim_retrieveanim("goodday");
  orgsokoban = anim_retrieveanim("sokoban");
  orgmushroom = anim_retrieveanim("mushroom");
  orgmouth = anim_retrieveanim("mouth");
  orgplant = anim_retrieveanim("plant");
  orgbarrel = anim_retrieveanim("barrel");
  orgwheel = anim_retrieveanim("wheel");
  orgmetbal = anim_retrieveanim("metbal");
  orgmetcan = anim_retrieveanim("metcan");
  orgsmallheart = anim_retrieveanim("smallheart");
  orglev4objects = anim_retrieveanim("lev4objects");
  orglev4bal = anim_retrieveanim("lev4bal");
  orgendoflevel = anim_retrieveanim("endoflevel");
  orgpaal = anim_retrieveanim("paal");
  orgbbot = anim_retrieveanim("brainbot");
  orgdooldoler = anim_retrieveanim("dooldoler");
  orgheatsneaker = anim_retrieveanim("heatsneaker");
  orgasteroids = anim_retrieveanim("asteroids");
  orgclaw = anim_retrieveanim("claw");
  orgglim = anim_retrieveanim("glim");
}

void prep_engine(void)
{
  int rc;
  int y;

  mc_autorun = 0;
  nextlvlflg = 0;
  lastkey = 0;

  log_out("alloced map");
  player1.loadedmap->map = (UINT16 *) malloc(player1.loadedmap->mapsizex*(player1.loadedmap->mapsizey+32)/(32*32) * sizeof(UINT16));
  player2.loadedmap->map = player1.loadedmap->map;

  log_out("clearing map");

  for (int k=0; k<player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(32*32); k++)
  {
    player1.loadedmap->map[k] = 0;
  }

  log_out("creating blitbufs");
  player1.loadedmap->blitbuf  = vidblitbuf;
  player2.loadedmap->blitbuf  = vidblitbuf;

  player1.loadedmap->blitbuf->set_clipping(cliptlx, cliptly, clipbrx, clipbry);

//  log_out("blitbuf1=%8x",player1.loadedmap->blitbuf);
//  log_out("blitbuf2=%8x",player2.loadedmap->blitbuf);

  para   = new Cblitbuf(levels[world][level].parallax, 0, 0);

  para->debug();
  
  patsmap->blitbuf   = 0; //new Cblitbuf(levels[world][level].patspic, 0, 0);
  patsmap->map       = (UINT16 *) malloc(patsmap->mapsizex*(patsmap->mapsizey+32)/(32*32) * sizeof(UINT16));
  memset(patsmap->map, 0, patsmap->mapsizex*(patsmap->mapsizey+32)/(32*32) * sizeof(UINT16));
//  log_out("patsbuf=%8x",patsmap->blitbuf);

  // Alloc collision map for pattern interaction
  colmap = (unsigned char *) malloc(player1.loadedmap->mapsizex*(player1.loadedmap->mapsizey+32)/(16*16) * sizeof(unsigned char));
  memset (colmap, 0, player1.loadedmap->mapsizex*(player1.loadedmap->mapsizey+32)/(16*16) * sizeof(unsigned char));

// filling in dummy map for editor-map

  for (y=0; y<patsmap->mapsizex*(patsmap->mapsizey+32)/(32*32); y++)
  {
    patsmap->map[y]=y;
  }
  
// LOAD MAPDATA AND DRAW LEVEL

  rc = loadfile(levels[world][level].map, (char *) player1.loadedmap->map, player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(32*32) *sizeof(UINT16));
  rc = loadfile(levels[world][level].colmap,(char *)colmap, player1.loadedmap->mapsizex*(player1.loadedmap->mapsizey+32)/(16*16) * sizeof(unsigned char));
  player1.curmap = player1.loadedmap;
  player2.curmap = player2.loadedmap;

//  map_build(&player1);
//  map_build(&player2);

// LOAD PATTERNS WHICH ARE USED IN MAP

  load_patterns(levels[world][level].patspic);
}


void load_patterns(char *patternpic)
{
  VG_BOOLEAN rc;
  UINT16 w;
  UINT16 h;
  UINT32 i;
  UINT16 actused;

  unsigned char *tijdbuf;
  
  UINT32 *priotab;            //pointer to table with pattern numbers and their priority
                              // which are both an int (so its: int int*)
//  vgassert(patternpic);

  rc = PCX_ff_stat(patternpic, &w, &h);
//  vgassert(rc);

//  log_out("Now loading patternpic-file \"%s\"", patternpic);
//  log_out("pcx is %d by %d",w,h);

  patsmap->mapsizey = h-128;    // fill in real size of patpic

  tijdbuf = (unsigned char *)malloc(w*h);
//  vgassert(tijdbuf);

  rc = PCX_ff_load(patternpic, tijdbuf, NULL);
//  vgassert(rc);

  actused = 0;
  nrofpats = ((w*h)/(32*32));

//  log_out("%d patterns in pattern picture",nrofpats);

  // Allocate transparency cross-reference map
  transmap = (char *)malloc(nrofpats);
  memset(transmap, 0, nrofpats);

  patterntab = (Cblitbuf **) malloc(nrofpats * sizeof(Cblitbuf *));
//  vgassert(patterntab);

  log_out("Optimising used patterns in order of nroftimes used");

  priotab = (UINT32 *)malloc(nrofpats * 8);
//  vgassert(priotab);

//  log_out("priotab = %d",priotab);

// Initialise tabels

  for (i=0; i<nrofpats; i++)
  {
    patterntab[i] = 0;          // pattern blitbufs on 0
    priotab[i*2]  = i;          // pattern number
    priotab[i*2+1]  = 0;          // all the same priority
  }

  log_out("clear prio");

// ADD HERE CODE WHICH UPDATES THE PRIOTAB ACCORDING TO THE NUMBER OF TIMES
// A PATTERN IS USED... BASICALLY A HISTOGRAM

  for (i=0; i<20; i++)
  {
    priotab[(i*2)+1] = 1;
  }

#ifdef DISCARD_UNUSED_PATS

  for (i=0; i<(UINT32)(player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(32*32)); i++)
  {
    UINT16 patnr;
    patnr = player1.loadedmap->map[i];
    if (patnr >= nrofpats)
    {
      patnr = 0;    // pattern used which doesn't exist
      player1.loadedmap->map[i] = patnr;
    }

    priotab[(patnr*2)+1]++;    // heighten histogram
  }
#endif


  // magically appearing floor we'll mark off here!

  if (world == 0 && level == 3)
    {
      log_out("magic floors afkrassen");
      priotab[(1020*2)+1]++;
      priotab[(1021*2)+1]++;
      priotab[(1022*2)+1]++;
      priotab[(1023*2)+1]++;
      priotab[(1024*2)+1]++;
      priotab[(1025*2)+1]++;
      priotab[(1026*2)+1]++;
      priotab[(1027*2)+1]++;
      priotab[(1028*2)+1]++;
      priotab[(1029*2)+1]++;
      priotab[(1030*2)+1]++;
    }

  if (world == 3 && level == 0)
    {
      log_out("colorpatterns afkrassen");
      priotab[(300*2)+1]++;
      priotab[(301*2)+1]++;
      priotab[(302*2)+1]++;
      priotab[(303*2)+1]++;
      priotab[(304*2)+1]++;
      priotab[(305*2)+1]++;
      priotab[(306*2)+1]++;
      priotab[(307*2)+1]++;
      priotab[(308*2)+1]++;
      priotab[(309*2)+1]++;
      priotab[(310*2)+1]++;
      priotab[(311*2)+1]++;
      priotab[(312*2)+1]++;
      priotab[(313*2)+1]++;
      priotab[(314*2)+1]++;
      priotab[(315*2)+1]++;
      priotab[(316*2)+1]++;
      priotab[(317*2)+1]++;
      priotab[(318*2)+1]++;
      priotab[(319*2)+1]++;
      priotab[(320*2)+1]++;
      priotab[(321*2)+1]++;
      priotab[(322*2)+1]++;
      priotab[(323*2)+1]++;
      priotab[(324*2)+1]++;
      priotab[(325*2)+1]++;
      priotab[(326*2)+1]++;
      priotab[(327*2)+1]++;
      priotab[(328*2)+1]++;
      priotab[(329*2)+1]++;
      priotab[(330*2)+1]++;
      priotab[(331*2)+1]++;
      priotab[(332*2)+1]++;
  }
      // ADD HERE THE CODE WHICH SORTS THE PRIOTAB

#ifdef DISCARD_UNUSED_PATS
  log_out("Discarding unused patterns");
#endif

  for (i=0; i<nrofpats; i++)
  {
#ifdef DISCARD_UNUSED_PATS
//    if (priotab[i*2+1] != 0 || world == 3)  // pattern not used? let's discard it
    if (priotab[i*2+1] != 0 )  // pattern not used? let's discard it
    {
#endif
      unsigned char  *dest;
      unsigned char  *src;
      UINT16 pitch;
      UINT16 x,y;
      UINT16 curpat;
      UINT16 emptyflg, fullflg;

      curpat = priotab[i*2];

//      log_out("pattern #%d = %d",i,curpat);

      actused++;  // count used pattern

      patterntab[curpat] = new Cblitbuf(32,32,0,0);
//      vgassert(patterntab[curpat]);

      patterntab[curpat]->debug();

      dest = patterntab[curpat]->lock_buffer();
//      vgassert(dest);
      pitch = patterntab[curpat]->get_pitch();

      src = (unsigned char *) tijdbuf;

      src += ((curpat/(w/32))*32*w);
      src += ((curpat%(w/32))*32);

      emptyflg = 1;  // pattern is completely empty
      fullflg  = 1;  // pattern is completely full

      for (y=0; y<32; y++)
      {
          for (x=0; x<32; x++)
          {
              if (*(src))
              {
                emptyflg = 0;
              }
              else
              {
                fullflg = 0;
              }
              *(dest++) = *(src++);
          }
          src  += (w-32);
          dest += (pitch-32);
      }

      if (emptyflg == 1){ transmap[curpat] = 0; log_out("empty");}
      if (fullflg == 0 && emptyflg == 0){ transmap[curpat] = 1;  log_out("half");}
      if (fullflg == 1){ transmap[curpat] = 2;  log_out("full");}

      patterntab[curpat]->unlock_buffer();
#ifdef DISCARD_UNUSED_PATS
    }
#endif

  // magically appearing floor we'll mark off here!

  if (world == 0 && level == 3)
    {
      transmap[1020] = 1;
      transmap[1021] = 1;
      transmap[1022] = 1;
      transmap[1023] = 1;
      transmap[1024] = 1;
      transmap[1025] = 1;
      transmap[1026] = 1;
      transmap[1027] = 1;
      transmap[1028] = 1;
      transmap[1029] = 1;
      transmap[1030] = 1;
    }

  }

//  log_out("%d of %d patterns actually used",actused,nrofpats);

  free(tijdbuf);
  free(priotab);
  log_out("Ready parsing patternpicture!");
}


void savelevelmap(void)
{
  int rc;
  rc = savedocfile(levels[world][level].map ,(char *) player1.loadedmap->map, player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(32*32) * sizeof(UINT16) );
  if (rc)
    {
      log_out("Map saved correctly");
    }
  else
    {
      log_out("Map not saved...!");
    }

  rc = savedocfile(levels[world][level].colmap ,(char *) colmap, player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(16*16) * sizeof(unsigned char) );
  if (rc)
    {
      log_out("Map saved correctly");
    }
  else
    {
      log_out("Map not saved...!");
    }
}


void level_init(void)
{
  levels[world][level].level_init_fn();  // call the routine for the inits of this level
}



void glob_init(void)
{
  int i,rc;

  //  player1.score = 0;
  //  player1.delayscore = 0;

  ingameflg = 0;
  
  blacksperlevel[0]  = 0;
  blacksperlevel[1]  = 0;
  blacksperlevel[2]  = 0;
  blacksperlevel[3]  = 0;
  blacksperlevel[4]  = 0;
  blacksperlevel[5]  = 0;
  blacksperlevel[6]  = 0;
  blacksperlevel[7]  = 0;
  blacksperlevel[8]  = 0;
  blacksperlevel[9]  = 0;
  blacksperlevel[10] = 0;
  blacksperlevel[11] = 0;
  blacksperlevel[12] = 0;

  scoreblacksperlevel[0]  = 0;
  scoreblacksperlevel[1]  = 0;
  scoreblacksperlevel[2]  = 0;
  scoreblacksperlevel[3]  = 0;
  scoreblacksperlevel[4]  = 0;
  scoreblacksperlevel[5]  = 0;
  scoreblacksperlevel[6]  = 0;
  scoreblacksperlevel[7]  = 0;
  scoreblacksperlevel[8]  = 0;
  scoreblacksperlevel[9]  = 0;
  scoreblacksperlevel[10] = 0;
  scoreblacksperlevel[11] = 0;
  scoreblacksperlevel[12] = 0;

  player1.newlife = 0;
  player1.lives = 3;
  maxlevel = 0;
  cheatmode = 0;

  log_out("calcing a 256 sine");
  for (i=0; i<256; i++)
    {
      sinus128[i] = (short) (sin(((PI*2)/256)*i) * 128);
    }

  log_out("calcing a 1024 sine");
  for (i=0; i<1024; i++)
    {
      sinus512[i] = (short) (sin(((PI*2)/1024)*i) * 512);
    }
  PCG32Seed(0);   //time()

  log_out("allocing memory for dots");
  dotbase = (UINTPTR *) malloc(sizeof(UINTPTR) * 10000); //10000 pixels
  dotpnt = dotbase;
  dotcolbase = (UINT8 *) malloc(10000); //10000 pixels
  dotcolpnt = dotcolbase;

  rc = PCX_ff_load("assets\\palet.pcx", NULL, ingamepal);
//  vgassert(rc);

  rc = PCX_ff_load("assets\\palet4.pcx", NULL, ingamepal4);
//  vgassert(rc);

  rc = PCX_ff_load("assets\\alldiams.pcx", NULL, alldiamspal);
//  vgassert(rc);

#ifdef DEMOVERSION2
  rc = PCX_ff_load("assets\\mcd2txt1.pcx", NULL, demopal1);
  vgassert(rc);
  rc = PCX_ff_load("assets\\mcd2txt2.pcx", NULL, demopal2);
  vgassert(rc);
  rc = PCX_ff_load("assets\\mcd2txt3.pcx", NULL, demopal3);
  vgassert(rc);
  rc = PCX_ff_load("assets\\mcd2txt4.pcx", NULL, demopal4);
  vgassert(rc);
#endif

  ingamepal[0] = 0;
  ingamepal[1] = 0;
  ingamepal[2] = 0;
  ingamepal4[0] = 0;
  ingamepal4[1] = 0;
  ingamepal4[2] = 0;

// prepare exhaustcols for level3

  memcpy(glowriginal   , ingamepal+(240*3), 48);
  memcpy(glowriginal+96, ingamepal+(240*3), 48);

  for (i=0; i<16; i++)
  {
    glowriginal[((31-i)*3)+0] = glowriginal[((i)*3)+0];
    glowriginal[((31-i)*3)+1] = glowriginal[((i)*3)+1];
    glowriginal[((31-i)*3)+2] = glowriginal[((i)*3)+2];
  }

// blitbufg for FULL VERSION text

#ifdef FULLVERSION
  fullversion = new Cblitbuf("assets\\full.pcx", 0, 0, 1);
#endif


}


void glob_game_init(void)
{
  slowmode = 0;
  lastkey = 0;
  autoscrollspd = 0;

  leftkey = 0;
  rightkey = 0;
  upkey = 0;
  downkey = 0;
  jumpkey = 0;
  shootkey = 0;
  intensity = 0;
  exitflg = 0;
  streepflg = 0;

  scoreshift = 128;    /// score board slides in

  dotpnt = dotbase;
  dotcolpnt = dotcolbase;

  player1.nrofblack = 7;
  player1.energy = 33;

// doing bullet collision initialisations

  bullet_init();

// reset evt. sokobans from prev. levels

  soko1 = 0;
  soko2 = 0;
  soko3 = 0;
  soko4 = 0;

/* calc some prefs */

  log_out("calcing some pref values");

  prefs_calcvals();

  paraflg = 1;
  restartpara = 1;
  log_out("ready with glob init");
}


int normalize (int val, int minval, int maxval)
{
    // error detection:
    if ((maxval-minval) == 0)
    {
        return(0);      
    }

    // zero-base:
    val -= minval;

    // normalize to 0..200:
    val = (200L * val) / (maxval-minval);

    // shift to -100 .. 100:
    val -= 100;

    return(val);    
}


//JOYCAPS joycaps;
//JOYINFO joypos;
int     joyactive;

void init_joystick(int joynumber)    // 0 or 1 !!
{
  joyactive = 0;

#if 0
  memset(&joycaps, 0, sizeof(JOYCAPS));

  log_out("initting joystick");
  if (joyGetPos(JOYSTICKID1+joynumber, &joypos) == JOYERR_NOERROR)
  {
    // there *is* a joystick 'x' installed!
    joyactive = 1;
    joyGetDevCaps(JOYSTICKID1+joynumber, &(joycaps), sizeof(JOYCAPS));

    log_out("joystick #%d is attached!",joynumber);
  }
#endif
}


void query_joystick(int joynumber)    // 0 or 1 !!
{
  static INT16 lastshoot = 0;
  static INT16 lastleft = 0;
  static INT16 lastright = 0;
  static INT16 lastup = 0;
  static INT16 lastdown = 0;
#if 0
  if (joyactive)
  {
    joyGetPos(JOYSTICKID1 + joynumber, &(joypos));

    // normalize the joypos to -100,0,100 scale....
    joypos.wXpos = normalize(joypos.wXpos, joycaps.wXmin, joycaps.wXmax);
    joypos.wYpos = normalize(joypos.wYpos, joycaps.wYmin, joycaps.wYmax);

    if ((int)joypos.wXpos < -50)
    {
      if (lastleft == 0)
      {
        leftkey = 1;
      }
      lastleft = 1;
    }
    else
    {
      lastleft = 0;
    }

    if ((int)joypos.wXpos >  50)
    {
      if (lastright == 0)
      {
        rightkey = 1;
      }
      lastright = 1;
    }
    else
    {
      lastright = 0;
    }

    if ((int)joypos.wYpos < -70)
    {
      if (lastup == 0)
      {
        upkey = 1;
      }
      lastup = 1;
    }
    else
    {
      lastup = 0;
    }


    if ((int)joypos.wYpos >  70)
    {
      if (lastdown == 0)
      {
        downkey = 1;
      }
      lastdown = 1;
    }
    else
    {
      lastdown = 0;
    }


    if (joypos.wButtons && JOY_BUTTON1)
    {
      if (lastshoot == 0)
      {
        shootkey = 1;
      }
      lastshoot = 1;
    }
    else
    {
      lastshoot = 0;
    }
  }
#endif
}


void query_joystickloop(int joynumber)    // 0 or 1 !!
{
  static INT16 lastshoot = 0;

#if 0
  if (joyactive)
  {
    joyGetPos(JOYSTICKID1 + joynumber, &(joypos));

    // normalize the joypos to -100,0,100 scale....
    joypos.wXpos = normalize(joypos.wXpos, joycaps.wXmin, joycaps.wXmax);
    joypos.wYpos = normalize(joypos.wYpos, joycaps.wYmin, joycaps.wYmax);

    if ((int)joypos.wXpos < -50) leftkey = 1;
    if ((int)joypos.wXpos >  50) rightkey = 1;
    if ((int)joypos.wYpos < -70) upkey = 1;
    if ((int)joypos.wYpos >  70) downkey = 1;

    if (joypos.wButtons && JOY_BUTTON1)
    {
      if (lastshoot == 0)
      {
        shootkey = 1;
      }
      lastshoot = 1;
    }
    else
    {
      lastshoot = 0;
    }
  }
#endif
}


void check_keys(void)
{

  if (keytab['N'] == 1)
    {
#ifndef DEMOVERSION
#ifdef EDITOR
      keytab['N'] = 0;
      player1.nrofblack = 0;
	  updatedarkdiam();
      nextlvlflg = 1;
      log_out("N pressed.... skipping to next level");
#endif
#endif
    }

//  if (keytab['S'] == 1)
//    {
#ifndef DEMOVERSION
#ifdef EDITOR
//      keytab['S'] = 0;
//      slowmode ^= 1;
#endif
#endif
//    }

//  if (keytab['D'] == 1)
//    {
#ifndef DEMOVERSION
#ifdef EDITOR
//      keytab['D'] = 0;
//      gen_doolhof();
#endif
#endif
//    }


  if (keytab['U'] == 1)
    {
#ifndef DEMOVERSION
#ifdef EDITOR
      if (editflg)
      {
        keytab['U'] = 0;
        savelevelmap();
      }
#endif
#endif
    }

  if (keytab['E'] == 1)
    {
#ifndef DEMOVERSION
#ifdef EDITOR
      editflg ^= 1;
      keytab['E'] = 0;
#endif
#endif
    }

  if (keytab[VK_ESCAPE] == 1)
    {
      keytab[VK_ESCAPE] = 0;

      if (hoi->frame)   // Hoi wappert niet op dit moment?
	  {
		  exitflg = 2;  // okay dan mogen we exitten
	  }
    }
      
      
  if (keytab['P'] == 1)
    {
      keytab['P'] = 0;
        
        mouserbut ^= 1;
        mouserchng = 1;
        
//      pauseflg ^= 1;
    }
      
#if 0      
  if (keytab['1'] == 1 && world == 3)
    {
      paraflg = 3;
	  keytab[1] = 0;
	  hoi->x = 4288;
	  hoi->y = 458;
	  player1.worldx = 4016;
	  player1.worldy = 261;
  
    }
  if (keytab['2'] == 1 && world == 3)
    {
      paraflg = 4;
	  keytab[2] = 0;
	  hoi->x = 6113;
	  hoi->y = 362;
	  player1.worldx = 5841;
	  player1.worldy = 165;
  
    }
  if (keytab['3'] == 1 && world == 3)
    {
      paraflg = 5;
	  keytab[3] = 0;
	  hoi->x = 9002;
	  hoi->y = 746;
	  player1.worldx = 8733;
	  player1.worldy = 535;
  
    }
	  
#endif
	  
  if (keytab['C'] == 1)
    {
      keytab['C'] = 0;

#if 0
      for (int k=0; k<player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(32*32); k++)
  {
    player1.loadedmap->map[k] = 0;
  }

      for (int l=0; l<player1.loadedmap->mapsizex*player1.loadedmap->mapsizey/(16*16); l++)
  {
    colmap[l] = 0;
  }

//      map_build(&player1);
//      map_build(&player2);
#endif

    }

  handleinputloop();

}


void handleinput1shot(void)
{
  InputBridge::SetContext((ingameflg != 0 && puzzleactiveflg == 0) ? InputBridge::INPUT_CONTEXT_GAME : InputBridge::INPUT_CONTEXT_MENU);
  InputBridge::SetBindings(prefs->upkey, prefs->downkey, prefs->leftkey, prefs->rightkey, prefs->jumpkey, prefs->shootkey);

  leftkey  = 0;
  rightkey = 0;
  upkey    = 0;
  downkey  = 0;
  jumpkey  = 0;
  shootkey = 0;



  if (ingameflg == 0 || puzzleactiveflg)     // are we in the titlescreen right now??
  {
    if (keytab[VK_LEFT])
    {
      keytab[VK_LEFT] = 0;
      leftkey  = 1;
    }
    if (keytab[VK_RIGHT])
    {
      keytab[VK_RIGHT] = 0;
      rightkey = 1;
    }
    if (keytab[VK_UP])
    {
      keytab[VK_UP] = 0;
      upkey    = 1;
    }
    if (keytab[VK_DOWN])
    {
      keytab[VK_DOWN] = 0;
      downkey  = 1;
    }
    if (keytab[' '] || keytab[VK_RETURN])
    {
      keytab[' '] = 0;
      keytab[VK_RETURN] = 0;
      shootkey = 1;
    }
  }
  else
  {
    if (keytab[prefs->leftkey])
    {
      keytab[prefs->leftkey] = 0;
      leftkey  = 1;
    }
    if (keytab[prefs->rightkey])
    {
      keytab[prefs->rightkey] = 0;
      rightkey = 1;
    }
    if (keytab[prefs->upkey])
    {
      keytab[prefs->upkey] = 0;
      upkey    = 1;
      if (prefs->jumpkey == prefs->upkey)
        jumpkey  = 1;
    }
    if (keytab[prefs->downkey])
    {
      keytab[prefs->downkey] = 0;
      downkey  = 1;
    }
    if (prefs->jumpkey != prefs->upkey && keytab[prefs->jumpkey])
    {
      keytab[prefs->jumpkey] = 0;
      jumpkey  = 1;
    }
    if (keytab[prefs->shootkey])
    {
      keytab[prefs->shootkey] = 0;
      shootkey = 1;
    }
  }

  query_joystick(0);

  keytab[prefs->shootkey] = 0;
}


void handleinputloop(void)
{
  InputBridge::SetContext((ingameflg != 0 && puzzleactiveflg == 0) ? InputBridge::INPUT_CONTEXT_GAME : InputBridge::INPUT_CONTEXT_MENU);
  InputBridge::SetBindings(prefs->upkey, prefs->downkey, prefs->leftkey, prefs->rightkey, prefs->jumpkey, prefs->shootkey);

  leftkey  = 0;
  rightkey = 0;
  upkey    = 0;
  downkey  = 0;
  jumpkey  = 0;
  shootkey = 0;

  if (ingameflg == 0 || puzzleactiveflg)
    {
      if (keytab[VK_LEFT])
        {
          leftkey  = 1;
        }
      if (keytab[VK_RIGHT])
        {
          rightkey = 1;
        }
      if (keytab[VK_UP])
        {
          upkey    = 1;
        }
      if (keytab[VK_DOWN])
        {
          downkey  = 1;
        }
      if (keytab[' '] || keytab[VK_RETURN])
        {
          shootkey = 1;
        }
    }
  else
    {
      if (keytab[prefs->leftkey])
        {
          leftkey  = 1;
        }
      if (keytab[prefs->rightkey])
        {
          rightkey = 1;
        }
      if (keytab[prefs->upkey])
        {
          upkey    = 1;
        }
      if (keytab[prefs->downkey])
        {
          downkey  = 1;
        }
      if (keytab[prefs->jumpkey])
        {
          jumpkey  = 1;
        }
      if (keytab[prefs->shootkey])
        {
          shootkey = 1;
        }
    }

  query_joystickloop(0);

  keytab[prefs->shootkey] = 0;
}


void eventhandle(void)
{
  #if 0
  while(EVT_getNext(&evt,EVT_KEYEVT|EVT_MOUSEEVT))
    {
      mouselchng = 0;
      mouserchng = 0;
      switch(evt.what)
  {
  case EVT_KEYUP:
    UINT16 dumkey;
    dumkey = EVT_asciiCode(evt.message);
    if (dumkey == 0)
      {
       switch (EVT_scanCode(evt.message))
    {
    case 75:
      dumkey = ASCII_LEFTKEY;
      break;
    case 72:
      dumkey = ASCII_UPKEY;
      break;
    case 77:
      dumkey = ASCII_RIGHTKEY;
      break;
    case 80:
      dumkey = ASCII_DOWNKEY;
      break;
    }
      }
    keytab[dumkey] = 0;   /* keyupflag */
    break;
  case EVT_KEYDOWN:
    lastkey = EVT_asciiCode(evt.message);
    if (lastkey == 0)
      {
        switch (EVT_scanCode(evt.message))
    {
    case 75:
      lastkey = ASCII_LEFTKEY;
      break;
    case 72:
      lastkey = ASCII_UPKEY;
      break;
    case 77:
      lastkey = ASCII_RIGHTKEY;
      break;
    case 80:
      lastkey = ASCII_DOWNKEY;
      break;
    }
      }
    
    keytab[lastkey] = 1;   /* keydownflag */
    break;
  case EVT_MOUSEDOWN:
    if (evt.message & EVT_LEFTBMASK)
      {
        mouselbut = 1;
        mouselchng = 1;
      }
    if (evt.message & EVT_RIGHTBMASK)
      {
        mouserbut = 1;
        mouserchng = 1;
      }
    mousex = evt.where_x;
    mousey = evt.where_y;
    break;
  case EVT_MOUSEUP:
    if (evt.message & EVT_LEFTBMASK)
      {
        mouselbut = 0;
        mouselchng = 1;
      }
    if (evt.message & EVT_RIGHTBMASK)
      {
        mouserbut = 0;
        mouserchng = 1;
      }
    mousex = evt.where_x;
    mousey = evt.where_y;
    break;
  case EVT_MOUSEMOVE:
    mousex = evt.where_x;
    mousey = evt.where_y;
    break;
  case EVT_USEREVT:
  case EVT_TIMERTICK:
  case EVT_KEYREPEAT:
  case EVT_NULLEVT:
  default:
    break;
  }
    }
#endif
}


void mouse_handling(void)
{
  MC_POINT mypoint;
#if 0
  MS_getPos(&mousex,&mousey);
  MS_moveTo((prefs->reso == PREFS_LORES) ? 160 : 320  , (prefs->reso == PREFS_LORES) ? 120 : 240 );
#endif

//  GetCursorPos(&mypoint);
//  SetCursorPos(320,240);
  mousex = g_MouseXCurrent;
  mousey = g_MouseYCurrent;

  if (editflg)
    {
        if(g_MouseFlg)
        {
      hoi->x += (mousex-((prefs->reso == PREFS_LORES) ? 160 : 320 ))/4;
      hoi->y += (mousey-((prefs->reso == PREFS_LORES) ? 120 : 240 ))/4;
        }      
      if ((INT16) hoi->x < 0)
  {
    hoi->x = 0;
  }
      
      if ((INT16) hoi->y < 0)
  {
    hoi->y = 0;
  }
      
      if (hoi->x > (player1.curmap->mapsizex+ ((editflg && mouserbut) ? 64 : 0)))
  {
    hoi->x = player1.curmap->mapsizex + ((editflg && mouserbut) ? 64 : 0);
  }
      
      if (hoi->y > (player1.curmap->mapsizey+ ((editflg && mouserbut) ? 64 : 0)))
  {
    hoi->y = player1.curmap->mapsizey + ((editflg && mouserbut) ? 64 : 0);
  }
      HOI_BLOOD *hoiblood;
      hoiblood = (HOI_BLOOD *) hoi->blood;

      hoiblood->xfloat = (hoi->x << 8);
      hoiblood->yfloat = (hoi->y << 8);
    }
  else
    {
      globmousex += (mousex-((prefs->reso == PREFS_LORES) ? 160 : 320 ));
      globmousey += (mousey-((prefs->reso == PREFS_LORES) ? 120 : 240 ));

    }
}


void globals_clear()
{
  player1.quakex = 0;
  player1.quakey = 0;

}


// BULLET COLLISION HANDLER

#define MAXBULLET (256)

UINT16 bullcnt;

OBJECT *bullettab1[MAXBULLET];
OBJECT *bullettab2[MAXBULLET];

OBJECT **curbulltab;
OBJECT **curbulladd;

void bullet_init(void)
{
  UINT16 i;

  for (i=0; i<MAXBULLET; i++)
  {
    bullettab1[i] = 0;
    bullettab2[i] = 0;
  }

  bullcnt = 0;
  curbulltab = bullettab2;
  curbulladd = bullettab1;
}


// Objects which are bullets register via this function/macro

void bullet_add(OBJECT *bullet)
{
  bullcnt++;
//  if (bullcnt > (MAXBULLET-1)) vgassert(0); // should be made impossible
  *(curbulladd++) = bullet;
}


// objects which should collide with bullets use this routine

OBJECT *bullet_check(OBJECT *enemy)
{
  OBJECT **bulletpnt;
  OBJECT *bullet;
  BULLET_BLOOD *bulletblood;

  INT16 x;
  INT16 y;
  UINT16 t;

  bulletpnt = curbulltab;
  t = 0;

  while (bullet = *(bulletpnt++))
  {
    if (bullet->frame)   // is bullet still 'active'
    {
//    log_out("t = %d",t);
      t++;
      x = (INT16)(enemy->x+enemy->coloffsetx) - (INT16)(bullet->x+bullet->coloffsetx);
      x += enemy->colwidth;
      if ((UINT16)x < bullet->colwidth+enemy->colwidth)
      {
        y = (INT16)(enemy->y+enemy->coloffsety) - (INT16)(bullet->y+bullet->coloffsety);
        y += enemy->colheight;
        if ((UINT16)y < bullet->colheight+enemy->colheight)
        {

//        play_mcdrain();

          bulletblood = (BULLET_BLOOD *) bullet->blood;
          bulletblood->remove = 2; // remove bullet from list
                                   // after 2 rounds
        
          bullet->frame = 0;

          return bullet;
        }
      }
    }
  }
  return 0;
}


// clears old bullet crap, swaps pointers

void bullet_swap(void)
{

  if (curbulltab == bullettab1)
  {
    curbulltab = bullettab2;
    curbulladd = bullettab1;
  }
  else
  {
    curbulltab = bullettab1;
    curbulladd = bullettab2;
  }

  bullcnt = 0;
  memset(curbulladd ,0, sizeof(OBJECT *)*MAXBULLET);
}


// SHORT MACRO FOR CHECK IF COLLISION WITH SCENERY TOOK PLACE

int collision(UINT16 x, UINT16 y)
{
  if(x<0 || y<0 || x>=levelmap->mapsizex || y>=levelmap->mapsizey) return 0;
  return colmap[((y>>4)*levelmap->mapsizex>>4) + (x>>4)];
}



// LOAD/SAVE ROUTINES

VG_BOOLEAN loadfile(char * fname, char *buffer, UINT32 length)
{
    FILE *fp;
    
    
    fp = fopen(FullPath(fname), "rb");
    if (!fp)
    {
        return VG_FALSE;
    }
    
    fread(buffer, length, 1, fp);
    
    fclose(fp);
    
    return VG_TRUE;
}


VG_BOOLEAN savefile(char * fname, char *buffer, UINT32 length)
{
    FILE *fp;
    
    fp = fopen(FullPath(fname), "wb");
    if (!fp)
    {
        return VG_FALSE;
    }
    
    fwrite(buffer, length, 1, fp);
    
    fclose(fp);
    
    return VG_TRUE;
}

VG_BOOLEAN loaddocfile(char * fname, char *buffer, UINT32 length)
{
    FILE *fp;
    
    
    fp = fopen(FullWritablePath(fname), "rb");
    if (!fp)
    {
        return VG_FALSE;
    }
    
    fread(buffer, length, 1, fp);
    
    fclose(fp);
    
    return VG_TRUE;
}


VG_BOOLEAN savedocfile(char * fname, char *buffer, UINT32 length)
{
    FILE *fp;
    
    fp = fopen(FullWritablePath(fname), "wb");
    if (!fp)
    {
        return VG_FALSE;
    }
    
    fwrite(buffer, length, 1, fp);
    
    fclose(fp);

#ifdef __EMSCRIPTEN__
    SyncPersistentStorage();
#endif
    
    return VG_TRUE;
}


/*****************************************/
/*         TITLE SEQUENCE STUFF          */
/*****************************************/

HEARTBEAT_FN MC_preptitlesequence(void)
{
  UINT16 rc;

  recplay = 0;
  ingameflg = 0;
  musicrestartflg = 0;  // music DOESN'T rerstart automatically...

//  video->clear(0);
  memset(gamepal, 0, 768);
  video->palette(gamepal);
//  video->clear(0);
//  video->swap();
//  video->scansync();
//  video->clear(0);
//  video->swap();
//  video->scansync();
//  video->clear(0);
//  video->swap();
//  video->scansync(); 

//  if(FirstTimeShowCredzFlg==0)  //@@@
//  {
//	  video->DrawScreen1();
//	  FirstTimeShowCredzFlg = 1;
//  }
//  else
//  {
//	  video->DrawLoading();
//  }

  refreshpic = new Cblitbuf("assets\\title.pcx", 0, 0);   // only keep this one in vidmem
  chars32    = new Cblitbuf("assets\\char32.pcx", 0, 0);  // and this one too!
  titlepic   = new Cblitbuf("assets\\title.pcx", 0, 0);   // doesn't matter
  highpic    = new Cblitbuf("assets\\hiscore.pcx", 0, 0); // doesn't matter

//  vgassert(refreshpic);
//  vgassert(titlepic);
//  vgassert(highpic);
//  vgassert(chars32);

  rc = PCX_ff_load("assets\\title.pcx", NULL, titlepal);
//  vgassert(rc);

  rc = PCX_ff_load("assets\\hiscore.pcx", NULL, highpal);
//  vgassert(rc);

  rc = convscore();
  
  popupmenu = 0;
  start_afterbuilditem = 0;

  world = 5;     // play song 4!    (make it into titletrack (#4!))
  start_cd();

  if (rc == 1) return (HEARTBEAT_FN) MC_showentername;    // highscore entry


  if(FirstTimeShowCredzFlg==0)
  {
      FirstTimeShowCredzFlg = 1;
  }

  return (HEARTBEAT_FN) MC_showtitlesequence1;    // no highscore crap
}


UINT16 convscore(void)
{
  UINT32 deler;
  UINT32 score;
  UINT16 i;
  char   cyfer;
  char  *scorepoint;
  char  *score1;
  char  *score2;
  UINT16 spatieflg;

  scorepoint = tempscore;

  deler = 10;

  // WHAT IS SCORE?!?   NR OF BLACK DIAMONDS TOTAL?
  //  score = player1.score;

  score = 0;
  for (i=0; i<13; i++)
    {
      score += scoreblacksperlevel[i];      // add up all blacks picked up per level
    }

//  log_out("you picked %d diamonds up, total",score);

  spatieflg = 1;

  for (i=3;i<5;i++)
  {  
    cyfer = score / deler;
    score -= cyfer * deler;
    deler /= 10;
    *(scorepoint++) = cyfer+0x30;
    if (cyfer == 0)
      {
  if (spatieflg) *(scorepoint-1) = ' ';
      }
    else
      {
  spatieflg = 0;
      }
  }

  score1 = tempscore;      // your score
  score2 = hightext+243;   // lowest score in list

  if (compscore(score1, score2) == 0) return 0;    // highscore entry

  return 1;
}

UINT16 compscore(char *score1, char *score2)
{
  UINT16 i;
  char let1, let2;

  for (i=0 ; i<2 ; i++)
    {

      let1 = *(score1++);
      let2 = *(score2++);

      if (let1 == ' ') let1 = '0';
      if (let2 == ' ') let2 = '0';

      if (let1 > let2) return 1;
      if (let1 < let2) return 0;
    }

  return 0;   // scores are equal so first one wins!

}


HEARTBEAT_FN MC_showentername(void)
{
  UINT16 i;

  log_out("point1");
  highpic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);
  log_out("point2");

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }
  log_out("point3");

  memcpy(gamepal, highpal, 768);
  seqcnt = 600;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_entername;
  log_out("point4");

  letchainpos = 0;
  letscrollspd = 0;

  strcpy(enter_menu_buf1, "+DARK DIAMONDS: 00  ");
  enter_menu_buf1[16] = tempscore[0];
  enter_menu_buf1[17] = tempscore[1];

  strcpy(enter_menu_buf2, "      ........      ");

//  enter_menu[0].menutext[16] = tempscore[0];
//  enter_menu[0].menutext[17] = tempscore[1];

//  enter_menu[2].menutext[ 6] = '.';
  //enter_menu[2].menutext[ 7] = '.';
//  enter_menu[2].menutext[ 8] = '.';
//  enter_menu[2].menutext[ 9] = '.';
//  enter_menu[2].menutext[10] = '.';
//  enter_menu[2].menutext[11] = '.';
//  enter_menu[2].menutext[12] = '.';
 // enter_menu[2].menutext[13] = '.';
  log_out("point5");

  letchainpos = 1152;
  chainvulcnt = 0;
  invulexit = 0;

  menupoint = enter_menu;
  MC_buildmenu();
  log_out("point6");

  return (HEARTBEAT_FN) MC_fadein;   // was fade half in.... maar helemaal ziet er ook goed uit en de darkdiamond is dan ten minste goed

}


HEARTBEAT_FN MC_entername(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinputloop();

  scrolltextline();

  menuitem = 2;
  highpic->draw_nokey(*vidblitbuf, 0, menupoint[menuitem].linenr*32-16, 0, menupoint[menuitem].linenr*32-16, 640, menupoint[menuitem].linenr*32+32+16);
  menupuntblit();

  if (invulexit)
    {
      swirlrotlock = 0;
      insertname();
      fadereturn = (HEARTBEAT_FN) MC_showhighscore;
      fadeout = 63;
	  save_highscores();
      return (HEARTBEAT_FN) MC_fadeout;
    }

  return (HEARTBEAT_FN) MC_entername;
}

void insertname(void)
{
  char *score1;
  char *score2;
  UINT16 i;

  score1 = tempscore;      // your score
  score2 = hightext+243;   // lowest score in list

  //place name at bottom position

  score2[0] = score1[0];
  score2[1] = score1[1];

  score2[ 6] = enter_menu[2].menutext[ 6];
  score2[ 7] = enter_menu[2].menutext[ 7];
  score2[ 8] = enter_menu[2].menutext[ 8];
  score2[ 9] = enter_menu[2].menutext[ 9];
  score2[10] = enter_menu[2].menutext[10];
  score2[11] = enter_menu[2].menutext[11];
  score2[12] = enter_menu[2].menutext[12];
  score2[13] = enter_menu[2].menutext[13];

  score1 = hightext+243;
  score2 = hightext+223;

  for (i=0; i<9; i++)
    {
      UINT16 j;
      if (compscore(score1, score2) == 0) return;    // highscore entry
      
      for (j=0; j<16; j++)
  {
    UINT16 swap;
    swap = score1[j];
    score1[j] = score2[j];
    score2[j] = (char) swap;
  }
      score1 -= 20;
      score2 -= 20;
    }
}

bool hiscorePrevMouseState = false;
int hiscorePrevChainPos = 0;
int hiscorePrevMouseX;
int mouseTouchTime = 0;
int hiscoreSelectLetter = 0;


void scrolltextline(void)
{
  UINT16 i;
  UINT16 lett;
  INT16  srcx,srcy,destx,desty;
  char  *letpoint;
  UINT16 rot;

  if (rightkey)
    {
      lastchaindir = 1;
      if (letscrollspd < 16) letscrollspd++;
    }
  if (leftkey)
    {
      lastchaindir = 0;
      if (letscrollspd > -16) letscrollspd--;
    }

  if (leftkey==0 && rightkey==0 && letscrollspd == 0)
    {
      letscrollspd = 0;

      if ((letchainpos & 31))
  {
    if (lastchaindir == 1)
      {
        letchainpos++;
      }
    else
      {
        letchainpos--;
      }
  }
    }

  if (letscrollspd !=0 && leftkey == 0 && rightkey == 0)
    {
      if (letscrollspd>0)
  {
    letscrollspd--;
    if (letscrollspd<0) letscrollspd = 0;
  }
      else
  {
    letscrollspd++;
    if (letscrollspd>0) letscrollspd = 0;
  }
    }

  if (shootkey == 1)
  {
    hiscoreSelectLetter = 1;
  }
      
    
    //@@@scroll

    if(g_MouseActualFlg== true && hiscorePrevMouseState==false)
    {
        hiscorePrevChainPos = letchainpos;
        hiscorePrevMouseX = g_MouseXCurrent;
        mouseTouchTime = 0;
        hiscoreSelectLetter = 0;
    }

    if(g_MouseActualFlg)
    {
        letchainpos = hiscorePrevChainPos + (hiscorePrevMouseX - g_MouseXCurrent);
        mouseTouchTime++;
        if( (hiscorePrevMouseX - g_MouseXCurrent)> 16 || (hiscorePrevMouseX - g_MouseXCurrent) < -16)
        {
            mouseTouchTime = 16; //invalidate potential mouseclick
        }
    }

    //short tap and mouse didn't move too much?
    if(mouseTouchTime>0 && mouseTouchTime < 16 && g_MouseActualFlg == false && hiscorePrevMouseState == true)
    {
        hiscoreSelectLetter = 1;
    }
    
    hiscorePrevMouseState = g_MouseActualFlg;
    
    //scroll to stable position
    int divx = (letchainpos)&31;
    if(divx>0 && divx<16)
    {
        letchainpos--;
    }
    if(divx>=16 && divx<32)
    {
        letchainpos++;
    }
    letchainpos += (46*32)*2;
    letchainpos %= (46*32);

    enter_menu[2].menutext[ 6+chainvulcnt] = letchain[((letchainpos+16)>>5)+10];

    if (hiscoreSelectLetter)
    {
        hiscoreSelectLetter = 0;
        if ((letchainpos&31) == 0)
        {
            if (letchainpos == 1024)   //RUB
            {
                if (chainvulcnt)
                {
                    enter_menu[2].menutext[ 6+chainvulcnt] = ' ';
                    chainvulcnt--;
                }
            }
            else
            {
                if (letchainpos != 1088)    //END
                {
                    if (chainvulcnt<7)
                    {
                        chainvulcnt++;
                    }
                    else
                    {
                        invulexit = 1;
                    }
                }
                else
                {
                    UINT16 i;
                    i = chainvulcnt;
                    while (i<8)
                    {
                        enter_menu[2].menutext[ 6+i] = ' ';
                        i++;
                    }
                    invulexit = 1;
                }
            }
            shootkey = 0;
        }
    }


  letchainpos += (letscrollspd/2);
  if (letchainpos < 0) letchainpos += (46*32);
  if (letchainpos >= (46*32)) letchainpos -= (46*32);

  letpoint = letchain + (letchainpos>>5);

  destx = (0) - (letchainpos & 31);

  rot = destx-320;

  for (i=0; i<22; i++)
    {
      INT16  temp;
      lett  = *(letpoint++);
      lett -= 0x20;

      srcy = lett/20;
      srcx = lett%20;
      srcx <<= 5;
      srcy <<= 5;
  
      desty = 370;
      temp = ((sinus512[(rot)       & 1023] * 128) >> 10);

      if (temp<0) temp = -temp;

      desty += temp;
      
      rot+=32;
      
      chars32->draw(*vidblitbuf, destx, desty, srcx, srcy, srcx+32, srcy+32);
      destx +=32;
    }
}

void killsequence()
{
  if (highpic)    delete highpic;
  if (titlepic)   delete titlepic;
  if (chars32)    delete chars32;
  if (refreshpic) delete refreshpic;
  refreshpic = 0;
  chars32 = 0;
  titlepic = 0;
  highpic = 0;
}

//-----------

HEARTBEAT_FN MC_showtitlesequence1(void)
{
  UINT16 i;

  titlepic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, titlepal, 768);
  seqcnt = 200;              // 2 seconds
  titlebrightness = 0;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_titlesequence1;
  keytab[' '] = 0;

  if (popupmenu)
  {
     darken = 128;
     popupmenu = 0;
     fadereturn = (HEARTBEAT_FN) MC_showoptions;
     return (HEARTBEAT_FN) MC_fadehalfin;
  }
  else
  {
     return (HEARTBEAT_FN) MC_fadein;
  }
      
        
}


HEARTBEAT_FN MC_titlesequence1(void)
{

  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();

  handleinput1shot();

  seqcnt--;
  if (!seqcnt)
    {
      fadereturn = (HEARTBEAT_FN) MC_showcredz1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (shootkey == 1 || popupmenu == 1 ||g_MouseFlg==1)
    {
      popupmenu = 0;
      darken = 256;
      return (HEARTBEAT_FN) MC_darkentitlesequence;
    }

  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }
  
  return (HEARTBEAT_FN) MC_titlesequence1;
}


HEARTBEAT_FN MC_darkentitlesequence(void)
{
  if (darken > 128)
    {
      darken -= 8;
      video->palette(gamepal, darken, 240);
//      video->palette(gamepal, darken);
      video->scansync();
      video->swap();
      return (HEARTBEAT_FN) MC_darkentitlesequence;
    }

  return (HEARTBEAT_FN) MC_showoptions;
}


HEARTBEAT_FN MC_showoptions(void)
{

  seqcnt = 164;

  swirlrot = 0;
  swirlrotspd  = 0;
  swirlrotspd2 = 0x0200;
  swirlrotlock = 1;

  swirlinpoint = optionstext;
  letterswirlreset();

  keyquery = 0;

  return (HEARTBEAT_FN) MC_options;
}


HEARTBEAT_FN MC_options(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  letterswirlscan();
  letterswirlin();

  seqcnt--;
  if (!seqcnt)
    {
      menupoint = menu1;
      menuitem = 0;
      return (HEARTBEAT_FN) MC_buildmenu;
    }

  return (HEARTBEAT_FN) MC_options;
}

static void sync_menu_input_context(void)
{
  InputBridge::SetContext(keyquery == 1 ? InputBridge::INPUT_CONTEXT_REMAP : InputBridge::INPUT_CONTEXT_MENU);
}

HEARTBEAT_FN MC_menu(void)
{
  static int menutimeout = 1000;      // 20 seconds

  menuleavefunc = 0;

  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  if (musicstoppedflg) start_cd();   // did the music stop when we where IN the menu??

  if (keytab[VK_ESCAPE])
  {
	keytab[VK_ESCAPE] = 0;
    if (menupoint == menu1)
	{
      titlepic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);
      menuleavefunc = (HEARTBEAT_FN) MC_leavemenu;
	}
    if (menupoint == menu12 || menupoint == menu13)
	{
      menupoint = menu1;
      menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
	}
    if (menupoint == menu121 || menupoint == menu1211 || menupoint == menu12111 || menupoint == menu121111 || menupoint == menu1211111 || menupoint == menu12111111 || menupoint == menu122 || menupoint == menu123)
	{
      menupoint = menu12;
      menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
	}
    if (menuleavefunc)
	{
      keyquery = 0;
	  menutimeout = 1000;
	  sync_menu_input_context();
	  return (HEARTBEAT_FN) menuleavefunc;
	}
  }

  if (keyquery == 1)    // ask keyboardkey
  {
      if(lastkey)
      {
        menupoint[menuitem].menu_fn();
        menutimeout = 1000;
        if (menuleavefunc)
          {
            sync_menu_input_context();
            return (HEARTBEAT_FN) menuleavefunc;
          }
      }

  titlepic->draw_nokey(*vidblitbuf, 0, menupoint[menuitem].linenr*32-16, 0, menupoint[menuitem].linenr*32-16, 640, menupoint[menuitem].linenr*32+32+16);
  menupuntblit();

  sync_menu_input_context();
  return (HEARTBEAT_FN) MC_menu;

  }



  if (downkey == 1)    //down in menu
    {
      menutimeout = 1000;
      downkey = 0;
      
      if (menupoint[menuitem+1].linenr != 0)
  {
    menuitem++;
  }
    }

  if (upkey == 1)    //up in menu
    {
      menutimeout = 1000;
	  upkey = 0;
      if (menuitem != 0)
  {
    menuitem--;
  }
    }


  //handle mouseclick

	if(g_MouseFlg)
	{
		int y;
		int menuidx;
		int bestd;
		UINT16 mi;

		y = g_MouseYDown;
		y /= 32;

		menuidx = 0;
		bestd = 1000;
		for (mi = 0; menupoint[mi].linenr != 0; mi++)
		{
			int d = y - (int)menupoint[mi].linenr;
			if (d < 0) d = -d;
			if (d < bestd)
			{
				bestd = d;
				menuidx = (int)mi;
			}
		}

		if (bestd <= 2)
		{
			shootkey = 0;
			menuitem = (UINT16)menuidx;
			menupoint[menuitem].menu_fn();
			menutimeout = 1000;
			if (menuleavefunc)
			  {
			    sync_menu_input_context();
			    return (HEARTBEAT_FN) menuleavefunc;
			  }
		}
	}

    if (shootkey == 1)
    {
        shootkey = 0;
        menupoint[menuitem].menu_fn();
        menutimeout = 1000;
        if (menuleavefunc)
          {
            sync_menu_input_context();
            return (HEARTBEAT_FN) menuleavefunc;
          }
    }


  menutimeout--;
  if (menutimeout==0)
  {
    menutimeout = 1000;
    titlepic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);
    menuleavefunc = (HEARTBEAT_FN) MC_leavemenu;
    if (menuleavefunc)
      {
        sync_menu_input_context();
        return (HEARTBEAT_FN) menuleavefunc;
      }
  }
 
  titlepic->draw_nokey(*vidblitbuf, 0, menupoint[menuitem].linenr*32-16, 0, menupoint[menuitem].linenr*32-16, 640, menupoint[menuitem].linenr*32+32+16);
  menupuntblit();

  sync_menu_input_context();
  return (HEARTBEAT_FN) MC_menu;
}

void menupuntblit(void)
{
  static UINT16 wobblex = 0;
  char  *textje;

  UINT16 rot;
  UINT16 lett,srcx,srcy,destx,desty;

  UINT16 i;

  textje = menupoint[menuitem].menutext;

  wobblex += 4;
  wobblex &= 255;

  rot = wobblex;

  destx = 0;

  for (i=0; i<20; i++)
    {
      lett = *(textje++) & 0x7f;

      desty = menupoint[menuitem].linenr*32;

      if (lett == '+')
  {
    destx += 16;
  }

      if (lett != ' ' && lett != '=' && lett != '+')
  {
    lett -= 0x20;

    srcy = lett/20;
    srcx = lett%20;
    srcx <<= 5;
    srcy <<= 5;
  
    desty += ((sinus512[(rot*4)       & 1023] * 8) >> 10);
    
    chars32->draw(*vidblitbuf, destx, desty, srcx, srcy, srcx+32, srcy+32);
  }
      destx += 32;
      rot += 16;
      rot &= 1023;
    }
};


void menuf11(void)
{
  fadereturn = (HEARTBEAT_FN) MC_endsequence;
  fadeout = 31;
  menuleavefunc = (HEARTBEAT_FN) MC_fadeout;
};

void menuf12(void)
{
  menupoint = menu12;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
};

void menuf121(void)
{
  keyquery = 1;
  lastkey = 0;
  menupoint = menu121;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf1210(void)
{
  keyquery = 1;
  prefs->jumpkey = lastkey;
  lastkey = 0;
  menupoint = menu1211;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf1211(void)
{
  keyquery = 1;
  prefs->shootkey = lastkey;
  lastkey = 0;
  if (prefs->shootkey == prefs->jumpkey) return;
  menupoint = menu12111;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf12111(void)
{
  keyquery = 1;
  prefs->upkey = lastkey;
  lastkey = 0;
  if (prefs->upkey == prefs->shootkey) return;
  menupoint = menu121111;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf121111(void)
{
  keyquery = 1;
  prefs->downkey = lastkey;
  lastkey = 0;
  if (prefs->downkey == prefs->jumpkey) return;
  if (prefs->downkey == prefs->upkey) return;
  if (prefs->downkey == prefs->shootkey) return;
  menupoint = menu1211111;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf1211111(void)
{
  keyquery = 1;
  prefs->leftkey = lastkey;
  lastkey = 0;
  if (prefs->leftkey == prefs->jumpkey) return;
  if (prefs->leftkey == prefs->upkey) return;
  if (prefs->leftkey == prefs->shootkey) return;
  if (prefs->leftkey == prefs->downkey) return;
  menupoint = menu12111111;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf12111111(void)
{
  prefs->rightkey = lastkey;
  lastkey = 0;
  if (prefs->rightkey == prefs->jumpkey) return;
  if (prefs->rightkey == prefs->upkey) return;
  if (prefs->rightkey == prefs->downkey) return;
  if (prefs->rightkey == prefs->leftkey) return;
  if (prefs->rightkey == prefs->shootkey) return;
  keyquery = 0;
  menupoint = menu12;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;

  if (prefs->jumpkey != 'C') return;
  if (prefs->shootkey != 'H') return;
  if (prefs->upkey    != 'E') return;
  if (prefs->downkey  != 'A') return;
  if (prefs->leftkey  != 'T') return;
  if (prefs->rightkey != 'S') return;

  cheatmode = 1;
  maxlevel = 12;
  blacksperlevel[0]  = 7;
  blacksperlevel[1]  = 7;
  blacksperlevel[2]  = 7;
  blacksperlevel[3]  = 7;
  blacksperlevel[4]  = 7;
  blacksperlevel[5]  = 7;
  blacksperlevel[6]  = 7;
  blacksperlevel[7]  = 7;
  blacksperlevel[8]  = 7;
  blacksperlevel[9]  = 7;
  blacksperlevel[10] = 7;
  blacksperlevel[11] = 7;
  blacksperlevel[12] = 0;

  prefs->jumpkey = 38;
  prefs->shootkey = 32;
  prefs->leftkey = 37;
  prefs->rightkey = 39;
  prefs->downkey = 40;
  prefs->upkey = 38;
  
}

void menuf122(void)
{
  menupoint = menu122;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
void menuf1221(void)
{
  const int fullscreenSlot = 0;

  fullscreenflg ^= 1;

  if (fullscreenflg)
    {
      menu122[fullscreenSlot].menutext = fullscreenontxt;
    }
  else
    {
      menu122[fullscreenSlot].menutext = fullscreenofftxt;
    }
  DisplayBridge::SetFullscreen(fullscreenflg ? 1 : 0);
  start_afterbuilditem = fullscreenSlot;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}

void menuf1222(void)
{
  const int gamespeedSlot = 1;

  gamespeedflg = (UINT16)((gamespeedflg + 1u) % 3u);
  menu122[gamespeedSlot].menutext =
      (gamespeedflg == MC_GAME_SPEED_50HZ)  ? gamespeedmenu_50
    : (gamespeedflg == MC_GAME_SPEED_60HZ) ? gamespeedmenu_60
                                          : gamespeedmenu_vs;
  DisplayBridge::SetVSync(gamespeedflg == MC_GAME_SPEED_VSYNC ? 1 : 0);
  start_afterbuilditem = gamespeedSlot;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
#endif

void menuf1223(void)
{
  easiervisualsflg ^= 1;

#ifdef MOONCHILD_HAS_DISPLAY_OPTIONS
  const int easierSlot = 2;
#else
  const int easierSlot = 0;
#endif

  if (easiervisualsflg)
    {
      menu122[easierSlot].menutext = easyvisontxt;
    }
  else
    {
      menu122[easierSlot].menutext = easyvisofftxt;
    }
  start_afterbuilditem = easierSlot;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}

void menuf1224(void)
{
  menupoint = menu12;
  start_afterbuilditem = 1;    // after rebuild start at second item instead of top one!
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
};

void menuf123(void)
{
  menupoint = menu123;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}

void menuf1231(void)
{
  // toggle MUSIC

  musicflg ^= 1;

  if (musicflg)
    {
      menu123[0].menutext = musicontxt;
      start_cd();
    }
  else
    {
      menu123[0].menutext = musicofftxt;
      stop_cd();
    }
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}
void menuf1232(void)
{
  // toggle SFX

  sfxflg ^= 1;

  if (sfxflg)
    {
      menu123[1].menutext = sfxontxt;
    }
  else
    {
      menu123[1].menutext = sfxofftxt;
    }
  start_afterbuilditem = 1;    // after rebuild start at second item instead of top one!
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}

void menuf1233(void)
{
  altmenutuneflg ^= 1;

  if (altmenutuneflg)
    {
      menu123[2].menutext = altmenuontxt;
    }
  else
    {
      menu123[2].menutext = altmenuofftxt;
    }
  if (musicflg)
    {
      start_cd();
    }
  start_afterbuilditem = 2;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
}

void menuf1234(void)
{
  menupoint = menu12;
  start_afterbuilditem = 2;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
};

void menuf124(void)
{
  menupoint = menu1;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
};

void menuf13(void)
{
  menupoint = menu13;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
};

void menuf131(void)
{
  menuleavefunc = (HEARTBEAT_FN) MC_towindows;
};

void menuf132(void)
{
  menupoint = menu1;
  menuleavefunc = (HEARTBEAT_FN) MC_buildmenu;
};

void menuf14(void)
{
//  titlepic->draw(*refreshpic, 0, 0, 0, 0, 640, 480);
//  menuleavefunc = (HEARTBEAT_FN) MC_leavemenu;

  fadereturn = (HEARTBEAT_FN) MC_startdemo;
  fadeout = 31;
  menuleavefunc = (HEARTBEAT_FN) MC_fadeout;
};


HEARTBEAT_FN MC_startdemo(void)
{
  stop_cd();
  killsequence();

  // game inits    make a nice function for it lazy ass

  world = 4;
  level = 0;

  player1.lives = 3;
  player1.newlife = 0;          // when it reaches 32 you gain a life

#if 1      // remember all black diamonds gained, but not the diamonds picked up with 1 go
  scoreblacksperlevel[0]  = 0;
  scoreblacksperlevel[1]  = 0;
  scoreblacksperlevel[2]  = 0;
  scoreblacksperlevel[3]  = 0;
  scoreblacksperlevel[4]  = 0;
  scoreblacksperlevel[5]  = 0;
  scoreblacksperlevel[6]  = 0;
  scoreblacksperlevel[7]  = 0;
  scoreblacksperlevel[8]  = 0;
  scoreblacksperlevel[9]  = 0;
  scoreblacksperlevel[10] = 0;
  scoreblacksperlevel[11] = 0;
  scoreblacksperlevel[12] = 0;
#endif

  recpnt = recbuf;
  recplay = 1;
  ingameflg = 1;              // from this point onward we are IN the game
  configure_level(world+1,level+1);
  return (HEARTBEAT_FN) MC_loadlevel;
}



HEARTBEAT_FN MC_endsequence(void)
{
  stop_cd();
	LOG("E\n");
  killsequence();
	LOG("F\n");

  // game inits    make a nice function for it lazy ass

  world = maxlevel / 4;
  level = maxlevel % 4;

  //  player1.score = 0;          // to make sure the highscore won't start right away!
  //  player1.delayscore = 0;
  player1.lives = 3;
  player1.newlife = 0;          // when it reaches 32 you gain a life

#if 1      // remember all black diamonds gained, but not the diamonds picked up with 1 go
  scoreblacksperlevel[0]  = 0;
  scoreblacksperlevel[1]  = 0;
  scoreblacksperlevel[2]  = 0;
  scoreblacksperlevel[3]  = 0;
  scoreblacksperlevel[4]  = 0;
  scoreblacksperlevel[5]  = 0;
  scoreblacksperlevel[6]  = 0;
  scoreblacksperlevel[7]  = 0;
  scoreblacksperlevel[8]  = 0;
  scoreblacksperlevel[9]  = 0;
  scoreblacksperlevel[10] = 0;
  scoreblacksperlevel[11] = 0;
  scoreblacksperlevel[12] = 0;
#endif

  puzzleinteractive = 1;      // if not first level become interactive (choose puzzlepiece!)

  ingameflg = 1;              // from this point onward we are IN the game

	LOG("G\n");
  configure_level(world+1,level+1);

  return (HEARTBEAT_FN) MC_preppuzzleselect;
}

HEARTBEAT_FN MC_towindows(void)
{
	return 0;
}


HEARTBEAT_FN MC_buildmenu(void)
{
  char  *textje;

  UINT16 itemcnt;
  UINT16 lett,srcx,srcy,destx,desty;

  UINT16 i;

  if (menupoint != enter_menu)  // niet echt geweledig mooi... magoed, deadlines enzo..
    {
      titlepic->draw_nokey(*refreshpic, 0, 160, 0, 160, 640, 480);
    }

  itemcnt = 0;
  while (menupoint[itemcnt].linenr != 0)
    {
      textje = menupoint[itemcnt].menutext;
      
      destx = 0;
      
      for (i=0; i<20; i++)
  {
    lett = *(textje++) & 0x7f;
    
    desty = menupoint[itemcnt].linenr*32;
    
    if (lett == '+')
      {
        destx += 16;
      }
    
    if (lett != ' ' && lett != '=' && lett != '+')
      {
        lett -= 0x20;
        
        srcy = lett/20;
        srcx = lett%20;
        srcx <<= 5;
        srcy <<= 5;
        
        chars32->draw(*refreshpic, destx, desty, srcx, srcy, srcx+32, srcy+32);
      }
    destx += 32;
  }
      itemcnt++;
    }

  menuitem = start_afterbuilditem;
  start_afterbuilditem = 0;

  sync_menu_input_context();
  return (HEARTBEAT_FN) MC_menu;
};


HEARTBEAT_FN MC_leavemenu(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  if (darken < 256)
    {
      darken += 8;
      video->palette(gamepal, darken, 240);
//      video->palette(gamepal, darken);
      video->scansync();
      video->swap();
      return (HEARTBEAT_FN) MC_leavemenu;
    }

  seqcnt = 200;              // 2 seconds
  return (HEARTBEAT_FN) MC_titlesequence1;
}

//-------------

HEARTBEAT_FN MC_showcredz1(void)
{
  UINT16 i;

  highpic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, highpal, 768);
  seqcnt = 600;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_credz1;

  swirlrotlock = 0;
  swirlrotspd  = 8;
  swirlrotspd2 = 0x0200;

  swirlinpoint = credstext;
  letterswirlreset();

  return (HEARTBEAT_FN) MC_fadein;
}


HEARTBEAT_FN MC_credz1(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  letterswirlscan();
  letterswirlin();

  seqcnt--;

  if (!seqcnt)
    {
      fadeout = 63;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence2;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (shootkey == 1 || popupmenu == 1 || g_MouseFlg==1)
    {
      popupmenu = 1;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }


  return (HEARTBEAT_FN) MC_credz1;
}


//-------------

HEARTBEAT_FN MC_showtitlesequence2(void)
{
  UINT16 i;

  titlepic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, titlepal, 768);
  seqcnt = 200;              // 2 seconds
  titlebrightness = 0;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_titlesequence2;

  if (popupmenu)
  {
     darken = 128;
     popupmenu = 0;
     fadereturn = (HEARTBEAT_FN) MC_showoptions;
     return (HEARTBEAT_FN) MC_fadehalfin;
  }
  else
  {
     return (HEARTBEAT_FN) MC_fadein;
  }
}


HEARTBEAT_FN MC_titlesequence2(void)
{
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();

  handleinput1shot();

  seqcnt--;
  if (!seqcnt)
    {
      fadereturn = (HEARTBEAT_FN) MC_showhighscore;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (shootkey == 1 || g_MouseFlg==1)
    {
      darken = 256;
      return (HEARTBEAT_FN) MC_darkentitlesequence;
    }

  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }


  return (HEARTBEAT_FN) MC_titlesequence2;
}

//-----------

HEARTBEAT_FN MC_showhighscore(void)
{
  UINT16 i;

  highpic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, highpal, 768);
  seqcnt = 600;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_highscore;

  swirlrotspd  = 4;
  swirlrotspd2 = 0x0100;

  swirlinpoint = hightext;
  letterswirlreset();

  return (HEARTBEAT_FN) MC_fadein;
}


HEARTBEAT_FN MC_highscore(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  letterswirlscan();
  letterswirlin();

  seqcnt--;

  if (!seqcnt)
    {
      fadeout = 63;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence3;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (shootkey == 1 || g_MouseFlg==1)
    {
      popupmenu = 1;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }

  return (HEARTBEAT_FN) MC_highscore;
}

//-------------

HEARTBEAT_FN MC_showtitlesequence3(void)
{
  UINT16 i;

  titlepic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, titlepal, 768);
  seqcnt = 200;              // 2 seconds
  titlebrightness = 0;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_titlesequence3;

  if (popupmenu)
  {
     darken = 128;
     popupmenu = 0;
     fadereturn = (HEARTBEAT_FN) MC_showoptions;
     return (HEARTBEAT_FN) MC_fadehalfin;
  }
  else
  {
     return (HEARTBEAT_FN) MC_fadein;
  }
}


HEARTBEAT_FN MC_titlesequence3(void)
{
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();

  handleinput1shot();

  seqcnt--;
  if (!seqcnt)
    {
      fadereturn = (HEARTBEAT_FN) MC_showcredz2;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (shootkey == 1 || g_MouseFlg==1)
    {
      darken = 256;
      return (HEARTBEAT_FN) MC_darkentitlesequence;
    }

  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }


  return (HEARTBEAT_FN) MC_titlesequence3;
}

//----------

HEARTBEAT_FN MC_showcredz2(void)
{
  UINT16 i;

  highpic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, highpal, 768);
  seqcnt = 600;
  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_credz2;

  swirlrotspd  = 8;
  swirlrotspd2 = (unsigned short) 0xfe00;

  swirlinpoint = creds2text;
  letterswirlreset();

  return (HEARTBEAT_FN) MC_fadein;
}

HEARTBEAT_FN MC_credz2(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  letterswirlscan();
  letterswirlin();

  seqcnt--;
  if (!seqcnt)
    {
      luxaleavefunc = (HEARTBEAT_FN) MC_showcredz3;
      fadereturn = (HEARTBEAT_FN) MC_luxaflex;
      memset(luxatab, 0, 14);
      luxacnt = 13;
      return (HEARTBEAT_FN) MC_luxaflex;
    }

  if (shootkey == 1 || popupmenu == 1 || g_MouseFlg==1)
    {
      popupmenu = 1;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }

  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }

  return (HEARTBEAT_FN) MC_credz2;
}


HEARTBEAT_FN MC_luxaflex(void)
{
  UINT16 i;
  static UINT16 luxadelay = 0;
  static UINT16 luxaflip = 0;

  for (i=luxacnt; i<14; i++)
    {
      if (luxatab[i] < 33) luxatab[i] += 1;
    }

  if (luxacnt && luxaflip) luxacnt--;

  luxaflip ^= 1;

  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  for (i=0; i<14; i++)
    {
      if (luxatab[i] > 0  && luxatab[i] < 33)
  {
    highpic->draw_nokey(*refreshpic, 0, (i*32)+32-luxatab[i], 0, (i*32), 640, (i*32)+luxatab[i]);
  }
    }

  if (luxatab[0] != 33)
    {
      luxadelay = 8;
    }
  else
    {
      luxadelay--;
      if (!luxadelay)  return (HEARTBEAT_FN) luxaleavefunc;
    }

  return (HEARTBEAT_FN) MC_luxaflex;
}

//----------

HEARTBEAT_FN MC_showcredz3(void)
{

#if 0
  highpic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, highpal, 768);

  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_credz3;

#endif
  seqcnt = 600;

  swirlrotspd  = -4;
  swirlrotspd2 = (unsigned short) 0xff00;

  swirlinpoint = creds3text;
  letterswirlreset();

  return (HEARTBEAT_FN) MC_credz3;
}


HEARTBEAT_FN MC_credz3(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  letterswirlscan();
  letterswirlin();

  handleinput1shot();

  seqcnt--;
  if (!seqcnt)
    {
      luxaleavefunc = (HEARTBEAT_FN) MC_showcredz4;
      fadereturn = (HEARTBEAT_FN) MC_luxaflex;
      memset(luxatab, 0, 14);
      luxacnt = 13;
      return (HEARTBEAT_FN) MC_luxaflex;

	  /*
	  fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
*/

    }

  if (shootkey == 1 || popupmenu == 1 || g_MouseFlg==1)
    {
      popupmenu = 1;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }
  
  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }


  return (HEARTBEAT_FN) MC_credz3;
}

//----------

HEARTBEAT_FN MC_showcredz4(void)
{

#if 0
  highpic->draw_nokey(*refreshpic, 0, 0, 0, 0, 640, 480);

  for (i =0; i < 20; i++)
    {
      video->swap();
      video->scansync();
      video->clear(0);
      refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);
    }

  memcpy(gamepal, highpal, 768);

  fadein = 0;
  fadereturn = (HEARTBEAT_FN) MC_credz4;

#endif
  seqcnt = 600;

  swirlrotspd  = -2;
  swirlrotspd2 = (unsigned short) 0xff00;

  swirlinpoint = creds4text;
  letterswirlreset();

  return (HEARTBEAT_FN) MC_credz4;
}


HEARTBEAT_FN MC_credz4(void)
{
  video->swap();
  video->scansync();
  refreshpic->draw_nokey(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  letterswirlscan();
  letterswirlin();

  handleinput1shot();

  seqcnt--;
  if (!seqcnt)
    {
	  fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;

    }

  if (shootkey == 1 || popupmenu == 1)
    {
      popupmenu = 1;
      fadereturn = (HEARTBEAT_FN) MC_showtitlesequence1;
      fadeout = 63;
      return (HEARTBEAT_FN) MC_fadeout;
    }
  
  if (musicstoppedflg)
  {
    fadereturn = (HEARTBEAT_FN) MC_startdemo;
    fadeout = 63;
    return (HEARTBEAT_FN) MC_fadeout;
  }


  return (HEARTBEAT_FN) MC_credz4;
}



void letterswirlreset()
{
  swirlx = 0;
  swirly = 0;
  memset(swirltab, 0, sizeof(swirltab));
}


void swirladd(char lett, UINT16 swirlx, UINT16 swirly)
{
  UINT16 i;

  for (i=0; i<64; i++)
    {
      if (swirltab[i*4 + 2] == 0)  // empty spot somewhere?
  {
    swirltab[i*4] = swirlx;
    swirltab[i*4 + 1] = swirly;
    swirltab[i*4 + 2] = 800;      // nice start amp?
    swirltab[i*4 + 3] = (lett-32) + (swirlrot<<8);
    return;
  }
    }
}


void letterswirlin()
{
  UINT16 i;

  swirlrot += swirlrotspd;
  swirlrot &= 255;

  for (i=0; i<64; i++)
    {
      if (swirltab[i*4 + 2] != 0)  // no empty spot?
  {
    UINT16 rot,amp;
    UINT16 lett,srcx,srcy,destx,desty;

    swirltab[i*4 + 3] += swirlrotspd2;     // advance rotation
    rot = (swirltab[i*4 + 3] & 0xff00) >> 8;

    swirltab[i*4 + 2] -= 10;        // decrease amplitude
    amp = swirltab[i*4 + 2];

    lett = swirltab[i*4 + 3] & 0x7f;

    srcy = lett/20;
    srcx = lett%20;
    srcx <<= 5;
    srcy <<= 5;

    //    srcx = 0;
    //    srcy = 32;

    destx = swirltab[i*4    ] + ((sinus512[(rot*4)       & 1023] * amp) >> 10);
    desty = swirltab[i*4 + 1] + ((sinus512[((rot*4)+256) & 1023] * amp) >> 10);

    if (swirlrotlock) destx = swirltab[i*4];

    chars32->draw(*vidblitbuf, destx, desty, srcx, srcy, srcx+32, srcy+32);
    if (amp == 0)
      {
        chars32->draw(*refreshpic, destx, desty, srcx, srcy, srcx+32, srcy+32);
      }
    
  }
    }
}


void letterswirlscan()
{
  static UINT16 throwcnt = 0;

  if (*(swirlinpoint) != '@')
    {

// skip blank spaces

skipspace:

      if (*(swirlinpoint) == ' ')
  {
    while(*(swirlinpoint) == ' ')
      {
        swirlinpoint++;
        swirlx += 32;
        if (swirlx == 640)
    {
      swirlx = 0;
      swirly += 32;
    }
      }
  }

      if (*(swirlinpoint) == '+')
  {
    swirlextrax = 16;
    swirlinpoint++;
    swirlx += 32;
    if (swirlx == 640)
      {
        swirlx = 0;
        swirly += 32;
      }
    goto skipspace;
  }
      if (*(swirlinpoint) == '=')
  {
    swirlextrax = 0;
    swirlinpoint++;
    swirlx += 32;
    if (swirlx == 640)
      {
        swirlx = 0;
        swirly += 32;
      }
    goto skipspace;
  }


      if (throwcnt)     //throw delay for letter thrower
  {
    throwcnt--;
    return;
  }
      else
  {
    throwcnt = 1;
    if (*(swirlinpoint) != '@')
      {
        swirladd(*(swirlinpoint), swirlx+swirlextrax, swirly);

        swirlinpoint++;
        swirlx += 32;
        if (swirlx == 640)
    {
      swirlx = 0;
      swirly += 32;
    }
      }
  }
    }
}


HEARTBEAT_FN MC_exitgame(void)
{
	return (HEARTBEAT_FN) NULL;
}

UINT16 puzzleoffset[26] = {
  0,0,     // x,y
  0,89,
  129,89,
  129,0,
  320,0,
  320,89,
  449,89,
  449,0,
  0,240,
  0,329,
  129,329,
  129,240,
  320,240
};

Cblitbuf *puzzlepieces[26];

char *puzzlenames[26] = { "assets\\gpiece11.pcx", "assets\\cpiece11.pcx",
        "assets\\gpiece12.pcx", "assets\\cpiece12.pcx",
        "assets\\gpiece13.pcx", "assets\\cpiece13.pcx",
        "assets\\gpiece14.pcx", "assets\\cpiece14.pcx",
        "assets\\gpiece21.pcx", "assets\\cpiece21.pcx",
        "assets\\gpiece22.pcx", "assets\\cpiece22.pcx",
        "assets\\gpiece23.pcx", "assets\\cpiece23.pcx",
        "assets\\gpiece24.pcx", "assets\\cpiece24.pcx",
        "assets\\gpiece31.pcx", "assets\\cpiece31.pcx",
        "assets\\gpiece32.pcx", "assets\\cpiece32.pcx",
        "assets\\gpiece33.pcx", "assets\\cpiece33.pcx",
        "assets\\gpiece34.pcx", "assets\\cpiece34.pcx",
        "assets\\gpiece4.pcx", "assets\\cpiece4.pcx"
};

Cblitbuf *puzzlecyfers[11];

Cblitbuf *puzzlediamond;

char *puzzlecyfernames[11] = {
  "assets\\cyf_0.pcx",
  "assets\\cyf_1.pcx",
  "assets\\cyf_2.pcx",
  "assets\\cyf_3.pcx",
  "assets\\cyf_4.pcx",
  "assets\\cyf_5.pcx",
  "assets\\cyf_6.pcx",
  "assets\\cyf_7.pcx",
  "assets\\cyf_8.pcx",
  "assets\\cyf_9.pcx",
  "assets\\cyf_s.pcx"
};

UINT16 cyferoffset[26] = {
    0,  0,
    0,120,
  160,120,
  160,  0,

  320,  0,
  320,120,
  480,120,
  480,  0,

    0,240,
    0,360,
  160,360,
  160,240,

  320,240,
};


HEARTBEAT_FN MC_preppuzzleselect(void)
{
  UINT16 i;
  UINT16 rc;

	LOG("H\n");
  video->DrawLoading();
  puzzlepic = new Cblitbuf(640,480,0,0);
	LOG("I\n");
  for (i=0; i<26; i++)
  { 
	  char logbuf[100];
	  sprintf(logbuf, "%d)%s\n",i,puzzlenames[i]);
	  LOG(logbuf);
      puzzlepieces[i] = new Cblitbuf (puzzlenames[i], 0, 0);
    }
	LOG("J\n");

  for (i=0; i<11; i++)
  {
      puzzlecyfers[i] = new Cblitbuf (puzzlecyfernames[i], 0, 0);
  }
	LOG("K\n");

  puzzlediamond  = new Cblitbuf ("assets\\diamond.pcx", 0, 0);

  rc = PCX_ff_load("assets\\cpiece11.pcx", NULL, puzzlepal);
//  vgassert(rc);
  puzzlepal[0] = 0;
  puzzlepal[1] = 0;
  puzzlepal[2] = 0;

	LOG("L\n");

  video->clear(0);
  video->swap();
  video->clear(0);
  video->swap();
  video->clear(0);
  video->swap();
  video->palette(puzzlepal);

  for (i=0; i<13; i++)
  {
      if (i <= maxlevel)
	  {
		  puzzlepieces[i*2+1]->draw(*puzzlepic, puzzleoffset[i*2], puzzleoffset[i*2+1]);
	  }
      else
	  {
		  puzzlepieces[i*2  ]->draw(*puzzlepic, puzzleoffset[i*2], puzzleoffset[i*2+1]);
	  }
	  
#ifndef POCKETPCDEMO     // bij de demo build de diamonds niet laten zien
      if (i <= maxlevel && i!=12)
	  {
		  puzzlediamond->draw(*puzzlepic, cyferoffset[i*2]+30, cyferoffset[i*2+1]+44);
		  puzzlecyfers[blacksperlevel[i]]->draw(*puzzlepic, cyferoffset[i*2]+70, cyferoffset[i*2+1]+44);
	  }
      else
	  {
	  }
#endif
  }

// afdrukken totaal diamanten nodig voor world 4
  
#ifndef POCKETPCDEMO     // bij de demo build de diamonds niet laten zien
  UINT16 totblack;

  totblack = 0;
  for (i=0; i<12; i++)
  {
    totblack += blacksperlevel[i];
  }

  puzzlediamond->draw(*puzzlepic, cyferoffset[24]+70, cyferoffset[25]+104);
  puzzlecyfers[totblack/10]->draw(*puzzlepic, cyferoffset[24]+102, cyferoffset[25]+104);
  puzzlecyfers[totblack%10]->draw(*puzzlepic, cyferoffset[24]+134, cyferoffset[25]+104);
  puzzlecyfers[10]->draw(*puzzlepic, cyferoffset[24]+166, cyferoffset[25]+104);
  puzzlecyfers[7]->draw(*puzzlepic, cyferoffset[24]+197, cyferoffset[25]+104);
  puzzlecyfers[7]->draw(*puzzlepic, cyferoffset[24]+229, cyferoffset[25]+104);
#endif

  
  puzzleflash = 0;
  curpuzzle = world*4+level;

#ifdef POCKETPCDEMO
  curpuzzle = 1;
#endif

  puzzleactiveflg = 1;

//    if (maxlevel != 0 && puzzleinteractive)   return (HEARTBEAT_FN) MC_puzzleselect;
    if (puzzleinteractive)   return (HEARTBEAT_FN) MC_puzzleselect;

  return (HEARTBEAT_FN) MC_puzzleshow;
}




HEARTBEAT_FN MC_puzzleselect(void)
{
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  puzzleflash++;
  if ((puzzleflash/12) & 1) 
    {
      puzzlepieces[curpuzzle*2+1]->draw(*vidblitbuf, puzzleoffset[curpuzzle*2], puzzleoffset[curpuzzle*2+1]);
    }
  else
    {
      puzzlepieces[curpuzzle*2  ]->draw(*vidblitbuf, puzzleoffset[curpuzzle*2], puzzleoffset[curpuzzle*2+1]);
    }

  if (rightkey && (curpuzzle < maxlevel))
    {
      rightkey = 0;
      curpuzzle++;
      puzzleflash = 0;
    }

  if (leftkey && (curpuzzle > 0))
    {
      leftkey = 0;
      curpuzzle--;
      puzzleflash = 0;
    }

	if(g_MouseFlg)
	{
		int x,y;
//		x = 320-g_MouseYDown;
//		y = g_MouseXDown;
        x = g_MouseXDown;
        y = g_MouseYDown;

		x /= 160;
		y /= 120;
		y*=4;

		x+=y;
        {
            int newlevel;
            switch (x)
            {
                default:
                case 0:
                    world = 0;
                    level = 0;
                    break;
                case 1:
                    world = 0;
                    level = 3;
                    break;
                case 2:
                    world = 1;
                    level = 0;
                    break;
                case 3:
                    world = 1;
                    level = 3;
                    break;
                case 4:
                    world = 0;
                    level = 1;
                    break;
                case 5:
                    world = 0;
                    level = 2;
                    break;
                case 6:
                    world = 1;
                    level = 1;
                    break;
                case 7:
                    world = 1;
                    level = 2;
                    break;
                case 8:
                    world = 2;
                    level = 0;
                    break;
                case 9:
                    world = 2;
                    level = 3;
                    break;
                case 10:
                    world = 3;
                    level = 0;
                    break;
                case 11:
                    world = 3;
                    level = 0;
                    break;
                case 12:
                    world = 2;
                    level = 1;
                    break;
                case 13:
                    world = 2;
                    level = 2;
                    break;
                case 14:
                    world = 3;
                    level = 0;
                    break;
                case 15:
                    world = 3;
                    level = 0;
                    break;
            }
            if((world*4+level)>maxlevel)
            {
                g_MouseFlg = 0;
                shootkey = 0;
            }
        }

	}
	if(g_MouseFlg)
	{
//#ifdef POCKETPCDEMO
//		if((world==0 && level==1) || (world==2 && level==0))
//		{
//#endif
            
			shootkey = 0;
			configure_level(world+1, level+1);   // numbers starting from 1 (not 0)
			return (HEARTBEAT_FN) MC_endpuzzle;
//#ifdef POCKETPCDEMO
//		}
//#endif
    }


  if (keytab[VK_ESCAPE])
  {
      keytab[VK_ESCAPE] = 0;
      return (HEARTBEAT_FN) MC_abortpuzzle;
  }  
  if ( shootkey ) 
    {
      world = curpuzzle/4;
      level = curpuzzle%4;
      shootkey = 0;

#ifdef POCKETPCDEMO
		if((world==0 && level==1) || (world==2 && level==0))
		{
#endif
      
		  configure_level(world+1, level+1);   // numbers starting from 1 (not 0)
		  return (HEARTBEAT_FN) MC_endpuzzle;
#ifdef POCKETPCDEMO
		}
#endif
    }

  return (HEARTBEAT_FN) MC_puzzleselect;
}


HEARTBEAT_FN MC_puzzleshow(void)
{
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  handleinput1shot();

  puzzleflash++;

  curpuzzle = world*4+level;
  if ((puzzleflash/25) & 1) 
    {
      puzzlepieces[curpuzzle*2+1]->draw(*vidblitbuf, puzzleoffset[curpuzzle*2], puzzleoffset[curpuzzle*2+1]);
    }
  else
    {
      puzzlepieces[curpuzzle*2  ]->draw(*vidblitbuf, puzzleoffset[curpuzzle*2], puzzleoffset[curpuzzle*2+1]);
    }

  if (puzzleflash > 178 || shootkey) 
    {
      shootkey = 0;
      return (HEARTBEAT_FN) MC_endpuzzle;
    }

  if (keytab[VK_ESCAPE])
  {
      keytab[VK_ESCAPE] = 0;
      return (HEARTBEAT_FN) MC_abortpuzzle;
  }  

  return (HEARTBEAT_FN) MC_puzzleshow;
}


HEARTBEAT_FN MC_endpuzzle(void)
{
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  killpuzzle();
  return (HEARTBEAT_FN) MC_loadlevel;
}

HEARTBEAT_FN MC_abortpuzzle(void)
{
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);
  video->swap();
  video->scansync();
  puzzlepic->draw(*vidblitbuf, 0, 0, 0, 0, 640, 480);

  killpuzzle();
  return (HEARTBEAT_FN) MC_preptitlesequence;
}

void killpuzzle(void)
{
  UINT16 i;

    puzzleactiveflg = 0;
  if (puzzlepic) delete puzzlepic;
  puzzlepic = 0;

  for (i=0; i<26; i++)
    {
      if (puzzlepieces[i]) delete puzzlepieces[i];
      puzzlepieces[i] = 0;
    }

  for (i=0; i<10; i++)
    {
      if (puzzlecyfers[i]) delete puzzlecyfers[i];
      puzzlecyfers[i] = 0;
    }

  if (puzzlediamond) 
  {
	delete puzzlediamond;
	puzzlediamond = 0;
  }
}

