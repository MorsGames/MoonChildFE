#include "frm_int.hpp"
#include <stdlib.h>
Cspr_frame::Cspr_frame(COMP_SPRITE *dc, UINT16 w, UINT16 h, INT16 hx, INT16 hy)
{
}


// if compile flg = TRUE  =>  compile the bitmap

Cspr_frame::Cspr_frame(Cblitbuf &blitbuf, INT16 hx, INT16 hy, UINT16 compileflg)
{
}

Cspr_frame::Cspr_frame(char *pcx_filename, INT16 hx, INT16 hy, UINT16 compileflg)
{
//  INT32 code_len;
//  BYTE *code_buffer;

  sprblbuf = new Cblitbuf(pcx_filename, 0, 0);
      dc_user_allocated = VG_FALSE;
      draw_code = NULL;
      
      bb_user_allocated = VG_FALSE;
      width  = sprblbuf->get_width();
      height = sprblbuf->get_height();
      hotx = hx;
      hoty = hy;
}

Cspr_frame::~Cspr_frame(void)
{
  if (!dc_user_allocated)
    {
      if(draw_code)
        {
//          free(draw_code);
        }
    }

  if (!bb_user_allocated)
    {
      if(sprblbuf)
        {
          delete sprblbuf;
        }
    }
}

unsigned char *Cspr_frame::lock_buffer(void)
{
	BYTE *Ptr;
    Ptr = sprblbuf->lock_buffer();

    pitch = sprblbuf->get_pitch();

    return Ptr;
}


void Cspr_frame::unlock_buffer(void)
{
	sprblbuf->unlock_buffer();
    pitch = 0;
}


UINT16 Cspr_frame::draw(Cblitbuf &blitbuf, INT16 x, INT16 y)
{
  if (draw_code)
    {
    }
  else
    {
      return frmwrk_fastblit(sprblbuf, blitbuf, 0, 0, x, y, width, height, USEKEY);
    }
  return 0;
}


UINT16 Cspr_frame::draw(Cblitbuf &blitbuf, INT16 x, INT16 y,
          UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
  if (draw_code)
    {
    }
  else
    {
      return frmwrk_fastblit(sprblbuf, blitbuf, x1, y1, x, y, x2-x1, y2-y1, USEKEY);
    }
  return 0;
}


UINT16 Cspr_frame::draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y)
{
  if (draw_code)
    {
    }
  else
    {
      return frmwrk_fastblit(sprblbuf, blitbuf, 0, 0, x, y, width, height, NOKEY);
    }
  return 0;
}


UINT16 Cspr_frame::draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y,
          UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
  if (draw_code)
    {
    }
  else
    {
      return frmwrk_fastblit(sprblbuf, blitbuf, x1, y1, x, y, x2-x1, y2-y1, NOKEY);
    }
  return 0;
}


extern short g_FrameDropper;
extern char *g_OffScreenBuf;
extern short g_RenderMode;

