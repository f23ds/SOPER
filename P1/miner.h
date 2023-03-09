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
 * @brief inicializa los hilos
 * @author fabio desio and Álvaro de santos
 *
 * @param id id del objeto
 * @return objeto inicializado
 */
pthread_t *threads_init();

/**
 * @brief inicializa un objeto
 * @author fabio desio and Álvaro de santos
 *
 * @param id id del objeto
 * @return objeto inicializado
 */
Args *args_init();

/**
 * @brief inicializa un objeto
 * @author fabio desio and Álvaro de santos
 *
 * @param id id del objeto
 * @return objeto inicializado
 */
void *hash_search(void *args);

/**
 * @brief inicializa un objeto
 * @author fabio desio and Álvaro de santos
 *
 * @param id id del objeto
 * @return objeto inicializado
 */
void multi_thread(void *threads, void *args);

/**
 * @brief inicializa un objeto
 * @author fabio desio and Álvaro de santos
 *
 * @param id id del objeto
 * @return objeto inicializado
 */
void rounds_exec(int objv, int nthreadsv, int nroundsv, int rangev, int *fd1, int *fd2);

#endif
