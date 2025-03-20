
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

#define MAX_DEVICES  3  // Adjust based on how many devices you want to monitor

/* ./03_input_read_poll /dev/input/event0 noblock*/
int main(int argc, char **argv)
{
	int fd[MAX_DEVICES];
	int err;
	int len, ret;
	int i;
	unsigned char byte;
	int bit;
	struct input_id id;
	unsigned int evbit[2];
	struct input_event event;
	nfds_t nfds = argc - 1;
	struct pollfd fds[MAX_DEVICES];

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
	
	if (argc < 2)
	{
		printf("Usage: %s <dev1> <dev2> <dev3>\n", argv[0]);
		return -1;
	}
	else if (argc > MAX_DEVICES)
	{
		printf("Too many devices");
		return -1;
	} 
	
	for (int i = 0; i < nfds; i++)
	{
		fd[i] = open(argv[i + 1], O_RDWR | O_NONBLOCK);
		if (fd[i] < 0)
		{
			printf("open %s err\n", argv[i + 1]);
			return -1;
		}

		err = ioctl(fd[i], EVIOCGID, &id);
		if (err == 0)
		{
			printf("bustype = 0x%x\n", id.bustype );
			printf("vendor	= 0x%x\n", id.vendor  );
			printf("product = 0x%x\n", id.product );
			printf("version = 0x%x\n", id.version );
		}

		len = ioctl(fd[i], EVIOCGBIT(0, sizeof(evbit)), &evbit);
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
		fds[i].fd = fd[i];
		fds[i].events = POLLIN;
	}

	while (1) 
	{
		ret = poll(fds, nfds,5000);
		if (ret > 0)
		{
			for (int i = 0; i < nfds; i++)
			{
				if (fds[i].revents == POLLIN)
				{	
					while (read(fd[i], &event, sizeof(event)) == sizeof(event)) 
					{
						printf("get event from device%d: type = 0x%x, code = 0x%x, value = 0x%x\n", i, event.type, event.code, event.value);
					}
				}
			}	
		}
		else if (ret == 0)
		{
			printf("timeout\n");
		}
		else if (ret < 0)
		{
			printf("poll err\n");
		} 
	}

	for (int i = 0; i < nfds; i++) {
        close(fd[i]);
    }

	return 0;
}

