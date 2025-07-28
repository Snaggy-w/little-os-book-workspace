
### 2.3.4 Building an ISO Image


The Little OS Book is tad old so it uses old technologies. After going through versioning hell, I decided to go with Grub 2 instead of Grub legacy, and QEMU instead of Bochs. 

The book suggests the following architecture to use with `genisoimage`:

```
 iso
    |-- boot
      |-- grub
      | |-- menu.lst
      | |-- stage2_eltorito
      |-- kernel.elf

```

My grub config architecture that will use `grub-mkrescue` will be as follows:

```
project/
├── iso/
│   └── boot/
│       └── grub/
│           └── grub.cfg
│       └── kernel.elf

```

```
set timeout=0  
set default=0  
  
menuentry "os" {  
   multiboot /boot/kernel.elf  
}  
```

```bash
grub-mkrescue -o os.iso iso
```

with the qemu monitor we can check the registers to get the values:

```bash
qemu-system-i386 -cdrom os.iso -monitor stdio  
QEMU 7.2.17 monitor - type 'help' for more information  
(qemu) info registers  
  
CPU#0  
EAX=cafebabe EBX=00010000 ECX=00000000 EDX=00000000  
ESI=00000000 EDI=00000000 EBP=00000000 ESP=0007ff00  
EIP=00100011 EFL=00000046 [---Z-P-] CPL=0 II=0 A20=1 SMM=0 HLT=0  
ES =0018 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
CS =0010 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]  
SS =0018 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
DS =0018 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
FS =0018 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
GS =0018 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
LDT=0000 00000000 0000ffff 00008200 DPL=0 LDT  
TR =0000 00000000 0000ffff 00008b00 DPL=0 TSS32-busy  
GDT=     000010b0 00000020  
IDT=     00000000 00000000  
CR0=00000011 CR2=00000000 CR3=00000000 CR4=00000000  
DR0=00000000 DR1=00000000 DR2=00000000 DR3=00000000    
DR6=ffff0ff0 DR7=00000400  
EFER=0000000000000000  
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80  
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000  
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000  
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000  
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000  
XMM00=0000000000000000 0000000000000000 XMM01=0000000000000000 0000000000000000  
XMM02=0000000000000000 0000000000000000 XMM03=0000000000000000 0000000000000000  
XMM04=0000000000000000 0000000000000000 XMM05=0000000000000000 0000000000000000  
XMM06=0000000000000000 0000000000000000 XMM07=0000000000000000 0000000000000000  
(qemu)
```

>*Note:* All other commands and tool chains are the same as the book