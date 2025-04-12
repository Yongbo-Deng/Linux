#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

#include "led_opr.h"

#define LED_NUM 2

/* Decide major device number. */
static int major = 0;
static struct class *led_class;
static struct led_operations *p_led_opr;
static char led_state[LED_NUM] = {1, 0};	/* LED status array */

#define MIN(a, b) (a < b ? a : b)

/* Implement open/read_write etc. functions, and completet the file_operations structure. */
static ssize_t led_drv_read(struct file *file, char __user *buf, size_t size, loff_t * offset) {
	/* Read the LED status according to devicde number. */
	char state;
	int err;
	int minor = iminor(file_inode(file));

	// Allow only a single byte to be read
	if (*offset != 0) {
		return 0;
	}

	state = led_state[minor];
	err = copy_to_user(buf, &state, 1);
	if (err) {
		printk("copy_to_user error\n");
		return -EFAULT;
	}
	*offset = 1;	
	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 1;
}
static ssize_t led_drv_write(struct file *file, const char __user *buf, size_t size, loff_t * offset) {
	int err;
	char status;
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);

	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(&status, buf, 1);

	/* Control the LED according to devicde number and status. */
	p_led_opr->ctl(minor, status);
	led_state[minor] = status;	/* Update the LED status array */

	return 1;
}
static int led_drv_open(struct inode *node, struct file *file) {
	int minor = iminor(node);

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* Initialize the kernel buffer. */
	p_led_opr->init(minor);

	/* Initialize the LED status array */
	// led_state[minor] = 0;	/* Set the initial status to off */

	return 0;
}
static int led_drv_close(struct inode *node, struct file *file) {
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* Define your owm file_operations structure. */
static struct file_operations led_drv = {
	.owner  	= THIS_MODULE,
	.open 		= led_drv_open,
	.read 		= led_drv_read,
	.write  	= led_drv_write,
	.release 	= led_drv_close,
};


/* Send the structure to the kernel. */

/* An entry function for dirver registration. */
static int __init led_init(void) {
	int err;
	int i;

	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "myled", &led_drv);		/* /dev/led */

	led_class = class_create(THIS_MODULE, "led_class");
	err = PTR_ERR(led_class);
	if (IS_ERR(led_class)) {
		unregister_chrdev(major, "myled");
		return -1;
	}

	for (i = 0; i < LED_NUM; i++) {
		device_create(led_class, NULL, MKDEV(major, i), NULL, "myled%d", i);	/* /dev/myledi */	
	}

	p_led_opr = get_board_led_opr();
	return 0;
}

/* An exit function for unistalling driver. */
static void __exit led_exit(void) {

	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_destroy(led_class, MKDEV(major, 0));
	class_destroy(led_class);
	unregister_chrdev(major, "myled");
}

/* Additional: Device Info, Auto generate device node. */
module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
