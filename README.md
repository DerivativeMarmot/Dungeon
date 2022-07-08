# Dungeon Source

#define dungeon_shm_name "/DungeonMem"

    " const char* " in nested .h file will cause error of "multiple definition of ..." => declared these virable using #define

(fixed)
asynDisplay() in ./dungeon_source/dungeon.c is not working properly