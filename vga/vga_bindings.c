#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SHMSZ (800*600+1)

long int shmid, shmkey;
int *shm;

int initialize()
{
    int index, dont_wait=0;

    shmkey = 0x414141414101;

    if ((shmid = shmget(shmkey, SHMSZ*4, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget VGA");
        return -1;
    }
    shm = (int *)shmat(shmid, NULL, 0);
    if ((void *)shm == (void *)-1)
    {
        perror("shmat");
        return -1;
    }

    int t = time(0);
    if (!dont_wait){
        while (*(shm+SHMSZ-1) != 0x3000000) {
            if ((time(0)-t) > 3)
            {
                printf("--+\t\tthere was a problem connecting to the VGA process, continuing headless...\n");
                return 1;
            }
        }
    }
    *(shm+SHMSZ-1) = 0;
    memset(shm, 0xff000000, sizeof(int)*SHMSZ-2);

    *(shm+SHMSZ-1) = 0x1000000;

    return 0;
}

int write_byte(uint32_t x, uint32_t y, uint32_t color) {
    *(shm + (x) + y*800) = color;
    *(shm+SHMSZ-1) = 0x1000000;
    return 0;
}

int shutdown()
{
    //printf("VGA SHUTDOWN CALLED\n");
    *(shm+SHMSZ-1) = 0x2000000;
    return 0;
}
