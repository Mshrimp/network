#include <stdio.h>
#include <unistd.h>


int main(void)
{
	int fd1 = -1;
	int fd2 = -1;

	char str1[] = "Hello world!\n";
	char str2[] = "What is wrong?\n";

	fd1 = dup(1);
	if (fd1 == -1)
	{
		printf("dup() fd1 error!\n");
	}

	fd2 = dup2(fd1, 7);
	if (fd2 == -1)
	{
		printf("dup2() fd2 error!\n");
	}

	printf("fd1 = %d, fd2 = %d\n", fd1, fd2);
	write(fd1, str1, sizeof(str1));
	write(fd2, str2, sizeof(str2));

	close(fd1);
	close(fd2);

	write(1, str1, sizeof(str1));
	close(1);
	write(1, str2, sizeof(str2));

	return 0;
}

