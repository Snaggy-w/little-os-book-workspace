
As described by the book, it doesn't guide us through everything. We want to setup a stack in which the kernel could allocate memory dynamically at runtime. We could point the esp register (stack pointer in x86) to a random location, but that's dangerous as it could conflict with other critical memory locations. 

the best practice is to setup a `.bss` section in the `ELF` file created by the loader script (`loader.s`):

```asm
 KERNEL_STACK_SIZE equ 4096                  ; size of stack in bytes

    section .bss
    align 4                                     ; align at 4 bytes
    kernel_stack:                               ; label points to beginning of memory
        resb KERNEL_STACK_SIZE                  ; reserve stack for the kernel
```

then we point to that stack via the stack pointer via the loader before calling the kernel main:
```asm
  mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
                                                ; stack (end of memory area)
```

