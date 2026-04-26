#include <frm_wrk.hpp>
#include <sound.hpp>
#include <objects.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <mc.hpp>

#define DEMOVERSION2

void calcvp(INT32 *v, INT32 *p, INT32 x, INT32 y);  //snelle vervaag
void calcvp2(INT32 *v, INT32 *p, INT32 x, INT32 y);  //langzame vervaag

int wav_id_mcskid = 16;
int wav_id_spring = 17;
int wav_id_mcwall = 18;
int wav_id_mcfall = 20;
int wav_id_mcdrain = 19;
int wav_id_blub = 21;
int wav_id_door = 22;
int wav_id_switch = 23;
int wav_id_bonus = 24;
int wav_id_warp = 25;
int wav_id_shoot = 26;
int wav_id_rumble = 27;
int wav_id_waterval = 28;
int wav_id_mcloop1 = 29;
int wav_id_mcloop2 = 30;
int wav_id_sapstap1 = 31;
int wav_id_sapstap2 = 32;
int wav_id_helmlp1 = 33;
int wav_id_helmlp2 = 34;
int wav_id_houtpunt = 35;
int wav_id_ball = 36;
int wav_id_segmshot = 37;
int wav_id_segmexpl = 38;
int wav_id_segmhit = 39;
int wav_id_vlamwerp = 40;
int wav_id_tanden = 41;
int wav_id_tandenm = 42;
int wav_id_camstart = 43;
int wav_id_camstop = 44;
int wav_id_cammove = 45;
int wav_id_madeit = 46;
int wav_id_heks = 47;
int wav_id_graspod = 48;
int wav_id_bat = 49;
int wav_id_vogel = 50;
int wav_id_vuur = 51;
int wav_id_drilboor = 52;
int wav_id_loopband = 53;
int wav_id_ptoei = 57;
int wav_id_bee = 55;
int wav_id_bee2 = 56;
int wav_id_ventltor = 54;
int wav_id_schuif = 58;
int wav_id_smexp = 59;
int wav_id_backpak = 60;
int wav_id_restart = 61;
int wav_id_bigexp = 62;
int wav_id_cannon = 64;
int wav_id_stroom = 63;
int wav_id_gewicht = 65;
int wav_id_wheel = 66;
int wav_id_appel = 67;
int wav_id_mcdood = 68;
int wav_id_mcfart = 69;
int wav_id_chemo = 70;
int wav_id_tik = 71;
int wav_id_tak = 72;
int wav_id_raket = 73;
int wav_id_chemo2 = 74;
int wav_id_helmdood = 75;
int wav_id_ketting = 76;
int wav_id_dimndsht = 77;
int wav_id_glasblok = 78;
int wav_id_hyprlift = 79;
int wav_id_lightwav = 80;
int wav_id_morphsht = 81;
int wav_id_mushup = 82;
int wav_id_mushdwn = 83;
int wav_id_plntlft = 84;
int wav_id_pltfdwn = 85;
int wav_id_pltfup = 86;
int wav_id_qbert1 = 87;
int wav_id_qbert2 = 88;
int wav_id_roltnlp = 89;
int wav_id_slowlift = 90;
int wav_id_tangjmp = 91;
int wav_id_tangclos = 92;
int wav_id_woeiwoei = 93;
int wav_id_heatsnkr = 94;
int wav_id_demo = 95;

HSNDOBJ wav_mcskid = 0;
HSNDOBJ wav_spring = 0;
HSNDOBJ wav_mcwall = 0;
HSNDOBJ wav_mcfall = 0;
HSNDOBJ wav_mcdrain = 0;
HSNDOBJ wav_blub = 0;
HSNDOBJ wav_door = 0;
HSNDOBJ wav_switch = 0;
HSNDOBJ wav_bonus = 0;
HSNDOBJ wav_warp = 0;
HSNDOBJ wav_shoot = 0;
HSNDOBJ wav_rumble = 0;
HSNDOBJ wav_waterval = 0;
HSNDOBJ wav_mcloop1 = 0;
HSNDOBJ wav_mcloop2 = 0;
HSNDOBJ wav_sapstap1 = 0;
HSNDOBJ wav_sapstap2 = 0;
HSNDOBJ wav_helmlp1 = 0;
HSNDOBJ wav_helmlp2 = 0;
HSNDOBJ wav_houtpunt = 0;
HSNDOBJ wav_ball = 0;
HSNDOBJ wav_segmshot = 0;
HSNDOBJ wav_segmexpl = 0;
HSNDOBJ wav_segmhit = 0;
HSNDOBJ wav_vlamwerp = 0;
HSNDOBJ wav_tanden =0;
HSNDOBJ wav_tandenm = 0;
HSNDOBJ wav_camstart = 0;
HSNDOBJ wav_camstop = 0;
HSNDOBJ wav_cammove = 0;
HSNDOBJ wav_madeit = 0;
HSNDOBJ wav_heks = 0;
HSNDOBJ wav_graspod = 0;
HSNDOBJ wav_bat = 0;
HSNDOBJ wav_vogel = 0;
HSNDOBJ wav_vuur = 0;
HSNDOBJ wav_drilboor = 0;
HSNDOBJ wav_loopband = 0;
HSNDOBJ wav_ptoei = 0;
HSNDOBJ wav_bee = 0;
HSNDOBJ wav_bee2 = 0;
HSNDOBJ wav_ventltor = 0;
HSNDOBJ wav_schuif = 0;
HSNDOBJ wav_smexp = 0;
HSNDOBJ wav_backpak = 0;
HSNDOBJ wav_restart = 0;
HSNDOBJ wav_bigexp = 0;
HSNDOBJ wav_cannon = 0;
HSNDOBJ wav_stroom = 0;
HSNDOBJ wav_gewicht = 0;
HSNDOBJ wav_wheel = 0;
HSNDOBJ wav_appel = 0;
HSNDOBJ wav_mcdood = 0;
HSNDOBJ wav_mcfart = 0;
HSNDOBJ wav_chemo = 0;
HSNDOBJ wav_tik = 0;
HSNDOBJ wav_tak = 0;
HSNDOBJ wav_raket = 0;
HSNDOBJ wav_chemo2 = 0;
HSNDOBJ wav_helmdood = 0;
HSNDOBJ wav_ketting = 0;
HSNDOBJ wav_dimndsht = 0;
HSNDOBJ wav_glasblok = 0;
HSNDOBJ wav_hyprlift = 0;
HSNDOBJ wav_lightwav = 0;
HSNDOBJ wav_morphsht = 0;
HSNDOBJ wav_mushup = 0;
HSNDOBJ wav_mushdwn = 0;
HSNDOBJ wav_plntlft = 0;
HSNDOBJ wav_pltfdwn = 0;
HSNDOBJ wav_pltfup = 0;
HSNDOBJ wav_qbert1 = 0;
HSNDOBJ wav_qbert2 = 0;
HSNDOBJ wav_roltnlp = 0;
HSNDOBJ wav_slowlift = 0;
HSNDOBJ wav_tangjmp = 0;
HSNDOBJ wav_tangclos = 0;
HSNDOBJ wav_woeiwoei = 0;
HSNDOBJ wav_heatsnkr = 0;
HSNDOBJ wav_demo = 0;



