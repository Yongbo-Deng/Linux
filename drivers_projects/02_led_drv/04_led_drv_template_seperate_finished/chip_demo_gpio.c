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
#include <linux/platform_device.h>
#include <asm/io.h>

#include "led_opr.h"
#include "ledDrv.h"
#include "led_resource.h"

static int g_led_pins[100];
static int g_led_cnt = 0;

static volatile unsigned int *CCM_CCGR[100];
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[100];
static volatile unsigned int *GPIO_GDIR[100];
static volatile unsigned int *GPIO_DR[100]; 


static int board_demo_led_init (int which) 
{	
	printk("init gpio: group %d, pin %d\n", GROUP(g_led_pins[which]), PIN(g_led_pins[which]));
	printk("init pin of group %d ...\n", GROUP(g_led_pins[which]));

	if (!CCM_CCGR[which]) {
		CCM_CCGR[which] = ioremap(0x20C406C, 4);	// bind to CCM_CCGR1
		*CCM_CCGR[which] |= (3 << 30); // enable GPIO5
	}

	if (!IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[which]) {
		int val;
		IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[which] = ioremap(0x2290014, 4);	// bind to IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3
		val = *IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[which];
		val &= ~(0xf);
		val |= (0x5); 
		*IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[which] = val; // set GPIO5_IO03 as GPIO
	}
	if (!GPIO_GDIR[which]) {
		GPIO_GDIR[which] = ioremap(0x020AC000 + 0x4, 4);
		*GPIO_GDIR[which] |= (1 << 3); // set GPIO_IO3 as output pin
	}
	if (!GPIO_DR[which]) {
		GPIO_DR[which] = ioremap(0x020AC000 + 0, 4);
		*GPIO_DR[which] |= (1 << 3); // configure GPIO5_IO3 as high level(off)
	}

	return 0;
}

static int board_demo_led_ctl (int which, char status) 
{
	printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GROUP(g_led_pins[which]), PIN(g_led_pins[which]));

	if (status == 1) {
		*GPIO_DR[which] &= ~(1 << 3); // set GPIO5_IO3 as low level(on)
	} else {
		*GPIO_DR[which] |= (1 << 3); // set GPIO5_IO3 as high level(off)
	}

	return 0;
}

static void board_demo_led_exit(int which) 
{
	printk("exit gpio: group %d, pin %d\n", GROUP(g_led_pins[which]), PIN(g_led_pins[which]));
	iounmap(CCM_CCGR[which]);
	CCM_CCGR[which] = NULL;
	iounmap(IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[which]);
	IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[which] = NULL;
	iounmap(GPIO_GDIR[which]);
	GPIO_GDIR[which] = NULL;
	iounmap(GPIO_DR[which]);
	GPIO_DR[which] = NULL;
}

static struct led_operations board_demo_led_opr = {
	.init = board_demo_led_init,
	.ctl  = board_demo_led_ctl,
	.exit = board_demo_led_exit,
};

struct led_operations *get_board_led_opr(void)
{
	return &board_demo_led_opr;
}

static int chip_demo_gpio_probe(struct platform_device *dev)
{
	int i = 0;
	struct resource *res;
	while (1) {
		/* Record pin. */
		res = platform_get_resource(dev, IORESOURCE_IRQ, i++);
		if (!res) {
			break;
		}
		g_led_pins[g_led_cnt] = res->start;

		/* Create device */
		led_device_create(g_led_cnt);
		g_led_cnt++;
	}
	return 0;
}

static int chip_demo_gpio_remove(struct platform_device *dev)
{
	int i = 0;
	for (i = 0; i < g_led_cnt; i++) {
		led_device_destroy(i);
	}
	g_led_cnt = 0;
	return 0;
}


static struct platform_driver chip_demo_gpio_driver = {
	.probe = chip_demo_gpio_probe,
	.remove = chip_demo_gpio_remove,
	.driver = {
		.name = "myled",
	}
};

static int __init chip_demo_gpio_drv_init(void)
{
	int err;

	err = platform_driver_register(&chip_demo_gpio_driver);
	register_led_operations(&board_demo_led_opr);

	return 0;
}

static void __exit chip_demo_gpio_drv_exit(void)
{
	platform_driver_unregister(&chip_demo_gpio_driver);
}

module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");

