#include "pic.h"
#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x11  // Init command
#define ICW4_8086    0x01  // 8086/88 (MCS-80/85) mode

void pic_remap() {
    unsigned char a1, a2;

    // Save masks (current interrupt state)
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // 1. Start initialization (ICW1)
    outb(PIC1_COMMAND, ICW1_INIT); io_wait();
    outb(PIC2_COMMAND, ICW1_INIT); io_wait();

    // 2. Map Vector Offsets (ICW2)
    // Master PIC = 0x20 (Interrupt 32)
    // Slave PIC  = 0x28 (Interrupt 40)
    outb(PIC1_DATA, 0x20); io_wait();
    outb(PIC2_DATA, 0x28); io_wait();

    // 3. Configure Cascading (ICW3)
    // Tell Master that Slave is at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4); io_wait();
    // Tell Slave its cascade identity (0000 0010)
    outb(PIC2_DATA, 2); io_wait();

    // 4. Set Mode (ICW4)
    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();

    // Restore saved masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

// Disable all interrupts (useful for debugging)
void pic_disable() {
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

// End of Interrupt (EOI)
// We must send this at the end of every ISR, or the PIC won't send more!
void pic_send_eoi(unsigned char irq) {
    if(irq >= 8)
        outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}