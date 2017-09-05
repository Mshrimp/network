//TCP
//Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling_bak(char *message)
{
	printf("\033[1;31m%s\033[0m(function: %s, line: %d)\n", message, __func__, __LINE__);
}

#define error_handling(message, args...)	\
	printf("\033[1;31m"message"\033[0m(function: %s, line: %d)\n", ## args, __func__, __LINE__);

int main(int argc, char *argv[])
{
	int ret = -1;
	int count = 0;
	int fds[2] = {0};

	int serv_sockfd;
	int clnt_sockfd;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	fd_set reads, cpy_reads;
	struct timeval timeout;

	socklen_t clnt_addr_size;
	int str_len = 0;
	int status = 0;
	int fd_max = 0;
	int fd_num = 0;
	int i = 0;

	char message[BUF_SIZE] = {0};

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sockfd == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	ret = bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1)
	{
		error_handling("bind() error");
	}
	
	ret = listen(serv_sockfd, 5);
	if (ret == -1)
	{
		error_handling("listen() error");
	}

	FD_ZERO(&reads);
	FD_SET(serv_sockfd, &reads);
	fd_max = serv_sockfd;

	while (count < 10)
	{
		printf("count = %d\n", ++count);
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		printf("Add select()\n");
		fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout);
		if (fd_num == -1)
		{
			error_handling("select() error");
			break;;
		}
		else if (fd_num == 0)
		{
			printf("Timeout!\n");
			continue;
		}

		for (i = 0; i < fd_max + 1; i++)
		{
			if (FD_ISSET(i, &cpy_reads))
			{
				if (i == serv_sockfd)
				{
					clnt_addr_size = sizeof(clnt_addr);
					clnt_sockfd = accept(serv_sockfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
					if (clnt_sockfd == -1)
					{
						error_handling("accept() error");
						continue;
					}
					else
					{
						printf("New client connected, clnt_sockfd = %d\n", clnt_sockfd);
						FD_SET(clnt_sockfd, &reads);
						if (fd_max < clnt_sockfd)
						{
							fd_max = clnt_sockfd;
						}
					}	
				}
				else
				{
					memset(message, 0, BUF_SIZE);
					str_len = read(i, message, BUF_SIZE);
					if (str_len == -1)
					{
						error_handling("read() error");
					}
					else if (str_len == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("Close client: %d\n", i);
					}
					else
					{
						printf("Read message: %s\n", message);
						ret = write(i, message, str_len);
						if (ret == -1)
						{
							error_handling("write() error");
						}
						else
						{
							printf("Message echo OK!\n");
						}
					}
				
				}
			}
		}
	}

	close(serv_sockfd);

	return 0;
}




