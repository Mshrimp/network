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

void error_handling(char *message)
{
	printf("\033[1;31m%s\033[0m(function: %s, line: %d)\n", message, __func__, __LINE__);
}

void read_child_proc(int sig)
{
	pid_t pid;
	int status;

	pid = waitpid(-1, &status, WNOHANG);
	printf("Single: remove child proc, id = %d\n", pid);
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int count = 0;
	int fds[2] = {0};

	int serv_sockfd;
	int clnt_sockfd;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	pid_t pid;
	struct sigaction act;
	
	socklen_t clnt_addr_size;
	int str_len = 0;
	int status = 0;
	int i = 0;

	char message[BUF_SIZE] = {0};

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_child_proc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, 0);

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

	ret = pipe(fds);
	pid = fork();
	if (pid == -1)
	{
		printf("fork() to read pipe failed!\n");
		close(serv_sockfd);
		return -1;
	}

	if (pid == 0)
	{
		FILE *fp = fopen("message.txt", "wt");
		char mesgbuf[BUF_SIZE] = {0};
		int i = 0;
		int len = 0;
	
		printf("Child: creat child proc to read pipe, and write file\n");

		for (i = 0; i < 10; i++)
		{
			memset(mesgbuf, 0, BUF_SIZE);
			len = read(fds[0], mesgbuf, BUF_SIZE);
			if (len == -1)
			{
				error_handling("Child: read() error");
			}
			printf("Child: read message from pipe, message: %s\n", mesgbuf);
			ret = fwrite(mesgbuf, 1, len, fp);
			if (ret == -1)
			{
				error_handling("Child: write() error");
			}
			printf("Child: write message to file success!\n");
		}
		printf("Child: child proc close!\n");
		fclose(fp);
		return 0;
	}

	while (count < 10)
	{
		printf("count = %d, id = %d\n", count++, getpid());
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sockfd = accept(serv_sockfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		printf("clnt_sockfd = %d, pid = %d\n", clnt_sockfd, getpid());
		if (clnt_sockfd == -1)
		{
			error_handling("accept() error");
			continue;
		}
		else
		{
			printf("Connected client, clnt_sockfd = %d\n", clnt_sockfd);
		}

		memset(message, 0, BUF_SIZE);

		pid = fork();
		if (pid == -1)
		{
			printf("fork() failed!\n");
			close(clnt_sockfd);
			continue;
		}

		if (pid == 0)
		{
			printf("Child: creat child procd\n");
			close(serv_sockfd);
			while ((str_len = read(clnt_sockfd, message, BUF_SIZE)) != 0)
			{
				printf("Child: read message: %s\n", message);
				ret = write(clnt_sockfd, message, str_len);
				if (ret == -1)
				{
					error_handling("Child: write() error");
				}
				printf("Child: write ret = %d\n", ret);
				ret = write(fds[1], message, str_len);
				if (ret == -1)
				{
					error_handling("Child: write() error");
				}
			}

			close(clnt_sockfd);
			printf("Child: client disconnected!\n");
			return 0;
		}
		else
		{
			printf("Parent: creat child proc, id = %d\n", pid);
			close(clnt_sockfd);
		}
	}

	close(serv_sockfd);

	return 0;
}