INT32 rumblevol = -10000;
INT32 chemo2vol = -10000;


INT32	proxvogellvolume = -10000;
INT32	proxvogelrvolume = -10000;
INT32	proxstroomlvolume = -10000;
INT32	proxstroomrvolume = -10000;
INT32	proxbatlvolume = -10000;
INT32	proxbatrvolume = -10000;
INT32	proxbeelvolume = -10000;
INT32	proxbeervolume = -10000;
INT32	proxbee2lvolume = -10000;
INT32	proxbee2rvolume = -10000;
INT32	proxvlamwerplvolume = -10000;
INT32	proxvlamwerprvolume = -10000;
INT32	proxventltorlvolume = -10000;
INT32	proxventltorrvolume = -10000;
INT32	proxwatervallvolume = -10000;
INT32	proxwatervalrvolume = -10000;
INT32	proxvuurlvolume = -10000;
INT32	proxvuurrvolume = -10000;
INT32	proxdrilboorlvolume = -10000;
INT32	proxdrilboorrvolume = -10000;
INT32	proxloopbandlvolume = -10000;
INT32	proxloopbandrvolume = -10000;
INT32	proxchemolvolume = -10000;
INT32	proxchemorvolume = -10000;
INT32	proxslowliftlvolume = -10000;
INT32	proxslowliftrvolume = -10000;
INT32	proxroltnlplvolume = -10000;
INT32	proxroltnlprvolume = -10000;
INT32	proxwoeiwoeilvolume = -10000;
INT32	proxwoeiwoeirvolume = -10000;
INT32	proxheatsnkrlvolume = -10000;
INT32	proxheatsnkrrvolume = -10000;


