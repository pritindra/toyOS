#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define BLOCK_SIZE 4096
#define RAM_SIZE   1024*1024*128 // 128 MB
#define TOTAL_BLOCKS (RAM_SIZE / BLOCK_SIZE)
#define BITMAP_SIZE  (TOTAL_BLOCKS / 8)

void pmm_init();
void* pmm_alloc_block();
void pmm_free_block(void* addr);

#endif