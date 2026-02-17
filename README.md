# Toy OS (x86)

A 32-bit operating system built for fun and to tinker with, following the initial implementation steps from the [Nanobytes OS Development Series](https://youtu.be/9t-SPC7Tczc?si=Uu74xiFowo0F_s7W).

## Current Features

As of the latest build, the kernel supports the following features:

### Core Architecture
- **Multiboot Bootloader:** Custom 32-bit assembly bootloader.
- **Protected Mode:** Boots directly into 32-bit Protected Mode.
- **GDT (Global Descriptor Table):** Configured for flat memory model.

### Drivers & Hardware
- **VGA Text Mode:** Custom `printf` implementation writing directly to video memory (`0xB8000`).
- **UART Serial Port:** Logging support for debugging via QEMU serial console.
- **IDT & ISRs:** Full Interrupt Descriptor Table with CPU Exception handling (e.g., Divide by Zero, Page Faults).
- **PIC Remapping:** Programmable Interrupt Controller remapped to avoid CPU exception conflicts.
- **Keyboard Driver:** PS/2 Keyboard support with Scancode translation and a basic interactive shell.

### Memory Management
- **PMM (Physical Memory Manager):** Bitmap-based allocator managing 128MB of RAM.
- **VMM (Virtual Memory Manager):** Paging enabled with Identity Mapping for the kernel.
- **Heap:** Dynamic memory allocation (`kmalloc`/`kfree`) using a linked-list allocator.

---

## Prerequisites

To build and run this OS, you need a Linux environment (or WSL on Windows) with the following tools:

* **GCC** (Cross-compiler or standard `gcc` with `-m32` flags)
* **NASM** (Assembler)
* **GNU Make** (Build system)
* **QEMU** (Emulator)
