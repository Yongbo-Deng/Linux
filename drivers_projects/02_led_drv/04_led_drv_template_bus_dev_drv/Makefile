KERN_DIR = ~/100ask_imx6ull-sdk/Linux-4.9.88

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o ledtest ledtest.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f ledtest

# ab-y := a.o b.o
# obj-m += ab.o

obj-m += leddrv.o chip_demo_gpio.o board_A_led.o

