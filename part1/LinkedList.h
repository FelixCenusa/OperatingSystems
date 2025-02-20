#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function prototypes
Node* createNode(int data);
void append(Node** head, int data);
int deleteNode(Node** head, int key);
void displayList(Node* head);
void freeList(Node** head);
int pop(Node** head); // New function to pop the head node
int size(Node* head);
int leastCommonInFuture(Node** head, char* fileName, int nrOF , int pageRow,int pageSize);
#endif // LINKEDLIST_H
