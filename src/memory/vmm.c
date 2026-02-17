#include "vmm.h"
#include "pmm.h"
#include "stdio.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

// One Page Table to map the first 4MB (0 - 0x400000)
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

extern void load_page_directory(unsigned int*);
extern void enable_paging();

void vmm_init() {
    // Virtual Address 0 -> Physical Address 0
    // Virtual Address 4096 -> Physical Address 4096
    for (int i = 0; i < 1024; i++) {
        // Address = Index * 4KB
        // Flags = Present | Writable
        first_page_table[i] = (i * 4096) | 3; 
    }

    // Entry 0 covers Virtual Address 0MB - 4MB
    page_directory[0] = ((unsigned int)first_page_table) | 3;

    // Clear the rest of the directory
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0 | 2; // Writable, but Not Present
    }

    // Load the Page Directory
    load_page_directory(page_directory); // CR3

    enable_paging(); // CR0

    printf("VMM Initialized. Paging Enabled.\n");
}