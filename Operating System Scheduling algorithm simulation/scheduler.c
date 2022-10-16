#include "headers.h"
#include "Queue.h"
#include "time.h"
#include <string.h>
#include "MemoryFile.c"

void output(int time,int id , int state,int arrived ,int total, int remaining,int waiting);
struct msgbuff
{
    long mtype;
    int parameters[70];
};
////// Global Variables ////////////
FILE *fpout;
FILE * schperf;
float * waitArr;
float *TAArr;
int * diff;
int algo;
int currentTime;
int idleTime;
int doNotEnter;
int endTime;
//////////////////////////////////////
void sum (int processno) //gets the average stuff to print in scheduler.perf
{
    float wait=0;
    float AVGTA=0;
    // result= malloc(sizeof(float)*2);
    for(int i=0;i < processno;i++)
    {
        wait+=waitArr[i];
        AVGTA+=TAArr[i];
    }
    wait= (float)wait/processno;
    AVGTA= (float)AVGTA/processno;
    printf("IdleTime is %d , endTime is %d\n",idleTime,endTime);
    float cpuU = (float)(endTime - idleTime)/(endTime);
    // printf("CPU utilization is %d\n",cpuU);
    cpuU *= 100;
    fprintf(schperf,"CPU UTILIZATION = %.2f\nTA = %.2f\nAvgWaiting = %.2f\n",cpuU,AVGTA,wait);
    return;
}
int main(int argc, char *argv[])
{
    initClk();
    idleTime = 0;
    fpout = fopen("scheduler.log", "w");
    memoryFile = fopen("memory.log","w");
    fprintf(memoryFile, "#At time x allocated y bytes for process z from i to j\n");
    //////////////////////////////////////////////////////////////////////////////////
    int processAlgo = atoi(argv[1]); //process algorithm in the second argument
    int processno = atoi(argv[2]);
    waitArr = malloc(sizeof(float) * processno);
    TAArr = malloc(sizeof(float) * processno);
    diff = malloc(sizeof(int) * processno);
    algo = processAlgo;
    int quantum;
    if (argv[3] != ""){
        quantum = atoi(argv[3]);
    }
    pid_t parentID = getpid();
    // printf("%d from scheduler\n",getpid());
    pid_t pid;
    int msgq_id = msgget(MSGKEY,0666 |IPC_CREAT);
    struct msgbuff message;
    LinkedQueue * Q = malloc(sizeof(LinkedQueue));
    _LinkedQueue(Q);
    /////// Creating shared memory ///////////////////////
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0644);
    char * theShared = intTochar(shmid);
    int *shmaddr = shmat(shmid, (void *)0, 0);
    for (int i = 0; i < processno * 4;i++){
        shmaddr[i] = 0;
    }
    if (*((int*)(shmaddr)) == -1)
    {
        perror("Error in attach in writer");
        exit(-1);
    }
    // Processes shared variables that they use in common
    int endedALL = 0; //ended all the processes the number that ended
    int ok;
    int opPriority = 100; 
    int operatingProcess = 0;
    int now;
    struct node* rootNode = newNode(1024,0);
    // rootNode->left = newNode((rootNode->nBytes) /2,rootNode->start);
    // rootNode->right = newNode((rootNode->nBytes)/2,rootNode->start + rootNode->nBytes);
    int memSize;
    ///////////////
    struct nodeData data;
    struct nodeData data3;
    struct nodeData data2;
    
    while(endedALL != processno){
        if (processAlgo == RR){
            // struct nodeData data;
            int flag = 0;
            now = getClk();          
            if (!isEmpty(Q)){
                dequeue(Q,&data);
                
                operatingProcess = data.id;
                if (operatingProcess)
                {
                    if (data.is_firstTime == 0){ 
                        shmaddr[3*operatingProcess] = STARTED;
                        output(getClk(),operatingProcess,STARTED,data.arrivalTime,data.runningTime,
                    data.runningTime,shmaddr[3*operatingProcess+2]);
                        data.is_firstTime  = 1;
                    }
                    else{
                        shmaddr[3*operatingProcess] = RESUMED;
                    output(getClk(),operatingProcess,RESUMED,data.arrivalTime,data.runningTime,
                    shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                    }
                    flag = 0; 
                    while ((getClk() - now) < (quantum)){  
                        if(shmaddr[3*operatingProcess] == FINISHED){
                            output(getClk(),(operatingProcess),FINISHED,data.arrivalTime,
                            data.runningTime,shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                            endedALL++;
                            deallocate(rootNode,operatingProcess,getClk());
                            printf("ended all %d\n",endedALL);
                            flag = 1;
                            break;
                        }
                        ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
                        if (ok != -1){
                            // struct nodeData data2;
                            data2.id = message.parameters[0];
                            data2.arrivalTime = message.parameters[1];
                            data2.runningTime = message.parameters[2];
                            data2.priority = message.parameters[3];
                            memSize = message.parameters[4];
                            data2.is_firstTime = 0;
                            doNotEnter = 0;
                            enqueue2(Q,data2,0);
                            ///////////////////////////////
                            allocate(rootNode,memSize,data2.id,getClk());
                            ///////////////////////////////
                            argv[1] = intTochar(data2.id);
                            argv[2] = intTochar(data2.runningTime);
                            argv[3] = theShared;
                            if (operatingProcess){
                                shmaddr[3*data2.id] = STOPPED;
                            }
                            pid = fork();
                            if (getpid() != parentID){
                                execv("process.out",argv);
                            }
                        }  
                    }
                }
            }
            else{               
                operatingProcess = 0; 
                if (!doNotEnter){
                    currentTime = getClk(); 
                    doNotEnter = 1; 
                }   
                if (getClk() - currentTime == 1){
                    idleTime++;
                    currentTime = getClk();
                }
            }
            if (!flag && operatingProcess){
                sleep(0.05);
                if (shmaddr[3*operatingProcess] != FINISHED){
                    shmaddr[3*operatingProcess] = STOPPED; 
                    output(getClk(),operatingProcess,STOPPED,data.arrivalTime,
                    data.runningTime,shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                    enqueue2(Q,data,0);
                }
                else{
                    // deallocate(rootNode,operatingProcess,getClk());
                    output(getClk(),operatingProcess,FINISHED,data.arrivalTime,
                    data.runningTime,shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                    endedALL++;
                }
            } 
            ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
            if (ok != -1){
                // struct nodeData data2;
                data2.id = message.parameters[0];
                data2.arrivalTime = message.parameters[1];
                data2.runningTime = message.parameters[2];
                data2.priority = message.parameters[3];
                data2.is_firstTime = 0;
                doNotEnter = 0;
                memSize = message.parameters[4];
                //////////////////////////////////////////////
                allocate(rootNode,memSize,data2.id,getClk());
                /////////////////////////////////////////////
                enqueue2(Q,data2,0);
                argv[1] = intTochar(data2.id);
                argv[2] = intTochar(data2.runningTime);
                argv[3] = theShared;
                if (operatingProcess){
                    shmaddr[3*data2.id] = STOPPED;
                }
                pid = fork();
                if (getpid() != parentID){
                    execv("process.out",argv);
                }
                // ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
            }  
        }
        
        else if (processAlgo == HPF){
            now = getClk();
	        ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
            while (ok != -1){
                
                data2.id = message.parameters[0];
                data2.arrivalTime = message.parameters[1];
                data2.runningTime = message.parameters[2];
                data2.priority = message.parameters[3];
                data2.is_firstTime = 0;
                doNotEnter = 0;
                enqueue(Q,data2,data2.priority);
                memSize = message.parameters[4];
                allocate(rootNode,memSize,data2.id,getClk());
                argv[1] = intTochar(data2.id);
                argv[2] = intTochar(data2.runningTime);
                argv[3] = theShared;
                pid = fork();
                if (getpid() != parentID){
                    execv("process.out",argv);
                }
                ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
            }
	        if (!isEmpty(Q)){
                if (!operatingProcess){
                    peek(Q,&data3);
                    operatingProcess = data3.id;
                    opPriority = data3.priority;
                        if (data3.is_firstTime == 0){ 
                            shmaddr[3*operatingProcess] = STARTED;
                            output(getClk(),operatingProcess,STARTED,data3.arrivalTime,data3.runningTime,
                            data3.runningTime,shmaddr[3*operatingProcess+2]);
                            data3.is_firstTime  = 1;
                        }
                        else{
                            shmaddr[3*operatingProcess] = RESUMED;
                            output(getClk(),operatingProcess,RESUMED,data3.arrivalTime,data3.runningTime,
                            shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                        }
                }
                else{
                    peek(Q,&data3);                   
                    opPriority = data3.priority;
                    if (opPriority == data2.priority && data2.id == data3.id){
                        shmaddr[3*operatingProcess] = STOPPED; 
                        output(getClk(),operatingProcess,STOPPED,data3.arrivalTime,
                        data3.runningTime,shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                        operatingProcess = data2.id;
                        opPriority = data2.priority;
                        output(getClk(),operatingProcess,STARTED,data2.arrivalTime,data2.runningTime,
                        data2.runningTime,shmaddr[3*operatingProcess+2]); 
                        shmaddr[operatingProcess* 3] = STARTED;
                    }
                    else{
                        shmaddr[3*data2.id] = STOPPED;
                    }
                }
                while(getClk() - now == 0){
                }
                sleep(0.1);
                if (shmaddr[operatingProcess*3] == FINISHED){
                    output(getClk(),operatingProcess,FINISHED,data3.arrivalTime,
                    data3.runningTime,shmaddr[3*operatingProcess+1],shmaddr[3*operatingProcess+2]);
                    endedALL++;
                    deallocate(rootNode,operatingProcess,getClk());
                    printf("Ended l %d \n",endedALL);
                    operatingProcess = 0;
                    dequeue(Q,&data3);
                }
            }
            else{
                if (!doNotEnter){
                    currentTime = getClk(); 
                    doNotEnter = 1; 
                }   
                if (getClk() - currentTime == 1){
                    idleTime++;
                    currentTime = getClk();
                }      
            }      
        }
	else if (processAlgo == SJF) {
            
            // struct nodeData data2;
            ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
            while (ok != -1){
                // struct nodeData data2;  
                data2.id = message.parameters[0];
                data2.arrivalTime = message.parameters[1];
                data2.runningTime = message.parameters[2];
                data2.priority = message.parameters[3];
                data2.is_firstTime = 0;
                memSize = message.parameters[4];
                doNotEnter = 0;
                allocate(rootNode,memSize,data2.id,getClk());
                enqueue(Q, data2,data2.runningTime);       
                argv[1] = intTochar(data2.id); // used the new Integer Convertor
                argv[2] = intTochar(data2.runningTime);
                argv[3] = theShared;
                shmaddr[data2.id* 3] = STOPPED;
                pid = fork();
                if (getpid() != parentID)
                {
                    execv("process.out", argv); // excutes the process till it's ending 
                }  
                ok = msgrcv(msgq_id, &message, sizeof(message.parameters), MSGTYPE, IPC_NOWAIT);
            }
            
            // when the process ends we get inside this condition
            while(getClk() - now == 0){
                now = getClk();
            }
            if (shmaddr[3 * operatingProcess] == FINISHED)
            {
                // printf("arrival is %d\n",data.id);
                output(getClk(), operatingProcess, FINISHED, data.arrivalTime,
                data.runningTime, shmaddr[3*operatingProcess+1], shmaddr[3*operatingProcess+2]);
                endedALL++;
                deallocate(rootNode,operatingProcess,getClk());
                printf("Ended %d\n",endedALL);
                operatingProcess = 0;
            }
            if (!isEmpty(Q)){  
                if (!operatingProcess)
                {   
                        dequeue(Q,&data);                   
                        operatingProcess = data.id;         
                        shmaddr[3*operatingProcess] = STARTED;
                        output(getClk(), operatingProcess, STARTED, data.arrivalTime,
                        data.runningTime,shmaddr[3*operatingProcess+1], shmaddr[3*operatingProcess+2]);                                          
                } 
                else{
                    if (!doNotEnter){
                        currentTime = getClk(); 
                        doNotEnter = 1; 
                    }   
                    if (getClk() - currentTime == 1){
                        idleTime++;
                        currentTime = getClk();
                    }      
            }         
        }
    }
        else if(processAlgo == MLFQ){

        }
    }
    endTime = getClk();
    puts("DID YOU END?");
    //TODO: upon termination release the clock resources.
    fclose(fpout);
    schperf = fopen("Scheduler.perf","w");
    // fpout = fopen("scheduler.log", "w");
    sum(processno);
    fclose(schperf);
    // destroyClk(true);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *)0);
    sleep(5);
    return 0;
}


void output(int time,int id, int state,int arrived,int total,int remaining,int waiting)
{ 

    if (state == FINISHED){
        float TA = (time - arrived);
        float WTA = (float)(time - arrived)/(float)total;
        TAArr[id - 1] = WTA;
        // if (algo == RR)
        //     waiting += diff[id - 1];
        waitArr [id - 1] = waiting;
        fprintf(fpout,"At  time  %d  process  %d  finished arr %d total %d  remain  %d  wait  %d  TA  %.2f  WTA  %.2f\n",time,id,arrived,total,remaining,waiting,TA,WTA);
    }
    else if (state == STARTED){
        if (algo == RR){          
            // diff[id - 1] = time - arrived;
            // waiting+=diff[id -1];
        }   
        fprintf(fpout,"At  time  %d  process  %d  started arr %d total %d  remain  %d  wait  %d\n",time,id,arrived,total,remaining,waiting);  
    }
    else if (state == STOPPED){
        if (algo == RR){
            // waiting+=diff[id -1];
        }  
        fprintf(fpout,"At  time  %d  process  %d  stopped arr %d total %d  remain  %d  wait  %d\n",time,id,arrived,total,remaining,waiting);  
    }
    else if (state == RESUMED){
        if (algo == RR){
            // waiting+=diff[id -1];
        }  
        fprintf(fpout,"At  time  %d  process  %d  resumed arr %d total %d  remain  %d  wait  %d\n",time,id,arrived,total,remaining,waiting);  

    }
}

