#include "../shmmgr.h"
struct Dungeon *dungeon;

void asyncDisplay(char *str){
    //char out[sizeof(str)+1];
    //memset(out, 0, sizeof(out));

    write(STDOUT_FILENO, str, strlen(str));
}

void wizard_job(pid_t wizard){
    char encoded_context[] = "Rwqmpi xshec erh qeoi wsqifshc wqmpi!";
    char decoded_context[] = "smile today and make somebody smile!";
    memset(dungeon->barrier.spell, 0, sizeof(dungeon->barrier.spell));
    memset(dungeon->wizard.spell, 0, sizeof(dungeon->wizard.spell));
    asyncDisplay("Break this barrier: ");
    asyncDisplay(encoded_context);
    asyncDisplay("\n");
    strcpy(dungeon->barrier.spell, encoded_context);

    kill(wizard, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_GUESS_BARRIER);

    if (!strcmp(dungeon->wizard.spell, decoded_context)){
        //asyncDisplay("SUCCESS\n");
        write(STDOUT_FILENO, "SUCCESS\n", strlen("SUCCESS\n"));
    }
    else{
        write(STDOUT_FILENO, "FAILED\n", strlen("FAILED\n"));
    }
    //asyncDisplay("the magical phrase was: ");
    asyncDisplay(decoded_context);
}

void rogue_job(){
    ;
}

void barbarian_job(pid_t barbarian){
    asyncDisplay("Monster!!\n");
    dungeon->enemy.health = 5487695;

    kill(barbarian, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_ATTACK);

    if (dungeon->barbarian.attack == dungeon->enemy.health){
        write(STDOUT_FILENO, "SUCCESS\n", strlen("SUCCESS\n"));
    }
    else{
        //asyncDisplay("FAILED\n");
        write(STDOUT_FILENO, "FAILED\n", strlen("FAILED\n"));
    }
    asyncDisplay("Monster: \n");
    asyncDisplay("Barbarian: \n");

}

void pidChecker(pid_t wizard, pid_t rogue, pid_t barbarian){
    if (kill(wizard, 0) == -1){
        asyncDisplay("wizard is not running\n");
    }
    if (!kill(rogue, 0) == -1){
        asyncDisplay("rogue is not running\n");
    }
    if (!kill(barbarian, 0) == -1){
        asyncDisplay("barbarian is not running\n");
    }
}

void killProcesses(pid_t wizard, pid_t rogue, pid_t barbarian){
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

