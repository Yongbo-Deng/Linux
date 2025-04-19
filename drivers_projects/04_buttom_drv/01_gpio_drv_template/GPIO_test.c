
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * ./button_test /dev/GPIO0 read/on/off
 *
 */
int main(int argc, char **argv)
{
	int fd;
	char val;
	
	if (argc != 3) {
		printf("Usage: %s <dev> read/on/off \n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		printf("can not open file %s\n", argv[1]);
		return -1;
	}
	
	if (strcmp(argv[2], "read") == 0) {
		read(fd, &val, 1);
		printf("get GPIO : %d\n", val);
	} else if (strcmp(argv[2], "on") == 0) {
		val = 1;
		write(fd, &val, 1);
	} else if (strcmp(argv[2], "off") == 0) {
		val = 0;
		write(fd, &val, 1);
	} else {
		printf("Usage: %s <dev> read/on/off \n", argv[0]);
	}

	close(fd);
	
	return 0;
}


