#ifndef VGADRV_H
#define VGADRV_H

void clean_screen(int dimX,int dimY);
void draw_star5(int x_offset, int y_offset, unsigned char color);
void clear_star5(int x_offset, int y_offset);
void put_char(char c, int x, int y, unsigned char color);
void write_str(char* c, int len, int startX, int startY, unsigned char color);
int write(char *buf, unsigned int len);
void fb_move_cursor(unsigned short pos);
unsigned short fb_get_cursor();
#endif
