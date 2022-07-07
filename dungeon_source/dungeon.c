#include "../shmmgr.h"
#include <stdlib.h> // itoa()
struct Dungeon *dungeon;

void asyncDisplay(char *pattern, void* argv){
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

void int2str(int n, char *s){
    sprintf(s, "%d", n);
}

void wizard_job(pid_t wizard){
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

void rogue_job(){
    ;
}

void barbarian_job(pid_t barbarian){
    asyncDisplay("Monster!!\n", "\0");
    dungeon->enemy.health = 31208923;

    kill(barbarian, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_ATTACK);

    if (dungeon->barbarian.attack == dungeon->enemy.health){
        asyncDisplay("\033[;32mSUCCESS\033[0m\n", "\0");
    }
    else{
        asyncDisplay("[;31mFAILED\033[0m\n", "\0");
    }
    /*char str[10];
    sprintf(str, "%d", dungeon->enemy.health);
    asyncDisplay("Monster: %d\n", str);
    memset(str, 0, sizeof(str));
    sprintf(str, "%d", dungeon->barbarian.attack);
    asyncDisplay("Barbarian: %d\n", str);*/
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
    pidChecker(wizard, rogue, barbarian);

    dungeon = getShmAddr(openShm(), DUNGEON_SIZE);
    barbarian_job(barbarian);
    wizard_job(wizard);

    killProcesses(wizard, rogue, barbarian);
    

}

