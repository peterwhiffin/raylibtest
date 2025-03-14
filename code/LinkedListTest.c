#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    int data;
    struct node* previous;
    struct node* next;
} Node;

Node* getNewNode(){
    Node* newNode = (Node*)malloc(sizeof(Node));
    
    if(newNode == NULL){
        exit(-1);
    }

    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

void appendToList(Node** head, int value){
    Node* newNode = getNewNode();
    newNode->data = value;
    
    if(*head == NULL){
        *head = newNode;
        return;
    }

    Node* currentNode = *head;

    while(currentNode->next != NULL){

        currentNode = currentNode->next;
    }
    
    currentNode->next = newNode;
    newNode->previous = currentNode;
}

int main() {

    Node* head = NULL;

    for(int i = 0; i < 100; i++){
        appendToList(&head, i);
    }

    return 0;
}