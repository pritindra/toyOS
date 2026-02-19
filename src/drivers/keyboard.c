#include "io.h"
#include "pic.h"
#include "stdio.h"
#include "isr.h"
#include "keyboard_buffer.h"

#define MAX_CMD_SIZE 100
char key_buffer[MAX_CMD_SIZE];
int buffer_idx = 0;

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
    unsigned char scancode = inb(0x60);

    // Ignore key releases
    if (scancode & 0x80) {
        pic_send_eoi(1);
        return;
    }

    if (scancode < 128) {
        char c = kbdus[scancode];
        if (c != 0) {
            // Producer: Write to buffer
            kb_buffer_write(c);
        }
    }

    pic_send_eoi(1);
}


// -----------------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------------
void keyboard_init() {
    kb_buffer_init();
    printf("Keyboard initialized.\n");
}