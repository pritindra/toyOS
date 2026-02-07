#include "io.h"
#include "pic.h"
#include "stdio.h"
#include "isr.h"

// -----------------------------------------------------------------------------
// Scancode Table (US QWERTY)
// Maps Scancode (Index) -> ASCII Character
// -----------------------------------------------------------------------------
unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, // Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0, // Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, // Right Shift
    '*',
    0, // Alt
    ' ', // Space
    0, // Caps Lock
};

// -----------------------------------------------------------------------------
// Keyboard Handler
// -----------------------------------------------------------------------------
void keyboard_handler(registers_t* regs) {
    // 1. Read from the keyboard's data buffer
    unsigned char scancode = inb(0x60);

    // 2. Check if the top bit is set (Key Release)
    if (scancode & 0x80) {
        // We ignore key releases for now
        // (scancode & 0x7F) would give us the pressed key
    } else {
        // 3. It's a key press! Translate it to ASCII
        char c = kbdus[scancode];
        if (c != 0) {
            printf("%c", c); // Print directly to screen/serial
        }
    }

    // 4. Tell the PIC we are done
    pic_send_eoi(1); // IRQ 1 = Keyboard
}

// -----------------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------------
void keyboard_init() {
    printf("Keyboard initialized.\n");
}