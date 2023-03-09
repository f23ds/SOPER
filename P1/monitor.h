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
 * @brief inicializa un objeto
 * @author Felix Lopez Laria
 *
 * @param id id del objeto
 * @return objeto inicializado
 */
void monitor_exec(int *fd1, int *fd2, int nrounds);

#endif
