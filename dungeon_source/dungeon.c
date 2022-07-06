#include "../shmmgr.h"
struct Dungeon *dungeon;

void asyncDisplay(char *str){
    write(STDOUT_FILENO, str, strlen(str));
}

void wizard_job(pid_t wizard){
    char encoded_context[] = "Rwqmpi xshec erh qeoi wsqifshc wqmpi!";
    char decoded_context[] = "smile today and make somebody smile!";
    asyncDisplay("Break this barrier: ");
    asyncDisplay(encoded_context);
    asyncDisplay("\n");
    strcpy(dungeon->barrier.spell, encoded_context);

    kill(wizard, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_GUESS_BARRIER);

    if (!strcmp(dungeon->wizard.spell, decoded_context)){
        asyncDisplay("SUCCESS\n");
    }
    else{
        asyncDisplay("FAILED\n");
    }
    asyncDisplay("the magical phrase was: ");
    asyncDisplay(decoded_context);
}
void rogue_job(){
    ;
}
void barbarian_job(pid_t barbarian){
    asyncDisplay("Monster!!");
    dungeon->enemy.health = 5487695;

    kill(barbarian, DUNGEON_SIGNAL);
    sleep(SECONDS_TO_ATTACK);

    if (dungeon->barbarian.attack == dungeon->enemy.health){
        asyncDisplay("SUCCESS\n");
    }
    else{
        asyncDisplay("FAILED\n");
    }
    asyncDisplay("Monster: ");
    asyncDisplay("Barbarian: ");

}

void pidChecker(pid_t wizard, pid_t rogue, pid_t barbarian){
    if (!kill(wizard, 0)){
        asyncDisplay("wizard is not running\n");
    }
    if (!kill(rogue, 0)){
        asyncDisplay("rogue is not running\n");
    }
    if (!kill(barbarian, 0)){
        asyncDisplay("barbarian is not running\n");
    }
}

void RunDungeon(pid_t wizard, pid_t rogue, pid_t barbarian){
    pidChecker(wizard, rogue, barbarian);

    dungeon = getShmAddr(openShm(), DUNGEON_SIZE);
    

}

