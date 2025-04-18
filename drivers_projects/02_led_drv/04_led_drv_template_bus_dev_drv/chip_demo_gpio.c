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
#include "leddrv.h"
#include "led_resource.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

static volatile unsigned int *CCM_CCGR[100];
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER[100];
static volatile unsigned int *GPIO_GDIR[100];
static volatile unsigned int *GPIO_DR[100];

static int board_demo_led_init (int which) /* 初始化LED, which-哪个LED */       
{   
    //printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
    
    printk("init gpio: group %d, pin %d\n", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));
    switch(GROUP(g_ledpins[which]))
    {
        case 0:
        {
            printk("init pin of group 0 ...\n");
            break;
        }
        case 1:
        {
            printk("init pin of group 1 ...\n");
            break;
        }
        case 2:
        {
            printk("init pin of group 2 ...\n");
            break;
        }
        case 3:
        {
            printk("init pin of group 3 ...\n");
            break;
        }
    }

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


static int board_demo_led_ctl (int which, char status) /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
{
    //printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");
    printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));

    switch(GROUP(g_ledpins[which]))
    {
        case 0:
        {
            printk("set pin of group 0 ...\n");
            break;
        }
        case 1:
        {
            printk("set pin of group 1 ...\n");
            break;
        }
        case 2:
        {
            printk("set pin of group 2 ...\n");
            break;
        }
        case 3:
        {
            printk("set pin of group 3 ...\n");
            break;
        }
    }
    if (status == 0) {
		*GPIO_DR[which] |= (1 << 3); // set GPIO5_IO3 as high level(off)
	} else {
		*GPIO_DR[which] &= ~(1 << 3); // set GPIO5_IO3 as low level(on)
	}
    
    return 0;
}

static void board_demo_led_exit(int which) 
{
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

static int chip_demo_gpio_probe(struct platform_device *pdev)
{
    struct resource *res;
    int i = 0;

    while (1)
    {
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i++);
        if (!res)
            break;
        
        g_ledpins[g_ledcnt] = res->start;
        led_class_create_device(g_ledcnt);
        g_ledcnt++;
    }
    return 0;
    
}

static int chip_demo_gpio_remove(struct platform_device *pdev)
{
    struct resource *res;
    int i = 0;

    while (1)
    {
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
        if (!res)
            break;
        
        led_class_destroy_device(i);
        i++;
        g_ledcnt--;
    }
    return 0;
}


static struct platform_driver chip_demo_gpio_driver = {
    .probe      = chip_demo_gpio_probe,
    .remove     = chip_demo_gpio_remove,
    .driver     = {
        .name   = "100ask_led",
    },
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

