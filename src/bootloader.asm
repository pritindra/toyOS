; ==============================================================================
; BOOTLOADER.ASM - Reads kernel from disk to memory
; ==============================================================================
org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

; Header for BIOS Parameter Block (BPB)
; This is needed so BIOS knows the disk geometry (standard 1.44MB Floppy)
jmp short start
nop

bdb_oem:                    db 'MSWIN4.1'   ; 8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 1
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880         ; 2880 * 512 = 1.44MB
bdb_media_descriptor_type:  db 0F0h         ; F0 = 3.5" floppy
bdb_sectors_per_fat:        dw 9
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
bdb_large_sector_count:     dd 0

; Extended Boot Record
ebr_drive_number:           db 0            ; 0x00 usually floppy, 0x80 usually hdd
                            db 0            ; reserved
ebr_signature:              db 29h
ebr_volume_id:              db 12h, 34h, 56h, 78h
ebr_volume_label:           db 'MY OS      ' ; 11 bytes
ebr_system_id:              db 'FAT12   '    ; 8 bytes

; ==============================================================================
; CODE ENTRY
; ==============================================================================
start:
    jmp main

; ------------------------------------------------------------------------------
; Function: puts
; Prints a null-terminated string to the screen
; Params: ds:si points to string
; ------------------------------------------------------------------------------
puts:
    push si
    push ax
    push bx
.loop:
    lodsb               ; load byte from ds:si into al, increment si
    or al, al           ; check if al is 0 (null terminator)
    jz .done
    mov ah, 0x0e        ; tty output
    mov bh, 0           ; page 0
    int 0x10
    jmp .loop
.done:
    pop bx
    pop ax
    pop si
    ret

; ------------------------------------------------------------------------------
; Function: lba_to_chs
; Converts Logical Block Address (LBA) to Cylinder-Head-Sector (CHS)
; Formula:
;   Sector   = (LBA % sectors_per_track) + 1
;   Head     = (LBA / sectors_per_track) % heads
;   Cylinder = (LBA / sectors_per_track) / heads
; Params: ax = LBA address
; Returns: cx [bits 0-5]: sector number
;          cx [bits 6-15]: cylinder
;          dh: head
; ------------------------------------------------------------------------------
lba_to_chs:
    push ax
    push dx

    xor dx, dx                          ; dx = 0
    div word [bdb_sectors_per_track]    ; ax = LBA / SectorsPerTrack
                                        ; dx = LBA % SectorsPerTrack

    inc dx                              ; Sector = (LBA % SectorsPerTrack) + 1
    mov cx, dx                          ; cx = Sector

    xor dx, dx                          ; dx = 0
    div word [bdb_heads]                ; ax = (LBA / SectorsPerTrack) / Heads = Cylinder
                                        ; dx = (LBA / SectorsPerTrack) % Heads = Head
    mov dh, dl                          ; dh = Head
    mov ch, al                          ; ch = Cylinder (lower 8 bits)
    shl ah, 6
    or cl, ah                           ; put upper 2 bits of cylinder in CL

    pop ax
    mov dl, al                          ; restore DL
    pop ax
    ret

; ------------------------------------------------------------------------------
; Function: disk_read
; Reads sectors from disk into memory
; Params: ax = LBA address
;         cl = number of sectors to read
;         dl = drive number
;         es:bx = memory location to store data
; ------------------------------------------------------------------------------
disk_read:
    push ax
    push bx
    push cx
    push dx
    push di

    push cx             ; temporarily save CL (number of sectors to read)
    call lba_to_chs     ; convert LBA to CHS
    pop ax              ; restore number of sectors to AL

    mov ah, 0x02        ; BIOS int 13h "Read Sectors"
    mov di, 3           ; retry count (floppies are unreliable, try 3 times)

.retry:
    pusha              ; save registers, int 13h might mess them up
    stc                 ; set carry flag
    int 0x13            ; Call BIOS
    jnc .done           ; jump if no carry (success)

    ; if failed, reset disk controller and try again
    popa
    call disk_reset
    dec di
    test di, di
    jnz .retry

.fail:
    ; panic here
    jmp floppy_error

.done:
    popa
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret

disk_reset:
    pusha
    mov ah, 0
    stc
    int 0x13
    jc floppy_error
    popa
    ret

; ------------------------------------------------------------------------------
; Global Error Handler
; ------------------------------------------------------------------------------
floppy_error:
    mov si, msg_read_failed
    call puts
    hlt
    jmp floppy_error


; ------------------------------------------------------------------------------
; MAIN
; ------------------------------------------------------------------------------
main:
    ; setup data segments
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Print greeting
    mov si, msg_hello
    call puts

    ; 1. Setup where to write the data (ES:BX)
    ; We will load the kernel to address 0x1000 (ES=0, BX=0x1000)
    mov ax, 0           ; Segment 0
    mov es, ax
    mov bx, 0x1000      ; Offset 0x1000 (This is where C kernel will land)

    ; 2. Call disk_read
    mov ax, 1           ; LBA=1 (start reading from 2nd sector, 1st is bootloader)
    mov cl, 1           ; Read 1 sector
    mov dl, [ebr_drive_number] ; Drive number (passed by BIOS in DL, saved in BPB)
    call disk_read

    ; Print success
    mov si, msg_loaded
    call puts

    ; 3. Jump to the new code!
    ; In the future, you will uncomment this line:
    ; jmp 0x0000:0x1000

    hlt

.halt:
    jmp .halt

; Strings
msg_hello:          db "Booting OS...", ENDL, 0
msg_read_failed:    db "Disk read failed!", ENDL, 0
msg_loaded:         db "Sector loaded to 0x1000", ENDL, 0

times 510-($-$$) db 0
dw 0AA55h