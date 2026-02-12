#include "stdio.h"
#include "idt.h"
#include "pic.h"
#include "pmm.h"
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

    pmm_init();

    // --- TEST PMM ---
    printf("Allocating A...\n");
    void* a = pmm_alloc_block();
    printf("A: 0x%x\n", (unsigned int)a);

    printf("Allocating B...\n");
    void* b = pmm_alloc_block();
    printf("B: 0x%x\n", (unsigned int)b);

    printf("Freeing A...\n");
    pmm_free_block(a);

    printf("Allocating C (Should reuse A)...\n");
    void* c = pmm_alloc_block();
    printf("C: 0x%x\n", (unsigned int)c);
    

    printf("Play with the toy OS..\n");
    printf("> ");

    enable_interrupts();

    while(1) {
        asm volatile("hlt");
    }

}