typedef unsigned char BYTE;

typedef unsigned int UINT;

#include <anim.hpp>
#include <globals.hpp>


ANIM *copy_anim(ANIM *oldanim)
{
  ANIM* newanim;

  if(!oldanim)
  {
    log_out("copy_anim: anim pointer is zero!");
//    vgassert(0);
  }

  newanim = (ANIM *) malloc (sizeof(ANIM));
  memcpy(newanim, oldanim, sizeof(ANIM));

  return newanim;
}


Cspr_frame *anim_forceframe(ANIM *anim, INT16 framenr)
{
//  if (!anim) vgassert(0);

  anim->delay = anim->delaycnt;

  if (framenr > anim->maxstep)
    {
      framenr = anim->maxstep;
    }

  anim->stepnr = framenr;

  return anim->sequences[anim->cursequence]->frame[anim->stepnr];
}


Cspr_frame *anim_prevframe(ANIM *anim)
{
//  if (!anim) vgassert(0);

  if(anim->delay--)   return anim->sequences[anim->cursequence]->frame[anim->stepnr];
  anim->delay = anim->delaycnt;

  if (!anim->stepnr)
    {
      anim->stepnr = anim->maxstep;
    }

  anim->stepnr--;

  return anim->sequences[anim->cursequence]->frame[anim->stepnr];
}

Cspr_frame *anim_nextframe(ANIM *anim)
{
//  if (!anim) vgassert(0);

  if(anim->delay--)   return anim->sequences[anim->cursequence]->frame[anim->stepnr];
  anim->delay = anim->delaycnt;
  anim->stepnr++;

  if (anim->stepnr == anim->maxstep)
    {
      anim->stepnr = 0;
    }
  return anim->sequences[anim->cursequence]->frame[anim->stepnr];
}

Cspr_frame *anim_setsequence(ANIM *anim, int seq, int force)
{
//  if (!anim) vgassert(0);

  if (!force)
    {
      if (anim->cursequence == seq)
  {
    return anim->sequences[anim->cursequence]->frame[anim->stepnr];
  }
    }

  anim->stepnr = 0;
  anim->cursequence = seq;
  anim->maxstep = anim->sequences[seq]->nrofsteps;
  anim->delay = 3;   //anim->sequences[seq]->delay;
  anim->delaycnt = anim->delay;

  return anim->sequences[anim->cursequence]->frame[anim->stepnr];
}


ANIM *anim_retrieveanim(char *animname)
{
  ANIM *myanim;
  vg_dll_first(allanims);

  do
  {
    myanim = (ANIM *)vg_dll_collect(allanims);
    if (strcmp(myanim->name, animname) == 0)
    {
//      log_out("    FOUND: %s",animname);
      return myanim;
    }
  } while (vg_dll_next(allanims));

//  log_out("NOT FOUND: %s",animname);
  return 0;
}


