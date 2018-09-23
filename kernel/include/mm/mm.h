#ifndef MM_H_
#define MM_H_

#define KERNEL_BASE 0x80000000

#ifndef __ASSEMBLER__

#include <types.h>

#define SECTION_INDEX(x)    ((u32)x >> 20)
#define PAGE_INDEX(x)       (((u32)x >> 12) & 0xFF)
#define PTABLE_TO_BASE(x)   ((u32)x >> 10)
#define BASE_TO_PTABLE(x)   ((void *)((u32)x << 10))
#define PAGE_TO_BASE(x)     ((u32)x >> 12)

#define PHYS_TO_VIRT(x)     (((u32)x) + KERNEL_BASE)
#define VIRT_TO_PHYS(x)     (((u32)x) - KERNEL_BASE)

#define ROUND_DOWN(x,align) ((x) & ~(align - 1))
#define ROUND_UP(x,align)   (((x) + align - 1) & ~(align - 1))

extern char kernel_end[];

#define KERNEL_STACK        (KERNEL_BASE - (2 * PAGE_SIZE))
#define USER_STACK          (KERNEL_BASE - (3 * PAGE_SIZE))
#define KERNEL_SECT_TABLE   ((u32)kernel_end)
#define MMIO_BASE           (KERNEL_BASE + 0x40000000)
#define MMIO_BASE_PHYSICAL  0x20000000
#define ALLOC_MEM_START     (KERNEL_SECT_TABLE + 0x200000)
#define IRQ_VECTOR_BASE     0xFFFF0000
#define MMIO_P2V(x)         ((volatile u32 *) (MMIO_BASE + (x - MMIO_BASE_PHYSICAL)))

#define PAGES_PER_SECTION   1024
#define SECTION_COUNT       4096
#define PAGE_SIZE           4096
#define SECTION_SIZE        (PAGE_SIZE * PAGES_PER_SECTION)
#define INITIAL_MEMORY_SIZE 0x2000000 /* 32MB  */
#define PAGE_TABLE_ALIGN    1024
#define SECTION_TABLE_ALIGN 16384
#define PAGE_TABLE_SIZE     1024
#define SECTION_TABLE_SIZE  16384

#define PAGE_DESCRIPTOR     0x2 /* binary 10 */
#define SECTION_DESCRIPTOR  0x1 /* binary 01 */

/* access permissions */
#define AP_RW_D             0x55
#define AP_RW_R             0xAA
#define AP_RW_RW            0xFF

struct section_table_entry
{
    unsigned int desc_type : 2;
    unsigned int : 3;
    unsigned int domain : 4;
    unsigned int : 1;
    unsigned int base_address : 22;
};

struct page_table_entry
{
    unsigned int desc_type : 2;
    unsigned int bufferable : 1;
    unsigned int cacheable : 1;
    unsigned int access_perms : 8;
    unsigned int base_address : 20;
};

struct mm
{
    u32 virtual_address;
    u32 physical_start;
    u32 physical_end;
    int access_perms;
};

extern struct section_table_entry *kernel_vm;

extern void vm_init(void);
extern void vm_kernel_init(struct section_table_entry *kernel_vm);
extern void map_pages(struct section_table_entry *vm, struct mm mapping);
extern void vm_page_tables_free(struct section_table_entry *vm);
extern void unmap_page(struct section_table_entry *vm, u32 virtual_address);
extern u32 get_physical_address(struct section_table_entry *vm, u32 virtual_address);

#endif /* __ASSEMBLER__ */

#endif /* MM_H_ */
