#include "io.h"
#include "pic.h"
#include "stdio.h"
#include "isr.h"

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
// Helper: String Compare
// Returns 0 if strings are equal
// -----------------------------------------------------------------------------
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// -----------------------------------------------------------------------------
// Keyboard Handler
// -----------------------------------------------------------------------------
void keyboard_handler(registers_t* regs) {

    unsigned char scancode = inb(0x60);

    if (scancode & 0x80){
        pic_send_eoi(1);
        return;
    }

    // Convert scancode to ASCII (simplistic)
    // You likely have a lookup table 'kbdus' from previous steps
    char c = kbdus[scancode]; 

    if (c == '\n') {
        // User hit Enter! Handle the command
        printf("\n");
        key_buffer[buffer_idx] = '\0'; // Null-terminate string
        
        if (strcmp(key_buffer, "help") == 0) {
            printf("Available commands: help, hello, reboot\n");
        } 
        else if (strcmp(key_buffer, "hello") == 0) {
            printf("Hello User!\n");
        }
        else if (strcmp(key_buffer, "reboot") == 0) {
            outb(0x64, 0xFE); // CPU Reset command
        }
        else {
            printf("Unknown command: %s\n", key_buffer);
        }

        // Reset buffer
        buffer_idx = 0;
        printf("> ");
    } 
    else if (c == '\b') {
        // Backspace
        if (buffer_idx > 0) {
            buffer_idx--;
            printf("\b \b"); // Visually delete
        }
    }
    else {
        // Normal character
        if (buffer_idx < MAX_CMD_SIZE - 1) {
            key_buffer[buffer_idx++] = c;
            printf("%c", c);
        }
    }
    pic_send_eoi(1); // IRQ 1 = Keyboard
}


// -----------------------------------------------------------------------------
// Initialization
// -----------------------------------------------------------------------------
void keyboard_init() {
    printf("Keyboard initialized.\n");
}