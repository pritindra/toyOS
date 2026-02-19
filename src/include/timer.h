#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(uint32_t frequency);
void timer_callback();
void sleep(uint32_t milliseconds);
uint32_t get_uptime_ms();

#endif