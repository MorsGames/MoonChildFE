#ifndef _HOI_H
#define _HOI_H

#include <objects.hpp>

typedef UINT16 (*HOI_AI )(OBJECT *hoi);


struct HOICOLLAPSE_BLOOD {
  INT16       sinx;
  INT16       siny;
  INT16       sinxspd;
  INT16       sinyspd;
  INT16       size;
  Cspr_frame *frame; /* current frame out of animation */
};


struct HOIRISE_BLOOD {
  INT16       sinx;
  INT16       siny;
  INT16       sx;
  INT16       sy;
  INT16       dx;
  INT16       dy;
  UINT16      distance;
  UINT16      deathcnt;
  INT16       sinxspd;
  INT16       sinyspd;
  INT16       size;
  INT16       sizeadd;
  INT16       fastmode;
  OBJECT     *newobject;
  Cspr_frame *frame; /* current frame out of animation */
};

/* possible ID'S */

#define ID_HOI    0
#define ID_ELF    1
#define ID_HELMUT 2
#define ID_BOMB   3


struct HOI_CAPS {
  INT16       id;        // what type of object am I?
  INT16       left;      // can Hoi move left?
  INT16       right;     // can Hoi move right?
  INT16       turn;      // can Hoi turn?
  INT16       maxspd;    // max walk/run spd;
  INT16       xspddelay; // X speed division factor
  INT16       jumpheight;// hoe hoog springen we nu? */
  INT16       orgheight; // hoe hoog springen we zonder blubber? */
};


struct HOI_BLOOD {
  INT16     gravy;
  INT16     spdx;
  INT16     curdirx;  /* actually moving  ( -1, 0 or 1) */
  INT16     lastdirx; /* when last moved  ( -1 or 1) */
  INT16     onfloor;  /* 1 when on the floor */
  INT16     floor;    /* current attribute of floor */
  INT16     animover; /* jump anim override */
  INT16     anmturn;  /* turn anim is currently busy */
  INT16     anmdir;   /* turn is right->left  or left->right */
  INT16     kneelcnt; /* kneel anim cnt */
  INT16     thudcnt;  /* thud animcnt */
  INT16     thuddir;  /* direction of thud anim */
  INT16     onobject; /* 1 if he's standing on a platform */
  INT16     holddown; /* look extra down with camera */
  INT16     leancnt;  /* is moonchild leaning? */
  INT16     leandir;  /* in which direction? */
  INT16     idlecnt;  /* idle counter */
  INT16     idleanim; /* current idle animframe */
  INT16     xahead;   /* when running camera focusses better */
  INT16     yahead;   /* when jumping camera focusses better */
  INT16     blinkanim2cnt; /* counter for blinking of eyes */
  HOI_CAPS *hoi_caps; /* table with all the capabilities */
  HOI_AI    hoi_ai;   /* artificial intelligence routine */
  OBJECT   *pal;      /* friend of object */
  UINT16    leftkey;  /* if set we want to move left */
  UINT16    rightkey; /* if set we want to move right */
  UINT16    upkey;    /* if set we want to move up */
  UINT16    downkey;  /* if set we want to move down */
  UINT16    shootkey; /* if set we want to shoot */
  UINT16    flashcnt; /* invincibility */
  UINT32    xfloat;   /* x-pos 256 times too big.*/
  UINT32    yfloat;   /* y-pos 256 times too big.*/
  INT16     xspd;     /* x acceleration */
  INT16     yspd;     /* y-acceleration */
  UINT16    hoovercnt;/* current hover position */
  UINT16    exhaustcnt; /* counter for exhaust */
  INT16     xcamxtra; /* extra add for camera in X */
  INT16     ycamxtra; /* extra add for camera in X */
};

OBJECT *hoi_globinit(ANIM *appearance, HOI_CAPS *capabilities, HOI_AI intelligence, UINT16 lethal, OBJECT *pal);
void hoi_init (OBJECT *newhoi, UINT16 x, UINT16 y);
void hoi_fly_init (OBJECT *newhoi, UINT16 x, UINT16 y);
int  hoi_live (OBJECT *object, UINT32 param);
int  hoi_fly_live (OBJECT *object, UINT32 param);
void hoi_death(OBJECT *object, UINT32 param);
void hoi_clear(OBJECT *object, UINT32 param);
void hoirise_init (OBJECT *newhoi, UINT16 sx, UINT16 sy, UINT16 dx, UINT16 dy);
int  hoirise_live (OBJECT *object, UINT32 param);
void hoirise_death(OBJECT *object, UINT32 param);
void hoirise_clear(OBJECT *object, UINT32 param);
void hoicollapse_init (UINT16 x, UINT16 y);
int  hoicollapse_live (OBJECT *object, UINT32 param);
void hoicollapse_death(OBJECT *object, UINT32 param);
void hoicollapse_clear(OBJECT *object, UINT32 param);

UINT16 hoi_ai(OBJECT *object);
UINT16 elf_ai(OBJECT *object);
UINT16 bomb_ai(OBJECT *object);
UINT16 helmut_ai(OBJECT *object);
UINT16 helmut2_ai(OBJECT *object);
void helmut_init(int x, int y, int iq, HOI_CAPS *);
int  helmut_live(OBJECT *object, UINT32 param);
void reinitlevels(void);

#endif
