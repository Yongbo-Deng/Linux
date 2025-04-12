#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <asm/io.h>
#include "led_opr.h"

static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR; 

static int board_demo_led_init(int which)
{
    unsigned int val;

    if (which == 0) {
        if (!CCM_CCGR1) {
            CCM_CCGR1 = ioremap(0x20C406C, 4);
            IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014, 4);
            GPIO5_GDIR = ioremap(0x020AC000 + 0x4, 4);
            GPIO5_DR = ioremap(0x020AC000 + 0, 4);
        }

        /* a. enable GPIO5.       */
        *CCM_CCGR1 |= (3 << 30);

        /* b. set GPIO5_IO03 as GPIO.   */
        val = *IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3;
        val &= ~(0xf);
        val |= (0x5);
        *IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3 = val;
        
        /* b. set GPIO_IO3 as output pin. */
        *GPIO5_GDIR |= (1 << 3);
    }

    return 0;
}

static int board_demo_led_ctl(int which, char status)
{
    if (which == 0) {
        /* c. configure GPIO5_IO3 as high level */
        if (status == 1) {
            *GPIO5_DR &= ~(1 << 3);
        } else {
            *GPIO5_DR |= (1 << 3);
        }
    }
    return 0;
}

static void board_demo_led_exit(int which)
{
    iounmap(CCM_CCGR1);
    CCM_CCGR1 = NULL;
    iounmap(IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3);
    IOMUXC_SW_MUX_CTL_PAD_SNVS_TAMPER3 = NULL;
    iounmap(GPIO5_GDIR);
    GPIO5_GDIR = NULL;
    iounmap(GPIO5_DR);
    GPIO5_DR = NULL;
}

static struct led_operations board_demo_led_opr = {
    .num = 2,
    .init = board_demo_led_init,
    .ctl = board_demo_led_ctl,
    .exit = board_demo_led_exit,
};

struct led_operations *get_board_led_opr(void)
{
    return &board_demo_led_opr;
}

int led_getNum(void)
{
    return board_demo_led_opr.num;
}