void sound_load(void)
{

  if (audio->get_dsound() && sfxflg) // is dsound on?
  {

    if ((world == 0 || world == 4))   // or demolevel
    {
      wav_graspod = audio->create_sound(wav_id_graspod,1);
      wav_bat     = audio->create_sound(wav_id_bat,1);
      wav_vogel   = audio->create_sound(wav_id_vogel,1);
    }

    if (world == 1)
    {
      wav_vuur       = audio->create_sound(wav_id_vuur,1);
      wav_drilboor   = audio->create_sound(wav_id_drilboor,1);
      wav_loopband   = audio->create_sound(wav_id_loopband,1);
    }

    if ((world == 0 && level == 0) || (world == 0 && level == 2))
    {
      wav_waterval = audio->create_sound(wav_id_waterval,1);
    }

    if ((world == 0 && level == 2) || (world == 1 && level == 1)) // sap geluidejs door blubber
    {
      wav_sapstap1 = audio->create_sound(wav_id_sapstap1,1);
      wav_sapstap2 = audio->create_sound(wav_id_sapstap2,1);
    }
    
    if ((world == 0 && level == 0) || (world == 0 && level == 1) || (world == 0 && level == 3) || (world == 1 && level == 1) || (world == 1 && level == 3) || (world == 4))  // switch en deur
    {
      wav_door     = audio->create_sound(wav_id_door,1);
      wav_switch   = audio->create_sound(wav_id_switch,1);
    }

    if ((world == 0 && level == 1))
    {
      wav_spring   = audio->create_sound(wav_id_spring,1);
      wav_ptoei     = audio->create_sound(wav_id_ptoei,1);
    }

	  if ((world == 0 && level == 2))  // aardbeving   en houtpunt   en bal
    {
      wav_heks     = audio->create_sound(wav_id_heks,1);
      wav_rumble   = audio->create_sound(wav_id_rumble,1);
      wav_houtpunt = audio->create_sound(wav_id_houtpunt,2);
      wav_ball     = audio->create_sound(wav_id_ball,2);
      wav_bee      = audio->create_sound(wav_id_bee,1);
    }

    if ((world == 0 && level == 3))  // segment shit en tanden
    {
      wav_segmshot = audio->create_sound(wav_id_segmshot,1);
      wav_segmexpl = audio->create_sound(wav_id_segmexpl,1);
      wav_segmhit  = audio->create_sound(wav_id_segmhit,1);
      wav_tanden   = audio->create_sound(wav_id_tanden,2);
      wav_tandenm  = audio->create_sound(wav_id_tandenm,2);
      wav_camstart = audio->create_sound(wav_id_camstart,1);
      wav_camstop  = audio->create_sound(wav_id_camstop,1);
      wav_cammove  = audio->create_sound(wav_id_cammove,1);
      wav_glasblok = audio->create_sound(wav_id_glasblok,1);
      wav_mushup   = audio->create_sound(wav_id_mushup,1);
      wav_mushdwn  = audio->create_sound(wav_id_mushdwn,1);
      wav_plntlft  = audio->create_sound(wav_id_plntlft,1);
      wav_roltnlp  = audio->create_sound(wav_id_roltnlp,1);
    }

    if ((world == 1 && level == 0) || (world == 1 && level == 1)) // loop geluid helmut
    {
      wav_helmlp1  = audio->create_sound(wav_id_helmlp1,1);
      wav_helmlp2  = audio->create_sound(wav_id_helmlp2,1);
      wav_helmdood = audio->create_sound(wav_id_helmdood,1);
    }

    if ((world == 1 && level == 0) )
    {
      wav_ketting  = audio->create_sound(wav_id_ketting,1);
    }
    
    if ((world == 1 && level == 0) || (world == 1 && level == 2))
    {
      wav_gewicht  = audio->create_sound(wav_id_gewicht,1);
    }

    if ( (world == 1 && level == 1))
    {
      wav_ventltor = audio->create_sound(wav_id_ventltor,1);
      wav_chemo = audio->create_sound(wav_id_chemo,1);
      wav_chemo2 = audio->create_sound(wav_id_chemo2,1);
    }

    if ((world == 1 && level == 1) || (world == 1 && level == 2))
    {
      wav_bee2  = audio->create_sound(wav_id_bee2,1);
    }

    if ((world == 1 && level == 2))
    {
      wav_cannon   = audio->create_sound(wav_id_cannon,4);
      wav_wheel    = audio->create_sound(wav_id_wheel,4);
      wav_pltfdwn  = audio->create_sound(wav_id_pltfdwn,8);
      wav_pltfup   = audio->create_sound(wav_id_pltfup,8);
    }

    if ((world == 1 && level == 3))
    {
      wav_graspod = audio->create_sound(wav_id_graspod,1);
      wav_hyprlift = audio->create_sound(wav_id_hyprlift,1);
      wav_slowlift = audio->create_sound(wav_id_slowlift,1);
    }

    if (world == 2)      // schietgeluid
    {
      wav_shoot    = audio->create_sound(wav_id_shoot,1);
      wav_vlamwerp = audio->create_sound(wav_id_vlamwerp,1);
      wav_backpak  = audio->create_sound(wav_id_backpak,1);
      wav_schuif   = audio->create_sound(wav_id_schuif,1);
      wav_stroom   = audio->create_sound(wav_id_stroom,1);
      wav_cannon   = audio->create_sound(wav_id_cannon,1);
      wav_raket    = audio->create_sound(wav_id_raket,1);
    }

    if ((world == 2 && level == 1))
	{
      wav_qbert1     = audio->create_sound(wav_id_qbert1,1);
      wav_qbert2     = audio->create_sound(wav_id_qbert2,1);
	}

    if ((world == 2 && level == 2))
	{
      wav_morphsht     = audio->create_sound(wav_id_morphsht,1);
	}

    if ((world == 2 && level == 3))
	{
      wav_heatsnkr     = audio->create_sound(wav_id_heatsnkr,1);
      wav_tangjmp     = audio->create_sound(wav_id_tangjmp,1);
      wav_tangclos     = audio->create_sound(wav_id_tangclos,1);
	}

    if ((world == 0 && level == 1))  // blub geluidje van robynen kogeltje
    {
      wav_blub     = audio->create_sound(wav_id_blub,2);
    }
    
    if (world == 3)
    {
      wav_graspod = audio->create_sound(wav_id_graspod,1);
      wav_tik     = audio->create_sound(wav_id_tik,1);
      wav_tak     = audio->create_sound(wav_id_tak,1);
      wav_lightwav= audio->create_sound(wav_id_lightwav,1);
      wav_dimndsht= audio->create_sound(wav_id_dimndsht,1);
      wav_woeiwoei= audio->create_sound(wav_id_woeiwoei,1);
    }

    // deze sound-fx altijd:

    wav_smexp    = audio->create_sound(wav_id_smexp,4);
    wav_bigexp   = audio->create_sound(wav_id_bigexp,4);

    wav_mcskid   = audio->create_sound(wav_id_mcskid,1);
    wav_mcwall   = audio->create_sound(wav_id_mcwall,1);
    wav_mcfall   = audio->create_sound(wav_id_mcfall,1);
    wav_mcdood   = audio->create_sound(wav_id_mcdood,1);
    wav_mcfart   = audio->create_sound(wav_id_mcfart,1);
    wav_mcdrain  = audio->create_sound(wav_id_mcdrain,8);
    wav_bonus    = audio->create_sound(wav_id_bonus,1);
    wav_appel    = audio->create_sound(wav_id_appel,1);
    wav_madeit   = audio->create_sound(wav_id_madeit,1);
    wav_restart  = audio->create_sound(wav_id_restart,1);
    wav_mcloop1  = audio->create_sound(wav_id_mcloop1,1);
    wav_mcloop2  = audio->create_sound(wav_id_mcloop2,1);
    wav_warp     = audio->create_sound(wav_id_warp,1);

	wav_demo = 0;

#ifdef DEMOVERSION2
//    wav_demo     = audio->create_sound(WAV_DEMO,1);
#endif
  }
}

