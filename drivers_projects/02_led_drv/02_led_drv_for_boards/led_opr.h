#ifndef LED_OPR_H
#define LED_OPR_H

struct led_operations {
    int num; 
    int (*init) (int which);     
    int (*ctl) (int which, char status);
};

struct led_operations *get_board_led_opr(void);

int led_getNum(void);


#endif