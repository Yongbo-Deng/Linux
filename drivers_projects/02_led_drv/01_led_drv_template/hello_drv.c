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

/* Decide major device number. */
static int major = 0;
static char kernel_buf[1024];
static struct class *hello_class;


#define MIN(a, b) (a < b ? a : b)

/* Implement open/read_write etc. functions, and completet the file_operations structure. */
static ssize_t hello_drv_read(struct file *file, char __user *buf, size_t size, loff_t * offset) {
	int err;
	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_to_user(buf, kernel_buf, MIN(1024, size));
	return MIN(1024, size);
}
static ssize_t hello_drv_write(struct file *file, const char __user *buf, size_t size, loff_t * offset) {
	int err;
	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(kernel_buf, buf, MIN(1024, size));
	return MIN(1024, size);
}
static int hello_drv_open(struct inode *node, struct file *file) {
	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}
static int hello_drv_close(struct inode *node, struct file *file) {
	printk ("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}


/* Define your owm file_operations structure. */
static struct file_operations hello_drv = {
	.owner  	= THIS_MODULE,
	.open 		= hello_drv_open,
	.read 		= hello_drv_read,
	.write  	= hello_drv_write,
	.release 	= hello_drv_close,
};


/* Send the structure to the kernel. */

/* An entry function for dirver registration. */
static int __init hello_init(void) {
	int err;
	major = register_chrdev(0, "hello", &hello_drv);		/* /dev/hello */

	hello_class = class_create(THIS_MODULE, "hello_class");
	err = PTR_ERR(hello_class);
	if (IS_ERR(hello_class)) {
		unregister_chrdev(major, "hello");
		return -1;
	}

	device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello");	

	return 0;
}

/* An exit function for unistalling driver. */
static void __exit hello_exit(void) {
	device_destroy(hello_class, MKDEV(major, 0));
	class_destroy(hello_class);
	unregister_chrdev(major, "hello");
}

/* Additional: Device Info, Auto generate device node. */
module_init(hello_init);

module_exit(hello_exit);

MODULE_LICENSE("GPL");