void sound_destroy(void)
{
//  log_out("deleting sounds");
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    if (wav_mcskid) audio->destroy_sound(wav_mcskid);
    wav_mcskid = 0;
    if (wav_spring) audio->destroy_sound(wav_spring);
    wav_spring = 0;
    if (wav_mcwall) audio->destroy_sound(wav_mcwall);
    wav_mcwall = 0;
    if (wav_mcfall) audio->destroy_sound(wav_mcfall);
    wav_mcfall = 0;
    if (wav_mcdrain) audio->destroy_sound(wav_mcdrain);
    wav_mcdrain = 0;
    if (wav_blub) audio->destroy_sound(wav_blub);
    wav_blub = 0;
    if (wav_door) audio->destroy_sound(wav_door);
    wav_door = 0;
    if (wav_switch) audio->destroy_sound(wav_switch);
    wav_switch = 0;
    if (wav_bonus) audio->destroy_sound(wav_bonus);
    wav_bonus = 0;
    if (wav_warp) audio->destroy_sound(wav_warp);
    wav_warp = 0;
    if (wav_shoot) audio->destroy_sound(wav_shoot);
    wav_shoot = 0;
    if (wav_rumble) audio->destroy_sound(wav_rumble);
    wav_rumble = 0;
    if (wav_waterval) audio->destroy_sound(wav_waterval);
    wav_waterval = 0;
    if (wav_mcloop1) audio->destroy_sound(wav_mcloop1);
    wav_mcloop1 = 0;
    if (wav_mcloop2) audio->destroy_sound(wav_mcloop2);
    wav_mcloop2 = 0;
    if (wav_sapstap1) audio->destroy_sound(wav_sapstap1);
    wav_sapstap1 = 0;
    if (wav_sapstap2) audio->destroy_sound(wav_sapstap2);
    wav_sapstap2 = 0;
    if (wav_helmlp1) audio->destroy_sound(wav_helmlp1);
    wav_helmlp1 = 0;
    if (wav_helmlp2) audio->destroy_sound(wav_helmlp2);
    wav_helmlp2 = 0;
    if (wav_houtpunt) audio->destroy_sound(wav_houtpunt);
    wav_houtpunt = 0;
    if (wav_ball) audio->destroy_sound(wav_ball);
    wav_ball = 0;
    if (wav_segmshot) audio->destroy_sound(wav_segmshot);
    wav_segmshot = 0;
    if (wav_segmexpl) audio->destroy_sound(wav_segmexpl);
    wav_segmexpl = 0;
    if (wav_segmhit) audio->destroy_sound(wav_segmhit);
    wav_segmhit = 0;
    if (wav_vlamwerp) audio->destroy_sound(wav_vlamwerp);
    wav_vlamwerp = 0;
    if (wav_tanden) audio->destroy_sound(wav_tanden);
    wav_tanden = 0;
    if (wav_tandenm) audio->destroy_sound(wav_tandenm);
    wav_tandenm = 0;
    if (wav_camstart) audio->destroy_sound(wav_camstart);
    wav_camstart = 0;
    if (wav_camstop) audio->destroy_sound(wav_camstop);
    wav_camstop = 0;
    if (wav_cammove) audio->destroy_sound(wav_cammove);
    wav_cammove = 0;
    if (wav_madeit) audio->destroy_sound(wav_madeit);
    wav_madeit = 0;
    if (wav_heks) audio->destroy_sound(wav_heks);
    wav_heks = 0;
    if (wav_graspod) audio->destroy_sound(wav_graspod);
    wav_graspod = 0;
    if (wav_bat) audio->destroy_sound(wav_bat);
    wav_bat = 0;
    if (wav_vogel) audio->destroy_sound(wav_vogel);
    wav_vogel = 0;
    if (wav_vuur) audio->destroy_sound(wav_vuur);
    wav_vuur = 0;
    if (wav_drilboor) audio->destroy_sound(wav_drilboor);
    wav_drilboor = 0;
    if (wav_loopband) audio->destroy_sound(wav_loopband);
    wav_loopband = 0;
    if (wav_ptoei) audio->destroy_sound(wav_ptoei);
    wav_ptoei = 0;
    if (wav_bee) audio->destroy_sound(wav_bee);
    wav_bee = 0;
    if (wav_bee2) audio->destroy_sound(wav_bee2);
    wav_bee2 = 0;
    if (wav_ventltor) audio->destroy_sound(wav_ventltor);
    wav_ventltor = 0;
    if (wav_schuif) audio->destroy_sound(wav_schuif);
    wav_schuif = 0;
    if (wav_smexp) audio->destroy_sound(wav_smexp);
    wav_smexp = 0;
    if (wav_backpak) audio->destroy_sound(wav_backpak);
    wav_backpak = 0;
    if (wav_restart) audio->destroy_sound(wav_restart);
    wav_restart = 0;
    if (wav_bigexp) audio->destroy_sound(wav_bigexp);
    wav_bigexp = 0;
    if (wav_gewicht) audio->destroy_sound(wav_gewicht);
    wav_gewicht = 0;
    if (wav_stroom) audio->destroy_sound(wav_stroom);
    wav_stroom = 0;
    if (wav_cannon) audio->destroy_sound(wav_cannon);
    wav_cannon = 0;
    if (wav_wheel) audio->destroy_sound(wav_wheel);
    wav_wheel = 0;
    if (wav_appel) audio->destroy_sound(wav_appel);
    wav_appel = 0;
    if (wav_mcdood) audio->destroy_sound(wav_mcdood);
    wav_mcdood = 0;
    if (wav_mcfart) audio->destroy_sound(wav_mcfart);
    wav_mcfart = 0;
    if (wav_chemo) audio->destroy_sound(wav_chemo);
    wav_chemo = 0;
    if (wav_tik) audio->destroy_sound(wav_tik);
    wav_tik = 0;
    if (wav_tak) audio->destroy_sound(wav_tak);
    wav_tak = 0;
    if (wav_raket) audio->destroy_sound(wav_raket);
    wav_raket = 0;
    if (wav_chemo2) audio->destroy_sound(wav_chemo2);
    wav_chemo2 = 0;
    if (wav_helmdood) audio->destroy_sound(wav_helmdood);
    wav_helmdood = 0;
    if (wav_ketting) audio->destroy_sound(wav_ketting);
    wav_ketting = 0;
    if (wav_dimndsht) audio->destroy_sound(wav_dimndsht);
    wav_dimndsht = 0;
    if (wav_glasblok) audio->destroy_sound(wav_glasblok);
    wav_glasblok = 0;
    if (wav_hyprlift) audio->destroy_sound(wav_hyprlift);
    wav_hyprlift = 0;
    if (wav_lightwav) audio->destroy_sound(wav_lightwav);
    wav_lightwav = 0;
    if (wav_morphsht) audio->destroy_sound(wav_morphsht);
    wav_morphsht = 0;
    if (wav_mushup) audio->destroy_sound(wav_mushup);
    wav_mushup = 0;
    if (wav_mushdwn) audio->destroy_sound(wav_mushdwn);
    wav_mushdwn = 0;
    if (wav_plntlft) audio->destroy_sound(wav_plntlft);
    wav_plntlft = 0;
    if (wav_pltfdwn) audio->destroy_sound(wav_pltfdwn);
    wav_pltfdwn = 0;
    if (wav_pltfup) audio->destroy_sound(wav_pltfup);
    wav_pltfup = 0;
    if (wav_qbert1) audio->destroy_sound(wav_qbert1);
    wav_qbert1 = 0;
    if (wav_qbert2) audio->destroy_sound(wav_qbert2);
    wav_qbert2 = 0;
    if (wav_roltnlp) audio->destroy_sound(wav_roltnlp);
    wav_roltnlp = 0;
    if (wav_slowlift) audio->destroy_sound(wav_slowlift);
    wav_slowlift = 0;
    if (wav_tangjmp) audio->destroy_sound(wav_tangjmp);
    wav_tangjmp = 0;
    if (wav_tangclos) audio->destroy_sound(wav_tangclos);
    wav_tangclos = 0;
    if (wav_woeiwoei) audio->destroy_sound(wav_woeiwoei);
    wav_woeiwoei = 0;
    if (wav_heatsnkr) audio->destroy_sound(wav_heatsnkr);
    wav_heatsnkr = 0;
    if (wav_demo) audio->destroy_sound(wav_demo);
    wav_demo = 0;
  }
  audio->reset_audio();
}


