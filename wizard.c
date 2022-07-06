#include "shmmgr.h"

struct Dungeon *dungeon;

void signalHandler(int signum)
{
    // improve readability
    char *spell = dungeon->barrier.spell;
    char letter;

    // reduce the shift amount using mod
    int shift = spell[0] % 26;
    int i;
    for (i = 1; i < strlen(spell); i++)
    {
        letter = spell[i];
        // lower case letter
        if (letter >= 65 && letter <= 90)
        {
            // no space for shifting backwards, shift forwards.
            if (letter - shift < 65)
                letter += (26 - shift);
            // shift backwards
            else
                letter -= shift;
        }
        // uppper case letter
        else if (letter >= 97 && letter <= 122)
        {
            if (letter - shift < 97)
                letter += (26 - shift);
            // shift backwards
            else
                letter -= shift;
        }
        // not a letter
        else
            ;
        dungeon->wizard.spell[i - 1] = letter; // assign decoded letter into spell array
    }
    dungeon->wizard.spell[i - 1] = '\0'; // the end of the string
}

void semSignalHandler()
{
    // open semaphore for lever one and rogue
    sem_t *sem_w = sem_open(dungeon_lever_one, O_RDWR);
    if (sem_w == SEM_FAILED)
    {
        perror("sem_open/one");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_r = sem_open(ROGUE_SEM, O_RDWR);
    if (sem_r == SEM_FAILED)
    {
        perror("sem_open/rogue");
        exit(EXIT_FAILURE);
    }

    sem_post(sem_w); // pull lever one
    sem_wait(sem_r); // wait for rogue grabbing all the treasure

    sem_close(sem_w); // close wizard semaphore
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
    sem_t *sem_w_ready = sem_open(WIZ_READY_SEM, O_RDWR);
    if (sem_w_ready == SEM_FAILED)
    {
        perror("sem_open/w_ready");
        exit(EXIT_FAILURE);
    }
    sem_post(sem_w_ready);

    // continue when signal is received, otherwise pause
    while (1)
        pause;
    return 0;
}