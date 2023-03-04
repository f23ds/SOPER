#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t miner, monitor;

    miner = fork();
    if (miner < 0)
    {
        perror("Miner fork");
        exit(EXIT_FAILURE);
    }
    else if (miner == 0)
    {
        /* creamos el siguiente proceso */
        monitor = fork();
        if (monitor < 0)
        {
            perror("Miner fork");
            exit(EXIT_FAILURE);
        }
        else if (monitor == 0)
        {
            exit(EXIT_SUCCESS);
        }

        printf("Monitor: %d\n", monitor);
    }
    else
    {
        printf("Minero: %d\n", miner);
        wait(NULL);
    }

    exit(EXIT_SUCCESS);
}