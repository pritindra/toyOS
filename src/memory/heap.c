#include "heap.h"
#include "stdio.h"

// Pointer to the first block in the list
header_t* head = 0;

void heap_init() {
    // one massive free block covering the entire heap
    head = (header_t*)HEAP_START;
    head->next = 0;
    head->size = HEAP_SIZE - sizeof(header_t);
    head->is_free = 1;

    printf("Kernel Heap Initialized at 0x%x (Size: 1MB)\n", HEAP_START);
}

void* kmalloc(uint32_t size) {
    header_t* curr = head;

    while (curr) {
        if (curr->is_free && curr->size >= size) {
            
            if (curr->size > size + sizeof(header_t)) {
                // Calculate address of the new split block
                header_t* new_block = (header_t*)((uint32_t)curr + sizeof(header_t) + size);
                
                // Update new block details
                new_block->is_free = 1;
                new_block->size = curr->size - size - sizeof(header_t);
                new_block->next = curr->next;

                // Update current block details
                curr->size = size;
                curr->next = new_block;
            }

            // Mark as used and return pointer to DATA
            curr->is_free = 0;
            return (void*)((uint32_t)curr + sizeof(header_t));
        }

        curr = curr->next;
    }

    printf("[Heap] Out of Memory!\n");
    return 0;
}


void kfree(void* ptr) {
    if (!ptr) return;

    // Get the header (located just before the pointer)
    header_t* header = (header_t*)((uint32_t)ptr - sizeof(header_t));
    
    // Mark as free
    header->is_free = 1;

    // TODO: Merge with next block if it is also free (Coalescing)
}