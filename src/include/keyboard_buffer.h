#ifndef KEYBOARD_BUFFER_H
#define KEYBOARD_BUFFER_H

#define KB_BUFFER_SIZE 256

void kb_buffer_init();
void kb_buffer_write(char c);
char kb_buffer_read();

#endif