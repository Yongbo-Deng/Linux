#ifndef GPIO_DRV_H
#define GPIO_DRV_H

struct GPIO_operations {
    int count;
    void (*init) (int which);
    int (*ctl) (int which, char status);
    int (*read) (int which);
};

void register_GPIO_operations(struct GPIO_operations *opr);
void unregister_GPIO_operations(void);
struct led_operations *get_board_led_opr(void);

#endif

