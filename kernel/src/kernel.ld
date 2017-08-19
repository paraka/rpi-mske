ENTRY(_start)
 
SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x8000;
    __start = .;
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
    __text_end = .;
 
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
    __rodata_end = .;
 
    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
    __data_end = .;
 
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;

    . = . + 0x1000;
    __svc_stack_top = .; /* 4KB of SVC stack space */

    . = . + 0x1000;
    __irq_stack_top = .; /* 4KB of IRQ stack space */

    . = . + 0x1000;
    __abt_stack_top = .; /* 4KB of ABORT stack space */

    . = . + 0x1000;
    __und_stack_top = .; /* 4KB of UNDEFINED stack space */

    __end = .;
}
