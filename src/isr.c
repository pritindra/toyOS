#include "isr.h"
#include "stdio.h"

void isr_handler(registers_t regs) {
    if (regs.int_no == 0) {
        printf("\n[PANIC] DIVISION BY ZERO DETECTED!\n");
        printf("Instruction Pointer (EIP): 0x%x\n", regs.eip);
        printf("System Halted.\n");
        
        // Hang the system
        for(;;);
    } else {
        printf("Received interrupt: %d\n", regs.int_no);
    }
}