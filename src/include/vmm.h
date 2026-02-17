#ifndef VMM_H
#define VMM_H

#include <stdint.h>

// Page Table Entry (PTE) Flags
#define I86_PTE_PRESENT       0x01
#define I86_PTE_WRITABLE      0x02
#define I86_PTE_USER          0x04

// Page Directory Entry (PDE) Flags
#define I86_PDE_PRESENT       0x01
#define I86_PDE_WRITABLE      0x02
#define I86_PDE_USER          0x04

// 1024 entries per table/directory
#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR  1024

void vmm_init();

#endif