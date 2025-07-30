
I decided to update the structure of the project a bit as so:

```bash
4.Output git:(main) ✗ tree .  
.  
├── include  
│   └── vgadrv.h  
├── iso  
│   └── boot  
│       ├── grub  
│       │   └── grub.cfg  
│       └── kernel.elf  
├── kernel.elf  
├── link.ld  
├── loader.o  
├── loader.s  
├── Makefile  
├── os.iso  
└── src  
   ├── kmain.c  
   ├── kmain.o  
   ├── vgadrv.c  
   └── vgadrv.o  
  
6 directories, 13 files
```

The include directory contains header files, the src well... C source file and kept other stuff in the same place to keep the make file a bit simpler. Speaking of which, I updated the make file to reflect those changes:

```make
# Compiler and tools  
# added the src directory to the src files
OBJECTS = loader.o src/kmain.o src/vgadrv.o  
CC = gcc  
# added the flags to let gcc lookup header and src files
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \  
        -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -Iinclude -Isrc
```

