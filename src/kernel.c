#include "stdio.h"
#include "idt.h"
// 0xB8000 is the video memory address for VGA text mode color
volatile unsigned char* video_memory = (unsigned char*)0xB8000;

void serial_init();
void serial_write_str(const char* str);

void kernel_main() {

    serial_init();
    
    // printf("toy kernel");

    // int x = 45;
    // printf("Integer: %d\n", x);

    // char* status = "Running";
    // printf("System Status: %s\n", status);

    // int pointer = 0x1000;
    // printf("Kernel loaded at: %x\n", pointer);

    // printf("Test negative: %d\n", -50);

    idt_init();
    printf("Kernel started. IDT initialized.\n");
    printf("Testing Divide by Zero...\n");

    // 2. Force the crash
    int a = 10;
    int b = 0;
    int c = a / b;

    printf("DEBUG: the exception failed! Result: %d\n", c);

    while(1) {}

}