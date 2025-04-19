#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/signal.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/capi.h>
#include <linux/kernelcapi.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/moduleparam.h>

#include "GPIO_drv.h"

static int major = 0;

static struct GPIO_operations *p_GPIO_opr;
static struct class *GPIO_class;

static int GPIO_open (struct inode *inode, struct file *file)
{
    int minor = iminor(inode);
    p_GPIO_opr->init(minor);
    return 0;
}

static ssize_t GPIO_read (struct file *file, char __user *buf, size_t size, loff_t *off)
{
    unsigned int minor = iminor(file_inode(file));
    char level;
    int err;
    
    level = p_GPIO_opr->read(minor);
    err = copy_to_user(buf, &level, 1);
    return 1;
}


static struct file_operations GPIO_fops = {
    .open = GPIO_open,
    .read = GPIO_read,
};

void register_GPIO_operations(struct GPIO_operations *opr)
{
    int i;

    p_GPIO_opr = opr;
    for (i = 0; i < opr->count; i++)
    {
        device_create(GPIO_class, NULL, MKDEV(major, i), NULL, "ebean_GPIO%d", i);
    }
}

void unregister_GPIO_operations(void)
{
    int i;

    for (i = 0; i < p_GPIO_opr->count; i++)
    {
        device_destroy(GPIO_class, MKDEV(major, i));
    }
}


EXPORT_SYMBOL(register_GPIO_operations);
EXPORT_SYMBOL(unregister_GPIO_operations);


int GPIO_init(void)
{
    major = register_chrdev(0, "ebean_GPIO", &GPIO_fops);

    GPIO_class = class_create(THIS_MODULE, "ebean_GPIO");
    if (IS_ERR(GPIO_class))
        return -1;
    
    return 0;
}

void GPIO_exit(void)
{
    class_destroy(GPIO_class);
    unregister_chrdev(major, "ebean_GPIO");
}

module_init(GPIO_init);
module_exit(GPIO_exit);
MODULE_LICENSE("GPL");


