#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MSG_LEN 50

void *pthread_main(void *arg)
{
	int i = 0;
	int count = *((int *)arg);
	char *mesg = NULL;

	mesg = (char *)malloc(sizeof(char) * MSG_LEN);
	if (mesg == NULL)
	{
		printf("pthread: malloc() error!\n");
		return NULL;
	}

	strcpy(mesg, "Hello, This is pthread message!\n");

	for (i = 0; i < count; i++)
	{
		printf("Running pthread: %d\n", i);
		sleep(1);
	}

	return (void *)mesg;
}


int main(void)
{
	pthread_t t_id;
	int thread_param = 5;
	void *thread_retp = NULL;
	int ret = -1;

	ret = pthread_create(&t_id, NULL, pthread_main, (void *)&thread_param);
	if (ret != 0)
	{
		printf("pthread_create() error!\n");
		return -1;
	}

	ret = pthread_join(t_id, &thread_retp);
	if (ret != 0)
	{
		printf("pthread_join() error!\n");
		return -1;
	}

	if (thread_retp == NULL)
	{
		printf("Pthread return NULL!\n");
	}
	else
	{
		printf("Pthread return message: %s\n", (char *)thread_retp);
	}

	printf("End of main()\n");
	free(thread_retp);

	return 0;
}


