//TCP套接字传输的数据不存在数据边界
//Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024


#define error_handling(fmt, args...)	\
	printf("\033[1;31m"fmt"\033[0m(function: %s, line: %d)\n", ##args, __func__, __LINE__);

int main(int argc, char *argv[])
{
	int ret = -1;
	int clnt_sockfd;
	struct sockaddr_in serv_addr;

	FILE *readfp = NULL;
	FILE *writefp = NULL;
	char *retp = NULL;

	char message[BUF_SIZE];
	int str_len = 0;
	int index = 0;
	int read_len = 0;
	int read_cnt = 0;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	clnt_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (clnt_sockfd == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	ret = connect(clnt_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1)
	{
		error_handling("connect() error");
	}
	else
	{
		printf("Connected!\n");
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

	while (1)
	{
		printf("Please input message(Q to quit):\n");
		fgets(message, BUF_SIZE, stdin);

		if (!(strcmp(message, "q\n")) || !(strcmp(message, "Q\n")))
		{
			break;
		}

		ret = fputs(message, writefp);
		if (ret == EOF)
		{
			error_handling("fputs() error");
		}
		fflush(writefp);
		printf("fputs: %s\n", message);

		memset(message, 0, sizeof(message));
		retp = fgets(message, BUF_SIZE, readfp);
		if (retp == NULL)
		{
			error_handling("fgets() error");
		}
		printf("Message from server: %s\n", message);
	}

	fclose(readfp);
	fclose(writefp);

	return 0;
}




