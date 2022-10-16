#include "headers.h"
#include <ctype.h>

void clearResources(int);
struct msgbuff
{
    long mtype;
    int parameters[70];
};

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    char * filePath = argv[1];
    FILE* fpointer= fopen(filePath,"r");
    if (!fpointer){
        // puts("Not OK\n");
        exit(-1);
    }
    else{
        // puts("OK\n");
    }
    char *line = NULL;
    size_t len = 0;
    int processno=0;
    char buffer[1024];
    fgets(buffer, 1024, fpointer);
    while(getline(&line, &len, fpointer) != -1) {
        processno++;
    }
    // printf("%d\n",processno);
    int* processes=(int*) malloc(5*processno*sizeof(int));
    rewind(fpointer);
    char bufferr[1024];
    fgets(bufferr, 1024, fpointer);   
    for (int i= 0;i < 5* processno;i++)
    {
       fscanf(fpointer,"%d",&processes[i]);
    //    printf("%d\n",processes[i]);
    }
    free(line); 
    fclose(fpointer);
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    int alg;
    char* argument = (char*)argv[2];
    if (!checkString("-sch",argv[2])){
        puts("Error in the arguments sent");
        puts("Terminating...");
        sleep(2);
        exit(-1);
    }
    int processAlgo = atoi(argv[3]);
    int q = 0;
    if (processAlgo == RR){
        if (checkString(argv[4],"-q")){
            q = atoi(argv[5]);
        }
        else{
            puts("Error in the arguments sent");
            puts("Terminating...");
            sleep(2);
            exit(-1);
        }
    }
    // 3. Initiate and create the scheduler and clock processes.
    pid_t processGenID = getpid();
    pid_t Clk = fork();
    if (getpid() != processGenID) // clk process
    { 
        execv("clk.out", argv);
    }
    sleep(1); //sleep to ensure that the clock starts
    pid_t scheduler = fork();
    if (getpid() != processGenID) // scheduler process
    {
        argv[1] = intTochar(processAlgo);
        argv[2] = intTochar(processno);
        argv[3] = intTochar(q);
        // printf("%d from generator\n",getpid());
        execv("scheduler.out",argv);
    }
    // sleep(2);
    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function. 
    int x = getClk();
    printf("Current Time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    int msgq_id = msgget(MSGKEY, 0666 | IPC_CREAT); // creating the message queue

    struct msgbuff message; 
    message.mtype = MSGTYPE; //enum to indicate that the message is process message
    int j = 1;
    for(int i = 0; i < 5* processno ; i += 5)
    {
        // printf("%d The I\n",i);
        while(getClk() < processes[j])
        {
            // sleep(processes[j]-getClk());
        }
        // message.mtype = processes[i];
        message.parameters[0] = processes[i];
        message.parameters[1] = processes[i+1];
        message.parameters[2] = processes[i+2];
        message.parameters[3] = processes[i+3];
        message.parameters[4] = processes[i+4];
        int send_val = msgsnd(msgq_id, &message, sizeof(message.parameters), !IPC_NOWAIT);
        j += 5;
    }
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources

    // sleep(12);
    int status;
    int id = waitpid(scheduler,&status,0);
    if (WIFEXITED(status))
    {
        int msgq_del;
        msgq_del = msgctl(msgq_id, IPC_RMID, 0);
        destroyClk(true);
        printf("THE END!!!");
        exit(0);
    }
}

void clearResources(int msgq_id)
{
    int msgq_del;
    msgq_del = msgctl(msgq_id, IPC_RMID, 0);
    destroyClk(true);
    exit(0);    
}

