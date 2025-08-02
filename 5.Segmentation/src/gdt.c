#include "gdt.h"

#define GDT_ENTRIES 3
struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdtp;

extern void gdt_flush(uint32_t); // from segmentation.s

static void gdt_set_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[idx].base_low    = base & 0xFFFF;
    gdt[idx].base_middle = (base >> 16) & 0xFF;
    gdt[idx].base_high   = (base >> 24) & 0xFF;

    gdt[idx].limit_low   = limit & 0xFFFF;
    gdt[idx].granularity = (limit >> 16) & 0x0F;

    gdt[idx].granularity |= gran & 0xF0;
    gdt[idx].access       = access;
}

void gdt_init() {
    gdtp.size = sizeof(gdt) - 1;
    gdtp.address = (uint32_t)&gdt;

    // Null segment
    gdt_set_entry(0, 0, 0, 0, 0);
    // Code segment: base 0, limit 4GB, access=0x9A, gran=0xCF
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // Data segment: base 0, limit 4GB, access=0x92, gran=0xCF
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32_t)&gdtp);
}

