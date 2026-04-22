/* The audio classes */

#ifndef _AUDIO_HPP
#define _AUDIO_HPP

#include <cstdio>

typedef intptr_t HSNDOBJ;


class Caudio {

public:
  Caudio(void);
  ~Caudio(void);

  void	  reset_audio();

  void    checkVolume();

  uint16_t  play_cd(uint16_t tracknr);
  void    play_cd_cb(uint16_t tracknr);
  void    stop_cd(void);
  HSNDOBJ create_sound(int SoundID, int nrof_simult);
  void    destroy_sound(HSNDOBJ sound);
  void    play_sound_1shot(HSNDOBJ sound, int32_t volume, int32_t pan);
  void    play_sound_loop (HSNDOBJ sound, int32_t volume, int32_t pan);  // only works if nr of simult was 1 !
  void    stop_sound(HSNDOBJ sound);
  void    stop_cursound(HSNDOBJ sound);
  void    sound_volume(HSNDOBJ sound, int32_t volume);
  void    sound_pan(HSNDOBJ sound, int32_t pan);

  int	get_dsound(void);
  
  void    dump(FILE *fd);
  
private:
};


#endif
