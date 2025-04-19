
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static char read_key_value(int fd);

/*
 * ./button_test /dev/100ask_button0 <control>
 *
 */
int main(int argc, char **argv)
{
	int fd;
	char val;

	if (argc == 3 && strcmp(argv[2], "control") == 0) 
	{
		int led_fd;
		int status = 0;

		fd = open(argv[1], O_RDWR);
		if (fd == -1) 
		{
			printf("can not open file %s\n", argv[1]);
			return -1;
		}

		led_fd = open("/dev/myled", O_RDWR);
		if (led_fd == -1) 
		{
			printf("can not open file /dev/myled0\n");
			return -1;
		}

		while (1) {
			status = read_key_value(fd);
			if (status == 0) {
				val = 1;
				write(led_fd, &val, 1);
			} else if (status == 1) {
				val = 0;
				write(led_fd, &val, 1);
			}

			usleep(30000); // 30ms
		}
	
	}
	
	/* 1. 判断参数 */
	if (argc != 2) 
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	/* 2. 打开文件 */
	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	/* 3. 写文件 */
	read(fd, &val, 1);
	printf("get button : %d\n", val);
	
	close(fd);
	
	return 0;
}

static char read_key_value(int fd)
{
	char val;
	int ret;

	/* read button value */
	ret = read(fd, &val, 1);
	if (ret < 0) {
		perror("read");
		return -1;
	}
	return val;
}


