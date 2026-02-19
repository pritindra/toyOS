#include "keyboard_buffer.h"

volatile char kb_buffer[KB_BUFFER_SIZE];
volatile int kb_write_idx = 0;
volatile int kb_read_idx = 0;

void kb_buffer_init() {
    kb_write_idx = 0;
    kb_read_idx = 0;
}

void kb_buffer_write(char c) {
    kb_buffer[kb_write_idx] = c;
    kb_write_idx = (kb_write_idx + 1) % KB_BUFFER_SIZE;
    
    // Overwrite if full
    if (kb_write_idx == kb_read_idx) {
        kb_read_idx = (kb_read_idx + 1) % KB_BUFFER_SIZE;
    }
}

char kb_buffer_read() {
    if (kb_read_idx == kb_write_idx) {
        return 0; // Buffer empty
    }
    
    char c = kb_buffer[kb_read_idx];
    kb_read_idx = (kb_read_idx + 1) % KB_BUFFER_SIZE;
    return c;
}