void play_demosound(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
//    audio->play_sound_loop(wav_demo,0,0);
  }
}

void play_mcskid(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_mcskid,v,p);
  }
}

void play_spring(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_spring);
    audio->play_sound_1shot(wav_spring,v,p);
  }
}

void play_mcfart(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_mcfart);
    audio->play_sound_1shot(wav_mcfart,v,p);
  }
}

void play_appel(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_appel);
    audio->play_sound_1shot(wav_appel,v,p);
  }
}

void play_mcdood(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_mcdood);
    audio->play_sound_1shot(wav_mcdood,v,p);
  }
}

void play_wheel(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
//    audio->stop_cursound(wav_wheel);
    audio->play_sound_1shot(wav_wheel,v,p);
  }
}

void play_mcwall(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_mcwall);
    audio->play_sound_1shot(wav_mcwall,v,p);
  }
}

void play_mcfall(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_mcfall,v,p);
  }
}

void play_mcdrain(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_1shot(wav_mcdrain,0,0);
  }
}

void play_blub(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_blub,v,p);
  }
}

void play_door(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->stop_sound(wav_door);
    audio->play_sound_1shot(wav_door,0,0);
  }
}

void play_switch(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->stop_sound(wav_switch);
    audio->play_sound_1shot(wav_switch,0,0);
  }
}

void play_bonus(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_bonus);
    audio->play_sound_1shot(wav_bonus,v,p);
  }
}

void play_warp(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->stop_sound(wav_warp);
    audio->play_sound_1shot(wav_warp,0,0);
  }
}

void play_shoot(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_shoot);
    audio->play_sound_1shot(wav_shoot,v,p);
  }
}

void play_rumble(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
//    log_out("playing rumble");
    rumblevol = -10000;
    audio->play_sound_loop(wav_rumble,0,0);
    rumblevolume(-10000);
//    log_out("rumble started");
  }
}


void stop_rumble(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->stop_sound(wav_rumble);
  }
}


void rumblevolume(INT32 volume)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->sound_volume(wav_rumble,volume);
  }
}


void rumblesoundharder()
{
  if (rumblevol < 300) rumblevol += 200;
  rumblevolume((rumblevol>0) ? 0 : rumblevol);
}

void rumblesoundsofter()
{
  if (rumblevol > -10000) rumblevol -= 100;
  rumblevolume((rumblevol>0) ? 0 : rumblevol);
}


void play_waterval(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_waterval,0,0);
  }
}


void stop_waterval(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->stop_sound(wav_waterval);
  }
}


void play_mcloop(UINT16 sapflg, INT32 x, INT32 y)
{
  static UINT16 switcher;

  switcher ^= 1;

  if (switcher)
  {
    switch (sapflg)
      {
      case 0:
  play_mcloop1(x,y);
  break;
      case 1:
  play_sapstap1(x,y);
  break;
      case 2:
  play_helmlp1(x,y);
  break;
      }
  }
  else
  {
    switch (sapflg)
      {
      case 0:
  play_mcloop2(x,y);
  break;
      case 1:
  play_sapstap2(x,y);
  break;
      case 2:
  play_helmlp2(x,y);
  break;
      }
  }
}

void play_mcloop1(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_mcloop1,v,p);
  }
}

void play_mcloop2(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_mcloop2,v,p);
  }
}

void play_sapstap1(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_sapstap1,v,p);
  }
}

