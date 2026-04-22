#ifndef _PCX_HPP
#define _PCX_HPP

VG_BOOLEAN PCX_stat(char *fname, UINT16 *width, UINT16 *height);
VG_BOOLEAN PCX_load(char *fname, BYTE *buffer, BYTE *palette);
VG_BOOLEAN PCX_ff_save(char *fname, BYTE *buffer, UINT16 width, UINT16 height, BYTE *palette);
void       PCX_pal8to6(BYTE *palette, int count);
void       PCX_pal6to8(BYTE *palette, int count);
void       PCX_dump(char *filename, FILE *out);

#endif /* _PCX_HPP */
