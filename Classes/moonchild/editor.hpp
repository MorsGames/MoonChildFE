#ifndef _EDITOR_H
#define _EDITOR_H

#include <globals.hpp>
#include <frm_wrk.hpp>

/* prototypes */
void editor(VIEWPORT *player);


/* structures */

struct MAP_DESCR
{
  Cblitbuf *blitbuf;
  UINT16 *map;
  INT16 mapsizex;
  INT16 mapsizey;
};


#include <scroll.hpp>


#endif
