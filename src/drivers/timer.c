#include "timer.h"
#include "io.h"
#include "stdio.h"

volatile uint32_t timer_ticks = 0;

void timer_init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));

    printf("PIT Initialized at %d Hz.\n", frequency);
}

void timer_callback() {
    timer_ticks++;
}

uint32_t get_uptime_ms() {
    return timer_ticks;
}

void sleep(uint32_t milliseconds) {
    uint32_t start_time = timer_ticks;
    while (timer_ticks < start_time + milliseconds) {
        process_schedule();
    }
}