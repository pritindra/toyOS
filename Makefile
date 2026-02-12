ASM = nasm
CC = gcc
LD = ld

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = src/include

CFLAGS = -m32 -ffreestanding -fno-pic -fno-pie -fno-stack-protector -nostdlib -c -I$(INCLUDE_DIR)
LDFLAGS = -m elf_i386 -T linker/linker.ld --oformat binary

# ------------------------------------------------------------------------------
# File Discovery
# ------------------------------------------------------------------------------

# 1. C Files
C_SOURCES = $(wildcard $(SRC_DIR)/kernel/*.c) \
            $(wildcard $(SRC_DIR)/drivers/*.c) \
            $(wildcard $(SRC_DIR)/cpu/*.c) \
            $(wildcard $(SRC_DIR)/lib/*.c)

# 2. Assembly Files (Exclude bootloader & entry)
ASM_SOURCES = $(wildcard $(SRC_DIR)/boot/*.asm)
ASM_SOURCES := $(filter-out $(SRC_DIR)/boot/bootloader.asm, $(ASM_SOURCES))
ASM_SOURCES := $(filter-out $(SRC_DIR)/boot/entry.asm, $(ASM_SOURCES))

# 3. Generate Object Lists
# We use full paths under build/ (e.g., build/src/kernel/kernel.o)
C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJS = $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_SOURCES))

# 4. entry.o MUST be separate so we can put it first
ENTRY_OBJ = $(BUILD_DIR)/$(SRC_DIR)/boot/entry.o

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

all: $(BUILD_DIR)/bootloader_floppy.img

$(BUILD_DIR)/bootloader_floppy.img: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@
	truncate -s 1440k $@

$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/boot/bootloader.asm
	@mkdir -p $(dir $@)
	$(ASM) $< -f bin -o $@

# CRITICAL: Link ENTRY_OBJ first!
$(BUILD_DIR)/kernel.bin: $(ENTRY_OBJ) $(C_OBJS) $(ASM_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# ------------------------------------------------------------------------------
# Rules
# ------------------------------------------------------------------------------

# Rule for C files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Rule for Assembly files
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $< -f elf32 -o $@

run: $(BUILD_DIR)/bootloader_floppy.img
	qemu-system-i386 -fda $(BUILD_DIR)/bootloader_floppy.img -serial stdio -boot order=a

clean:
	rm -rf $(BUILD_DIR)