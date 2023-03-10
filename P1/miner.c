#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "pow.h"
#include "monitor.h"

/* VARIABLES GLOBALES */
long solution = -1; /* almacena el valor encontrado */
long nthreads = -1; /* almacena el numero de hilos */
long obj = -1;		/* almacena el objetivo actual */
int range = -1;		/* almacena el tamaño en el que busca cada hilo */

/* estructura de datos que nos permite pasar varios argumentos a pthread_create */
typedef struct
{
	int first; /* primer elemento en el que busca */
	int last;  /* ultimo elemento en el que busca (no incluido)*/
} Args;

/* inicializa los threads a utilizar */
pthread_t *threads_init()
{
	pthread_t *threads = NULL;

	threads = (pthread_t *)calloc(nthreads, sizeof(pthread_t));
	if (threads == NULL)
	{
		return NULL;
	}

	return threads;
}

/* inicializa los argumentos a utilizar */
Args *args_init()
{
	Args *args = NULL;
	int i;

	args = (Args *)calloc(nthreads, sizeof(Args));
	if (args == NULL)
	{
		return NULL;
	}

	for (i = 0; i < nthreads; i++)
	{
		args[i].first = i * range;

		/* el ultimo hilo busca hasta el limite  */
		if (i == nthreads - 1)
		{
			args[i].last = POW_LIMIT;
		}
		else
		{
			args[i].last = (i + 1) * range;
		}
	}

	return args;
}

/* funcion para buscar objetivo */
void *hash_search(void *args)
{
	int i;
	Args *a;

	/* CASTING DEL ARGUMENTO */
	a = (Args *)args;

	/* busca el objetivo */
	for (i = a->first; i < a->last && solution == -1; i++)
	{
		if (pow_hash(i) == obj)
		{
			solution = i;
		}
	}

	return NULL;
}

/* crea y pone en ejecucion todos los hilos de la ronda */
void multi_thread(void *threads, void *args)
{
	pthread_t *ts;
	Args *as;
	int i;
	int error;

	ts = (pthread_t *)threads;
	as = (Args *)args;

	/* Creamos hilos en el array para buscar el objetivo por rangos en cada uno de ellos */
	for (i = 0; i < nthreads; i++)
	{
		error = pthread_create(&ts[i], NULL, hash_search, (void *)(&as[i]));
		if (error != 0)
		{
			fprintf(stderr, "Error creando hilo");
			free(threads);
			free(args);
			exit(EXIT_FAILURE);
		}
	}

	/* En caso de no encontrar el valor, esperamos a que acabe la ejecución de todos los hilos */
	for (i = 0; i < nthreads; i++)
	{
		error = pthread_join(ts[i], NULL);
		if (error != 0)
		{
			fprintf(stderr, "Error creando hilo");
			free(threads);
			free(args);
			exit(EXIT_FAILURE);
		}
	}

	return;
}

/* ejecuta el numero de rondas */
void rounds_exec(int objv, int nthreadsv, int nroundsv, int *fd1, int *fd2, STATUS *solst)
{
	pthread_t *threads; /* array de hilos */
	Args *args;			/* array de argumentos para cada hilo */
	int i;
	ssize_t nbytes;

	/* Cerramos los canales de las pipes pertinentes */
	close(fd1[0]);
	close(fd2[1]);

	/*inicializa variables globales */
	obj = objv;
	nthreads = nthreadsv;
	range = (long)(POW_LIMIT / nthreads);

	/* inicializa los hilos y los argumentos correspondientes a cada hilo */
	threads = threads_init();
	if (threads == NULL)
	{
		perror("Allocating error miner.");
		close(fd1[1]);
		close(fd2[0]);
		free(threads);
		exit(EXIT_FAILURE);
	}

	args = args_init();
	if (threads == NULL)
	{
		perror("Allocating error miner.");
		close(fd1[1]);
		close(fd2[0]);
		free(threads);
		free(args);
		exit(EXIT_FAILURE);
	}

	*solst = ACCEPTED;

	for (i = 0; i < nroundsv && (*solst) == ACCEPTED; i++)
	{
		/* Realizamos la primera escritura del objetivo a buscar */
		nbytes = write(fd1[1], &obj, sizeof(long int));
		if (nbytes == -1)
		{
			perror("Writing error in miner.");
			close(fd1[1]);
			close(fd2[0]);
			free(threads);
			free(args);
			exit(EXIT_FAILURE);
		}

		/* Computamos las soluciones */
		multi_thread(threads, args);

		/* Realizamos la segunda escritura con la solución encontrada */
		nbytes = write(fd1[1], &solution, sizeof(long int));
		if (nbytes == -1)
		{
			perror("Writing error in miner.");
			close(fd1[1]);
			close(fd2[0]);
			free(threads);
			free(args);
			exit(EXIT_FAILURE);
		}

		/* leemos la respuesta del monitor */
		nbytes = read(fd2[0], solst, sizeof(STATUS));
		if (nbytes == -1)
		{
			perror("Reading error in miner.");
			close(fd1[1]);
			close(fd2[0]);
			free(threads);
			free(args);
			exit(EXIT_FAILURE);
		}

		/* actualiza el nuevo objetivo */
		obj = solution;
		solution = -1;
	}

	/* Cerramos */
	close(fd1[1]);
	close(fd2[0]);

	free(threads);
	free(args);
}