void play_sapstap2(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_sapstap2,v,p);
  }
}

void play_helmlp1(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_helmlp1,v,p);
  }
}

void play_helmlp2(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_helmlp2,v,p);
  }
}

void play_houtpunt(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_houtpunt,v,p);
  }
}

void play_ball(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
//    audio->stop_sound(wav_ball);
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
	audio->play_sound_1shot(wav_ball,v,p);
  }
}

void calcvp(INT32 *v, INT32 *p, INT32 x, INT32 y)
{
	INT32 distxl,distyl;
	INT32 distxr,distyr;

	distxl = abs(hoi->x - (x-64));
	distyl = abs(hoi->y - y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (x+64));
	distyr = abs(hoi->y - y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	distxl = -(distxl*4);
	distxr = -(distxr*4);   // proximity.... is equal to volume

	*v = (distxl > distxr) ? distxl : distxr;
	*p = distxl-distxr;

    if (*v > 0) *v = 0;
}

void calcvp2(INT32 *v, INT32 *p, INT32 x, INT32 y)    // iets langzamere vervaag
{
	INT32 distxl,distyl;
	INT32 distxr,distyr;

	distxl = abs(hoi->x - (x-64));
	distyl = abs(hoi->y - y);
	if (distxl < distyl) distxl = distyl;    // de verste distance wint

	distxr = abs(hoi->x - (x+64));
	distyr = abs(hoi->y - y);
	if (distxr < distyr) distxr = distyr;    // de verste distance wint

	distxl = -(distxl*2);
	distxr = -(distxr*2);   // proximity.... is equal to volume

	*v = (distxl > distxr) ? distxl : distxr;
	*p = distxl-distxr;

    if (*v > 0) *v = 0;
}

void play_tanden(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_tanden,v,p);
  }
}

void play_vlamwerp(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_vlamwerp,0,0);
  }
}

void play_segmhit(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_segmhit,v,p);
  }
}

void play_segmexpl(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_segmexpl,v,p);
  }
}

void play_segmshot(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_segmshot,v,p);
  }
}

void play_tandenm(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_tandenm,v,p);
  }
}

void play_camstop(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_1shot(wav_camstop,0,0);
  }
}

void play_camstart(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_1shot(wav_camstart,0,0);
  }
}

void play_cammove(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_cammove,0,0);
  }
}

void stop_cammove(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    if (wav_cammove) audio->stop_sound(wav_cammove);
  }
}

void play_madeit(void)
{
  if (audio->get_dsound() && sfxflg && !speedrun_state.level_complete) // is dsound on?
  {
    audio->play_sound_1shot(wav_madeit,0,0);
  }
  speedrun_state.level_complete = true;
}

void play_heks(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_heks,v,p);
  }
}

void play_graspod(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_graspod);
    audio->play_sound_1shot(wav_graspod,v,p);
  }
}

void play_loopband(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_loopband,0,0);
  }
}

void play_drilboor(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_drilboor,0,0);
  }
}

void play_vuur(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_vuur,0,0);
  }
}

void play_bat(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_bat,0,0);
  }
}

void play_vogel(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_vogel,0,0);
  }
}

void prox_loopband_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxloopbandlvolume) proxloopbandlvolume = lvolume;
	if (rvolume > proxloopbandrvolume) proxloopbandrvolume = rvolume;
}

void prox_drilboor_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxdrilboorlvolume) proxdrilboorlvolume = lvolume;
	if (rvolume > proxdrilboorrvolume) proxdrilboorrvolume = rvolume;
}

void prox_vuur_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxvuurlvolume) proxvuurlvolume = lvolume;
	if (rvolume > proxvuurrvolume) proxvuurrvolume = rvolume;
}

void prox_bat_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxbatlvolume) proxbatlvolume = lvolume;
	if (rvolume > proxbatrvolume) proxbatrvolume = rvolume;
}

void prox_vogel_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxvogellvolume) proxvogellvolume = lvolume;
	if (rvolume > proxvogelrvolume) proxvogelrvolume = rvolume;
}

void real_loopband_volume(void)
{
  if ((audio->get_dsound()) && (wav_loopband != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_loopband, (proxloopbandlvolume > proxloopbandrvolume) ? proxloopbandlvolume : proxloopbandrvolume);
		audio->sound_pan(wav_loopband, proxloopbandlvolume-proxloopbandrvolume);

		proxloopbandlvolume = -10000;
		proxloopbandrvolume = -10000;
	}
}

void real_drilboor_volume(void)
{
  if ((audio->get_dsound()) && (wav_drilboor != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_drilboor, (proxdrilboorlvolume > proxdrilboorrvolume) ? proxdrilboorlvolume : proxdrilboorrvolume);
		audio->sound_pan(wav_drilboor, proxdrilboorlvolume-proxdrilboorrvolume);

		proxdrilboorlvolume = -10000;
		proxdrilboorrvolume = -10000;
	}
}

void real_vuur_volume(void)
{
  if ((audio->get_dsound()) && (wav_vuur != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_vuur, (proxvuurlvolume > proxvuurrvolume) ? proxvuurlvolume : proxvuurrvolume);
		audio->sound_pan(wav_vuur, proxvuurlvolume-proxvuurrvolume);

		proxvuurlvolume = -10000;
		proxvuurrvolume = -10000;
	}
}


void real_bat_volume(void)
{
  if ((audio->get_dsound()) && (wav_bat != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_bat, (proxbatlvolume > proxbatrvolume) ? proxbatlvolume : proxbatrvolume);
		audio->sound_pan(wav_bat, proxbatlvolume-proxbatrvolume);

		proxbatlvolume = -10000;
		proxbatrvolume = -10000;
	}
}

