#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// ledTect /dev/myled on
// ledTest /dev/myled off

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: %s <device> <on/off>\n", argv[0]);
        printf("   eg: %s /dev/myled on\n", argv[0]);
        printf("       %s /dev/myled off\n", argv[0]);
        return -1;
    }

    int fd;
    char stat = 0;
    // open
    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    // write
    if (strcmp(argv[2], "on") == 0) {
        stat = 1;
    } 
    write(fd, &stat, sizeof(stat));
}