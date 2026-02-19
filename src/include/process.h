#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

typedef struct process process_t;

typedef struct process {
    uint32_t pid;
    uint32_t esp;
    struct process* next;
};

void process_init();
void process_create(void (*function)());
void process_schedule();

#endif