global outb

global inb

section .text
outb:
    mov al, [esp + 8]    ; load data byte
    mov dx, [esp + 4]    ; load port number
    out dx, al           ; send data to port
    ret


; inb - reads a byte from an I/O port
; stack: [esp + 4] = port
; returns: AL contains the byte (returned via EAX in C)

inb:
    mov dx, [esp + 4]   ; get port number
    in al, dx           ; read from port into AL
    movzx eax, al       ; zero-extend AL into EAX (so we return a full 32-bit value)
    ret

