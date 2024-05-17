/* MemoryList.c: functions which create ndes, link nodes, finds nodes, in first-fit contiguous memory allocation */
#include "MemoryList.h"

// create a new node
LinkedList* createNode(int isHole, int start, int length) {
    LinkedList* newNode = (LinkedList*) malloc(sizeof(LinkedList));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newNode->isHole = isHole;
    newNode->start = start;
    newNode->length = length;
    newNode->next = NULL;
    return newNode;
}

// append the node to the tail of the Linkedlist
void appendNode(LinkedList** head, int isHole, int start, int length) {
    LinkedList* newNode = createNode(isHole, start, length);
    if (newNode == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = newNode;
    } else {
        LinkedList* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// insert a process in hole assuming the current node is a hole and there is enough space
int insertProcessInHole(LinkedList* node, int length) {
    // no space for the process
    if (node->length < length) {
        return 0;
    }
    if (node->length > length) {
        LinkedList* newNode = createNode(1, node->start + length, node->length - length);
        newNode->next = node->next;
        node->next = newNode;
        node->length = length;
    }
    node->isHole = 0;
    return 1;
}

void releaseMemory(LinkedList** head, LinkedList* node) {
    node->isHole = 1;
    combineHole(head);
}

int percentageUsed(LinkedList** head) {
    LinkedList* cur = *head;
    float num = 0.0;
    while (cur != NULL) {
        if (!cur->isHole) {
            num += cur->length;
        }
        cur = cur->next;
    }
    return (int) ceil((num / (END - START)) * 100);
}


// iterate over the linkedlist and combine holes
void combineHole(LinkedList** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }
    LinkedList* cur = *head;
    while (cur != NULL && cur->next != NULL) {
        if (cur->isHole && cur->next->isHole) {
            // combine the current hole and the next hole together
            LinkedList* temp = cur->next;
            cur->length += temp->length;
            cur->next = temp->next;
            free(temp); // free the node
        } else {
            cur = cur->next; // check for the next one
        }
    }
}

// get the first fit hole for a process
LinkedList* firstFit(LinkedList** head, int length) {
    LinkedList* cur = *head;
    while (cur != NULL) {
        if (cur->isHole && cur->length >= length) {
            insertProcessInHole(cur, length);
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

// print the node
void printNode(LinkedList* node) {
    printf("%s \t %d \t %d\n", node->isHole == 1 ? "Hole\t" : "Process", node->start, node->length);
}

void printMemorySection(LinkedList** head) {
    LinkedList* cur = *head;
    while (cur != NULL) {
        printf("%s \t %d \t %d\n", cur->isHole == 1 ? "Hole:\t" : "Process:", cur->start, cur->length);
        cur = cur->next;
    }
}

// free the Linkedlist
void freeMemory(LinkedList** head) {
    LinkedList* cur = *head;
    LinkedList* prev = NULL;
    while (cur != NULL) {
        prev = cur;
        cur = cur->next;
        free(prev);
    }
    *head = NULL;
}
