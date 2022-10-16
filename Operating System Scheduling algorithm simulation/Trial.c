#include <stdio.h>
#include <stdlib.h>
#include "Node.h"
#include "headers.h"
#include "time.h"
struct shmseg {
    int size;
    int* number;
}; 

int main(int argc, char*argv[]){
    // int shmid = atoi(argv[1]);
    clock_t t;
    t = clock();
    puts("Hello");
    int*shmaddr;
    // shmaddr = shmat(shmid, (void *)0, 0);
    // printf("%d\n",number[0]);
    sleep(2);
    // while(strcmp((char *)shmaddr, "quit")){
    // printf("%d mli\n",shmaddr[5]);
    // }
    // printf("%d\n",millis());
    Node* node = malloc(sizeof(Node));
    _Node(node);
    Node* node2 = malloc(sizeof(Node));
    _Node(node2);
    struct nodeData data;
    data.runningTime = 32;
    data.priority = 3;
    data.arrivalTime = 0;
    // setItem(node2,data);
    puts("That");
    // setNext(node,node2);
    // printf("%d\n",node->nextNode->data.runningTime);
    // char* str1 = "-sch";
    // char* str2 = argv[1];
    // printf("%c\n",str1[4]);
    // puts("kill");
    t = clock() - t;
    double timeTaken = (double)t/CLOCKS_PER_SEC;
    printf("%f time taken\n",timeTaken);
    return 0;
}

