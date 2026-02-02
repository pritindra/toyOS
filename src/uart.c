#include "io.h"

#define COM1 0x3F8

// Offsets for specific registers
#define DATA_PORT    (COM1 + 0) // Read/Write data
#define INT_ENABLE   (COM1 + 1) // Interrupt Enable
#define FIFO_CTRL    (COM1 + 2) // FIFO Control
#define LINE_CTRL    (COM1 + 3) // Line Control 
#define MODEM_CTRL   (COM1 + 4) // Modem Control
#define LINE_STATUS  (COM1 + 5) // Line Status

void serial_init() {
    // 1. Disable interrupts (we want to poll for now)
    outb(INT_ENABLE, 0x00);

    // 2. Set Baud Rate (Speed)
    // We must set the "Divisor Latch Access Bit" (DLAB) to 1 to send the speed.
    outb(LINE_CTRL, 0x80); 
    
    // Send 0x03 (Low byte) and 0x00 (High byte) -> Divisor 3
    // 115200 baud / 3 = 38400 baud. Standard reliable speed.
    outb(DATA_PORT, 0x03); 
    outb(INT_ENABLE, 0x00);

    // 3. Configure Line Protocol: 8 bits, no parity, 1 stop bit
    // 0x03 = 00000011b
    outb(LINE_CTRL, 0x03);

    // 4. Enable FIFO (First In First Out buffer), clear them, with 14-byte threshold
    outb(FIFO_CTRL, 0xC7);

    // 5. Enable IRQs, RTS/DSR set
    outb(MODEM_CTRL, 0x0B);
}

int serial_receive(){
    return inb(LINE_STATUS) & 1;
}

int is_transmit_empty() {
    return inb(LINE_STATUS) & 0x20;
}

void serial_write_char(char a) {
    while (is_transmit_empty() == 0);
    outb(DATA_PORT, a);
}

void serial_write_str(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        serial_write_char(str[i]);
    }
}