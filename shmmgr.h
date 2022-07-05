#ifndef SHMMGR_H
#define SHMMGR_H
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdlib.h>
#include "dungeon_info.h"

#define DUNGEON_SIZE sizeof(struct Dungeon)
#define ROGUE_SEM "/roguesem"
#define WIZ_READY_SEM "/wizreadysem"
#define ROG_READY_SEM "/rogreadysem"
#define BAR_READY_SEM "/barreadysem"

int createShm();
int openShm();
int allocShm(int fd, int size);
void *getShmAddr(int fd, int size);
int detachShm(void *addr, int size);
void *closeShm(void *fd);

#endif