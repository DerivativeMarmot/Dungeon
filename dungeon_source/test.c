#include "test.h"
void asyncDisplay(char *str){
    write(STDOUT_FILENO, str, strlen(str));
}