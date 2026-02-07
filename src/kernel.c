#include "stdio.h"
#include "idt.h"
#include "pic.h"
// 0xB8000 is the video memory address for VGA text mode color
volatile unsigned char* video_memory = (unsigned char*)0xB8000;

void serial_init();
void serial_write_str(const char* str);

void enable_interrupts() {
    asm volatile("sti");
}

void kernel_main() {

    serial_init();

    pic_remap();

    idt_init();

    printf("Kernel Started ....");

    enable_interrupts();
    
    printf("Interrupts enabled. Type something!\n");

    while(1) {
        asm volatile("hlt");
    }

}