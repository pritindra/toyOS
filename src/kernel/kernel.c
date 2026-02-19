#include "stdio.h"
#include "idt.h"
#include "pic.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "process.h"
// 0xB8000 is the video memory address for VGA text mode color
volatile unsigned char* video_memory = (unsigned char*)0xB8000;

void serial_init();
void serial_write_str(const char* str);

void enable_interrupts() {
    asm volatile("sti");
}

void shell_task();

void kernel_main() {

    serial_init();

    pic_remap();

    idt_init();

    pmm_init();

    vmm_init();

    heap_init();

    process_init();

    process_create(shell_task);
    printf("Kernel Initialized. Starting Multitasking...\n");
    // printf("Play with the toy OS..\n");
    // printf("> ");

    enable_interrupts();

    while(1) {
        // PID 0 (Kernel is a task)
        process_schedule();
        // asm volatile("hlt");
    }

}