#include "time.h"

void delay(int ticks) {
    for (volatile int i = 0; i < ticks * 1000000; i++) {
        __asm__ __volatile__("nop");
    }
}

