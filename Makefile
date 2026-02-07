ASM=nasm
CC=gcc
LD=ld

SRC_DIR=src
LINKER_SCRIPT=linker/linker.ld
BUILD_DIR=build

# Compiler flags: 32-bit, no standard library, no built-in functions, no PIE/PIC/Stack protection
CFLAGS=-m32 -ffreestanding -fno-pic -fno-pie -fno-stack-protector -nostdlib -c

# Linker flags: x86 format, use our linker script, binary output
LDFLAGS=-m elf_i386 -T $(LINKER_SCRIPT) --oformat binary

# Default target
all: $(BUILD_DIR)/bootloader_floppy.img

# ------------------------------------------------------------------------------
# 1. Final Disk Image
# ------------------------------------------------------------------------------
$(BUILD_DIR)/bootloader_floppy.img: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/bootloader_floppy.img
	truncate -s 1440k $(BUILD_DIR)/bootloader_floppy.img

# ------------------------------------------------------------------------------
# 2. Bootloader
# ------------------------------------------------------------------------------
$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/bootloader.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/bootloader.asm -f bin -o $(BUILD_DIR)/bootloader.bin

# ------------------------------------------------------------------------------
# 3. Kernel Linking
# ------------------------------------------------------------------------------
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/entry.o \
                         $(BUILD_DIR)/kernel.o \
                         $(BUILD_DIR)/uart.o \
                         $(BUILD_DIR)/printf.o \
                         $(BUILD_DIR)/interrupts.o \
                         $(BUILD_DIR)/idt.o \
                         $(BUILD_DIR)/isr.o
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.bin $^

# ------------------------------------------------------------------------------
# 4. Compilation Rules
# ------------------------------------------------------------------------------

# Assembly Files
$(BUILD_DIR)/entry.o: $(SRC_DIR)/entry.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/entry.asm -f elf32 -o $(BUILD_DIR)/entry.o

# NEW: Interrupts Assembly Stub
$(BUILD_DIR)/interrupts.o: $(SRC_DIR)/interrupts.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/interrupts.asm -f elf32 -o $(BUILD_DIR)/interrupts.o

# C Files
$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/uart.o: $(SRC_DIR)/uart.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o

$(BUILD_DIR)/printf.o: $(SRC_DIR)/printf.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/printf.c -o $(BUILD_DIR)/printf.o

# NEW: IDT and ISR C files
$(BUILD_DIR)/idt.o: $(SRC_DIR)/idt.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/idt.c -o $(BUILD_DIR)/idt.o

$(BUILD_DIR)/isr.o: $(SRC_DIR)/isr.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/isr.c -o $(BUILD_DIR)/isr.o

# ------------------------------------------------------------------------------
# 5. Run & Clean
# ------------------------------------------------------------------------------
run: $(BUILD_DIR)/bootloader_floppy.img
	qemu-system-i386 -fda $(BUILD_DIR)/bootloader_floppy.img -serial stdio

clean:
	rm -rf $(BUILD_DIR)/*