#ifndef IO_H
#define IO_H

#include <stdint.h>
// Send a byte to a hardware port
// val: The data to send
// port: The device address
static inline void outb(unsigned short port, unsigned char val){
    // "outb %%al, %%dx" is the actual assembly instruction
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Receive a byte from a hardware port
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    asm volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static inline void outw(uint16_t port, uint16_t data) {
    asm volatile("outw %0, %1" : : "a" (data), "Nd" (port));
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif