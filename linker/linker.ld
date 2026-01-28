/* linker.ld */
ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    /* We load the kernel at 0x1000 */
    . = 0x1000;

    /* Code section */
    .text : ALIGN(4K)
    {
        *(.text)
    }

    /* Read-only data */
    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Data (initialized) */
    .data : ALIGN(4K)
    {
        *(.data)
    }

    /* BSS (uninitialized variables) */
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}