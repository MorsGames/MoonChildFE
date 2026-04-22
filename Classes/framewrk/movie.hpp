/* The movie classes */

#ifndef _MOVIE_HPP
#define _MOVIE_HPP


typedef struct {
  s32 x;
  s32 y;
  s32 w;
  s32 h;
} whRECT;


// dummy smack class
class Smack
{
public:
	int i;
};


class Cmovie {

public:
  Cmovie    (void);
  Cmovie    (Caudio *audio);
  ~Cmovie   (void);

  Smack    *open(char *filename);
  void      close(Smack *smk);

  void      playtovideo(Smack *smk, Cvideo *video, Cblitbuf *hulpbuf, UINT16 zoomfactor);
  UINT16    stillplaying(void);
  void      returnpal(BYTE *destpal);  // copy this palette over

  void      dump(FILE *fd);
private:
  void      movieplay(void);
  void      DoAdvance();
  void      InitPal( void );
  void      mergerects(whRECT* r1, u32 r1num, whRECT* r2, u32 r2num, whRECT** o, u32* onum);
  void      mergeinterrect(whRECT* r1, whRECT* r2, whRECT* m, whRECT* i);
  void      DoPaint();
  void      blitrect( u32 x, u32 y, u32 w, u32 h);
  void      dopal(void);
  void      ClearBack(u32 flipafter);

  Smack    *cursmk;
  Cvideo   *curvideo;
  Cblitbuf *dumbuf;
  UINT16    playing;
  UINT16    zoom;

public:
    char *videoFilename;
    bool  videoReady;

    
};


#endif

