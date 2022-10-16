#include <stdio.h>
#include <stdlib.h>
#include "Node.h"
#include "headers.h"
#include <string.h>
struct shmseg {
    int size;
    int number [12];
}; 

void function(int shmid){

    int *shmaddr = shmat(shmid, (void *)0, 0);
    // shmaddr->number = malloc(4 * sizeof (int));
    if (*((int*)(shmaddr)) == -1)
    {
        perror("Error in attach in writer");
        exit(-1);
    }
    else
    {
        // printf("\nWriter: Shared memory attached at address %x\n", *((int*)(shmaddr)));
        // strcpy((char *)shmaddr, "Mama Mia!!");
        int sizey = 12;
        // shmaddr->number = malloc(sizeof(int) * sizey);
        // memset(shmaddr->number,0,sizeof(int)*12);
        int shared = 69;
        // shmaddr = malloc(sizeof(int) * 4);
        shmaddr[0] = shared;
        shmaddr [5] = (shared+1);
        printf("%d",shmaddr[0]);
        printf("%d meme\n",shmaddr[1]);
        // *(shmaddr) = shared;
        sleep(1);
        // strcpy((char *)shmaddr, "quit");
    }
}

int main(int argc, char*argv[]){
    int shmid, pid;
    // int num = 99223344;
    // char* str = intTochar(num);
    // printf("%s",str);
    shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0644);

    if (shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    else
        printf("\nShared memory ID = %d\n", shmid);
    
    int parentID = getpid();
    int savedshmid = shmid;
    char* hello = intTochar(shmid);
    // printf("%s\n",hello);
    argv[1] = hello ;
    pid = fork();
    if (getpid() != parentID){
        puts("yi");
        execv("Trial.out",argv);
    }
    // sleep(1);
    function(savedshmid);
    sleep(5);
}

