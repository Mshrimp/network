#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_child_proc(int sig)
{
	int status;

	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status))
	{
		printf("Remove proc id: %d\n", id);
		printf("Child send: %d\n", WEXITSTATUS(status));
	}

}

int main(int argc, char *argv[])
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler = read_child_proc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if (pid == 0)
	{
		printf("This is a child process!\n");
		sleep(10);

		return 12;
	}
	else
	{
		printf("Parent: Child proc pid = %d\n", pid);
		pid = fork();
		if (pid == 0)
		{
			printf("This is another child process!\n");
			sleep(10);
			exit(24);
		}
		else
		{
			int i;
			printf("Parent: Child 2 proc pid = %d\n", pid);

			for (i = 0; i < 5; i++)
			{
				printf("Parent: waiting...\n");
				sleep(5);
			}
		}
	}

	return 0;
}


