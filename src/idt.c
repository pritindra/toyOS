#include "idt.h"

// Define the IDT (256 entries)
idt_entry_t idt[256];
idtr_t idtr;

extern void isr0();

// Assembly helper
extern void idt_load(idtr_t* ptr);

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t type_attr) {
    idt[n].isr_low = (uint16_t)(handler & 0xFFFF);
    idt[n].kernel_cs = sel;
    idt[n].reserved = 0;
    idt[n].attributes = type_attr;
    idt[n].isr_high = (uint16_t)((handler >> 16) & 0xFFFF);
}

void idt_init() {
    // 1. Set up the IDT Pointer
    idtr.base = (uint32_t)&idt;
    idtr.limit = sizeof(idt_entry_t) * 256 - 1;

    // Set Gate 0 to point to isr0
    // 0x08 is the Code Segment Offset in GDT
    // 0x8E means "Present, Ring 0, Interrupt Gate"
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    
    // 3. Load the IDT
    idt_load(&idtr);
}