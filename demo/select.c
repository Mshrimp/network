#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	fd_set reads, temps;
	int result = 0;
	int read_len = 0;

	char buf[BUF_SIZE] = {0};
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);

	while(1)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1)
		{
			printf("select() error\n");
			break;
		}
		else if (result == 0)
		{
			printf("Timeout!\n");
		}
		else
		{
			if (FD_ISSET(0, &temps))
			{
				read_len = read(0, buf, BUF_SIZE);
				if (read_len == -1)
				{
					printf("read() error\n");
				}
				buf[read_len] = '\0';
				printf("Message from stdin: %s\n", buf);
			}
		}
	}

	return 0;
}

