
#include <linux/input.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>

int fd;

void my_sig_handler(int sig)
{
	struct input_event event;
	while (read(fd, &event, sizeof(event)) == sizeof(event))
	{
		printf("get event from device: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
	}
}

/* 
Usage: ./05_input_read_fsync /dev/input/event0 noblock
This C program is designed to read input events from a Linux input device (e.g., /dev/input/eventX). 
It utilizes asynchronous I/O notification and polling mechanisms to handle input events. 
The key functionalities include:
1. Opening an input device (open())
2. Retrieving device information (ioctl())
3. Registering a signal handler for event processing (signal(SIGIO, my_sig_handler))
4. Configuring asynchronous notification (fcntl() with F_SETOWN and F_SETFL)
5. Using a main loop to keep the process alive
*/
int main(int argc, char **argv)
{
	int err;
	int len, ret, flags, count;
	int i;
	unsigned char byte;
	int bit;
	struct input_id id;
	unsigned int evbit[2];
	struct input_event event;
	nfds_t nfds = argc - 1;
	struct pollfd fds;

	char *ev_names[] = {
		"EV_SYN ",
		"EV_KEY ",
		"EV_REL ",
		"EV_ABS ",
		"EV_MSC ",
		"EV_SW	",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"EV_LED ",
		"EV_SND ",
		"NULL ",
		"EV_REP ",
		"EV_FF	",
		"EV_PWR ",
	};

	/*Register signal function*/
	signal(SIGIO, my_sig_handler);

	/*Open driver program*/
	fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if (fd < 0)
	{
		printf("open %s error\n", argv[1]);
		return -1;
	}

	if (argc < 2)
	{
		printf("Usage: %s <dev1> \n", argv[0]);
		return -1;
	}
	
	for (int i = 0; i < nfds; i++)
	{
		fd = open(argv[1], O_RDWR | O_NONBLOCK);
		if (fd < 0)
		{
			printf("open %s err\n", argv[1]);
			return -1;
		}

		err = ioctl(fd, EVIOCGID, &id);
		if (err == 0)
		{
			printf("bustype = 0x%x\n", id.bustype );
			printf("vendor	= 0x%x\n", id.vendor  );
			printf("product = 0x%x\n", id.product );
			printf("version = 0x%x\n", id.version );
		}

		len = ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
		if (len > 0 && len <= sizeof(evbit))
		{
			printf("support ev type: ");
			for (int j = 0; j < len; j++)
			{
				byte = ((unsigned char *)evbit)[j];
				for (bit = 0; bit < 8; bit++)
				{
					if (byte & (1<<bit)) {
						printf("%s ", ev_names[j*8 + bit]);
					}
				}
			}
			printf("\n");
		}
	}

	/*Send app process id to driver*/
	fcntl(fd, F_SETOWN, getpid());

	/*Enable asynchronous notification*/
	flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flags | FASYNC);

	while (1) 
	{
		printf("main loop count = %d\n", count++);
		sleep(2);
	}

	return 0;
}


/*
Device Type	Typical 	/dev/input/eventX Assignment
Keyboard				/dev/input/event0
Mouse					/dev/input/event1
Touchpad (laptop)		/dev/input/event2
Game Controller			/dev/input/event3
Touchscreen				/dev/input/event4
Accelerometer (Sensor)	/dev/input/event5
External Input Devices (USB, Bluetooth)	/dev/input/event6



*/
