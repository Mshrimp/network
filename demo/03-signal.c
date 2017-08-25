#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void timeout(int sig)
{
	if (sig == SIGALRM)
	{
		printf("Alarm timeout!\n");
	}
	alarm(2);
}

void keycontrol(int sig)
{
	if (sig == SIGINT)
	{
		printf("Ctrl+C pressed!\n");
	}
}



int main(void)
{
	int i = 0;

	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);

	alarm(2);

	for (i = 0; i < 3; i++)
	{
		printf("Please wait...\n");
		sleep(100);
	}

	return 0;
}


