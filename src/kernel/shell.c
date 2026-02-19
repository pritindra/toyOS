#include "stdio.h"
#include "string.h"
#include "keyboard_buffer.h"
#include "process.h"
#include "io.h"

void shell_task() {
    char cmd_buffer[100];
    int cmd_idx = 0;

    printf("\nWelcome to ToyOS. Type 'help'.\n> ");

    while (1) {
        // Consumer: Read from buffer
        char c = kb_buffer_read();

        // If empty, yield CPU
        if (c == 0) {
            process_schedule();
            continue;
        }

        // Process Key
        if (c == '\n') {
            printf("\n");
            cmd_buffer[cmd_idx] = 0;

            if (cmd_idx > 0) {
                if (strcmp(cmd_buffer, "help") == 0) {
                    printf("Commands: help, hello, reboot\n");
                } 
                else if (strcmp(cmd_buffer, "hello") == 0) {
                    printf("Hello from Multitasking Shell!\n");
                }
                else if (strcmp(cmd_buffer, "reboot") == 0) {
                   outb(0x64, 0xFE); 
                   printf("Rebooting...\n");
                }
                else {
                    printf("Unknown command: %s\n", cmd_buffer);
                }
            }

            cmd_idx = 0;
            printf("> ");
        } 
        else if (c == '\b') {
            if (cmd_idx > 0) {
                cmd_idx--;
                printf("\b \b");
            }
        }
        else {
            if (cmd_idx < 99) {
                cmd_buffer[cmd_idx++] = c;
                printf("%c", c);
            }
        }
    }
}