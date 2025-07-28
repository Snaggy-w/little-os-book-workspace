I'm not gonna pretend that I'm a makefile guru, so I used the help of chat gpt to optimize the Makefile given by the book and adapt it to my case (i.e build the iso with grub 2 and use qemu as the test run).

```makefile
# Compiler and tools  
OBJECTS = loader.o kmain.o  
CC = gcc  
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \  
        -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c  
LDFLAGS = -T link.ld -melf_i386  
AS = nasm  
ASFLAGS = -f elf  
  
# Output  
ISO_NAME = os.iso  
KERNEL = kernel.elf  
ISO_DIR = iso  
GRUB_CFG = $(ISO_DIR)/boot/grub/grub.cfg  
  
# Default target  
all: $(ISO_NAME)  
  
# Kernel linking  
$(KERNEL): $(OBJECTS)  
       ld $(LDFLAGS) $(OBJECTS) -o $@  
  
# Build ISO using GRUB 2  
$(ISO_NAME): $(KERNEL) $(GRUB_CFG)  
       mkdir -p $(ISO_DIR)/boot  
       cp $(KERNEL) $(ISO_DIR)/boot/  
       grub-mkrescue -o $(ISO_NAME) $(ISO_DIR)  
  
# Create GRUB 2 config  
$(GRUB_CFG):  
       mkdir -p $(dir $@)  
       echo 'set timeout=0' > $@  
       echo 'set default=0' >> $@  
       echo '' >> $@  
       echo 'menuentry "My OS" {' >> $@  
       echo '    multiboot /boot/$(KERNEL)' >> $@  
       echo '}' >> $@  
  
# Run with QEMU (replaces Bochs)  
run: $(ISO_NAME)  
       qemu-system-i386 -cdrom $(ISO_NAME) -monitor stdio  
  
# Assembly and C rules  
%.o: %.c  
       $(CC) $(CFLAGS) $< -o $@  
  
%.o: %.s  
       $(AS) $(ASFLAGS) $< -o $@  
  
# Clean  
clean:  
       rm -rf *.o $(KERNEL) $(ISO_NAME) $(ISO_DIR)/boot
```

