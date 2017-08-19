#include <stddef.h>
#include <common/io.h>
#include <init/atags.h>

Atag *atags;
enum Model model;
const char *model_name;
u32 mem_total;
u32 initrd_start;
u32 initrd_size;

enum Tag 
{
    NONE = 0x00000000,
    CORE = 0x54410001,
    MEM = 0x54410002,
    INITRD2 = 0x54420005,
    CMDLINE = 0x54410009,
};

typedef struct Mem
{
    u32 size;
    u32 start;
} Mem;

typedef struct Initrd2
{
    u32 size;
    u32 start;
} Initrd2;

typedef struct CmdLine
{
    char line[1];
} CmdLine;

struct Atag
{
    u32 tag_size;
    u32 tag;
    union
    {
        Mem mem;
        Initrd2 initrd2;
        CmdLine cmdline;
    };
};

static const char *find(const char *str, const char *token)
{
    while(*str++)
    {
        const char *p = str;
        const char *q = token;
        
        /* token match */
        while(*p++ && *p++ == *q++)
            ;

        /* token found */
        if (*q == 0) return str;
    }

    /* end of string and nothing found */
    return NULL;
}

static const Atag *next(const Atag *atag)
{
    if (atag->tag == NONE)
        return NULL;

    return (const Atag *)(((u32 *) atag) + atag->tag_size);
}

void resume_atags(void)
{
    const char *cmdline = "";

    const Atag *atag = atags;

    while (atag)
    {
        switch(atag->tag)
        {
            case MEM:
            {
                mem_total = atag->mem.size;
                break;
            }
            case INITRD2:
            {
                initrd_start = atag->initrd2.start;
                initrd_size = atag->initrd2.size;
                break;
            }
            case CMDLINE:
            {
                //TODO: This is a good place to define peripheral_base etc...
                cmdline = atag->cmdline.line;
                if (find(cmdline, "bcm2708.disk_led_gpio=47")) 
                {
                    model = RPI_B_PLUS;
                    model_name = "Raspberri Pi B+";
                }
                else if(find(cmdline, "bcm2709.disk_led_gpio=47"))
                {
                    model = RPI_2;
                    model_name = "Raspberri Pi 2";
                }
                else
                {
                    model = RPI;
                    model_name = "Raspberry Pi";
                }
                break;
            }
            default:
            {
            }
        }
        atag = next(atag);
    }

    printk("\nDetected '%s'\n", model_name);
    printk("Memory      : 0x%08x\n", mem_total);
    printk("Initrd start: 0x%08x\n", initrd_start);
    printk("Initrd size : 0x%08x\n", initrd_size);
    printk("Commandline : '%s'\n", cmdline);
}
