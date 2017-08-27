//UDP套接字
//Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

#define error_handling(fmt, args...)	\
	printf("\033[1;31m"fmt"\033[0m(function: %s, line: %d)\n", ## args, __func__, __LINE__);

void error_handling_bak(char *buf)
{
	printf("\033[1;31m%s\033[0m", buf);
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int str_len = 0;
	int recv_sockfd;
	struct sockaddr_in recv_addr;
	struct ip_mreq join_addr;
	char buf[BUF_SIZE] = {0};

	if (argc != 3)
	{
		printf("Usage: %s <GroupIP> <port>\n", argv[0]);
		exit(1);
	}

	recv_sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (recv_sockfd == -1)
	{
		error_handling("UDP socket() error");
	}

	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_addr.sin_port = htons(atoi(argv[2]));

	ret = bind(recv_sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
	if (ret == -1)
	{
		error_handling("bind() error");
	}

	join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(recv_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_addr, sizeof(join_addr));

	while (1)
	{
		memset(buf, 0 , BUF_SIZE);
		str_len = recvfrom(recv_sockfd, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (str_len == -1)
		{
			break;
		}
		buf[str_len] = '\0';
		printf("Message recvived: %s\n", buf);
	}

	close(recv_sockfd);

	return 0;
}
