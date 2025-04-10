
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <asm/io.h>

/* Entrance function */
static int __init led_init(void)
{
    return 0;
}
static void __exit led_exit(void)
{
    return;
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("DUAL BSD/GPL");