#ifndef _THE_HEADER_H
#define _THE_HEADER_H
#include <stdio.h> //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "MyFunctions.h"
// #include <math.h>


typedef short bool;
#define true 1
#define false 0

#define SHKEY 300

///==============================
//don't mess with this variable//
int *shmaddr; //
// int msgq_id; //msq_id to be used by all processes
//===============================

int getClk()
{
    return *shmaddr;
}
enum processes{
    SJF = 1,
    HPF,
    RR,
    MLFQ,
};
//contains the attributes of the message whether it is the key or 
enum msgAttributes{ 
    MSGTYPE = 1, //process message
    MSGKEY,
};

enum STATUS{
    STOPPED = 1,
    STARTED,
    RESUMED,
    FINISHED,
};

FILE *memoryFile;
struct node* array;
int iterations;
/*
 * All processes call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *)shmat(shmid, (void *)0, 0);
}

/*
 * All processes call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}

#endif