
What’s the serial port setup about?
Serial ports (COM1, COM2, etc.) use I/O ports to communicate with the CPU.

Each serial port is assigned a base I/O port address, like COM1 = 0x3F8.

From the base port, different registers control different features of the serial port (data, line control, FIFO, modem, etc.).

```C
#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)
```

- These calculate the exact port address for each register based on the `base` address.

- For example, `SERIAL_LINE_COMMAND_PORT(SERIAL_COM1_BASE)` → `0x3F8 + 3 = 0x3FB`.

### The baud rate divisor and why:

- Serial ports have a **default clock rate** of 115,200 bits per second (bps).
    
- To set a **lower baud rate** (speed), you write a divisor value to specific registers.
    
- **Speed = 115,200 / divisor**.
    

Example:

- Divisor `1` → 115,200 bps (fastest)
    
- Divisor `12` → 9600 bps (common slow speed)

### About the divisor latch access:

The serial port has a special bit called **DLAB (Divisor Latch Access Bit)**, which is bit 7 (value `0x80`) of the line control register. Setting this bit tells the hardware:

- The **next two writes** to the data port set the **divisor value** (high byte then low byte).
    
- When DLAB is clear, the data port is used for actual data transfer.

```C
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);  // High byte
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);         // Low byte
}
```

- First, set DLAB = 1 to enable divisor register access (`outb` to line command port).
    
- Then write the **high 8 bits** of the divisor to the data port.
    
- Then write the **low 8 bits** of the divisor to the data port.

### Why two writes to `SERIAL_DATA_PORT`?

Because the divisor is a 16-bit number, but the port can only take 8 bits at a time. So, you send the upper 8 bits first, then the lower 8 bits.

## Serial configuration

The configuration given writes `0x03` to the COM port, that's equivalent to:

- **8 data bits** (`dl = 11` binary, bits 0 and 1)
    
- **No parity bits** (`prty = 000`, bits 3,4,5)
    
- **One stop bit** (`s = 0`, bit 2)
    
- **Break control disabled** (`b = 0`, bit 6)
    
- **No divisibility latch access bit set** (`d = 0`, bit 7)
    

---

### Why 0x03?

Bits breakdown:

|Bit|Name|Meaning|Value in your config|
|---|---|---|---|
|7|d (DLAB)|Divisor Latch Access Bit|0|
|6|b (Break)|Break control|0|
|5-3|prty|Parity bits|000 (no parity)|
|2|s (Stop)|Stop bits|0 (1 stop bit)|
|1-0|dl (Data)|Data length|11 (8 bits)|

Binary: `00000011` → Hex: `0x03`

## Serial init

Serial programming is painful especially late at night that's why I asked chatGPT to help me out a lot in writing the configuration stuff for that part.

So to use the serial functionality we could just call the relevant functions from the kernel:

```C
serial_init();  
serial_write(buf,str_len(buf,500));
```

And when we want to save the output of the serial output to a file we can update the qemu command used:

```bash
qemu-system-i386 -cdrom $(ISO_NAME) -monitor stdio -serial file:serial.log
```

