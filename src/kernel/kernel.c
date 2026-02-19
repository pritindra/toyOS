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

// Task A
void task_a() {
    while(1) {
        printf("A");
        
        // loop to slow it down so we can see it
        for(int i=0; i<10000000; i++); 
        
        process_schedule(); // Yield CPU to next task
    }
}

// Task B
void task_b() {
    while(1) {
        printf("B");
        for(int i=0; i<10000000; i++);
        process_schedule();
    }
}

void kernel_main() {

    serial_init();

    pic_remap();

    idt_init();

    pmm_init();

    vmm_init();

    heap_init();

    // // --- HEAP TEST ---
    // printf("Allocating string...\n");
    // char* str = (char*)kmalloc(10);
    // str[0] = 'H'; str[1] = 'i'; str[2] = '\0';
    // printf("String content: %s (Address: 0x%x)\n", str, (unsigned int)str);

    // printf("Allocating struct...\n");
    // int* numbers = (int*)kmalloc(3 * sizeof(int)); // Array of 3 ints
    // numbers[0] = 100;
    // numbers[1] = 200;
    // numbers[2] = 300;
    // printf("Numbers: %d, %d, %d\n", numbers[0], numbers[1], numbers[2]);

    // kfree(str);
    // kfree(numbers);

    process_init();

    // Create two separate threads
    process_create(task_a);
    process_create(task_b);

    // printf("Kernel is running with Paging enabled!\n");
    // --- TEST PMM ---
    // printf("Allocating A...\n");
    // void* a = pmm_alloc_block();
    // printf("A: 0x%x\n", (unsigned int)a);

    // printf("Allocating B...\n");
    // void* b = pmm_alloc_block();
    // printf("B: 0x%x\n", (unsigned int)b);

    // printf("Freeing A...\n");
    // pmm_free_block(a);

    // printf("Allocating C (Should reuse A)...\n");
    // void* c = pmm_alloc_block();
    // printf("C: 0x%x\n", (unsigned int)c);
    

    // printf("Play with the toy OS..\n");
    // printf("> ");

    enable_interrupts();

    while(1) {
        // PID 0 (Kernel is a task)
        printf("K");
        for(int i=0; i<10000000; i++);
        process_schedule();
        // asm volatile("hlt");
    }

}