UINT16 frmwrk_fastblit(Cblitbuf *src, Cblitbuf &dest, int x1, int y1, int x2, int y2, int w, int h, int key)
{
	MC_RECT    rect,drect;

	if(g_FrameDropper!=0 && g_RenderMode>=4)
	{
		if(dest.lock_buffer() == (unsigned char *)g_OffScreenBuf)
		{
			dest.unlock_buffer();
			return 1;
		}
		dest.unlock_buffer();
	}

	rect.left  = x1;
	rect.top   = y1;
	rect.right = x1+w;
	rect.bottom= y1+h;

	drect.left  = x2;
	drect.top   = y2;
	drect.right = w+x2;
	drect.bottom= h+y2;

	// Clipping in X-direction

	if (x2 <= dest.get_clipx()-w)
	{
	return 0;
	}

	if (x2 < dest.get_clipx())
	{
	drect.left = dest.get_clipx();
	rect.left += (dest.get_clipx()-x2);       // = -x
	}

	if (x2 >= (dest.get_clipwidth()))
	{
	return 0;
	}

	if (x2 > ((dest.get_clipwidth())-w))
	{
	drect.right = (dest.get_clipwidth());
	rect.right = (dest.get_clipwidth())-x2+x1;
	}

	// Clipping in Y-direction

	if (y2 <= dest.get_clipy()-h)
	{
	return 0;
	}

	if (y2 < dest.get_clipy())
	{
	drect.top = dest.get_clipy();
	rect.top += (dest.get_clipy()-y2);    //=-y
	}

	if (y2 >= (dest.get_clipheight()))
	{
	return 0;
	}

	if (y2 > ((dest.get_clipheight())-h))
	{
	drect.bottom = (dest.get_clipheight());
	rect.bottom = (dest.get_clipheight())-y2+y1;
	}


	//      log_out("(%d-%d)(%d-%d) - (%d-%d)(%d-%d)",
	//      rect.left,rect.top,rect.right,rect.bottom,
	//      drect.left,drect.top,drect.right,drect.bottom);

    if (key == USEKEY)
    {
		int	SrcPitch, DestPitch;
		BYTE *Src;
		BYTE *Dest;
		int x,y;
//          ddrval = dest.get_ddraw()->BltFast(
//                                                drect.left,
//                                                drect.top,
//                                                src->get_ddraw(),
//                                                &rect, 
//                                                DDBLTFAST_SRCCOLORKEY + DDBLTFAST_WAIT);
		
		Src = src->lock_buffer();
		Dest = dest.lock_buffer();

		SrcPitch  = src->get_pitch();
		DestPitch = dest.get_pitch();

		Src += (rect.top * SrcPitch) + rect.left;
		Dest += (drect.top * DestPitch) + drect.left;
		for (y=0; y<(rect.bottom-rect.top); y++)
		{
			BYTE *tSrc;
			BYTE *tDest;

			tSrc = Src;
			tDest = Dest;
			for (x=0; x<(rect.right-rect.left); x++)
			{
				if(*Src)
				{
					*Dest++ = *Src++;
				}
				else
				{
					Dest++;
					Src++;
				}
			}
			Src  = tSrc + SrcPitch;
			Dest = tDest + DestPitch;
		}
    }
    else
    {
#if 1
				int	SrcPitch, DestPitch;
		BYTE *Src;
		BYTE *Dest;
		int x,y;
		int a;
		int xbegin, xmiddle, xend;
//          ddrval = dest.get_ddraw()->BltFast(
//                                                drect.left,
//                                                drect.top,
//                                                src->get_ddraw(),
//                                                &rect, 
//                                                DDBLTFAST_SRCCOLORKEY + DDBLTFAST_WAIT);

		Src = src->lock_buffer();
		Dest = dest.lock_buffer();

		SrcPitch  = src->get_pitch();
		DestPitch = dest.get_pitch();

		xmiddle = ((drect.right&0xfffc) - (drect.left&0xfffc))/4;
		xbegin = drect.left & 3;
		xend = drect.right & 3;

		if(xbegin)
		{
			xmiddle--;
		}

		Src += (rect.top * SrcPitch) + rect.left;
		Dest += (drect.top * DestPitch) + drect.left;

		for (y=0; y<(rect.bottom-rect.top); y++)
		{
			BYTE *tSrc;
			BYTE *tDest;

			tSrc = Src;
			tDest = Dest;

		a=0;
			if(xbegin)	//begin stukje
			{
				for(x=0; x< (4-xbegin); x++)
				{
					*Dest++ = *Src++;
					a+=1;
				}
			}
			// middenstuk

			if(xmiddle>=0)
			{
				UBYTE *t1;
//				assert(((int)Dest&3) == 0);

				t1 = Src;
				// welke van de 4 middle copies?
				switch(((intptr_t)Src) & 3)
				{
				case 0:	// gewone copy
#if 1
					for (x=0; x<xmiddle; x++)
					{
						*(((unsigned int *)Dest)) = *(((unsigned int *)Src));
						Dest+=4;
						Src+=4;
						a+=4;
					}
#endif
					break;
				case 1:
				case 2:
				case 3:
					for (x=0; x<xmiddle; x++)
					{
						unsigned int temp;
						temp = 0;
						temp =  (*Src++);
						temp |= ((int)(*Src++)) << 8;
						temp |= ((int)(*Src++)) << 16;
						temp |= ((int)(*Src++)) << 24;
						*(((unsigned int *)Dest)) = temp;
						Dest+=4;
						a+=4;
					}
					break;
				}

				if(xend)	//eind stukje
				{
					for(x=0; x< xend; x++)
					{
						*Dest++ = *Src++;
						a+=1;
					}
				}
//				assert(a==(drect.right - drect.left));
			}
		
			Src  = tSrc + SrcPitch;
			Dest = tDest + DestPitch;
		}
    }
#endif
#if 0
		int	SrcPitch, DestPitch;
		BYTE *Src;
		BYTE *Dest;
		int x,y;
		int xbegin, xmiddle, xend;
//          ddrval = dest.get_ddraw()->BltFast(
//                                                drect.left,
//                                                drect.top,
//                                                src->get_ddraw(),
//                                                &rect, 
//                                                DDBLTFAST_SRCCOLORKEY + DDBLTFAST_WAIT);

		Src = src->lock_buffer();
		Dest = dest.lock_buffer();

		SrcPitch  = src->get_pitch();
		DestPitch = dest.get_pitch();

		xmiddle = ((drect.right&0xfffc) - (drect.left&0xfffc))/4;
		xbegin = drect.left & 3;
		xend = drect.right & 3;

		if(xbegin)
		{
			xmiddle--;
		}

		Src += (rect.top * SrcPitch) + rect.left;
		Dest += (drect.top * DestPitch) + drect.left;

		xend = rect.right-rect.left;
		for (y=0; y<(rect.bottom-rect.top); y++)
		{
			BYTE *tSrc;
			BYTE *tDest;

			tSrc = Src;
			tDest = Dest;

			for(x=0; x< xend; x++)
			{
				*Dest++ = *Src++;
			}
		
			Src  = tSrc + SrcPitch;
			Dest = tDest + DestPitch;
		}
    }
#endif

	return 1;
}




