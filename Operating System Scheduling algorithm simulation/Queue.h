#include "Node.h"
// #define nullptr ((void *)0)
typedef struct LinkedQueue
{
	Node* backPtr;
	Node* frontPtr;
	int Count;
}LinkedQueue;


void _LinkedQueue(LinkedQueue* Q){
	Q->frontPtr = nullptr;
	Q->backPtr = nullptr;
	Q->Count = 0;
	return;
}

void getCount(LinkedQueue* Q, int C)
{
	C = Q->Count;
}

int isEmpty(LinkedQueue* Q)
{
	return ((Q -> frontPtr)  == nullptr);
}


int enqueue(LinkedQueue* Q,struct nodeData data, int p)
{
	// Insert the new node
	Node* start = Q->frontPtr;
	Node* newNodePtr = malloc(sizeof(Node));
	_Node(newNodePtr);
	newNodePtr->data = data;
	if (isEmpty(Q)){	//special case if this is the first node to insert
		Q->frontPtr = newNodePtr; // The queue is empty
	    Q->backPtr = newNodePtr; // New node is the last node now
		// return 0;
	}
	else if (Q->frontPtr->data.priority > p){
		setNext(newNodePtr,Q->frontPtr);
		Q->frontPtr = newNodePtr;
	}
	else if (getNext(start)  == nullptr){
		if (Q->frontPtr->data.priority > p)
		{
			setNext(newNodePtr,Q->frontPtr);
			Q->frontPtr = nullptr;
			Q->frontPtr = newNodePtr;
		}
		else{
			setNext(newNodePtr,getNext(start));
			setNext(Q->frontPtr,newNodePtr);
		}
	}
    else {
		while(getNext(start) != nullptr && (getNext(start))->data.priority <= p){
			start = getNext(start); // The queue was not empty
			// puts("HERE");
		}
    	setNext(newNodePtr,getNext(start));
    	setNext(start,newNodePtr);
		if (!getNext(newNodePtr)){
			Q -> backPtr = newNodePtr;
		}
		// printf("%d The ID THAT ENTERED....................\n",(Q->backPtr->data.id));
	}
	if (getNext(newNodePtr) == nullptr){
		Q->backPtr = newNodePtr;
	}
	Q->Count = (Q->Count)+1;
		// printf("data recieved is %d\n",Q->frontPtr->data.id);

	// setNext(Q->backPtr,Q->frontPtr);
	return true;
} // end enqueue


int dequeue(LinkedQueue* Q, struct nodeData * frntEntry)
{
	if (isEmpty(Q))
	{
		frntEntry = nullptr;
		return false;
	}
	Node* nodeToDeletePtr = Q->frontPtr;
	(*frntEntry) = getItem(Q->frontPtr);
	Q->frontPtr = getNext(Q->frontPtr);
	// Queue is not empty; remove front
	if (nodeToDeletePtr == Q->backPtr)	
		Q->backPtr = nullptr;
	Q->Count = (Q->Count)-1;
	return true;
}

int enqueue2(LinkedQueue* Q,struct nodeData data, int p)
{
	// Node* start = Q->frontPtr;
	Node* newNodePtr = malloc(sizeof(Node));
	_Node(newNodePtr);
	newNodePtr->data = data;

	if (Q->backPtr != nullptr){
		setNext(Q->backPtr,newNodePtr);
	}
	Q->backPtr = newNodePtr;
	if (Q->frontPtr == nullptr){
		Q->frontPtr = newNodePtr;
	}
	return true;
}
//implement the peek function here

int peek(LinkedQueue * Q,struct nodeData * data){
	if (isEmpty(Q)){
		return false;
	}
	(*data) =  Q->frontPtr->data;
	return true;
}
int SJF_enqueue(LinkedQueue* Q,struct nodeData data, int burst)
{
	// Insert the new node
	Node* start = Q->frontPtr;
	Node* newNodePtr = malloc(sizeof(Node));
	_Node(newNodePtr);
	newNodePtr->data = data;
	if (isEmpty(Q)){	//special case if this is the first node to insert
		Q->frontPtr = newNodePtr; // The queue is empty
	    Q->backPtr = newNodePtr; // New node is the last node now
		
	}
	

	else if (getNext(start)  == nullptr){
		if (Q->frontPtr->data.runningTime > burst)
		{
			setNext(newNodePtr,Q->frontPtr);
			Q->frontPtr = nullptr;
			Q->frontPtr = newNodePtr;
		}
		else{
			setNext(newNodePtr,getNext(start));
			setNext(Q->frontPtr,newNodePtr);
		}
	}
    else {
		while(getNext(start) != nullptr && getNext(start)->data.runningTime <= burst){
			start = getNext(start); // The queue was not empty
		}
    	setNext(newNodePtr,getNext(start));
    	setNext(start,newNodePtr);
		if (!getNext(newNodePtr)){
			Q -> backPtr = newNodePtr;
		}
	
	}
	if (getNext(newNodePtr) == nullptr){
		Q->backPtr = newNodePtr;
	}
	Q->Count = (Q->Count)+1;

	
	return true;
} 




