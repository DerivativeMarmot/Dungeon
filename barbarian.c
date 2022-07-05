#include "shmmgr.c"

struct Dungeon *dungeon;

void signalHandler(int signum)
{
    dungeon->barbarian.attack = dungeon->enemy.health;
}

void semSignalHandler()
{
    // open semaphore for lever two and rogue
    sem_t *sem_b = sem_open(dungeon_lever_two, O_RDWR);
    if (sem_b == SEM_FAILED)
    {
        perror("sem_open/two");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_r = sem_open(ROGUE_SEM, O_RDWR);
    if (sem_r == SEM_FAILED)
    {
        perror("sem_open/rogue");
        exit(EXIT_FAILURE);
    }
    
    sem_post(sem_b); // pull lever two
    sem_wait(sem_r); // wait for rogue grabbing all the treasure
    
    sem_close(sem_b); // close barbarian semaphore
    sem_close(sem_r); // close rogue semaphore
}

int main(int argc, char **argv)
{
    // connect to shared memory
    dungeon = getShmAddr(openShm(), DUNGEON_SIZE);

    // setup signal struct for DUNGEON_SIGNAL
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(DUNGEON_SIGNAL, &sa, NULL);

    // setup signal struct for SEMAPHORE_SIGNAL
    struct sigaction sa_sem;
    sa_sem.sa_handler = semSignalHandler;
    sa_sem.sa_flags = SA_RESTART;
    sigaction(SEMAPHORE_SIGNAL, &sa_sem, NULL);

    // ready to receive signals
    sem_t *sem_b_ready = sem_open(BAR_READY_SEM, O_RDWR);
    if (sem_b_ready == SEM_FAILED)
    {
        perror("sem_open/b_ready");
        exit(EXIT_FAILURE);
    }
    sem_post(sem_b_ready);

    // continue when signal is received, otherwise pause
    while (1)
        pause;
    return 0;
}