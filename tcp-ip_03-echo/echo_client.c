//TCP套接字传输的数据不存在数据边界
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
	int clnt_sockfd;
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

	while (1)
	{
		printf("Please input message(Q to quit):\n");
		fgets(message, BUF_SIZE, stdin);

		if (!(strcmp(message, "q\n")) || !(strcmp(message, "Q\n")))
		{
			break;
		}

		str_len = write(clnt_sockfd, message, strlen(message));
		if (str_len == -1)
		{
			error_handling("write() error");	
		}
		else
		{
			printf("write: str_len = %d\n", str_len);
		}

		while (read_len < str_len)
		{
			read_cnt = read(clnt_sockfd, &message[read_len], BUF_SIZE - 1);
			if (read_len == -1)
			{
				error_handling("read() error");
			}
			read_len += read_cnt;
			printf("read: read_cnt = %d\n", read_cnt);
		}
		message[read_len] = '\0';
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

	printf("Message from server: %s\n", message);

	close(clnt_sockfd);

	return 0;
}




