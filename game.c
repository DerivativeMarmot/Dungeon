#include "shmmgr.h"

// sem_t *sem_w_ready;
// sem_t *sem_r_ready;
// sem_t *sem_b_ready;

/*void semSetup(){
    // unlink all the semaphore before creating them just in case
    sem_unlink(dungeon_lever_one);
    sem_unlink(dungeon_lever_two);
    sem_unlink(ROGUE_SEM);
    sem_unlink(WIZ_READY_SEM);
    sem_unlink(ROG_READY_SEM);
    sem_unlink(BAR_READY_SEM);
    // create semaphores
    sem_t *sem_w = sem_open(dungeon_lever_one, O_CREAT , 0644, 0);
    if (sem_w == SEM_FAILED){
        perror("sem_create/one");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_r = sem_open(ROGUE_SEM, O_CREAT , 0644, 0);
    if (sem_r == SEM_FAILED){
        perror("sem_create/rogue");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_b = sem_open(dungeon_lever_two, O_CREAT , 0644, 0);
    if (sem_b == SEM_FAILED){
        perror("sem_create/two");
        exit(EXIT_FAILURE);
    }
    sem_w_ready = sem_open(WIZ_READY_SEM, O_CREAT , 0644, 0);
    if (sem_b == SEM_FAILED){
        perror("sem_create/w_ready");
        exit(EXIT_FAILURE);
    }
    sem_r_ready = sem_open(ROG_READY_SEM, O_CREAT , 0644, 0);
    if (sem_b == SEM_FAILED){
        perror("sem_create/r_ready");
        exit(EXIT_FAILURE);
    }
    sem_b_ready = sem_open(BAR_READY_SEM, O_CREAT , 0644, 0);
    if (sem_b == SEM_FAILED){
        perror("sem_create/b_ready");
        exit(EXIT_FAILURE);
    }
    // close them
    sem_close(sem_w);
    sem_close(sem_r);
    sem_close(sem_b);
}*/

int main(void)
{
    //semSetup(); // set up semaphore
    
    allocShm(createShm(), DUNGEON_SIZE); // create shared memory

    // create three runnable classes
    pid_t pid[3]; // wizard, rogue, barbarian
    char *classes[3] = {"./wizard.dexe", "./rogue.dexe", "./barbarian.dexe"};
    for (int i = 0; i < 3; i++)
    {
        if ((pid[i] = fork()) == 0)
            if (execvp(classes[i], NULL) == -1){
                perror("Failed to exe\n");
            }
        else if (pid[i] > 0)
            ;
        else{
            perror("Failed to create a child\n");
        }
    }
    
    /*sem_wait(sem_w_ready); // wait for wizard to be ready to receive signal (important)
    sem_wait(sem_r_ready); // wait for rogue to be ready to receive signal (important)
    sem_wait(sem_b_ready); // wait for barbarian to be ready to receive signal (important)
    // close these sem, no longer useful
    sem_close(sem_w_ready);
    sem_close(sem_r_ready);
    sem_close(sem_b_ready);*/

    RunDungeon(pid[0], pid[1], pid[2]);

    if (kill(pid[0], 0) == 0){
        kill(pid[0], SIGKILL);
    }
    if (kill(pid[1], 0) == 0){
        kill(pid[1], SIGKILL);
    }
    if (kill(pid[2], 0) == 0){
        kill(pid[2], SIGKILL);
    }

    return 0;
}
