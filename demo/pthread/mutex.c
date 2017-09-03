#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM	100
#define SUM_MAX 50000000

long long sum = 0;
pthread_mutex_t mutex;

void *thread_inc(void *arg)
{
	int i = 0;

	pthread_mutex_lock(&mutex);

	for (i = 0; i < SUM_MAX; i++);
	{
		sum += 1;
	}
	pthread_mutex_unlock(&mutex);
	
	return NULL;
}

void *thread_des(void *arg)
{
	int i = 0;

	pthread_mutex_lock(&mutex);

	for (i = 0; i < SUM_MAX; i++);
	{
		sum -= 1;
	}
	pthread_mutex_unlock(&mutex);
	
	return NULL;
}


int main(void)
{
	pthread_t thread_id[THREAD_NUM] = {0};

	int i = 0;
	int ret = -1;

	printf("main: sizeof(long long) = %lu\n", sizeof(long long));

	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < THREAD_NUM; i++)
	{
		if (i % 2 == 0)
		{
			ret = pthread_create(&thread_id[i], NULL, thread_inc, NULL);
			if (ret != 0)
			{
				printf("pthread_create() %d error!\n", i);
				return -1;
			}
		}
		else
		{
			ret = pthread_create(&thread_id[i], NULL, thread_des, NULL);
			if (ret != 0)
			{
				printf("pthread_create() %d error!\n", i);
				return -1;
			}
		}
	}

	for (i = 0; i < THREAD_NUM; i++)
	{
		ret = pthread_join(thread_id[i], NULL);
		if (ret != 0)
		{
			printf("pthread_join() %d error!\n", i);
			return -1;
		}
	}

	printf("Result: %lld\n", sum);

	pthread_mutex_destroy(&mutex);

	printf("End of main()\n");

	return 0;
}


