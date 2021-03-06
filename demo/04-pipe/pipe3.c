#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 30


int main(int argc, char *argv[])
{
	int fds1[2] = {0};
	int fds2[2] = {0};
	int ret = -1;

	char str1[] = "Hello world!";
	char str2[] = "Thanks for your message!";
	char buf[BUF_SIZE] = {0};

	pid_t pid;

	ret = pipe(fds1);
	ret = pipe(fds2);

	pid = fork();
	if (pid == 0)
	{
		printf("Child: creat child proc\n");
		ret = write(fds1[1], str1, sizeof(str1));
		if (ret == -1)
		{
			printf("Child: write() failed!\n");
		}
		ret = read(fds2[0], buf, BUF_SIZE);
		if (ret == -1)
		{
			printf("Child: read() failed!\n");
		}
		printf("Child: read message: %s\n", buf);

		return 0;
	}
	else
	{
		ret = read(fds1[0], buf, BUF_SIZE);
		if (ret == -1)
		{
			printf("Parent: read() failed!\n");
		}
		printf("Parent: read buf: %s, size = %d\n", buf, ret);
		ret = write(fds2[1], str2, sizeof(str2));
		if (ret == -1)
		{
			printf("Parent: write() failed!\n");
		}

		sleep(3);
	}

	return 0;
}

