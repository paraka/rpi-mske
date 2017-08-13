#ifndef _DEVICE_H_
#define _DEVICE_H_

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

#include <types.h>

#define MAX_DEVICES 2

typedef enum _ret_codes_t
{
    MSKE_SUCESS             =  0,

    MSKE_ERR_OPEN           = -1,
    MSKE_ERR_READ           = -2,
    MSKE_ERR_WRITE          = -3,
    MSKE_ERR_DEV_INIT       = -4,
    MSKE_ERR_INVALID_DEV    = -5,
    MSKE_ERR_CLOSE          = -6,

    MSKE_NOT_IMPLEMENTED    = -100,
    MSKE_ERR                = -101
} mske_ret_code_t;

typedef enum _dev_type_t
{
    MSKE_TYPE_SERIAL = 0,
    MSKE_TYPE_SPI
} mske_dev_type_t;

typedef enum _mode_t
{
    MSKE_READ   = 1,
    MSKE_WRITE  = 2,
    MSKE_READWRITE  = MSKE_READ | MSKE_WRITE
} mske_mode_t;

struct dev_operations
{
    mske_ret_code_t (*read)(u8 dev_id, void *buffer, size_t nbytes);
    mske_ret_code_t (*write)(u8 dev_id, void *buffer, size_t nbytes);
    mske_ret_code_t (*init)(void);
    mske_ret_code_t (*destroy)(u8 dev_id);
};

typedef struct _device_table_entry
{
    struct dev_operations *dev_ops;
    bool valid;
} device_table_entry_s;

extern device_table_entry_s device_table[MAX_DEVICES];

int device_register(struct dev_operations *dev_ops);
mske_ret_code_t device_read(u8 dev_id, void *buffer, size_t nbytes);
mske_ret_code_t device_write(u8 dev_id, void *buffer, size_t nbytes);
mske_ret_code_t device_init(void);
mske_ret_code_t device_destroy(u8 dev_id);

/* all posible operations */
extern struct dev_operations device_operations;

#endif /* _DEVICE_H_ */
