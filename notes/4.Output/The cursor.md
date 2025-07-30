The VGA standard has a hardware cursor, which is the blinking underscore that appears at boot. This cursor moves via a privileged instruction that must be executed from assembly, so we either need to use inline assembly or write an assembly wrapper as the book suggests:

```asm 
; io.s
global outb             ; make the label outb visible outside this file

    ; outb - send a byte to an I/O port
    ; stack: [esp + 8] the data byte
    ;        [esp + 4] the I/O port
    ;        [esp    ] return address
    outb:
        mov al, [esp + 8]    ; move the data to be sent into the al register
        mov dx, [esp + 4]    ; move the address of the I/O port into the dx register
        out dx, al           ; send the data to the I/O port
        ret                  ; return to the calling function
```

then we add a header file to include that function in C:

```C
#ifndef INCLUDE_IO_H  
#define INCLUDE_IO_H  
  
void outb(unsigned short port, unsigned char data);  
  
#endif
```

then we can wrap that function in another function that makes it easier to use:

```C
void fb_move_cursor(unsigned short pos) {
    outb(0x3D4, 14);                // "I'm sending the high byte"
    outb(0x3D5, (pos >> 8) & 0xFF); // send high byte
    outb(0x3D4, 15);                // "I'm sending the low byte"
    outb(0x3D5, pos & 0xFF);        // send low byte
}
// And yes chatGPT explained it to me
```

I also implemented a write function as the book suggested that makes use of the cursor's position and updates it accordingly.
