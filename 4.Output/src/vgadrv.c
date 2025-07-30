#include "vgadrv.h"
#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define FB ((volatile short*) 0xB8000)

void put_char(char c, int x, int y, unsigned char color) {
    int index = y * VGA_WIDTH + x;
    FB[index] = (color << 8) | c;
}

void write_str(char* c, int len, int startX, int startY, unsigned char color){

	int index = startY * VGA_WIDTH + startX;
	for(int i = 0; i < len; i++){
		FB[index] = (color << 8) | *(c + i);
		index++;
	}
}

void clean_screen(int dimX,int dimY){
	volatile unsigned short * fb = (unsigned short*) 0x000B8000;
	for(int y = 0; y < dimY; y++)
		for(int x = 0; x < dimX; x++)
			fb[y*80 + x] = (0x0F << 8) | ' ';
}

int write(char *buf, unsigned int len) {
    unsigned short curr_pos = fb_get_cursor();

    for (unsigned int i = 0; i < len; i++) {
        char c = buf[i];

        if (c == '\n') {
            curr_pos += 80 - (curr_pos % 80);  // Move to next line's beginning
        } else {
            if (curr_pos >= 2000) {
                clean_screen(80, 25);
                curr_pos = 0;
            }

            FB[curr_pos] = (0x0F << 8) | c;
            curr_pos++;
        }

        // After writing or newline, check if we've overflowed
        if (curr_pos >= 2000) {
            clean_screen(80, 25);
            curr_pos = 0;
        }
    }

    fb_move_cursor(curr_pos);
    return len;
}


/*
 * Cursor operations:
 * 	fb_move_cursor moves the cursor to a certain position
 * 	fb_get_cursor gets the cursor position from the hardware register
 */

void fb_move_cursor(unsigned short pos) {
    outb(0x3D4, 14);                
    outb(0x3D5, (pos >> 8) & 0xFF); 
    outb(0x3D4, 15);                
    outb(0x3D5, pos & 0xFF);        
}
unsigned short fb_get_cursor() {
    unsigned short pos = 0;

    outb(0x3D4, 14);
    pos |= inb(0x3D5) << 8;

    outb(0x3D4, 15);
    pos |= inb(0x3D5);

    return pos;
}














/*
 * Something I played with a bit, a star...
 *
 */


void draw_star5(int x_offset, int y_offset, unsigned char color) {
    const char* star[] = {
        "    *    ",
        "   * *   ",
        "  *   *  ",
        " *     * ",
        "*********",
        " *     * ",
        "  *   *  ",
        "   * *   ",
        "    *    "
    };

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            char c = star[y][x];
            if (c != ' ') {
                put_char(c, x + x_offset, y + y_offset, color);
            }
        }
    }
}
void clear_star5(int x_offset, int y_offset) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            put_char(' ', x + x_offset, y + y_offset, 0x0F);
        }
    }
}
