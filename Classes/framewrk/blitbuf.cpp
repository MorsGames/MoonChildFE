#include "frm_int.hpp"


extern char *FullPath2( char *a_File );

//#define LOG(x)  { \
//	  FILE *log; \
//	  log = fopen(FullPath2("log.txt"),"a"); \
//	  if(log) \
//{ \
//		  fwrite((x), strlen(x), 1, log); \
//		  fclose(log); \
//		  fflush(log); \
//} \
//} 

#define LOG(x)


Cblitbuf::Cblitbuf(UINT16 w, UINT16 h, UINT16 top_margin, UINT16 bottom_margin)
{
//  log_out("Constructor: Cblitbuf(w=%d, h=%d, top=%d, bottom=%d)",
//    w, h, top_margin, bottom_margin);

  videoblitbuf = VG_FALSE;
  width = w;
  height = h;
  clipx = 0;
  clipy = 0;
  clipwidth = w;
  clipheight = h;
  pitch = 0;

  m_Buffer = (char *)malloc(w*h);

//  log_out("Construction succesful!");
}

Cblitbuf::Cblitbuf(UINT16 w, UINT16 h, UINT16 top_margin, UINT16 bottom_margin, UINT16 forcesystem)
{
//  log_out("Constructor: Cblitbuf in systemmem(w=%d, h=%d, top=%d, bottom=%d)",
//    w, h, top_margin, bottom_margin);

  m_Buffer = (char *)malloc(w*h);

  videoblitbuf = VG_FALSE;
  width = w;
  height = h;
  clipx = 0;
  clipy = 0;
  clipwidth = w;
  clipheight = h;
  pitch = 0;

//  log_out("Construction succesful!");
}

Cblitbuf::Cblitbuf(Cvideo *video)
{
 // log_out("Constructor: Cblitbuf(video)");

  videoblitbuf = VG_TRUE;
  width = video->get_width();
  height = video->get_height();

  clipx = 0;
  clipy = 0;
  clipwidth = video->get_width();
  clipheight = video->get_height();
  pitch = 0;

  m_Buffer = (char *)video->lock_bbuffer();
  video->unlock_bbuffer();

//  log_out("Construction succesful!");
}

Cblitbuf::Cblitbuf(char *pcx_filename, UINT16 top_margin, UINT16 bottom_margin, UINT16 forcesystemmem)
{
  VG_BOOLEAN rc;
  UINT16 w;
  UINT16 h;

  char logbuffer[100]; //@@@@
  sprintf(logbuffer, "blitbuf - %s\n",pcx_filename);
  LOG(logbuffer);

//  log_out("Constructor: Cblitbuf(pcx=\"%s\", top=%d, bottom=%d)",
//    pcx_filename, top_margin, bottom_margin);

 // vgassert(pcx_filename);

  if (frmwrk_usefastfile)
  {
    rc = PCX_ff_stat(pcx_filename, &w, &h);
 //   vgassert(rc);
  }
  else
  {
    rc = PCX_ff_stat(pcx_filename, &w, &h);
 //   vgassert(rc);
  }
//  log_out("pcx is %d by %d",w,h);

  sprintf(logbuffer, "w,h = %d,%d\n", w,h);
  LOG(logbuffer);

  m_Buffer = (char *)malloc(w*h);

  LOG("malloc\n");

  pitch = 0;

  videoblitbuf = VG_FALSE;
  width = w;
  height = h;

  clipx = 0;
  clipy = 0;
  clipwidth = w;
  clipheight = h;

//  log_out("Now loading pcx-file \"%s\"", pcx_filename);


  if (frmwrk_usefastfile)
  {
    rc = PCX_ff_load(pcx_filename, (BYTE *)m_Buffer, NULL);
//    vgassert(rc);
  }
  else
  {
    rc = PCX_ff_load(pcx_filename, (BYTE *)m_Buffer, NULL);
 //   vgassert(rc);
  }

  LOG("succes\n");
    
//  log_out("Construction succesful!");
}

Cblitbuf::~Cblitbuf(void)
{
	if(videoblitbuf==VG_FALSE)
	{
		free(m_Buffer);
	}

//	log_out("deleting blitbuf");
}


Cblitbuf &Cblitbuf::clear(UINT16 color_index)
{
	memset(m_Buffer, color_index, width*height);

	return *this;
}


void Cblitbuf::set_clipping(UINT16 cx, UINT16 cy, UINT16 cw, UINT16 ch)
{
  clipx      = cx;
  clipy      = cy;
  clipwidth  = cw;
  clipheight = ch;
}


unsigned char *Cblitbuf::lock_buffer(void)
{

    pitch = width;

    // Copy bitmap bits from system memory to video memory
    return (unsigned char *)m_Buffer;
}


void Cblitbuf::unlock_buffer(void)
{
    pitch = 0;
}


UINT16 Cblitbuf::draw(Cblitbuf &blitbuf, INT16 x, INT16 y)
{
      return frmwrk_fastblit(this, blitbuf, 0, 0, x, y, width, height, USEKEY);
}


UINT16 Cblitbuf::draw(Cblitbuf &blitbuf, INT16 x, INT16 y,
          UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
      return frmwrk_fastblit(this, blitbuf, x1, y1, x, y, x2-x1, y2-y1, USEKEY);
}


UINT16 Cblitbuf::draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y)
{
      return frmwrk_fastblit(this, blitbuf, 0, 0, x, y, width, height, NOKEY);
}


UINT16 Cblitbuf::draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y,
          UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
      return frmwrk_fastblit(this, blitbuf, x1, y1, x, y, x2-x1, y2-y1, NOKEY);
}


void Cblitbuf::save_mem(UINT16 x, UINT16 y, UINT16 w, UINT16 h, BYTE *buf)
{
}

void Cblitbuf::load_mem(UINT16 x, UINT16 y, UINT16 w, UINT16 h, BYTE *buf)
{
}

VG_BOOLEAN Cblitbuf::save_pcx(char *filename, BYTE *palette)
{
  BYTE vga_pal[256*3];
  VG_BOOLEAN rc;

//  vgassert(filename);

  if (!palette)
    {
      memset(vga_pal, 0, 256*3);
      palette = vga_pal;
    }

  rc = PCX_ff_save(filename, (BYTE *)m_Buffer, width, height, palette);

  return rc;

//  return VG_TRUE;  //weghalen
}

void Cblitbuf::compiled_sprite(UINT16 x, UINT16 y, COMP_SPRITE compspr)
{
}

void Cblitbuf::debug(void)
{
    return;
}

