//TCP套接字传输的数据不存在数据边界
//Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

#define error_handling(fmt, args...)	\
	printf("\033[1;31m"fmt"\033[0m(function: %s, line: %d)", ##args, __func__, __LINE__);


int main(int argc, char *argv[])
{
	int ret = -1;

	int serv_sockfd;
	int clnt_sockfd;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_size;
	int str_len = 0;
	int i = 0;

	char message[BUF_SIZE] = {0};

	FILE *readfp = NULL;
	FILE *writefp = NULL;
	char *retp = NULL;

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

	clnt_addr_size = sizeof(clnt_addr);

	for (i = 0; i < 5; i++)
	{
		clnt_sockfd = accept(serv_sockfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if (clnt_sockfd == -1)
		{
			error_handling("accept() error");
		}
		else
		{
			printf("Connect client %d\n",	i+1);
		}

		readfp = fdopen(clnt_sockfd, "r");
		if (readfp == NULL)
		{
			error_handling("fdopen() error, readfp = NULL");
		}

		writefp = fdopen(clnt_sockfd, "w");
		if (writefp == NULL)
		{
			error_handling("fdopen() error, writefp = NULL");
		}

		while (!feof(readfp))
		{
			memset(message, 0, BUF_SIZE);
			retp = fgets(message, BUF_SIZE, readfp);
			if (retp == NULL)
			{
				error_handling("fgets() error");
			}
			printf("read: str = %s\n", retp);
			ret = fputs(message, writefp);
			if (ret == EOF)
			{
				error_handling("fputs() error");
			}
			fflush(writefp);
		}

		fclose(readfp);
		fclose(writefp);
		printf("Close connection: %d\n", i+1);
	}

	close(serv_sockfd);

	return 0;
}




