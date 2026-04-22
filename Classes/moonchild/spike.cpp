#include <frm_wrk.hpp>
#include <spike.hpp>
#include <globals.hpp>
#include <prefs.hpp>
#include <anim.hpp>


void spike_init(UINT16 x, UINT16 y, UINT16 maxy)
{
  OBJECT *spike;
  spike = (OBJECT *) malloc (sizeof(OBJECT));
  spike->x = x;
  spike->y = y;
  spike->live  = spike_live;
  spike->death = spike_death;
  spike->clear = spike_clear;
  spike->blitsizex = 0;
  spike->blitsizey = 0;

  spike->animate = copy_anim(orgspike);

  spike->frame   = anim_setsequence(spike->animate, 0, SEQ_FORCE);

  spike->sizex = spike->frame->get_width();
  spike->sizey = spike->frame->get_height();

  spike->lethal = 3; // if hit then death!
  spike->coloffsetx = 15;
  spike->coloffsety = 0;
  spike->colwidth  = 2;
  spike->colheight = spike->sizey - 8;
  
  spike->blood = (void *) malloc(sizeof(SPIKE_BLOOD));
//  vgassert(spike->blood);
  SPIKE_BLOOD *spikeblood;
  spikeblood = (SPIKE_BLOOD *)spike->blood;

  spikeblood->x = x;
  spikeblood->y = y<<6;
  spikeblood->gravy = 960;
  spikeblood->maxy = maxy;
  
  object_add(spike);

  return;
}


int spike_live (OBJECT *object, UINT32 param)
{

  SPIKE_BLOOD *spikeblood;
  spikeblood = (SPIKE_BLOOD *)object->blood; /* in case of multiple spikes */

  spikeblood->y -= (fakespeedadd<<6);

  if (spikeblood->gravy > 480) spikeblood->gravy -= 1;
  spikeblood->y += spikeblood->gravy;
  object->x = spikeblood->x;
  object->y = spikeblood->y>>6;

  if (object->y > spikeblood->maxy) return 1;

  return 0;
}

void spike_death(OBJECT *object, UINT32 param)
{
}

void spike_clear(OBJECT *object, UINT32 param)
{
}


