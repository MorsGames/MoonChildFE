#ifndef ANIM_H
#define ANIM_H

#include <frm_wrk.hpp>
#include <mc.hpp>

ANIM *anim_retrieveanim(char *animname);
ANIM *copy_anim(ANIM *oldanim);
Cspr_frame *anim_forceframe(ANIM *anim, INT16 framenr);
Cspr_frame *anim_nextframe(ANIM *anim);
Cspr_frame *anim_prevframe(ANIM *anim);
Cspr_frame *anim_setsequence(ANIM *anim, int seq, int force);


#define SEQ_STANDLEFT  0
#define SEQ_STANDRIGHT 1
#define SEQ_WALKLEFT   2
#define SEQ_WALKRIGHT  3
#define SEQ_JUMPLEFT   4
#define SEQ_JUMPRIGHT  5
#define SEQ_TURNLEFT   6
#define SEQ_TURNRIGHT  7
#define SEQ_FLYLEFT    8
#define SEQ_FLYRIGHT   9
#define SEQ_BUMPLEFT   10
#define SEQ_BUMPRIGHT  11
#define SEQ_SKIDLEFT   12
#define SEQ_SKIDRIGHT  13
#define SEQ_KNEELLEFT  14
#define SEQ_KNEELRIGHT 15
#define SEQ_LEANLEFT   16
#define SEQ_LEANRIGHT  17
#define SEQ_WAITLEFT   18
#define SEQ_WAITRIGHT  19
#define SEQ_CURSOR     20
#define SEQ_DUMMY      21

#define MAXSEQUENCES SEQ_DUMMY

#define SEQ_NOFORCE 0
#define SEQ_FORCE   1

struct SEQUENCE
{
  char        *name;
  UINT16       nrofsteps;
  UINT16       delay;
  Cspr_frame  *frame[1];    /* table of framepntrs containing imagedata
                               for this animation sequence */
};


struct ANIM
{
  char     *name;
  UINT16    cursequence;
  UINT16    stepnr;
  UINT16    maxstep;
  UINT16    delay;
  UINT16    delaycnt;
  SEQUENCE *sequences[MAXSEQUENCES]; /* array of pointers to all sequences */
};


#endif



