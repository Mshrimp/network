#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int sum = 0;

void *thread_summaction(void *arg)
{
	int start = ((int *)arg)[0];
	int end = ((int *)arg)[1];

	while(start <= end)
	{
		sum += start;
		start++;
	}
	
	return NULL;
}


int main(void)
{
	pthread_t t_id1, t_id2;

	int thread_param_rang1[] = {1, 5};
	int thread_param_rang2[] = {6, 10};

	int ret = -1;

	ret = pthread_create(&t_id1, NULL, thread_summaction, (void *)thread_param_rang1);
	if (ret != 0)
	{
		printf("pthread_create() 1 error!\n");
		return -1;
	}

	ret = pthread_create(&t_id2, NULL, thread_summaction, (void *)thread_param_rang2);
	if (ret != 0)
	{
		printf("pthread_create() 2 error!\n");
		return -1;
	}

	ret = pthread_join(t_id1, NULL);
	if (ret != 0)
	{
		printf("pthread_join() 1 error!\n");
		return -1;
	}

	ret = pthread_join(t_id2, NULL);
	if (ret != 0)
	{
		printf("pthread_join() 2 error!\n");
		return -1;
	}

	printf("Sum result: %d\n", sum);

	printf("End of main()\n");

	return 0;
}


