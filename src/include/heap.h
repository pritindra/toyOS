#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

#define HEAP_START 0x00200000 // Place heap at 2MB
#define HEAP_SIZE  0x00100000 // 1MB Max Size for now

typedef struct header {
    struct header* next; // Pointer to the next block
    uint32_t size;       // Size of the data part (excluding header)
    uint8_t is_free;     // 1 = Free, 0 = Used
} header_t;

void heap_init();
void* kmalloc(uint32_t size);
void kfree(void* ptr);

#endif