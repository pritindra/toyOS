ASM=nasm
CC=gcc
LD=ld

SRC_DIR=src
LINKER_SCRIPT=linker/linker.ld
BUILD_DIR=build

# Compiler flags: 32-bit, no standard library, no built-in functions
CFLAGS=-m32 -ffreestanding -fno-pic -fno-pie -fno-stack-protector -nostdlib -c

# Linker flags: x86 format, use our linker script, binary output
LDFLAGS=-m elf_i386 -T $(LINKER_SCRIPT) --oformat binary

# Default target
all: $(BUILD_DIR)/bootloader_floppy.img

# 1. Create the final disk image (Bootloader + Kernel + Padding)
$(BUILD_DIR)/bootloader_floppy.img: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	# Combine bootloader and kernel
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/bootloader_floppy.img
	# Pad the rest of the file to 1.44MB (floppy size)
	truncate -s 1440k $(BUILD_DIR)/bootloader_floppy.img

# 2. Compile the Bootloader
$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/bootloader.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/bootloader.asm -f bin -o $(BUILD_DIR)/bootloader.bin

# 3. Link the Kernel (Entry + C code)
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/uart.o
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/uart.o

# 4. Assemble the Kernel Entry (32-bit assembly)
$(BUILD_DIR)/entry.o: $(SRC_DIR)/entry.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/entry.asm -f elf32 -o $(BUILD_DIR)/entry.o

# 5. Compile the C Kernel
$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/uart.o: $(SRC_DIR)/uart.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/uart.c -o $(BUILD_DIR)/uart.o

# 6. Run in QEMU (Convenience)
run: $(BUILD_DIR)/bootloader_floppy.img
	qemu-system-i386 -drive format=raw,file=$(BUILD_DIR)/bootloader_floppy.img --serial stdio

# 7. Clean build files
clean:
	rm -rf $(BUILD_DIR)/*