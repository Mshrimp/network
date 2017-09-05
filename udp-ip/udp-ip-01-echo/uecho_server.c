//UDP套接字
//Server

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
	int serv_sockfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	char message[BUF_SIZE];
	socklen_t clnt_addr_sz;
	int str_len = 0;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sockfd == -1)
	{
		error_handling("UDP socket() error");
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

	while (1)
	{
		clnt_addr_sz = sizeof(serv_addr);

		memset(message, 0 , BUF_SIZE);
		str_len = recvfrom(serv_sockfd, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);

		printf("Client: %s: %d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
		printf("Message from client: %s\n", message);

		ret = sendto(serv_sockfd, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, clnt_addr_sz);
		if (ret == -1)
		{
			error_handling("sendto() error");
		}
	}

	close(serv_sockfd);

	return 0;
}
