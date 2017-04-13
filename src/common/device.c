#include "device.h"

static int num_devices = 0;

device_table_entry_s device_table[MAX_DEVICES];

u8 device_register(struct dev_operations *dev_ops)
{
    u8 dev_id;

    if (num_devices >= MAX_DEVICES)
        return MSKE_ERR;
    
    int i = 0;
    
    /* look for position in table */
    while (device_table[i].valid) ++i;

    dev_id = i;

    if (dev_ops->init() != MSKE_SUCESS)
        return MSKE_ERR_DEV_INIT;

    device_table[i].valid = true;
    device_table[i].dev_ops = dev_ops;

    num_devices++;

    return dev_id;
}

mske_ret_code_t device_read(u8 dev_id, void *buffer, size_t nbytes)
{
    if (device_table[dev_id].valid)
        return device_table[dev_id].dev_ops->read(dev_id, buffer, nbytes);

    return MSKE_ERR_INVALID_DEV;
}

mske_ret_code_t device_write(u8 dev_id, void *buffer, size_t nbytes)
{
    if (device_table[dev_id].valid)
        return device_table[dev_id].dev_ops->write(dev_id, buffer, nbytes);

    return MSKE_ERR_INVALID_DEV;
}

mske_ret_code_t device_init(void)
{
    int i = 0;
    
    num_devices = 0;

    for (i=0; i<MAX_DEVICES; ++i)
        device_table[i].valid = false;

    return MSKE_SUCESS;
}

mske_ret_code_t device_destroy(u8 dev_id)
{
    if (device_table[dev_id].valid)
        return device_table[dev_id].dev_ops->destroy(dev_id);

    return MSKE_ERR_INVALID_DEV;
}
