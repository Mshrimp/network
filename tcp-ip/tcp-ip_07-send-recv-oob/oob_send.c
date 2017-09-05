//TCP
//Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024


void error_handling(char *message)
{
	printf("\033[1;31m%s\033[0m", message);
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int send_sockfd;
	struct sockaddr_in serv_addr;

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

	send_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (send_sockfd == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	ret = connect(send_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1)
	{
		error_handling("connect() error");
	}
	else
	{
		printf("Connected!\n");
	}

	ret = write(send_sockfd, "123", strlen("123"));
	if (ret == -1)
	{
		error_handling("write() error");
	}
	printf("write string1\n");
	ret = send(send_sockfd, "4", strlen("4"), MSG_OOB);
	if (ret == -1)
	{
		error_handling("send() error");
	}
	printf("write string2\n");
	ret = write(send_sockfd, "567", strlen("567"));
	if (ret == -1)
	{
		error_handling("write() error");
	}
	printf("write string3\n");
	ret = send(send_sockfd, "890", strlen("890"), MSG_OOB);
	if (ret == -1)
	{
		error_handling("send() error");
	}
	printf("write string4\n");

	close(send_sockfd);

	return 0;
}




