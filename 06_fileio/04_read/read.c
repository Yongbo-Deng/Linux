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
    int i;
    int len;
    unsigned char buf[100];
    
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDONLY);
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

    /*Read and print*/
    
    while(1)
    {
        len = read(fd, buf, sizeof(buf) - 1);
        if(len < 0)
        {
            perror("read");
            close(fd);
            return -1;
        }
        else if (len == 0)
        {
            break;
        }
        else
        {
            buf[len] = '\0';
            printf("%s", buf);
        }
    }
    
    close(fd);
    return 0;
}

