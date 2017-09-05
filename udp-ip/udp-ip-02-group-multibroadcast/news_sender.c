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
	struct sockaddr_in mul_addr;
	int time_val = TTL;
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

	memset(&mul_addr, 0, sizeof(mul_addr));
	mul_addr.sin_family = AF_INET;
	mul_addr.sin_addr.s_addr = inet_addr(argv[1]);
	mul_addr.sin_port = htons(atoi(argv[2]));

	setsockopt(send_sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &time_val, sizeof(time_val));

	fp = fopen("news.txt", "r");
	if (fp == NULL)
	{
		error_handling("fopen() error");
	}

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sockfd, buf, strlen(buf), 0, (struct sockaddr *)&mul_addr, sizeof(mul_addr));
		sleep(2);
	}

	fclose(fp);

	close(send_sockfd);

	return 0;
}




