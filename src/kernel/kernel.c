#include "stdio.h"
#include "idt.h"
#include "pic.h"
#include "ata.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "process.h"
#include "timer.h"
// 0xB8000 is the video memory address for VGA text mode color
volatile unsigned char* video_memory = (unsigned char*)0xB8000;

void serial_init();
void serial_write_str(const char* str);

void enable_interrupts() {
    asm volatile("sti");
}

void shell_task();

void clock_task() {
    while(1) {
        uint32_t seconds = get_uptime_ms() / 1000;
        printf("\n[Clock] Uptime: %d seconds\n> ", seconds);
        
        sleep(5000); // Sleep for exactly 5000 milliseconds
        
        process_schedule(); // Yield to the shell
    }
}

void test_ata_disk() {
    uint8_t write_buf[512];
    uint8_t read_buf[512];

    // Clear both buffers
    for(int i = 0; i < 512; i++) {
        write_buf[i] = 0;
        read_buf[i] = 0;
    }

    // Prepare our message
    const char* msg = "Hello from Sector 0! Permanent storage is working!";
    for(int i = 0; msg[i] != '\0'; i++) {
        write_buf[i] = msg[i];
    }

    printf("\n[ATA] Writing to Sector 0...\n");
    ata_write_sector(0, write_buf);

    printf("[ATA] Reading from Sector 0...\n");
    ata_read_sector(0, read_buf);

    printf("[ATA] Data read: %s\n\n", (char*)read_buf);
}

void kernel_main() {

    serial_init();

    pic_remap();

    idt_init();

    timer_init(1000);
    
    pmm_init();

    vmm_init();

    heap_init();

    // test_ata_disk();

    process_init();

    process_create(shell_task);
    process_create(clock_task);

    printf("Kernel Initialized. Starting Multitasking...\n");

    enable_interrupts();

    while(1) {
        asm volatile("hlt");
        // PID 0 (Kernel is a task)
        process_schedule();
        // asm volatile("hlt");
    }

}