#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	FILE *fp = NULL;

	int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
	{
		printf("open() error!\n");
		return -1;
	}

	fp = fdopen(fd, "w");
	if (fp == NULL)
	{
		printf("fdopen() error!\n");
		return -2;
	}

	fputs("New C programming!\n", fp);

	fclose(fp);

	return 0;
}


