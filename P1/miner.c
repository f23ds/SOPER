#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "pow.h"
#include "monitor.h"

/* VARIABLES GLOBALES */
long value = -1;	/* almacena el valor encontrado */
long nrounds = -1;	/* almacena el numero de rounds */
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
		if (i == nthreads)
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

	for (i = a->first; i < a->last && value == -1; i++)
	{
		if (pow_hash(i) == obj)
		{
			value = i;
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
			free(ts);
			free(as);
			exit(EXIT_FAILURE);
		}
	}

	return;
}

/* ejecuta el numero de rondas */
void rounds_exec()
{
	pthread_t *threads; /* array de hilos */
	Args *args;			/* array de argumentos para cada hilo */
	int i;

	threads = threads_init();
	args = args_init();

	for (i = 0; i < nrounds; i++)
	{
		multi_thread(threads, args);

		/* no se ha encontrado el valor entre todos los hilos */
		if (value == -1)
		{
			fprintf(stderr, "%08ld !-> %08ld\n", obj, value);
			free(threads);
			free(args);
			exit(EXIT_FAILURE);
		}

		printf("%08ld --> %08ld\n", obj, value);
		obj = value;
		value = -1;
	}

	free(threads);
	free(args);

	return;
}