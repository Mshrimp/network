#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM	100
#define SUM_MAX 50000000

static sem_t sem_one;
static sem_t sem_two;

static int num = 0;

void *input_data(void *arg)
{
	int i = 0;

	for (i = 0; i < 5; i++)
	{
		printf("Please input the %d num:\n", i+1);
		sem_wait(&sem_two);
		scanf("%d", &num);
		sem_post(&sem_one);
	}

	return NULL;
}

void *read_data(void *arg)
{
	int i = 0;
	int sum = 0;

	for (i = 0; i < 5; i++)
	{
		sem_wait(&sem_one);
		sum += num;
		sem_post(&sem_two);
	}

	printf("Result: sum = %d\n", sum);

	return NULL;
}

int main(void)
{
	pthread_t t_id1, t_id2;

	int i = 0;
	int ret = -1;

	printf("main: sizeof(long long) = %lu\n", sizeof(long long));

	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	ret = pthread_create(&t_id1, NULL, input_data, NULL);
	if (ret != 0)
	{
		printf("pthread_create() error!\n");
		return -1;
	}
	ret = pthread_create(&t_id2, NULL, read_data, NULL);
	if (ret != 0)
	{
		printf("pthread_create() error!\n");
		return -1;
	}

	ret = pthread_join(t_id1, NULL);
	if (ret != 0)
	{
		printf("pthread_join() error!\n");
		return -1;
	}

	ret = pthread_join(t_id2, NULL);
	if (ret != 0)
	{
		printf("pthread_join() error!\n");
		return -1;
	}

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);

	printf("End of main()\n");

	return 0;
}


