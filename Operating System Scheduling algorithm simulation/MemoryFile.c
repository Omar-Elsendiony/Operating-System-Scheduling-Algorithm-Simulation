#include "headers.h"

struct node
{
    int nBytes;
    int id;
    int start;
    bool isUsed;
    struct node *left;
    struct node *right;
    struct node * parent;
};



int max(int first , int second){
   return  (first > second)?first:second;
}

struct node *newNode(int nBytes, int start)
{
    struct node* mynode = (struct node *)malloc(sizeof(struct node));
    mynode->nBytes = nBytes;
    mynode->isUsed = false;
    mynode->start = start;
    mynode->left = nullptr;
    mynode->right = nullptr;
    mynode->id = -1;
    return (mynode);
}

int powerFun (double number,int power){
    int initial = 1;
    for (int i = 0; i < power ; i++){
        initial *= number;
    }
    return initial;
}

double SizeRounding(double value)
{
    int power = 0;
    while(powerFun(2,power) < value){
        power++;
    }
    return (powerFun(2,power));
}
// int logFun(double val){
//     int i = 1;
//     while()
// }


bool insert(struct node *node, int key, int pid)
{
    bool inserted = false;
    if (node->nBytes == key && node->isUsed == false)
    {
        node->isUsed = true;
        node->id = pid;
        return true;
    }
    else if (node->nBytes != 2 && node->left == nullptr && node->right == nullptr && node->nBytes > key)
    {
        node->left = newNode((node->nBytes) / 2, node->start);
        node->right = newNode((node->nBytes) / 2, node->start + (node->nBytes) / 2);
        inserted = insert(node->left, key, pid);
        node->isUsed = inserted;
        if (inserted == false)
            node->isUsed = insert(node->right, key, pid);

        return inserted = node->isUsed;
    }

    return false;
}

void deleteChildren(struct node *parent)
{
    // puts("WOOOHOOOO");
    parent->isUsed = false;
    free(parent->left);
    free(parent->right);
    parent->left = nullptr;
    parent->right = nullptr;
}

bool deallocate(struct node *node, int pid,int time)
{
    if (node == nullptr)
        return 0;
    
    // printf("The node id is %d and the sent id is %d \n",node->id,pid);
    bool deallocated = deallocate(node->left, pid,time);
    if ((node->id) == (int)pid)
    {
        // puts("HeHe");
        node->isUsed = false;
        node->id = -1;
        fprintf(memoryFile, "At time %d freed %d bytes from process %d from %d to %d\n",
                time, node->nBytes, pid, node->start, node->start + node->nBytes - 1);
        return 1;
    }
    if (!deallocated)
        deallocate(node->right, pid,time);
    if (node->left && node->left->isUsed == false && node->right && node->right->isUsed == false 
    && node->left->left == nullptr && node->right->right == nullptr)
         deleteChildren(node);
    return 0;
}


bool findNodeModified(struct node *node, int size, struct node **ptr, int bestsize,int id)
{
    // puts("Toor");
    if (node == nullptr)
        return false;
    bool check = false;
    int newOneCreated = 0;
    int donotEnter = 0;
    if (node->left == nullptr && node->right == nullptr)
    {
        if (node->nBytes >= size && (node->nBytes/2) < size  && node->isUsed == false)
        {
            // puts("First");
            (*ptr) = node;
            node->isUsed = true;
            node->id = id;
            // bestsize = node->nBytes;
            return true;
        }
        else if (node->isUsed != true) {
            if ((node->nBytes / 2) != 0){
                // puts("Second");
                node->left = newNode((node->nBytes) / 2, (node->start));
                node->right = newNode((node->nBytes) / 2, (node->start) + (node->nBytes) / 2);
                newOneCreated = 1;
                // check = findNode(node->right, size, ptr, bestsize);
            }
        }
        else {
            donotEnter = 1;
            return false;
        }
    }
    // if (!newOneCreated)
    //     return false;   
    // puts("Not OK");
    check = findNodeModified(node->left, size, ptr, bestsize,id);   
    if (check == false)
        check = findNodeModified(node->right, size, ptr, bestsize,id);
    return check;
}

bool allocate(struct node *node, int key, int pid,int time)
{
    int number = SizeRounding((double)key);
    struct node *ptr = nullptr;
    findNodeModified(node, key , &ptr, 3333,pid);
    if (ptr)
        fprintf(memoryFile, "At time %d allocated %d bytes for process %d from %d to %d\n",
                time, number, pid, ptr->start, ptr->start + ptr->nBytes - 1);
    return true;  // To be edited
}

int getLeft(int index){
    return (index<<1 | 1);
}

int getRight(int index){
    return (index<<1 + 2);
}




// bool findNode(struct node *node, int size, struct node **ptr, int bestsize)
// {
//     // printf("")
//     if (node == nullptr){
//         return false;
//     }
//     bool check = false;
//     check = findNode(node->left, size, ptr, bestsize);
//     if (node->left == nullptr && node->right == nullptr)
//     {
//         if (node->nBytes == size && node->isUsed == false)
//         {
//             (*ptr) = node;
//             bestsize = node->nBytes;
//             return true;
//         }
//         else if (node->nBytes > size && node->nBytes < bestsize && node->isUsed == false)
//         {
//             (*ptr) = node;
//             bestsize = node->nBytes;
//             return true;
//         }
//         return false;
//     }
//     // else{
//     //     node->left = newNode((node->nBytes) / 2, node->start);
//     //     node->right = newNode((node->nBytes) / 2, node->start + (node->nBytes) / 2);
//     // }

//     if (check == false)
//         check = findNode(node->right, size, ptr, bestsize);
//     return check;
// }