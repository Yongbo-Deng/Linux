#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        printf("Can not open file %s\n", argv[1]);
        printf("errno = %d\n", errno);
        printf("err: %s\n", strerror(errno));
        perror("open");
    }
    else
    {
        printf("fd = %d\n", fd);
    }
    
    while(1)
    {
        sleep(10);
    }
    
    close(fd);
    return 0;
}

