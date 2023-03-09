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

#include <pthread.h>

typedef struct _Args Args;

pthread_t *threads_init();
Args *args_init();
void *hash_search(void *args);
void multi_thread(void *threads, void *args);
void rounds_exec(int obj, int nthreads, int n);

#endif
