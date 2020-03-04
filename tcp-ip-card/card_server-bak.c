//TCP套接字传输的数据不存在数据边界
//Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define MAX_CLNT_NUM 256

#define error_handling(fmt, args...)	\
	printf("\033[1;31m"fmt"\033[0m(function: %s, line: %d)\n", ## args, __func__, __LINE__);

int clnt_count = 0;
int clnt_sockfds[MAX_CLNT_NUM] = { 0 };

pthread_mutex_t mutex;

void send_message(char *message, int len)
{
	int str_len = 0;
	int i = 0;

	pthread_mutex_lock(&mutex);

	for (i = 0; i < clnt_count; i++)
	{
		str_len = write(clnt_sockfds[i], message, len);
		if (str_len != len)
		{
			error_handling("Client: %d write() error!\n", clnt_sockfds[i]);
		}
	}
	printf("Send message: %s\n", message);

	pthread_mutex_unlock(&mutex);
}


void *handling_client(void *arg)
{
	int clnt_sockfd = *((int *)arg);
	int str_len = 0;
	int i = 0;

	char message[BUF_SIZE] = { 0 };

	while((str_len = read(clnt_sockfd, message, BUF_SIZE)) > 0)
	{
		printf("Recv client %d message: %s\n", clnt_sockfd, message);
		send_message(message, str_len);
		memset(message, 0, sizeof(message));
	}

	pthread_mutex_lock(&mutex);

	for (i = 0; i < clnt_count; i++)
	{
		if (clnt_sockfd == clnt_sockfds[i])
		{
			while(i++ < clnt_count-1)
			{
				clnt_sockfds[i] = clnt_sockfds[i+1];
			}
			break;
		}
	}

	clnt_count--;
	pthread_mutex_unlock(&mutex);

	printf("Client %d close!\n", clnt_sockfd);
	close(clnt_sockfd);

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret = -1;

	int serv_sockfd;
	int clnt_sockfd;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_addr_size;
	pthread_t t_id;
	int str_len = 0;
	int i = 0;

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


	while(1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sockfd = accept(serv_sockfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if (clnt_sockfd == -1)
		{
			error_handling("accept() error");
		}
		else
		{
			printf("Connected client %d\n",	clnt_sockfd);
		}

		pthread_mutex_lock(&mutex);
		clnt_sockfds[clnt_count++] = clnt_sockfd;
		pthread_mutex_unlock(&mutex);

		pthread_create(&t_id, NULL, handling_client, (void *)&clnt_sockfd);
		pthread_detach(t_id);

		printf("Connected %d client IP: %s\n", clnt_count, inet_ntoa(clnt_addr.sin_addr));
	}

	close(serv_sockfd);

	return 0;
}


