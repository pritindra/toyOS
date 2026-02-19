[bits 32]

global switch_task

switch_task:
    ;save state
    push ebp
    push edi
    push esi
    push ebx
    
    mov eax, [esp + 20]     ; first arg
    mov [eax], esp

    mov esp, [esp + 24]     ; second arg

    ; restore
    pop ebx
    pop esi
    pop edi
    pop ebp

    ret