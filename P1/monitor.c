/**
 * @file monitor.c
 * @author Fabio Desio and Álvaro de Santos
 * @brief Monitor functions implementation
 * @version 0.1
 * @date 2023-03-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pow.h"
#include "types.h"

void monitor_exec(int *fd1, int *fd2, int nrounds)
{
    ssize_t nbytes;
    int i;
    long int obj, value;
    STATUS status;

    /* Cerramos los canales de las pipes pertinentes */
    close(fd1[1]);
    close(fd2[0]);

    for (i = 0; i < nrounds; i++)
    {
        status = REJECTED;
        /* Leemos el valor objetivo de miner */
        nbytes = read(fd1[0], &obj, sizeof(long int));
        if (nbytes == -1)
        {
            perror("Reading error in monitor.");
            exit(EXIT_FAILURE);
        }

        /* Leemos la solución encontrada por el miner */
        nbytes = read(fd1[0], &value, sizeof(long int));
        if (nbytes == -1)
        {
            perror("Reading error in monitor.");
            exit(EXIT_FAILURE);
        }

        if (pow_hash(value) == obj)
        {
            status = ACCEPTED;
        }

        /* Ahora escribimos la solución */
        nbytes = write(fd2[1], &status, sizeof(STATUS));
        if (nbytes == -1)
        {
            perror("Write error in monitor.");
            exit(EXIT_FAILURE);
        }
    }

    close(fd1[0]);
    close(fd2[1]);

}