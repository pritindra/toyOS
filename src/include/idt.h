#ifndef IDT_H
#define IDT_H

#include <stdint.h> 

// 1. The IDT Entry Structure
typedef struct {
    uint16_t isr_low;   // Lower 16 bits of handler address
    uint16_t kernel_cs; // Code Segment Selector (GDT offset)
    uint8_t  reserved;  // Always 0
    uint8_t  attributes;// Type and attributes
    uint16_t isr_high;  // Upper 16 bits of handler address
} __attribute__((packed)) idt_entry_t;

// 2. The IDT Register (IDTR)
typedef struct {
    uint16_t limit;     // Size of IDT - 1
    uint32_t base;      // Address of first IDT entry
} __attribute__((packed)) idtr_t;

// Functions
void idt_init();
void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t type_attr);

#endif