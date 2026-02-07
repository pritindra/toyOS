[bits 32]

global idt_load
global isr0

extern isr_handler

; void idt_load(idtr_t* ptr);
idt_load:
    mov eax, [esp + 4]  ; the pointer passed as argument
    lidt [eax]          ; Load the Interrupt Descriptor Table Register
    ret

; ------------------------------------------------------------------------------
; ISR 0: Divide by Zero Exception
; ------------------------------------------------------------------------------
isr0:
    cli                 ; Disable interrupts
    push 0              ; Push dummy error code (ISR 0 doesn't generate one)
    push 0              ; Push Interrupt Number (0)
    jmp isr_common_stub ; Jump to common handler

; ------------------------------------------------------------------------------
; ISR 32: Timer Interrupt (IRQ 0)
; ------------------------------------------------------------------------------
global isr32
isr32:
    cli
    push 0              ; Dummy error code
    push 32             ; Interrupt Number
    jmp isr_common_stub

    
; ------------------------------------------------------------------------------
; ISR 33: Keyboard Interrupt (IRQ 1)
; ------------------------------------------------------------------------------
global isr33
isr33:
    cli
    push 0              ; Dummy error code
    push 33             ; Interrupt Number
    jmp isr_common_stub ; Jump to the existing C handler logic


; ------------------------------------------------------------------------------
; Common Stub: Saves state and calls C
; ------------------------------------------------------------------------------
isr_common_stub:
    pusha               ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds          ; Save the Data Segment descriptor
    push eax

    mov ax, 0x10        ; Load the Kernel Data Segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler    ; Call the C code!

    pop eax             ; Restore original Data Segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; Restore general purpose registers
    add esp, 8          ; Clean up the pushed error code and ISR number
    sti                 ; Re-enable interrupts
    iret                ; Return from interrupt (pops CS, EIP, EFLAGS)  