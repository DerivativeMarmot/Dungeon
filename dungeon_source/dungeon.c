#include "../shmmgr.h"
#include <pthread.h>
#include <stdlib.h> // rand()
#include <time.h>
struct Dungeon *dungeon;

void asyncDisplay(char *pattern, char* argv){
    if (strcmp(argv, "\0")){
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

void asyncDisplayFloat(char *pattern, float argv){
    size_t len = (size_t)snprintf(NULL, 0, pattern, argv);
    char *tempp = malloc(len);
    sprintf(tempp, pattern, argv);
    tempp[len] = '\0';
    write(STDOUT_FILENO, tempp, len);
    free(tempp);
}

void wizardDungeonJob(pid_t wizard){
    char encoded_context[] = "Rwqmpi xshec erh qeoi wsqifshc wqmpi!";
    char decoded_context[] = "smile today and make somebody smile!";
    memset(dungeon->barrier.spell, 0, sizeof(dungeon->barrier.spell));
    memset(dungeon->wizard.spell, 0, sizeof(dungeon->wizard.spell));
    
    asyncDisplay("Break this barrier: %s\n", encoded_context);

    strcpy(dungeon->barrier.spell, encoded_context);

    kill(wizard, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_GUESS_BARRIER);

    if (!strcmp(dungeon->wizard.spell, decoded_context)){
        asyncDisplay("\033[;32mSUCCESS\033[0m\n", "\0");
    }
    else{
        write(STDOUT_FILENO, "FAILED\n", strlen("FAILED\n"));
    }
    asyncDisplay("barrier: %s\n", encoded_context);
    asyncDisplay("your answer: %s\n", dungeon->wizard.spell);
    asyncDisplay("right answer: %s\n", decoded_context);
}

static void *rogueChild(void *n)
{
   // this thread can be cancelled at any time
   pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
   // set this thread to be running in a infinite loop until it has been cenceled
   float *target = n;
   *target = rand() % MAX_PICK_ANGLE;
   int old_pick = -1;
   //float *pick = &dungeon->rogue.pick;
   //char *dir = &dungeon->trap.direction;
   
   while (1)
   {
        //asyncDisplayFloat("current pick: %.2f\n", dungeon->rogue.pick);
        // check
        if (old_pick != dungeon->rogue.pick){
            asyncDisplayFloat("current pick: %.2f ===> ", dungeon->rogue.pick);
            asyncDisplayFloat("%.2f ===> ", *target);
            if (dungeon->rogue.pick + LOCK_THRESHOLD < *target){
                asyncDisplay("up\n", "0");
                dungeon->trap.direction = 'u';
            }
            else if (dungeon->rogue.pick - LOCK_THRESHOLD > *target){
                asyncDisplay("down\n", "0");
                dungeon->trap.direction = 'd';
            }
            else{
                asyncDisplay("Trap unlocked!\n", "\0");
                dungeon->trap.direction = '-';
                dungeon->trap.locked = false;
                break;
            }
            old_pick = dungeon->rogue.pick;
        }
        usleep(TIME_BETWEEN_ROGUE_TICKS);
   }
   asyncDisplay("Finished before the timer...\n", "\0");
   return NULL;
}

void rogueDungeonJob(pid_t rogue){
    dungeon->trap.direction = 't';
    dungeon->trap.locked = true;
    
    
    pthread_t child_thread;
    float target;
    int code;
    code = pthread_create(&child_thread, NULL, rogueChild, &target);
    if (code)
        fprintf(stderr, "pthread_create failed with code %d\n", code);
    
    kill(rogue, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_PICK);

    // when time is up, cancel the thread
    if (pthread_cancel(child_thread) == 0)
        printf("thread canceled\n");
    else{
        printf("thread not canceled\n");
    }
    // check if succeed
    if (!dungeon->trap.locked){
        asyncDisplay("\033[;32mSUCCESS\033[0m\n", "\0");
    }
    else{
        asyncDisplay("\033[;31mFAILED\033[0m\n", "\0");
    }
    asyncDisplayFloat("target: %.2f\n", target);
    asyncDisplayFloat("rogue's pick: %.2f\n", dungeon->rogue.pick);
}

void barbarianDungeonJob(pid_t barbarian){
    asyncDisplay("Monster!!\n", "\0");
    dungeon->enemy.health = rand() % 2147483648;

    kill(barbarian, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_ATTACK);

    if (dungeon->barbarian.attack == dungeon->enemy.health){
        asyncDisplay("\033[;32mSUCCESS\033[0m\n", "\0");
    }
    else{
        asyncDisplay("[;31mFAILED\033[0m\n", "\0");
    }
    printf("Monster: %d\nBarbarian: %d\n", dungeon->enemy.health, dungeon->barbarian.attack);

}

void pidChecker(pid_t wizard, pid_t rogue, pid_t barbarian){
    if (kill(wizard, 0) == -1){
        asyncDisplay("wizard is not running\n", "\0");
    }
    if (!kill(rogue, 0) == -1){
        asyncDisplay("rogue is not running\n", "\0");
    }
    if (!kill(barbarian, 0) == -1){
        asyncDisplay("barbarian is not running\n", "\0");
    }
}

void killProcesses(pid_t wizard, pid_t rogue, pid_t barbarian){
    asyncDisplay("terminating all three processes...", "\0");
    if (kill(wizard, 0) == 0){
        kill(wizard, SIGKILL);
    }
    if (kill(rogue, 0) == 0){
        kill(rogue, SIGKILL);
    }
    if (kill(barbarian, 0) == 0){
        kill(barbarian, SIGKILL);
    }
}

void RunDungeon(pid_t wizard, pid_t rogue, pid_t barbarian){
    srand((unsigned)time(NULL));
    pidChecker(wizard, rogue, barbarian);

    dungeon = getShmAddr(openShm(), DUNGEON_SIZE);
    barbarianDungeonJob(barbarian);
    wizardDungeonJob(wizard);
    rogueDungeonJob(rogue);

    killProcesses(wizard, rogue, barbarian);
    

}

