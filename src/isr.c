#include "isr.h"
#include "stdio.h"
#include "pic.h"

void keyboard_handler(registers_t* regs);

void isr_handler(registers_t regs) {
    // Handle IRQ 1 (Keyboard) -> Interrupt 33
    if (regs.int_no == 33) {
        keyboard_handler(&regs);
    }
    else if (regs.int_no == 32) {
        // TIMER INTERRUPT (IRQ 0)
        pic_send_eoi(0);
    }
    else if (regs.int_no == 0) {
        printf("\n[PANIC] DIVISION BY ZERO DETECTED!\n");
        printf("Instruction Pointer (EIP): 0x%x\n", regs.eip);
        printf("System Halted.\n");
        
        // Hang the system
        for(;;);
    } else {
        printf("Unhandled Interrupt: %d\n", regs.int_no);
        
        // If it's a hardware interrupt (>= 32), we must send EOI
        if (regs.int_no >= 32)
            pic_send_eoi(regs.int_no - 32);
    }
}