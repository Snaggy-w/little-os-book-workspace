#include "vgadrv.h"
#include "time.h"
#include "string.h"
#include "serial.h"
void kmain(void){


    clean_screen(80,25); // clean vga 80 x 25 text mode


    fb_move_cursor(0);
    char buf[] = "Hello World!\nThis is using my implementation of write which is crappy but it should work!";
    write(buf,str_len(buf,500));
    serial_init();
    serial_write(buf,str_len(buf,500));
}
