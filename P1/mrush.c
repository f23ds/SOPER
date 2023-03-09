#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "pow.h"
#include "monitor.h"
#include "miner.h"

/* Ejecución principal */
int main(int argc, char *argv[])
{
	pid_t pid1, pid2;
	int wstatus, pstatus, fd1[2], fd2[2];
	int obj, nrounds, nthreads;

	if (argc < 4)
	{
		printf("Error en los argumentos\n");
		exit(EXIT_FAILURE);
	}

	obj = atol(argv[1]);
	nrounds = atoi(argv[2]);
	nthreads = atoi(argv[3]);

	/* Creamos el proceso minero */
	pid1 = fork();
	if (pid1 > 0)
	{
		wait(&wstatus);
		if (WIFEXITED(wstatus))
		{
			fprintf(stdout, "Miner exited with status %d\n", WEXITSTATUS(wstatus));
		}
		else
		{
			fprintf(stderr, "Miner exited unexpectedly\n");
		}
	}
	else if (pid1 == 0) /* Minero */
	{
		/* Creamos las tuberías antes del siguiente fork para comunicar ambos procesos */
		pstatus = pipe(fd1);
		if (pstatus == -1)
		{
			perror("Pipe creation error");
			exit(EXIT_FAILURE);
		}

		pstatus = pipe(fd2);
		if (pstatus == -1)
		{
			perror("Pipe creation error");
			exit(EXIT_FAILURE);
		}

		pid2 = fork();
		if (pid2 > 0)
		{
			rounds_exec(obj, nthreads, nrounds, fd1, fd2);
			wait(&wstatus);
			if (WIFEXITED(wstatus))
			{
				fprintf(stdout, "Monitor exited with status %d\n", WEXITSTATUS(wstatus));
			}
			else
			{
				fprintf(stderr, "Monitor exited unexpectedly\n");
			}
		}
		else if (pid2 == 0)
		{
			monitor_exec(fd1, fd2, nrounds);
		}
		else
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}