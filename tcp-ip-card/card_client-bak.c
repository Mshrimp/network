//TCP套接字传输的数据不存在数据边界
//Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define NAME_SIZE 32

char message[BUF_SIZE] = { 0 };
char name[NAME_SIZE] = "Default";

#define error_handling(fmt, args...)	\
	printf("\033[1;31m"fmt"\033[0m(function: %s, line: %d)\n", ## args, __func__, __LINE__);

void *pthread_send_message(void *arg)
{
	int clnt_sockfd = *((int *)arg);
	char send_buf[BUF_SIZE + NAME_SIZE] = { 0 };
	int str_len = 0;

	while (1)
	{
		//printf("Please input message(Q to quit):\n");
		memset(message, 0, BUF_SIZE);
		fgets(message, BUF_SIZE, stdin);

		if (!(strcmp(message, "q\n")) || !(strcmp(message, "Q\n")))
		{
			printf("Close connection!\n");
			close(clnt_sockfd);
			break;
		}

		memset(send_buf, 0, sizeof(send_buf));
		sprintf(send_buf, "%s: %s", name, message);

		str_len = write(clnt_sockfd, send_buf, strlen(send_buf) + 1);
		if (str_len == -1)
		{
			error_handling("write() error");	
		}
		else
		{
			printf("write: str_len = %d\n", str_len);
		}
	}

	return NULL;
}

void *pthread_recv_message(void *arg)
{
	int clnt_sockfd = *((int *)arg);
	char recv_buf[BUF_SIZE + NAME_SIZE] = { 0 };
	int str_len = 0;

	while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		str_len = read(clnt_sockfd, recv_buf, sizeof(recv_buf));
		if (str_len == -1)
		{
			error_handling("read() error!\n");
			return (void *)-1;
		}
		recv_buf[str_len] = '\0';

		printf("recv: %s\n", recv_buf);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int clnt_sockfd;
	struct sockaddr_in serv_addr;

	pthread_t send_id, recv_id;
	void *thread_return = NULL;

	int str_len = 0;
	int index = 0;
	int read_len = 0;
	int read_cnt = 0;

	if (argc != 4)
	{
		printf("Usage: %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	memset(name, 0, NAME_SIZE);
	sprintf(name, "[%s]", argv[3]);

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
		printf("Connecting!\n");
	}

	pthread_create(&send_id, NULL, pthread_send_message, (void *)&clnt_sockfd);
	pthread_create(&recv_id, NULL, pthread_recv_message, (void *)&clnt_sockfd);

	pthread_join(send_id, &thread_return);
	pthread_join(recv_id, &thread_return);

	close(clnt_sockfd);

	return 0;
}
    /*
	 *while (read_len = read(clnt_sockfd, &message[index++], 1))
	 *{
	 *    if (str_len == -1)
	 *    {
	 *        error_handling("read() error");
	 *    }
	 *    printf("read: read_len = %d, %c\n", read_len, message[index-1]);
	 *    str_len += read_len;
	 *}
	 *printf("read: str_len = %d\n", str_len);
     */



