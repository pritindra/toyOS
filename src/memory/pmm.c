#include "pmm.h"
#include "stdio.h" 

uint8_t bitmap[BITMAP_SIZE];

void mmap_set(int bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

void mmap_unset(int bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

int mmap_test(int bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}

void pmm_init() {
    // 1. Clear bitmap (all free)
    for (int i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0;
    }

    // 2. Reserve Kernel Memory (First 1MB)
    // 1MB = 256 blocks of 4KB
    for (int i = 0; i < 256; i++) {
        mmap_set(i);
    }
    
    printf("PMM Initialized. Managing 128MB RAM.\n");
}

void* pmm_alloc_block() {
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        if (!mmap_test(i)) {
            mmap_set(i);
            return (void*)(i * BLOCK_SIZE);
        }
    }
    return 0; // Out of Memory
}

void pmm_free_block(void* addr) {
    uint32_t address = (uint32_t)addr;
    int block_index = address / BLOCK_SIZE;
    mmap_unset(block_index);
}