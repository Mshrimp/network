#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *pthread_main(void *arg)
{
	int i = 0;
	int count = *((int *)arg);

	for (i = 0; i < count; i++)
	{
		printf("Running pthread: %d\n", i);
		sleep(1);
	}

	return NULL;
}


int main(void)
{
	pthread_t t_id;
	int thread_param = 5;
	int ret = -1;

	ret = pthread_create(&t_id, NULL, pthread_main, (void *)&thread_param);
	if (ret != 0)
	{
		printf("pthread_create() error!\n");
		return -1;
	}

	sleep(6);
	printf("End of main()\n");

	return 0;
}


