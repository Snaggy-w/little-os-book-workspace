
The default frame buffer that we will be working with is in 80 x 25 text mode which is fine for our purposes. The way we can write stuff to it is by writing to this address as so:

```c
volatile unsigned short * fb = (unsigned short*) 0x000B8000;  
fb[20] = 'A' | (0x0F << 8) ; // 0x0F41
```

We are using the short data type as the VGA text mode standard uses two bytes for each character. the upper byte (because of little endianess) is split into the foreground and background colors (explained in the book), and the other byte is the ASCII character to print.


displaying a blinking star and some strings:

```C
#include "vgadrv.h"  
#include "time.h"  
#include "string.h"  
  
void kmain(void){  
  
   int x = 30;  
   int y = 8;  
  
   clean_screen(80,25); // clean vga 80 x 25 text mode  
   //volatile unsigned short * fb = (unsigned short*) 0x000B8000;  
   //fb[20] = 'A' | (0x0F << 8) ;  
   char c[] = "Snaggy is the best!";  
   write_str(c,str_len(c,255),0,0,0x0F); //White  
   while (1) {  
       draw_star5(x, y, 0x0E);  // Yellow  
       delay(20);               // Wait a bit  
       clear_star5(x, y);       // Erase star  
       delay(20);               // Wait again  
   }  
}

```

