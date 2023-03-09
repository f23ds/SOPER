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
	int obj, nrounds, nthreads, range;
	ssize_t nbytes;

	if (argc < 4)
	{
		printf("Error en los argumentos\n");
		exit(EXIT_FAILURE);
	}

	obj = atol(argv[1]);
	nrounds = atoi(argv[2]);
	nthreads = atoi(argv[3]);

	range = (long)(POW_LIMIT / nthreads);

	/* Creamos el proceso minero */
	pid1 = fork();
	if (pid1 > 0)
	{
		wait(&wstatus);
		if (WIFEXITED(wstatus))
		{
			printf("Miner exited with status %d\n", WIFEXITED(wstatus));
		}
		else
		{
			printf("Miner exited with status %d\n", WIFEXITSTATUS(wstatus));
		}
	}
	else if (pid1 == 0) /* Minero */
	{
		/* Creamos las tuberías antes del siguiente fork para comunicar ambos procesos */
		pstatus = pipe(fd1);
		if (pstatus == -1) {
			perror("Pipe creation error");
			exit(EXIT_FAILURE);
		}

		pstatus = pipe(fd2);
		if (pstatus == -1) {
			perror("Pipe creation error");
			exit(EXIT_FAILURE);
		}

		pid2 = fork();
		if (pid2 > 0)
		{
			rounds_exec(obj, nthreads, nrounds, range);
			

			wait(&wstatus);
			if (WIFEXITED(wstatus))
			{
				printf("Monitor exited with status %d\n", WIFEXITED(wstatus));
			}
			else
			{
				printf("Monitor exited with status %d\n", WIFEXITSTATUS(wstatus));
			}
		}
		else if (pid2 == 0)
		{
			printf("KLK\n");
		}
		else
		{
			perror("Monitor exited unexpectedly");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		perror("Miner exited unexpectedly");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}