void real_vogel_volume(void)
{
  if ((audio->get_dsound()) && (wav_vogel != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_vogel, (proxvogellvolume > proxvogelrvolume) ? proxvogellvolume : proxvogelrvolume);
		audio->sound_pan(wav_vogel, proxvogellvolume-proxvogelrvolume);

		proxvogellvolume = -10000;
		proxvogelrvolume = -10000;
	}
}

void prox_waterval_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxwatervallvolume) proxwatervallvolume = lvolume;
	if (rvolume > proxwatervalrvolume) proxwatervalrvolume = rvolume;
}

void real_waterval_volume(void)
{
  if ((audio->get_dsound()) && (wav_waterval != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_waterval, (proxwatervallvolume > proxwatervalrvolume) ? proxwatervallvolume : proxwatervalrvolume);
		audio->sound_pan(wav_waterval, proxwatervallvolume-proxwatervalrvolume);

		proxwatervallvolume = -10000;
		proxwatervalrvolume = -10000;
	}
}

void play_ptoei(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_ptoei,v,p);
  }
}

void play_ventltor(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_ventltor,0,0);
  }
}

void play_bee(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_bee,0,0);
  }
}

void play_bee2(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_bee2,0,0);
  }
}

void prox_ventltor_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxventltorlvolume) proxventltorlvolume = lvolume;
	if (rvolume > proxventltorrvolume) proxventltorrvolume = rvolume;
}

void real_ventltor_volume(void)
{
  if ((audio->get_dsound()) && (wav_ventltor != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_ventltor, (proxventltorlvolume > proxventltorrvolume) ? proxventltorlvolume : proxventltorrvolume);
		audio->sound_pan(wav_ventltor, proxventltorlvolume-proxventltorrvolume);

		proxventltorlvolume = -10000;
		proxventltorrvolume = -10000;
	}
}

void prox_bee_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxbeelvolume) proxbeelvolume = lvolume;
	if (rvolume > proxbeervolume) proxbeervolume = rvolume;
}

void real_bee_volume(void)
{
  if ((audio->get_dsound()) && (wav_bee != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_bee, (proxbeelvolume > proxbeervolume) ? proxbeelvolume : proxbeervolume);
		audio->sound_pan(wav_bee, proxbeelvolume-proxbeervolume);

		proxbeelvolume = -10000;
		proxbeervolume = -10000;
	}
}

void prox_bee2_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxbee2lvolume) proxbee2lvolume = lvolume;
	if (rvolume > proxbee2rvolume) proxbee2rvolume = rvolume;
}

void real_bee2_volume(void)
{
  if ((audio->get_dsound()) && (wav_bee2 != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_bee2, (proxbee2lvolume > proxbee2rvolume) ? proxbee2lvolume : proxbee2rvolume);
		audio->sound_pan(wav_bee2, proxbee2lvolume-proxbee2rvolume);

		proxbee2lvolume = -10000;
		proxbee2rvolume = -10000;
	}
}

void prox_vlamwerp_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxvlamwerplvolume) proxvlamwerplvolume = lvolume;
	if (rvolume > proxvlamwerprvolume) proxvlamwerprvolume = rvolume;
}

void real_vlamwerp_volume(void)
{
  if ((audio->get_dsound()) && (wav_vlamwerp != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_vlamwerp, (proxvlamwerplvolume > proxvlamwerprvolume) ? proxvlamwerplvolume : proxvlamwerprvolume);
		audio->sound_pan(wav_vlamwerp, proxvlamwerplvolume-proxvlamwerprvolume);

		proxvlamwerplvolume = -10000;
		proxvlamwerprvolume = -10000;
	}
}

void play_schuif(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->stop_sound(wav_schuif);
    audio->play_sound_1shot(wav_schuif,0,0);
  }
}

void play_smexp(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp2(&v,&p,x,y);
    if (v < -4000) return;
	audio->stop_sound(wav_smexp);
    audio->play_sound_1shot(wav_smexp,v,p);
  }
}

void play_backpak(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_backpak,0,0);
    set_backpak_volume(-10000);
  }
}

void set_backpak_volume(INT32 volume)
{
		audio->sound_volume(wav_backpak, volume);
}

void play_restart(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_1shot(wav_restart,0,0);
  }
}

void play_bigexp(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp2(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_bigexp);
    audio->play_sound_1shot(wav_bigexp,v,p);
  }
}

void play_cannon(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_cursound(wav_cannon);
    audio->play_sound_1shot(wav_cannon,v,p);
  }
}


void play_gewicht(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_gewicht,v,p);
  }
}


void play_stroom(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_stroom,0,0);
  }
}


void prox_stroom_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxstroomlvolume) proxstroomlvolume = lvolume;
	if (rvolume > proxstroomrvolume) proxstroomrvolume = rvolume;
}

void real_stroom_volume(void)
{
  if ((audio->get_dsound()) && (wav_stroom != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_stroom, (proxstroomlvolume > proxstroomrvolume) ? proxstroomlvolume : proxstroomrvolume);
		audio->sound_pan(wav_stroom, proxstroomlvolume-proxstroomrvolume);

		proxstroomlvolume = -10000;
		proxstroomrvolume = -10000;
	}
}

void play_raket(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_raket);
    audio->play_sound_1shot(wav_raket,v,p);
  }
}

void play_tik(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_1shot(wav_tik,0,0);
  }
}

void play_tak(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_1shot(wav_tak,0,0);
  }
}

void play_chemo(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_chemo,0,0);
  }
}

void play_chemo2(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_chemo2,0,0);
  }
}

void chemo2volume(INT32 volume)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->sound_volume(wav_chemo2,volume);
  }
}

void play_helmdood(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_helmdood);
    audio->play_sound_1shot(wav_helmdood,v,p);
  }
}

