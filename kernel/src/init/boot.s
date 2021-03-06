.section ".text.boot"

.global _start, vectors_start, vectors_end
.global proc, TASK_SIZE, scheduler, running
.global tswitch

// See ARM section A2.2 (Processor Modes)
.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

_start:

vectors_start:
ldr pc, reset_handler_addr
ldr pc, undef_handler_addr
ldr pc, swi_handler_addr
ldr pc, prefetch_abort_handler_addr
ldr pc, data_abort_handler_addr
ldr pc, unused_handler_addr
ldr pc, irq_handler_addr
ldr pc, fiq_handler_addr
reset_handler_addr:             .word handler_reset
undef_handler_addr:             .word stub_undef
swi_handler_addr:               .word stub_svc
prefetch_abort_handler_addr:    .word stub_prefetch_abort
data_abort_handler_addr:        .word stub_data_abort
unused_handler_addr:            .word handler_reset
irq_handler_addr:               .word stub_irq
fiq_handler_addr:               .word stub_fiq
vectors_end:

// Entry point for the kernel.
// r15 -> should begin execution at 0x8000.
// r0 -> 0x00000000
// r1 -> 0x00000C42
// r2 -> 0x00000100 - start of ATAGS

handler_reset:
    // set SVC stack to first process high end
    ldr r0, =proc
    ldr r1, =TASK_SIZE
    ldr r2, [r1, #0]
    add r0, r0, r2
    mov sp, r0

    // copy exception table to 0x0
    bl copy_vector_table

    // set ABT mode stack
    mov r0, #(CPSR_MODE_ABORT | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0  // ABT mode
    ldr sp, =__abt_stack_top

    // set UNDEF mode stack
    mov r0, #(CPSR_MODE_UNDEFINED | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0  // UND mode
    ldr sp, =__und_stack_top

    // set IRQ mode stack
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0         // IRQ mode
    ldr sp, =__irq_stack_top

    // back to SVC mode
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0         // go back to SVC mode

    // Clear out bss.
    bl clear_bss

    // store atags in global variable
    .extern atags
    ldr r2, =0x00000100 // start of atags in this address
    ldr r3, =atags
    str r2, [r3]
 
    // Call kernel_main
    ldr r3, =kernel_main
    blx r3

_halt:
    wfe // wait for events
    b _halt

tswitch:
    stmfd sp!, {r0-r12, lr}
    ldr r0, =running
    ldr r1, [r0, #0]
    str sp, [r1, #4] //running->ksp = sp
    bl scheduler
    ldr r0, =running
    ldr r1, [r0, #0]
    ldr sp, [r1, #4]
    ldmfd sp!, {r0-r12, pc}

// save all registers and SPSR on the SVC stack, (LR corrected by offset)
.macro saveall, num, offset
	// corrcet lr
	sub     lr, #\offset
    // save SPSR and LR onto the SVC stack
    srsdb   #0x13!
    // switch to SVC mode, interrupts disabled
    cpsid   i, #0x13
    // save user space SP/LR
    sub     sp, #8
    stmia   sp, {sp, lr}^
    // save all registers on the stack (+ LR for alignment)
    push    {r0-r12, lr}
    // move pointer to registers into first argument
    mov     r0, sp
	// move number for exception into second argument
	mov	    r1, #\num

	// align stack to 8 byte and save original SP
	// 8 byte alignment is required for ldrd but the exception may
	// have happened while the stack was only 4 byte aligned.

	// align stack to the next lower multiple of 8
	and	    sp, sp, #~7
	// push saved SP and dummy register so it again is 8 byte alinged
	push	{r0, r1}
.endm

// restore all registes and return from exception
.macro restoreall
	// restore saved unaligned stack
	ldr	sp, [sp]

    // restore all regs from stack (+ LR for alignment)
    pop     {r0-r12, lr}

    // restore user space SP/LR and skip dummy
    ldmia   sp, {sp, lr}^
    add     sp, #8

    // return to whence we came from
    rfeia   sp!
.endm

.macro	make, num, offset, name
	.global	stub_\name
stub_\name:
	saveall	\num, \offset
	bl	handler_\name
	restoreall
.endm

// num, offset, name
make 1, 4, undef
make 2, 0, svc
make 3, 4, prefetch_abort
make 4, 8, data_abort
make 6, 4, irq
make 7, 4, fiq
