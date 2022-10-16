#define nullptr ((void*)0)

struct nodeData {
    int id;
    int arrivalTime;
    int runningTime;
    int priority;
    int finishTime;
    int is_firstTime;
    // int remainingTime;
};

typedef struct Node
{
    struct nodeData data;
    struct Node *nextNode;
}Node;

void _Node(Node* node){
    // printf("%p\n",node);
    ((node->data).runningTime) = 0;
    (node->data).arrivalTime = 0;
    (node->data).priority = 0;
    (node->data).id = 0;
    node->nextNode = nullptr;
    (node->data).is_firstTime = 0;
    return;
}

void setNext(Node* node,Node*passedNode)  //set the next node in the list
{
    node->nextNode = passedNode;

    return;
}
Node* getNext(Node* node) //get the next node in the list
{
    return node->nextNode;
}
struct nodeData getItem(Node* node)  // get the item data in the node
{
    return node->data;
}

void setItem(Node* node,struct nodeData  i)
{
    node->data = i;
    return;
}