void play_ketting(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -3000) return;
    audio->stop_sound(wav_ketting);
    audio->play_sound_1shot(wav_ketting,v,p);
  }
}


void prox_chemo_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxchemolvolume) proxchemolvolume = lvolume;
	if (rvolume > proxchemorvolume) proxchemorvolume = rvolume;
}

void real_chemo_volume(void)
{
  if ((audio->get_dsound()) && (wav_chemo != 0) && (sfxflg != 0)) // is dsound on?
  {

		audio->sound_volume(wav_chemo, (proxchemolvolume > proxchemorvolume) ? proxchemolvolume : proxchemorvolume);
		audio->sound_pan(wav_chemo, proxchemolvolume-proxchemorvolume);

		proxchemolvolume = -10000;
		proxchemorvolume = -10000;
	}
}

void play_dimndsht(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_dimndsht);
    audio->play_sound_1shot(wav_dimndsht,v,p);
  }
}

void play_glasblok(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_glasblok);
    audio->play_sound_1shot(wav_glasblok,v,p);
  }
}

void play_hyprlift(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_hyprlift);
    audio->play_sound_1shot(wav_hyprlift,v,p);
  }
}

void play_lightwav(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_lightwav);
    audio->play_sound_1shot(wav_lightwav,v,p);
  }
}

void play_morphsht(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_morphsht);
    audio->play_sound_1shot(wav_morphsht,v,p);
  }
}

void play_mushup(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_mushup);
    audio->play_sound_1shot(wav_mushup,v,p);
  }
}

void play_mushdwn(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_mushdwn);
    audio->play_sound_1shot(wav_mushdwn,v,p);
  }
}

void play_plntlft(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_plntlft);
    audio->play_sound_1shot(wav_plntlft,v,p);
  }
}

void play_pltfdwn(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
//    audio->stop_sound(wav_pltfdwn);
    audio->play_sound_1shot(wav_pltfdwn,v,p);
  }
}

void play_pltfup(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
//    audio->stop_sound(wav_pltfup);
    audio->play_sound_1shot(wav_pltfup,v,p);
  }
}

void play_qbert1(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_qbert1);
    audio->play_sound_1shot(wav_qbert1,v,p);
  }
}

void play_qbert2(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_qbert2);
    audio->play_sound_1shot(wav_qbert2,v,p);
  }
}

void play_roltnlp(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_roltnlp,v,p);
  }
}

void play_tangjmp(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_tangjmp);
    audio->play_sound_1shot(wav_tangjmp,v,p);
  }
}

void play_tangclos(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->stop_sound(wav_tangclos);
    audio->play_sound_1shot(wav_tangclos,v,p);
  }
}

void play_woeiwoei(INT32 x, INT32 y)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    INT32 v,p;
    calcvp(&v,&p,x,y);
    if (v < -4000) return;
    audio->play_sound_1shot(wav_woeiwoei,v,p);
  }
}


void play_slowlift(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_slowlift,0,0);
  }
}


void prox_slowlift_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxslowliftlvolume) proxslowliftlvolume = lvolume;
	if (rvolume > proxslowliftrvolume) proxslowliftrvolume = rvolume;
}


void real_slowlift_volume(void)
{
  if ((audio->get_dsound()) && (wav_slowlift != 0) && (sfxflg != 0)) // is dsound on?
  {
		audio->sound_volume(wav_slowlift, (proxslowliftlvolume > proxslowliftrvolume) ? proxslowliftlvolume : proxslowliftrvolume);
		audio->sound_pan(wav_slowlift, proxslowliftlvolume-proxslowliftrvolume);

		proxslowliftlvolume = -10000;
		proxslowliftrvolume = -10000;
	}
}

void play_roltnlp(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_roltnlp,0,0);
  }
}

void prox_roltnlp_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxroltnlplvolume) proxroltnlplvolume = lvolume;
	if (rvolume > proxroltnlprvolume) proxroltnlprvolume = rvolume;
}

void real_roltnlp_volume(void)
{
  if ((audio->get_dsound()) && (wav_roltnlp != 0) && (sfxflg != 0)) // is dsound on?
  {
		audio->sound_volume(wav_roltnlp, (proxroltnlplvolume > proxroltnlprvolume) ? proxroltnlplvolume : proxroltnlprvolume);
		audio->sound_pan(wav_roltnlp, proxroltnlplvolume-proxroltnlprvolume);

		proxroltnlplvolume = -10000;
		proxroltnlprvolume = -10000;
	}
}

void play_woeiwoei(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_woeiwoei,0,0);
  }
}

void woeiwoeivolume(INT32 volume)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->sound_volume(wav_woeiwoei,volume);
  }
}

void play_heatsnkr(void)
{
  if (audio->get_dsound() && sfxflg) // is dsound on?
  {
    audio->play_sound_loop(wav_heatsnkr, 0, 0);
  }
}

void prox_heatsnkr_volume(INT32 lvolume, INT32 rvolume)
{
	if (lvolume < -10000) lvolume = -10000;
	if (rvolume < -10000) rvolume = -10000;
	if (lvolume > proxheatsnkrlvolume) proxheatsnkrlvolume = lvolume;
	if (rvolume > proxheatsnkrrvolume) proxheatsnkrrvolume = rvolume;
}

void real_heatsnkr_volume(void)
{
  if ((audio->get_dsound()) && (wav_heatsnkr != 0) && (sfxflg != 0)) // is dsound on?
  {
		audio->sound_volume(wav_heatsnkr, (proxheatsnkrlvolume > proxheatsnkrrvolume) ? proxheatsnkrlvolume : proxheatsnkrrvolume);
		audio->sound_pan(wav_heatsnkr, proxheatsnkrlvolume-proxheatsnkrrvolume);

		proxheatsnkrlvolume = -10000;
		proxheatsnkrrvolume = -10000;
	}
}

