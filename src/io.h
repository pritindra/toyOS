#ifndef IO_H
#define IO_H

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

#endif