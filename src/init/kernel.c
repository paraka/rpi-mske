#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <common/types.h>
#include <common/peripherals.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main(u32 r0, u32 r1, u32 atags)
{
	(void) r0;
	(void) r1;
	(void) atags;
}
