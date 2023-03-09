/**
 * @file monitor.h
 * @author Fabio Desio and Álvaro de Santos
 * @brief Miner implementation
 * @version 1.5
 * @date 2023-03-09
 *
 * @copyright Copyright (c) 2023
 */

#ifndef _MONITOR_H
#define _MONITOR_H

#include "types.h"

/**
 * @brief ejecuta el monitor. Por cada ronda: lee los datos recibidos del minero, compara y devuelve una respuest al minero (por tuberia)
 * @author Fabio Desio and Álvaro de Santos
 *
 * @param fd1 tuberia para recibir datos
 * @param fd2 tuberia para enviar datos
 * @param nrounds numero de rondas a ejecutar
 * @return void
 */
void monitor_exec(int *fd1, int *fd2, int nrounds);

#endif
