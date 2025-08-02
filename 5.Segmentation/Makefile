# Compiler and tools
OBJECTS = loader.o src/kmain.o src/vgadrv.o src/time.o src/string.o src/io.o src/serial.o \
	  src/segmentation.o src/gdt.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -Iinclude -Isrc
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
	echo '    multiboot /boot/$(KERNEL)' >> $@
	echo '}' >> $@

# Run with QEMU (replaces Bochs)
run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME) -monitor stdio -serial file:serial.log

# Assembly and C rules
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Clean
clean:
	rm -rf *.o $(KERNEL) $(ISO_NAME) $(ISO_DIR)/boot
 
