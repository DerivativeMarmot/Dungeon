#include "shmmgr.h"

// not used
int detachShm(void *addr, int size){
	if (munmap(addr, size) == -1)
	{
		perror("munmap");
		return 4;
	}
    return 1;
}

// create a new shared memory file descriptor
int createShm(){
    int fd = shm_open(dungeon_shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        char msg[30];
        sprintf(msg, "create shm, pid=%d", getpid());
        perror(msg);
        return 1;
    }
    return fd;
}

// open an existing shared memory file descriptor
int openShm(){
    int fd = shm_open(dungeon_shm_name, O_RDWR, 0);
    if (fd == -1)
    {
        char msg[30];
        sprintf(msg, "open shm, pid=%d", getpid());
        perror(msg);
        return 2;
    }
    return fd;
}

// allocate memory space for struct
int allocShm(int fd, int size){
    int res = ftruncate(fd, size);
    if (res == -1)
    {
        perror("ftruncate shm");
        return 3;
    }
    return 0;
}

// return shm address
void *getShmAddr(int fd, int size)
{
    struct Dungeon *dptr;
    // map shared memory to virtual memory space
    dptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (dptr == MAP_FAILED)
    {
        char msg[30];
        sprintf(msg, "mmap shm, pid=%d", getpid());
        perror(msg);
        return "3";
    }
    return dptr;
}