#include "process.h"
#include "heap.h"
#include "stdio.h"

process_t* current_process = 0;
process_t* process_list = 0;
int next_pid = 1;

extern void switch_task(uint32_t* old_esp_ptr, uint32_t new_esp);

void process_init() {
    current_process = (process_t*)kmalloc(sizeof(process_t));
    current_process->pid = 0;
    current_process->next = 0;
    
    current_process->next = current_process; 
    process_list = current_process;
}

void process_create(void (*function)()) {
    process_t* new_proc = (process_t*)kmalloc(sizeof(process_t));
    uint32_t* stack = (uint32_t*)kmalloc(4096);
    uint32_t* esp = stack + 1024; 

    *--esp = (uint32_t)function;
    *--esp = 0;
    *--esp = 0;
    *--esp = 0;
    *--esp = 0;

    new_proc->pid = next_pid++;
    new_proc->esp = (uint32_t)esp;
    new_proc->next = process_list->next;
    process_list->next = new_proc;
}

void process_schedule() {
    if (!current_process) return;

    process_t* next_task = current_process->next;
    
    if (next_task == current_process) return;

    process_t* old_task = current_process;
    current_process = next_task;
    
    switch_task(&(old_task->esp), next_task->esp);
}