#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	FILE *fp = NULL;
	int fd_fp = -1;

	int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
	{
		printf("open() error!\n");
		return -1;
	}
	printf("First file description: %d!\n", fd);

	fp = fdopen(fd, "w");
	if (fp == NULL)
	{
		printf("fdopen() error!\n");
		return -2;
	}

	fputs("New C programming!\n", fp);

	fd_fp = fileno(fp);
	if (fd_fp == -1)
	{
		printf("fileno() error!\n");
		return -3;
	}
	printf("Second file description: %d!\n", fd_fp);

	fclose(fp);

	return 0;
}


