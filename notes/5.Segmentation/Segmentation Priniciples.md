
Memory segmentation is used to split the memory regions based on privilege access levels.

Paging, which is the more modern way of memory management, uses fixed page sizes to split memory regions based on size, access levels and other parameters. 
### Why segmentation still exists:

Even though it's rarely used for fine-grained memory layout anymore, the x86 CPU still:

- **Requires you to set up segments (GDT)**
    
- **Uses `cs`, `ds`, etc.** to interpret instructions and memory access
    
- Enforces access rights **based on the segment type** (e.g., you can't execute from a read-only data segment)

In **modern protected mode**, segmentation is used **mainly to enforce access rights** like:

- **Execute-only or Execute-Read** (for `.text`)
    
- **Read-Write** (for `.data`, `.bss`, heap, stack)
    
- **Privilege level enforcement** (kernel vs user mode)


In any x86 based OS we need to setup the GDT to tell the CPU how we need to interpret the different segments andcs, ds, ss) accordingly.


## Why we cannot just set cs register to `0x08`

You **cannot just `mov cs, 0x08`**. That’s because:

- `cs` is tightly tied to control flow (it determines where you're executing from).
    
- The CPU **only allows changing `cs` via control-flow instructions**, like:
    

###  jmp 0x08:flush_cs

That’s called a **far jump**. It takes:

- **Segment selector**: `0x08` (for code segment)
    
- **Offset**: address of label `flush_cs`
    

So this instruction means:

> "Jump to the address `flush_cs`, but do so using GDT[1] as the code segment."

When it executes:

1. It **sets `cs = 0x08`**
    
2. It **jumps to `flush_cs`**
    

Which effectively transitions you to use your new code segment (like enabling the new GDT).

it's a weird way (a quirk) to set a register's value in x86 so yea...


The gdt_entry struct:

#### 🔹 `limit_low` (16 bits)

- This is the **lower part** of the segment limit.
    
- The **limit** defines how big the segment is (either in bytes or 4KiB units).
    

#### 🔹 `base_low` (16 bits)

- Lower 16 bits of the **base address** of the segment.
    

#### 🔹 `base_middle` (8 bits)

- Next 8 bits of the base (bits 16–23).
    

#### 🔹 `access` (8 bits)

- Contains flags like:
    
    - Segment present (bit 7)
        
    - Privilege level (bits 6–5)
        
    - Descriptor type (bit 4)
        
    - Executable (bit 3)
        
    - Direction/conforming (bit 2)
        
    - Readable/writable (bit 1)
        
    - Accessed (bit 0)
        

> Example: `0x9A` = `10011010b`
> 
> - Present: 1
>     
> - DPL: 00 (kernel)
>     
> - Code/data: 1
>     
> - Executable: 1
>     
> - Readable: 1
>     
> - Accessed: 0
>     

#### 🔹 `granularity` (8 bits)

- This packs **two separate things**:
    
    - **High 4 bits** of the segment **limit** (bits 16–19)
        
    - **Granularity flags** in the upper 4 bits:
        
        - Granularity: 1 = limit is in 4KiB units (default for kernel)
            
        - Size: 1 = 32-bit segment
            
        - 2 bits reserved
            

> Example: `0xCF` = `11001111b`
> 
> - Limit high: `1111` (bits 0–3)
>     
> - Granularity: `1100` (bits 4–7) → 4KiB, 32-bit mode
>     

#### 🔹 `base_high` (8 bits)

- Final 8 bits of the base address (bits 24–31)

>Again a lot of what I'm doing is to learn about operating systems in general and not a deep dive so AI is a blessing here... don't judge

```
  63                            0
+----------------------------------------+
| Base 31:24 | G | D | 0 | A | Limit19:16| ← granularity + base_high
+------------+---+---+---+---+-----------+
| Access Byte: P | DPL | S | Type       | ← access
+----------------------------------------+
| Base 23:16  | Base 15:0               | ← base_middle + base_low
+----------------------------------------+
| Limit 15:0                            | ← limit_low
+----------------------------------------+

```


sure enough when we run the image we can see the registers being populated properly:

```
ES =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]  
SS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
FS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]  
GS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
```

