#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <vgdll.hpp>
#include <frm_wrk.hpp>
#include <mc.hpp>
#include <globals.hpp>
#include <editor.hpp>
#include <scroll.hpp>


struct OBJECT
{
  UINT16       x;     /* current x-pos */
  UINT16       y;     /* current y-pos */
  UINT16       sizex; /* width of boundingbox */
  UINT16       sizey; /* height of boundingbox */
  UINT16       blitsizex; /* width of blit  */
  UINT16       blitsizey; /* height of blit */
  UINT16       blitstartx; /* beginposx of blit  */
  UINT16       blitstarty; /* beginposy of blit */

// no need to specify collision details when lethal is 0

  UINT16       coloffsetx; /* offset from x position for collision */
  UINT16       coloffsety; /* offset from y position for collision */
  UINT16       colwidth;   /* width  of collision box */
  UINT16       colheight;  /* height of collision box */

  UINT16       lethal;     /* 0 is friendly, 1=energy eat, 2=imminent death */
  UINT16       visible;    /* if 0 object is not on screen... 1 object is on screen */
  UINT16       collided;   /* Set collided flag if lethal==4 and object collisions with hoi */

  OBJECT_CBR  live;  /* routine which makes the object alive */
  OBJECT_CB   death; /* routine which will dispose of this object in a
                        quick and painless manner. This routine 
                        can spawn new objects */
  OBJECT_CB   clear; /* routine to clean up used structures etc... */
  void       *blood; /* pointer to the internal data structures the 'live'
                        routine needs */
  Cspr_frame *frame; /* current frame out of animation */
                     /* if 0 than don't bobbit! */
  ANIM       *animate; /* pointer to current animationstructure */
};


void object_initialise(void);
int cb_object_shutdown(VG_DLL_ITEM *item);
void object_shutdown(void);
void object_movement(void);
int cb_object_movement(VG_DLL_ITEM *item);
void object_collision(void);
int cb_object_collision(VG_DLL_ITEM *item);
void object_visualise(VIEWPORT *player);
int cb_object_visualise(VG_DLL_ITEM *item);
void object_add(OBJECT *obj);
void object_add_first(OBJECT *obj);
void object_del(OBJECT *obj);


#endif
