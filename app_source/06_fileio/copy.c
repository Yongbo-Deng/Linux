
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

/*
 * ./copy 1.txt 2.txt
 * argc    = 3
 * argv[0] = "./copy"
 * argv[1] = "1.txt"
 * argv[2] = "2.txt"
 */
int main(int argc, char **argv)
{
	int fd_old, fd_new;
	char buf[1024];
	int len;
	
	/* Argument counts */
	if (argc != 3) 
	{
		printf("Usage: %s <old-file> <new-file>\n", argv[0]);
		return -1;
	}

	/* Original file */
	fd_old = open(argv[1], O_RDONLY);
	if (fd_old == -1)
	{
		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	/* New file */
	fd_new = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd_new == -1)
	{
		printf("can not creat file %s\n", argv[2]);
		return -1;
	}

	/* Read original file and write new file */
	while ((len = read(fd_old, buf, 1024)) > 0)
	{
		if (write(fd_new, buf, len) != len)
		{
			printf("can not write %s\n", argv[2]);
			return -1;
		}
	}

	/* Close files */
	close(fd_old);
	close(fd_new);
	
	return 0;
}

