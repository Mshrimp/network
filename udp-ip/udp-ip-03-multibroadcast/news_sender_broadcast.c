//UDP套接字
//Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50
#define TTL 64

#define error_handling(fmt, args...)	\
	printf("\033[1;31m"fmt"\033[0m(function: %s, line: %d)\n", ## args, __func__, __LINE__);

void error_handling_bak(char *buf)
{
	printf("\033[1;31m%s\033[0m", buf);
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int send_sockfd;
	struct sockaddr_in broad_addr;
	int so_broad = 1;
	char buf[BUF_SIZE];
	FILE *fp = NULL;
	int str_len = 0;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	send_sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (send_sockfd == -1)
	{
		error_handling("socket() error");
	}

	memset(&broad_addr, 0, sizeof(broad_addr));
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
	broad_addr.sin_port = htons(atoi(argv[2]));

	setsockopt(send_sockfd, SOL_SOCKET, SO_BROADCAST, (void *)&so_broad, sizeof(so_broad));

	fp = fopen("news.txt", "r");
	if (fp == NULL)
	{
		error_handling("fopen() error");
	}

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sockfd, buf, strlen(buf), 0, (struct sockaddr *)&broad_addr, sizeof(broad_addr));
		sleep(2);
	}

	fclose(fp);

	close(send_sockfd);

	return 0;
}




