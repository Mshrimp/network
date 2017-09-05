#include <stdio.h>
#include <unistd.h>


int g_val = 10;

int main(int argc, char *argv[])
{
	pid_t pid;
	int l_val = 25;

	printf("Init: g_val = %d, l_val = %d\n", g_val, l_val);

	pid = fork();
	if (pid == 0)	// If child process
	{
		g_val += 2;
		l_val += 2;
	}
	else	// If parent process
	{
		g_val -= 2;
		l_val -= 2;
	}

	if (pid == 0)
	{
		printf("Child: g_val = %d, l_val = %d\n", g_val, l_val);
	}
	else
	{
		printf("Parent: g_val = %d, l_val = %d\n", g_val, l_val);
	}

	return 0;
}



