// src/kernel.c

// 0xB8000 is the video memory address for VGA text mode color
// We use 'volatile' so the compiler doesn't optimize these writes away
volatile unsigned char* video_memory = (unsigned char*)0xB8000;

void kernel_main() {
    // Write "K" to the top-left corner
    video_memory[0] = 'K';
    video_memory[1] = 0x0F; // White text on black background

    // Write "E"
    video_memory[2] = 'E';
    video_memory[3] = 0x0F;

    // Write "R"
    video_memory[4] = 'R';
    video_memory[5] = 0x0F;

    while(1) {
        // Infinite loop to keep the kernel running
    }
}