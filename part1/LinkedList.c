#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"


Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}


void append(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}


int deleteNode(Node** head, int key) {
    Node* temp = *head;
    Node* prev = NULL;

    if (temp != NULL && temp->data == key) {
        *head = temp->next;
        free(temp);
        return 0;
    }

    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        
        return-1;
    }

    prev->next = temp->next;
    free(temp);
    return 0;
}


int pop(Node** head) {
    if (*head == NULL) {
        printf("List is empty, nothing to pop.\n");
        return -1; 
    }

    Node* temp = *head;
    int data = temp->data; 
    *head = (*head)->next; 
    free(temp);            
    return data;           
}


void displayList(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}


void freeList(Node** head) {
    Node* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int size(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int leastCommonInFuture(Node** head, char* fileName, int nrOf, int pageRow, int pageSize) {
    if (head == NULL) {
        printf("List is empty.\n");
        return -1; 
    }

    int* array = malloc(nrOf * sizeof(int));
    int j = 0;
    Node* current = *head;
    Node* temp = *head;
    Node* temp2 = *head;

    while (temp != NULL) {
        array[j] = -1; 
        temp = temp->next;
        j++;
    }
    
    int number;
    int row = 0;
    FILE *pF = fopen(fileName, "r");
    if (pF == NULL) {
        perror("Failed to open file");
        free(array);
        return -1;
    }

    while (fscanf(pF, "%u", &number) == 1) {
        if (row >= pageRow) {
            int actualNumber = number / pageSize;
            current = *head;
            for (int i = 0; i < nrOf; i++) {
                if (current->data == actualNumber && array[i] == -1) {
                    array[i] = row; // Store first future occurrence NOT how ++ how many times that number exists in future and keep most discard least.
                    break;
                }
                current = current->next;
            }
        }
        row++;
    }
    fclose(pF);

    int farthest = 0;
    for (int i = 0; i < nrOf; i++) {
        if (array[i] == -1) { 
            farthest = i;
            break;
        }
        if (array[i] > array[farthest]) { 
            farthest = i;
        }
    }

    int listIndex = 0;
    while (temp2 != NULL) {
        if (listIndex == farthest) {
            return temp2->data;
        }
        listIndex++;
        temp2 = temp2->next;
    }

    free(array);
    return -1; 
}

