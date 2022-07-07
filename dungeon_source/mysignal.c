#include <stdio.h>
//#include <signal.h>
#include <unistd.h> // fork(), exec()
//#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
//#include "test.h"
//char buffer[100];
/*void asyncDisplay(int argc, char** argv){
    if (argc == 1){
        ;
    }else{
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, argv[0]);
        for (int i=1; i<argc; ++i){
            sprintf(buffer, buffer, argv[i]);
            sprintf
        }
    }
    write(STDOUT_FILENO, buffer, strlen(buffer));
}*/
/*
void asyncDisplay(char *pattern, char* argv){
    if (strlen(argv) != 0){
        sprintf(buffer, pattern, argv);
        write(STDOUT_FILENO, buffer, strlen(buffer));
        memset(buffer, 0, sizeof(buffer));
    }
    else{
        write(STDOUT_FILENO, pattern, strlen(pattern));
    }
}
*/
void asyncDisplay(char *pattern, char* argv){
    if (strlen(argv) != 0){
        int res_len = strlen(pattern) + strlen(argv);
        char *tempp = malloc(res_len + 1);
        sprintf(tempp, pattern, argv);
        tempp[res_len] = '\0';
        write(STDOUT_FILENO, tempp, res_len);
        free(tempp);
    }
    else{
        write(STDOUT_FILENO, pattern, strlen(pattern));
    }
}

int main(void)
{
    /*pid_t pid;
    // child process
    if ((pid = fork()) == 0)
    {
        if (execvp("./sigrec.dexe", NULL) == -1)
        {
            perror("Failed to exe\n");
            return 0;
        }
    }
    // parent process (here)
    else if (pid > 0)
        ;
    // fatal
    else{
        perror("Failed to create a child\n");
        return 0;
    }
        

    sleep(2);
    // send signal 
    for (int i = 0; i<5; i++){
        write(STDOUT_FILENO, "signal sent\n", strlen("signal sent\n"));
        kill(pid, SIGUSR1);
        sleep(1);
    }
    kill(pid, SIGKILL);*/
    //memset(buffer, 0, sizeof(buffer));
    asyncDisplay("end\n", "\0");
    asyncDisplay("e312312 1nd\n", "\0");
    asyncDisplay("john %s\n", "bob");
    // asyncDisplay(2, "enddqw%s\n", {"999"});
    // asyncDisplay(3, "enddqw%s %s\n", {"999", "bbb"});
    // asyncDisplay("en qrew d\n");
    // asyncDisplay("enqwe rqw4f d\n");

    return 0;
}