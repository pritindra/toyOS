#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_remap();
void pic_send_eoi(unsigned char irq);
void pic_disable();

#endif