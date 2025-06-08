#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdint.h>
#include <stddef.h>
#include "MemoryManagerADT.h"

typedef struct Node {
    void* data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedListCDT * LinkedList;

LinkedList createLinkedList();
Node* addLast(LinkedList list, void* data);
int deleteElement(LinkedList list, void* data);
Node* addNodeLast(LinkedList list, Node* node);
Node* getFirst(LinkedList list);
int isEmpty(LinkedList list);
void* removeNode(LinkedList list, Node* node);
void startIter(LinkedList list);
int hasNext(LinkedList list);
void freeLinkedList(LinkedList list);

#endif