#include "../include/linkedList.h"

struct LinkedListCDT {
    Node* first;
    Node* last;
    Node* curr;
    int len;
};

LinkedList createLinkedList() {
    LinkedList list = (LinkedList)myMalloc(sizeof(struct LinkedListCDT));
    if (list == NULL) return NULL;
    list->len = 0;
    list->first = NULL;
    list->last = NULL;
    list->curr = NULL;
    return list;
}

Node* addLast(LinkedList list, void *data) {
    if (list == NULL) return NULL;
    Node* newNode = (Node *)myMalloc(sizeof(Node));
    if (newNode == NULL) return NULL;
    newNode->data = data;
    return addNodeLast(list, newNode);
}

int deleteElement(LinkedList list, void *data) {
    if (list == NULL) return -1;
    startIter(list);
    while (hasNext(list)) {
        Node* current = list->curr;
        list->curr = current->next;
        if (current->data == data) {
            removeNode(list, current);
            return 0;
        }
    }
    return -1;
}

Node* addNodeLast(LinkedList list, Node *node) {
    if (list == NULL || node == NULL) return NULL;
    node->next = NULL;
    if (list->len > 0) {
        list->last->next = node;
    } else {
        list->first = node;
    }
    node->prev = list->last;
    list->last = node;
    list->len++;
    return node;
}

Node* addNodeFirst(LinkedList list, Node *node) {
    if (list == NULL || node == NULL) return NULL;
    node->prev = NULL;
    if (list->len > 0) {
        list->first->prev = node;
    } else {
        list->last = node;
    }
    node->next = list->first;
    list->first = node;
    list->len++;
    return node;
}

Node* getFirst(LinkedList list) {
    if (list == NULL) return NULL;
    return list->first;
}

int isEmpty(LinkedList list) {
    if (list == NULL) return -1;
    return !list->len;
}

int getLength(LinkedList list) {
    if (list == NULL) return -1;
    return list->len;
}

void* removeNode(LinkedList list, Node *node) {
    if (list == NULL || node == NULL) return NULL;

    if (list->first == node) {
        list->first = node->next;
    } else {
        node->prev->next = node->next;
    }

    if (list->last == node) {
        list->last = node->prev;
    } else {
        node->next->prev = node->prev;
    }

    list->len--;
    void *data = node->data;
    myFree(node);
    return data;
}

void startIter(LinkedList list) {
    if (list == NULL) return;
    list->curr = list->first;
}

int hasNext(LinkedList list) {
    if (list == NULL) return -1;
    return list->curr != NULL;
}

void* getNext(LinkedList list) {
    if (!hasNext(list)) return NULL;
    void* data = list->curr->data;
    list->curr = list->curr->next;
    return data;
}

void freeLinkedList(LinkedList list) {
    if (list == NULL) return;
    Node* current = list->first;
    while (current != NULL) {
        Node* next = current->next;
        myFree(current);
        current = next;
    }
    myFree(list);
}