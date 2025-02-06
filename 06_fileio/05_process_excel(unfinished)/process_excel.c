#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*  Example:
 *  ./process_excel data.csv result.csv
 *  argc = 3
 *  argv[0] = ./precess_excel
 *  argv[1] = data.csv
 *  argv[2] = result.csv
 */

static int read_line(int fd, unsigned char *buf)
{
    usigned char c;
    int len;
    int i = 0;
    int err = 0;

    while (1)
    {
        len = read(fd, &c, 1);
        if (len < 0)
        {
            err = -1;
            break;
        }
        else
        {
            if (c != '\n' && c != '\r')
            {
                buf[i] = c;
                i++;
            }
            else
            {
                err = 0;
                break;
            }
        }
    }
    
    buf[i] = '\0';

    if (err && (i == 0))
    {
        return -1;
    }
    else
    {
        
    }
}


int main(int argc, char **argv)
{
    int fd_data, fd_result;
    int i;
    int len;
    unsigned char buf[1000], result_buf[1000];
    
    if (argc != 3)
    {
        printf("Usage: %s <data file> <result file>\n", argv[0]);
        return -1;
    }

    /* Open date file*/
    fd_data = open(argv[1], O_RDONLY);
    if (fd_data < 0)
    {
        printf("Can not open file %s\n", argv[1]);
        perror("open");
    }
    else
    {
        printf("data file fd = %d\n", fd_data);
    }

    /*Create result file*/
    fd_result = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd_result < 0)
    {
        printf("Can not create file %s\n", argv[2]);
        perror("create");
    }
    else
    {
        printf("result file fd = %d\n", fd_result);
    }

    while(1)
    {
        /*Read one line from data file*/

        /*Process raw data*/

        /*Write prcessed data to result file*/
    }
    printf("lseek to offset 3 from file head\n");
    lseek(fd, 3, SEEK_SET);
    write(fd, "123", 3);

    close(fd);
    return 0;
}