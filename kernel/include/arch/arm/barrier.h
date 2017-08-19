#ifndef _BARRIER_H_
#define _BARRIER_H_

/* 
 * Instruction Synchronization Barrier / Flush Prefetch Buffer
 *
 * The Flush Prefetch Buffer operation flushes the pipeline in the processor,
 * so that all instructions following the pipeline flush are fetched from
 * memory, including the cache, after the instruction has been
 * completed.
 */
static inline void isb(void)
{
    // ARMv7
    // asm volatile ("isb");
    asm volatile ("mcr p15, 0, r12, c7, c5, 4");
}

/* 
 * Data Synchronization Barrier
 *
 * This memory barrier completes when all explicit memory transactions
 * occurring in program order before this instruction are completed. No
 * explicit memory transactions occurring in program order after this
 * instruction are started until this instruction completes. In fact, no
 * instructions occurring in program order after the Data Synchronization
 * Barrier complete, or change the interrupt masks, until this instruction
 * completes.
 */
static inline void dsb(void) 
{
    // ARMv7
    // asm volatile ("dsb");
    asm volatile ("mcr p15, 0, r12, c7, c10, 4");
}

/* 
 * Data Memory Barrier
 *
 * This memory barrier ensures that all explicit memory transactions occurring
 * in program order before this instruction are completed. No explicit memory
 * transactions occuring in program order after this instrcution are started
 * until this instruction completes. Other instructions can complete out of
 * order with the Data Memory Barrier instruction.
 */
static inline void dmb(void) 
{
    // ARMv7
    // asm volatile ("dmb");
    asm volatile ("mcr p15, 0, r12, c7, c10, 5");
}

static inline void barrier(void)
{
    dsb();
    isb();
}

#endif /* _BARRIER_H_ */
