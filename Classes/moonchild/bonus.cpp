#include <frm_wrk.hpp>
#include <bonus.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>

UINT16 scores[] = {500,1000};

void bonus_init(UINT16 x, UINT16 y, UINT16 sequencenr, UINT16 getpoints)
{
  OBJECT *bonus;
  bonus = (OBJECT *) malloc (sizeof(OBJECT));
  bonus->x = x;
  bonus->y = y;
  bonus->live  = bonus_live;
  bonus->death = bonus_death;
  bonus->clear = bonus_clear;
  bonus->blitsizex = 0;
  bonus->blitsizey = 0;
  bonus->lethal = 0;

  bonus->animate = copy_anim(orgbonus);

  bonus->frame   = anim_setsequence(bonus->animate, 0, SEQ_FORCE);

  bonus->sizex = bonus->frame->get_width();
  bonus->sizey = bonus->frame->get_height();

  //  bonus->x -= bonus->sizex>>1;
  //  bonus->y += bonus->sizey>>1;

  bonus->blood = (void *) malloc(sizeof(BONUS_BLOOD));
//  vgassert(bonus->blood);
  BONUS_BLOOD *bonusblood;
  bonusblood = (BONUS_BLOOD *)bonus->blood;
  bonusblood->lifespan = 31;

  object_add(bonus);

  if (sequencenr == 2)
  {
    play_appel(x,y);
  }
  else
  {
    play_bonus(x,y);
  }

  //  if (getpoints) player1.score += scores[sequencenr];

  return;
}


int bonus_live (OBJECT *object, UINT32 param)
{

  BONUS_BLOOD *bonusblood;
  bonusblood = (BONUS_BLOOD *)object->blood; /* in case of multiple bonuss */


  object->frame = anim_nextframe(object->animate);
  object->frame = anim_nextframe(object->animate);

  bonusblood->lifespan -= 1;

  if (!bonusblood->lifespan) return 1;
  return 0;
}

void bonus_death(OBJECT *object, UINT32 param)
{
}

void bonus_clear(OBJECT *object, UINT32 param)
{
}


