#ifndef _BUTTON_DRV_H
#define _BUTTON_DRV_H

struct GPIO_operations {
    int count;
    void (*init) (int which);
    int (*read) (int which);
};

void register_button_operations(struct GPIO_operations *opr);
void unregister_button_operations(void);

#endif

