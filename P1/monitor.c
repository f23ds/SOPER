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
#include "mrush.h"

int monitor_exec(int *fd1, int *fd2) {
    int fd1[2], fd2[2], status; 
    pid_t pid;
    ssize_t nbytes;

    /* Creamos la primera tubería, de minero a monitor */
    status = pipe(fd1);
    if (status == -1) {
        perror("Error creando la primera tubería.");
    }
}   