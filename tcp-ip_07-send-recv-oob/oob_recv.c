//TCP
//Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

#define error_handling(message, args...)	\
	printf("\033[1;31m"message"\033[0m(function: %s, line: %d)\n", ## args, __func__, __LINE__);

void error_handling_bak(char *message)
{
	printf("\033[1;31m%s\033[0m(function: %s, line: %d)\n", message, __func__, __LINE__);
}

int recv_sockfd;
int acpt_sockfd;

void urg_handler(int signo)
{
	int str_len = 0;
	char message[BUF_SIZE] = {0};

	printf("%s: recv\n", __func__);
	str_len = recv(recv_sockfd, message, BUF_SIZE - 1, MSG_OOB);
	if (str_len == -1)
	{
		error_handling("recv() error");
	}
	else
	{
		message[str_len] = '\0';
		printf("Urgent message: %s\n", message);
	}
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int count = 0;
	int fds[2] = {0};

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	struct sigaction act;

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

	act.sa_handler = urg_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	recv_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (recv_sockfd == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	ret = bind(recv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1)
	{
		error_handling("bind() error");
	}
	
	ret = listen(recv_sockfd, 5);
	if (ret == -1)
	{
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	acpt_sockfd = accept(recv_sockfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	if (acpt_sockfd == -1)
	{
		error_handling("accept() error");
		return -1;
	}
	else
	{
		printf("New client connected, acpt_sockfd = %d\n", acpt_sockfd);
	}	

	fcntl(recv_sockfd, F_SETOWN, getpid());
	status = sigaction(SIGURG, &act, 0);

	printf("Ready to recv message!\n");
	while((str_len = recv(recv_sockfd, message, BUF_SIZE - 1, 0)) != 0)
	{
		if (str_len == -1)
		{
			continue;
		}
		message[str_len] = '\0';
		printf("recv message: %s\n", message);
	}

	close(recv_sockfd);
	close(acpt_sockfd);

	return 0;
}




