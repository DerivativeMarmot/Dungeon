#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


void signalHandler(int signum){
    write(STDOUT_FILENO, "signal received\n", strlen("signal received\n"));
}

int main(void){
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    while(1) pause;
    return 0;
}