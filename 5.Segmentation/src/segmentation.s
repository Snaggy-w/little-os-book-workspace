global gdt_flush

gdt_flush:
    lgdt [eax]         ; Load the new GDT
    mov ax, 0x10       ; Data segment selector (index 2, ring 0)
    mov ds, ax	       ; all the other registers are considered in the data segment
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Now reload code segment using far jump
    jmp 0x08:flush_cs    ; Code segment selector (index 1)
flush_cs:
    ret

