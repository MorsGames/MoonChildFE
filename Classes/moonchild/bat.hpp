#ifndef _BAT_H
#define _BAT_H

#include <objects.hpp>
#include <basic.hpp>

typedef struct BAT_BLOOD BAT_BLOOD;


struct BAT_BLOOD {
  UINT16      x;
  UINT16      y;
  BASIC_CB    moveroutx;
  BASIC_CB    moverouty;
  INT16      x1;
  INT16      x2;
  INT16      x3;
  INT16      y1;
  INT16      y2;
  INT16      y3;
  UINT16      shootdelay;
  UINT16      shootoverride;
  UINT16      shootdirx;
  UINT16      lastdirx;
};


void bat_init(UINT16 x, UINT16 y,
                BASIC_CB basicroutx, UINT16 x1, UINT16 x2, UINT16 x3,
                BASIC_CB basicrouty, UINT16 y1, UINT16 y2, UINT16 y3);
int  bat_live (OBJECT *object, UINT32 param);
void bat_death(OBJECT *object, UINT32 param);
void bat_clear(OBJECT *object, UINT32 param);

#endif
