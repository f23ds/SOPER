/**
 * @file miner.h
 * @author Fabio Desio and Álvaro de Santos
 * @brief Miner implementation
 * @version 1.5
 * @date 2023-03-09
 *
 * @copyright Copyright (c) 2023
 */

#ifndef  _MINERO_H
#define _MINERO_H

#include "types.h"

#include <pthread.h>

typedef struct _Args Args;

typedef 

/**
 * @brief inicializa los hilos con numero de hilos el almacenado en la variable global nthreads
 * @author Fabio Desio and Álvaro de Santos
 *
 * @return array de hilos inicializados
 */
pthread_t *threads_init();

/**
 * @brief inicializa los argumentos correspondientes a cada hilo (valor inicial y final donde busca)
 * @author Fabio Desio and Álvaro de Santos
 *
 * @return array de argumentos correspondientes a cada hilo
 */
Args *args_init();

/**
 * @brief hace una busqueda del objetivo en el valor inicial y final guardados en argumentos
 * @author Fabio Desio and Álvaro de Santos
 *
 * @param args array de argumentos 
 * @return NULL
 */
void *hash_search(void *args);

/**
 * @brief abre numero de hilos y los pone en ejecucion
 * @author Fabio Desio and Álvaro de Santos
 *
 * @param threads array de hilos
 * @param args array de argumentos correspondientes a los hilos
 * @return void
 */
void multi_thread(void *threads, void *args);

/**
 * @brief ejecuta el minero el numero de rondas solicitados. Para cada ronda: abre el multihilo, pasa los datos al monintor, y comprueba la respuesta del monitor.
 * @author Fabio Desio and Álvaro de Santos
 *
 * @param objv objetivo - se almacena inmediatamente en la variable global correspondiente
 * @param nthreadsv numero de hilos - se almacena inmediatamente en la variable global correspondiente
 * @param nroundsv numero de rondas a ejecutar
 * @param fd1 tuberia para enviar datos 
 * @param fd2 tuberia para recibir datos
 * @return void
 */
void rounds_exec(int objv, int nthreadsv, int nroundsv, int *fd1, int *fd2);

#endif
