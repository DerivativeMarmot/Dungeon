#include "shmmgr.h"

struct Dungeon *dungeon;

void signalHandler(int signum)
{
    float upper = MAX_PICK_ANGLE;
    float lower = 0.0;
    dungeon->rogue.pick = (upper + lower) / 2; // initialize pick
    dungeon->trap.direction = 't'; // initialize direction (important)

    while (1)
    {
        // take a breath
        if (dungeon->trap.direction == 't')
        {
            usleep(100);
        }
        // too small
        else if (dungeon->trap.direction == 'u')
        {
            lower = dungeon->rogue.pick;
            dungeon->rogue.pick = (upper + lower) / 2;
            dungeon->trap.direction = 't'; // do not know why, but it's mentioned in the doc
        }
        // too large
        else if (dungeon->trap.direction == 'd')
        {
            upper = dungeon->rogue.pick;
            dungeon->rogue.pick = (upper + lower) / 2;
            dungeon->trap.direction = 't'; // do not know why, but it's mentioned in the doc
        }
        // right position
        else
            break;
    }
}

void semSignalHandler()
{
    // open semaphore for lever one and two and rogue
    sem_t *sem_w = sem_open(dungeon_lever_one, O_RDWR);
    if (sem_w == SEM_FAILED)
    {
        perror("sem_open/one");
        exit(EXIT_FAILURE);
    }
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
    sem_wait(sem_w); // wait for lever one to be pulled
    sem_wait(sem_b); // wait for lever two to be pulled
    
    // copy chars from treasure into spoils until rogue gets 4 chars
    while (strlen(dungeon->spoils) != 4)
    {
        strncpy(dungeon->spoils, dungeon->treasure, 4);
        usleep(100);
    }
    sem_post(sem_r); // rogue done getting treasure.
}

int main()
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
    sem_t *sem_r_ready = sem_open(ROG_READY_SEM, O_RDWR);
    if (sem_r_ready == SEM_FAILED)
    {
        perror("sem_open/r_ready");
        exit(EXIT_FAILURE);
    }
    sem_post(sem_r_ready);

    // continue when signal is received, otherwise pause
    while (1)
        pause;
    return 0;
}