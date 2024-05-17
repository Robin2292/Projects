/* MemoryList.h: structs to represent creation of contiguous memory allocation. */
#ifndef _MEMORYLIST_H
#define _MEMORYLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define START 0
#define END 2048

typedef struct LinkedList {
    int isHole;
    int start;
    int length;
    struct LinkedList* next;
} LinkedList;

// for test purposes
LinkedList* createSampleMemorySection();

// create a new node
LinkedList* createNode(int isHole, int start, int length);

// append the node to the tail of the queue
void appendNode(LinkedList** head, int isHole, int start, int length);


void releaseMemory(LinkedList** head, LinkedList* node);
void combineHole(LinkedList** head);
int percentageUsed(LinkedList** head);

// printing
void printNode(LinkedList* node);
void printMemorySection(LinkedList** head);

LinkedList* firstFit(LinkedList** head, int length);
LinkedList* bestFit(LinkedList** head, int length);
LinkedList* worstFit(LinkedList** head, int length);
int insertProcessInHole(LinkedList* node, int length);

void freeMemory(LinkedList** head);

#endif //_MEMORYLIST_H