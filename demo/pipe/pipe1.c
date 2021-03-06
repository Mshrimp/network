#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 30


int main(int argc, char *argv[])
{
	int fds[2] = {0};
	int ret = -1;

	char str[] = "Hello world!";
	char buf[BUF_SIZE] = {0};

	pid_t pid;

	ret = pipe(fds);

	pid = fork();
	if (pid == 0)
	{
		printf("Child: creat child proc\n");
		ret = write(fds[1], str, sizeof(str));
		if (ret == -1)
		{
			printf("Child: write() failed!\n");
		}

		return 0;
	}
	else
	{
		ret = read(fds[0], buf, BUF_SIZE);
		if (ret == -1)
		{
			printf("Parent: read() failed!\n");
		}
		printf("Parent: read buf: %s, size = %d\n", buf, ret);
	}

	return 0;
}

