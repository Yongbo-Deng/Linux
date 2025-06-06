#ifndef __LED_RESOURCE_H__
#define __LED_RESOURCE_H__

/* GPIO3_0 
 * bit[31:16] = group
 * bit[15:0]  = which pin
 */

#define GROUP(x) ((x) >> 16)
#define PIN(x) ((x) & 0xFFFF)
#define GROUP_PIN(g, p) ((g) << 16 | (p))

struct led_resource {
    int pin;
};

struct led_resource *get_led_resource(void);

#endif