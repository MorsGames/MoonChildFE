#include <frm_wrk.hpp>
#include <glim.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>


void glim_init(UINT16 x, UINT16 y)
{
  OBJECT *glim;
  glim = (OBJECT *) malloc (sizeof(OBJECT));
  glim->x = x;
  glim->y = y;
  glim->live  = glim_live;
  glim->death = glim_death;
  glim->clear = glim_clear;
  glim->blitsizex = 0;
  glim->blitsizey = 0;

  glim->lethal = 0; // harmless

  glim->animate = copy_anim(orgglim);

  glim->frame = anim_setsequence(glim->animate, 0, SEQ_FORCE);

  glim->sizex = glim->frame->get_width();
  glim->sizey = glim->frame->get_height();

  glim->y -= glim->sizey>>1;

  glim->blood = (void *) malloc(sizeof(GLIM_BLOOD));
//  vgassert(glim->blood);
  GLIM_BLOOD *glimblood;
  glimblood = (GLIM_BLOOD *)glim->blood;
  glimblood->lifespan = 27;

  object_add(glim);
}


int glim_live (OBJECT *object, UINT32 param)
{

  GLIM_BLOOD *glimblood;
  glimblood = (GLIM_BLOOD *)object->blood; /* in case of multiple glims */

  object->frame = anim_nextframe(object->animate);

  glimblood->lifespan -= 1;

  if (!glimblood->lifespan) return 1;
  return 0;
}

void glim_death(OBJECT *object, UINT32 param)
{
}

void glim_clear(OBJECT *object, UINT32 param)
{
}


