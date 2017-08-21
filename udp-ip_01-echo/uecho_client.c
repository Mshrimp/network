//UDP套接字
//Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30


void error_handling(char *message)
{
	printf("\033[1;31m%s\033[0m", message);
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int clnt_sockfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in from_addr;

	char message[BUF_SIZE];
	socklen_t serv_addr_sz;
	int str_len = 0;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	clnt_sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (clnt_sockfd == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while (1)
	{
		printf("Please input message(Q to quit):\n");
		memset(message, 0, BUF_SIZE);
		fgets(message, BUF_SIZE, stdin);

		if (!(strcmp(message, "q\n")) || !(strcmp(message, "Q\n")))
		{
			break;
		}
		
		ret = sendto(clnt_sockfd, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (ret == -1)
		{
			error_handling("sendto() error");
		}

		serv_addr_sz = sizeof(from_addr);

		str_len = recvfrom(clnt_sockfd, message, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &serv_addr_sz);
		message[str_len] = '\0';
		
		printf("Message from server: %s\n", message);
	}

	close(clnt_sockfd);

	return 0;
}




