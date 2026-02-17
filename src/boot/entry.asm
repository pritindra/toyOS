; src/entry.asm
[bits 16]
[extern kernel_main] ; We will call this C function later

global _start
_start:
    ; 1. Disable Interrupts (BIOS interrupts won't work in 32-bit mode anyway)
    cli

    ; 2. Load the GDT Descriptor
    lgdt [gdt_descriptor]

    ; 3. Switch to Protected Mode
    ; Set the first bit of CR0 (Control Register 0) to 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; 4. Far Jump to flush the pipeline and switch to 32-bit segment
    jmp CODE_SEG:start_protected_mode

; ==============================================================================
; GDT (Global Descriptor Table)
; Defines memory segments for 32-bit mode.
; ==============================================================================
gdt_start:
    dq 0x0                  ; Null Descriptor (Required)

gdt_code:                   ; Code Segment Descriptor
    dw 0xFFFF               ; Limit (0-15)
    dw 0x0                  ; Base (0-15)
    db 0x0                  ; Base (16-23)
    db 10011010b            ; Access Byte (Present, Ring 0, Code, Readable)
    db 11001111b            ; Flags (4KB blocks, 32-bit) + Limit (16-19)
    db 0x0                  ; Base (24-31)

gdt_data:                   ; Data Segment Descriptor
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b            ; Access Byte (Present, Ring 0, Data, Writable)
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size (Limit)
    dd gdt_start                ; Start Address

; Constants for segment offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; ==============================================================================
; 32-BIT PROTECTED MODE
; ==============================================================================
[bits 32]
start_protected_mode:
    ; 5. Update Segment Registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 6. Setup Stack (Top of free memory)
    mov ebp, 0x90000        ; 0x90000 is a safe place for the stack
    mov esp, ebp

    ; 7. Call the Kernel
    call kernel_main

    ; 8. Hang if kernel returns
    hlt
    jmp $