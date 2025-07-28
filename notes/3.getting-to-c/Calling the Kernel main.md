 To call the kernel main, we need to call it from the loader by importing it (the kmain will be linked with the loader via the linker) into the kernel ELF. 

```asm
global loader                   ; the entry symbol for ELF  
  
   MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant  
   FLAGS        equ 0x0            ; multiboot flags  
   CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum  
                                   ; (magic number + checksum + flags should equal 0)  
   KERNEL_STACK_SIZE equ 4096  
  
  
   section .bss  
   align 4  
   kernel_stack:  
       resb KERNEL_STACK_SIZE  
   section .text:                  ; start of the text (code) section  
   align 4                         ; the code must be 4 byte aligned  
       dd MAGIC_NUMBER             ; write the magic number to the machine code,  
       dd FLAGS                    ; the flags,  
       dd CHECKSUM                 ; and the checksum  
  
       extern kmain                ; kmain defined in the C file  
   loader:                         ; the loader label (defined as entry point in linker script)  
       mov eax, 0xCAFEBABE         ; place the number 0xCAFEBABE in the register eax  
       mov esp, kernel_stack + KERNEL_STACK_SIZE  
       call kmain                  ; call kmain  
   .loop:  
       jmp .loop                   ; loop forever
```

 
and to test it we could continuously modify the eax and ebx registers (via an endless loop) in the kernel's main (kmain) and inspect the registers in qemu. The loop is used as some optimizations might override the register assignments:

kmain.c:

```C
void kmain(void){  
   while(1)  
       __asm__ __volatile__(  
       "movl $0xDEADBEEF, %%ebx\n"  
       "movl $0xCAFED00D, %%eax\n"  
       ::: "ebx", "eax"  
   );  
}
```

and finally we do 

```bash
make
make run
qemu-system-i386 -cdrom os.iso -monitor stdio  
QEMU 7.2.17 monitor - type 'help' for more information  
(qemu) info registers  
  
CPU#0  
EAX=cafed00d EBX=deadbeef ECX=00000000 EDX=00000000  
ESI=00000000 EDI=00000000 EBP=00101ff8 ESP=00101ff4  
EIP=0010000e EFL=00000016 [----AP-] CPL=0 II=0 A20=1 SMM=0 HLT=0
...
```

