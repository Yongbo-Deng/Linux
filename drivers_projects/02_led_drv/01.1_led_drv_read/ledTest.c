
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * ./ledtest /dev/myled0 on
 * ./ledtest /dev/myled0 off
 * ./ledtest /dev/myled0 read
 */
int main(int argc, char **argv)
{
	int fd;
	char status;
	
	if (argc != 3) 
	{
		printf("Usage: %s <dev> <on | off | read>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	if (strcmp(argv[2], "on") == 0) {
		status = 1;
		if (write(fd, &status, 1) != 1) {
			perror("write error");
		} else {
			printf("LED turned on\n");
		}
	} else if (strcmp(argv[2], "off") == 0) {
		status = 0;
		if (write(fd, &status, 1) != 1) {
			perror("write error");
		} else {
			printf("LED turned off\n");
		}
	} else if (strcmp(argv[2], "read") == 0) {
		if (read(fd, &status, 1) != 1) {
			perror("read error");
		} else {
			printf("LED status: %s\n", status ? "on" : "off");
		}
	} else {
		printf("Invalid command. Use 'on', 'off', or 'read'.\n");
	}
	
	close(fd);
	return 0;
}
