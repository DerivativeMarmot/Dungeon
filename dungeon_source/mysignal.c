#include <stdio.h>
#include <signal.h>
#include <unistd.h> // fork(), exec()
#include <pthread.h>
#include <string.h>


int main(void)
{
    pid_t pid;
    // child process
    if ((pid = fork()) == 0)
    {
        if (execvp("./sigrec", NULL) == -1)
        {
            perror("Failed to exe\n");
        }
    }
    // parent process (here)
    else if (pid > 0)
        ;
    // fatal
    else
        perror("Failed to create a child\n");

    sleep(2);
    // send signal 
    for (int i = 0; i<5; i++){
        write(STDOUT_FILENO, "signal sent\n", strlen("signal sent\n"));
        kill(pid, SIGUSR1);
        sleep(1);
    }

    return 0;
}