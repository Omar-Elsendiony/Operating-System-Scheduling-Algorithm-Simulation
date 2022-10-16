#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{   
    initClk(); 
    int now = getClk(); //may be removed
    int id = atoi(argv[1]);
    // printf("%d\n",id);
    int remainingTime = atoi(argv[2]);
    int shmid = atoi(argv[3]);
    int*shmaddr;
    shmaddr = shmat(shmid, (void *)0, 0);
    // printf("%p\n",shmaddr);
    int * remainingP = (shmaddr +id*3+1);
    // printf("%p\n",remainingP);
    *remainingP = remainingTime;
    int waitingTime = 0;
    int * waitingP = (shmaddr +id*3+2);
    *waitingP = waitingTime;
    int entered = 0;
    // printf("%d status\n",shmaddr[3*id]);
    // sleep(0.3);
    while ((*remainingP) != 0)
    {
        
        // sleep(0.2);
            while(shmaddr[3*id] == STOPPED)
            {                
                if(getClk() - now){
                    (*waitingP)++;
                    now = getClk();
                }
            }
            while (shmaddr[3*id] == STARTED || shmaddr[3*id] == RESUMED)
            {
                if(getClk() - now){
                    (*remainingP)--;
                    // printf("%d remaining\n",(*remainingP));
                    now = getClk();
                }       
                if ((*remainingP) == 0){
                    shmaddr[3*id] = FINISHED;
                    // printf("YOUUUUUUUU %d\n",id);
                    destroyClk(false);
                    return 0;
                    break;
                }         
            }
                     
            //     if ((*remainingP) == 0){
            //         shmaddr[3*id] = FINISHED;
            //         break;
            //     }
            // }
            // now  = getClk();
        // Check on the shared memory if preempted
    }
    // shmaddr[3*id] = FINISHED;
    // puts("YOUUUUUUUU");
    destroyClk(false);
    return 0;
}
