#include <frm_wrk.hpp>
#include <cloud.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>
#include <sound.hpp>


void cloud_init(UINT16 x, UINT16 y, INT16 spdx, UINT16 sndflg)
{
  OBJECT *cloud;
  cloud = (OBJECT *) malloc (sizeof(OBJECT));
  cloud->x = x;
  cloud->y = y;
  cloud->live  = cloud_live;
  cloud->death = cloud_death;
  cloud->clear = cloud_clear;
  cloud->blitsizex = 0;
  cloud->blitsizey = 0;

  cloud->lethal = 0; // harmless

  cloud->animate = copy_anim(orgcloud);

  if (spdx == 0)
  {
    cloud->frame = anim_setsequence(cloud->animate, 2, SEQ_FORCE);
    if (!mc_autorun) play_mcwall(x,y); // als MC automatisch runt geen geluidjes.
  }
  else
  {
    if (spdx > 0)
    {
      cloud->frame = anim_setsequence(cloud->animate, 1, SEQ_FORCE);
      if (sndflg)play_mcskid(x,y);
    }
    else
    {
      cloud->frame = anim_setsequence(cloud->animate, 0, SEQ_FORCE);
      if (sndflg)play_mcskid(x,y);
    }
  }

  cloud->sizex = cloud->frame->get_width();
  cloud->sizey = cloud->frame->get_height();

  cloud->y -= cloud->sizey>>1;

  cloud->blood = (void *) malloc(sizeof(CLOUD_BLOOD));
//  vgassert(cloud->blood);
  CLOUD_BLOOD *cloudblood;
  cloudblood = (CLOUD_BLOOD *)cloud->blood;
  cloudblood->lifespan = 27;
  cloudblood->spdx = spdx;
  cloudblood->ytoggle = 0;

  object_add(cloud);
}


int cloud_live (OBJECT *object, UINT32 param)
{

  CLOUD_BLOOD *cloudblood;
  cloudblood = (CLOUD_BLOOD *)object->blood; /* in case of multiple clouds */

  object->frame = anim_nextframe(object->animate);

  object->x += cloudblood->spdx;

  cloudblood->ytoggle++;
//  cloudblood->ytoggle&=3;
  if (cloudblood->ytoggle == 3) cloudblood->ytoggle = 0;

  if (cloudblood->ytoggle==0) object->y -= 1;    //abs(cloudblood->spdx)>>2;
  cloudblood->lifespan -= 1;

  if (!cloudblood->lifespan) return 1;
  return 0;
}

void cloud_death(OBJECT *object, UINT32 param)
{
}

void cloud_clear(OBJECT *object, UINT32 param)
{
}


