;legacy mode BIOS

org 0x7C00      ;directive tells the assembler where to find the code
bits 16         ; emit 16 bit code, because x86 os backward compatible

%define ENDL 0x0D, 0x0A     ;nasm macro to define newline (line feed and character)


start:
    jmp main    ; main is the entry to the program



;function to print a string on the screen
;params : ds:si points to a string

puts:
    ;save registers that are modified
    push si
    push ax
    push bx

.loop:
    lodsb       ;loads from ds:si reg then ++ si by no. of bytes loaded (next char in al)
    or al, al   ;verify if next char is null? modifies zero flag
    jz .done

    mov ah, 0x0e    ;calls bios interrupt, video interrupt, then 
    mov bh, 0       ; page no. set to 0
    int 0x10        
     
    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret

main:
    ; setup data segments
    mov ax, 0      ;cannot write to ds/es segments easily
    mov ds, ax
    mov es, ax

    ;setup stack
    mov ss, ax
    mov sp, 0x7C00  ; stack grows downwards, os as the start point

    mov si, msg_hello
    call puts

    hlt         ; halt instr

.halt:
    jmp .halt   ;if cpu restart then loop

msg_hello: db "Damn it works!!" , ENDL , 0

times 510-($-$$) db 0    ;repeats 510 times, ($-$$) gives the size of the program, define bytes(db)

dw 0AA55h       ;dw define the end two bytes for BIOS

