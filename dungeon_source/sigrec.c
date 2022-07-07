#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


/*void asyncDisplay(char *pattern, char* arg){
    if (strlen(arg) != 0){
        memset(buffer, 0, strlen(buffer));
        sprintf(buffer, pattern, arg);
    }
    write(STDOUT_FILENO, pattern, strlen(pattern));
}*/


int main(void){
    // struct sigaction sa;
    // sa.sa_handler = signalHandler;
    // sa.sa_flags = SA_RESTART;
    // sigaction(SIGUSR1, &sa, NULL);

    //while(1) pause;
    return 0;
}