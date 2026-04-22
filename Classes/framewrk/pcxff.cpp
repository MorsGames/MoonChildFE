// pcxff.cpp - pcx stat and read functions to read PCX files from a "fastfile".
// The "fastfile" format is delivered by the DirectX SDK. 
// The "fastfile" package consists of two parts:
// 1. ffcreate - a program to pack a complete directory in a file
// 2. fastfile.c/h - the code to retrieve the data from the packed file
// These utilities are extended to allow the data to be XOR'ed with the
// XORVALUE as defined in the headerfile "ffent.h".

#if 1   // we gebruiken ff de gzip versie
#include "frm_int.hpp"


VG_BOOLEAN PCX_ff_save(char *fname, BYTE *buffer, UINT16 width, UINT16 height, BYTE *palette)
{
	return VG_TRUE;
}


VG_BOOLEAN PCX_ff_stat(char *fname, UINT16 *width, UINT16 *height)
{
  HFASTFILE fp;
  bool rc;

//	LOG("STAT\n");
  fp = FastFileOpen(fname);
  if (!fp)
    {
//	  	LOG("OPEN ERROR\n");

      return VG_FALSE;
    }

//	  	LOG("STAT OK\n");
  rc = FastFileSeek(fp, 8, SEEK_SET);
  rc = FastFileRead(fp, width, 2);
  rc = FastFileRead(fp, height, 2);
  

  *width += 1;  // PCX counts to width-1 and height-1
  *height += 1;

//	  	LOG("STAT CLOSE\n");
  rc = FastFileClose(fp);

//	  	LOG("STAT EXIT\n");
  return VG_TRUE;
}

// if <buffer> is NULL, no picture data will be loaded (palette will be loaded)
// if <palette> is NULL, no palette will be loaded

// gebuffered load versie!
#define BUFFERSIZE (128)

VG_BOOLEAN PCX_ff_load(char *fname, BYTE *buffer, BYTE *palette)
{
	unsigned char TempBuffer[BUFFERSIZE];
	HFASTFILE  fp;
	UINT16 xsize,ysize;
	UINT32 size;
	BYTE   PCX_byte, RLE_byte;
	BYTE  *buf_ptr = NULL;
	BYTE  *end_of_buf = NULL;
	bool rc;
	int Index = 0;
	
//	vgassert(fname); 
//	vgassert(buffer || palette);
	
	fp = FastFileOpen(fname);
	
	if (!fp) 
    {
		return VG_FALSE;
    } 
	
	rc = FastFileSeek(fp, 8, SEEK_SET);
//	vgassert(rc);
	rc = FastFileRead(fp, &xsize, 2);
//	vgassert(rc);
	rc = FastFileRead(fp, &ysize, 2);
//	vgassert(rc);
	
	xsize += 1;
	ysize += 1;
	
	size = (xsize * ysize);
	
	// Load 256 color PCX palette
	if (palette)
    {
		rc = FastFileSeek(fp, -768, SEEK_END);
//		vgassert(rc);
		rc = FastFileRead(fp, palette, 768);
//		vgassert(rc);
    }
	
	// Load the picture data
	if (buffer)
	{
		buf_ptr = buffer;
		end_of_buf = buf_ptr + size;
		
		rc = FastFileSeek(fp, 128, SEEK_SET);
//		vgassert(rc);
		
		rc = FastFileRead(fp, TempBuffer, BUFFERSIZE);
//		vgassert(rc);
		
		Index=0;
		
		while (buf_ptr < end_of_buf) 
		{
			
			// Read next packet
			PCX_byte = TempBuffer[Index++];
			if(Index==BUFFERSIZE)
			{
				rc = FastFileRead(fp, TempBuffer, BUFFERSIZE);
				Index=0;
			}
			//			rc = FastFileRead(fp, &PCX_byte, 1);
			//			vgassert(rc);
			
			if (PCX_byte < 192) 
			{
				// Raw Pixel
				*buf_ptr++ = PCX_byte;
			} 
			else // two highest bit set -> low 6 bits are count
			{
				int PixelsLeft;
				// RLE Pixels
				PCX_byte &= 0x3F;
				
				RLE_byte = TempBuffer[Index++];
				if(Index==BUFFERSIZE)
				{
					rc = FastFileRead(fp, TempBuffer, BUFFERSIZE);
					Index=0;
				}
				//				rc = FastFileRead(fp, &RLE_byte, 1);
				//				vgassert(rc);
				
				PixelsLeft = end_of_buf - buf_ptr;
				if(PCX_byte > PixelsLeft)
				{
					PCX_byte = PixelsLeft;
				}
				if(PCX_byte!=0)
				{
					memset(buf_ptr, RLE_byte, PCX_byte);
					buf_ptr += PCX_byte;
				}
			}
		}

		FastFileSeek(fp, -(BUFFERSIZE-Index), SEEK_SET);
	}

	rc = FastFileClose(fp);
//	vgassert(rc);
	return VG_TRUE;
}


void PCX_ff_dump(char *filename, FILE *out)
{
  UINT16 width, height;
  BYTE pal[256*3];
  HFASTFILE fp;
  int i;
  bool rc;

//  vgassert(filename);

  fp = FastFileOpen(filename);
//  vgassert(fp);

  rc = FastFileSeek(fp, 8, SEEK_SET);
//  vgassert(rc);
  rc = FastFileRead(fp, &width, 2*1);
//  vgassert(rc);
  rc = FastFileRead(fp, &height, 2*1);
//  vgassert(rc);

  width += 1;  // PCX counts to width-1 and height-1
  height += 1;

  fprintf(out, "PCX FILE  : \"%s\"\n", filename);
  fprintf(out, "DIMENSIONS: %dx%d\n", width, height);
  fprintf(out, "PALETTE DUMP:\n");

  rc = FastFileSeek(fp, -768, SEEK_END);
//  vgassert(rc);
  rc = FastFileRead(fp, pal, 768);
//  vgassert(rc);

  for (i=0; i<256; i++)
    {
      fprintf(out, "%d - (%3d,%3d,%3d)\n", i, 
        pal[i*3+0], pal[i*3+1], pal[i*3+2]);
    }

  rc = FastFileClose(fp);
//  vgassert(rc);
}

#endif